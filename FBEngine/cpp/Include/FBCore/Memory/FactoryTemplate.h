#ifndef FactoryInstanceAdaptor_h__
#define FactoryInstanceAdaptor_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/PoolAllocator.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/InstancePool.h>
#include <FBCore/Base/Pool.h>

#ifdef _DEBUG
#    include <FBCore/Thread/RecursiveMutex.h>
#endif

namespace fb
{
    template <class T>
    class FactoryTemplate : public CSharedObject<IFactory>
    {
    public:
        using type = T;

        /** Constructor
         */
        FactoryTemplate() : CSharedObject<IFactory>(), m_listener( this )
        {
        }

        /** Constructor
        @param
        factoryObjectName The type of object this factory creates.
        */
        FactoryTemplate( const String &objectType, hash64 objectTypeId ) :
            m_listener( this ),
            m_objectType( objectType ),
            m_objectTypeId( objectTypeId )
        {
        }

        ~FactoryTemplate() override
        {
            unload( nullptr );
        }

        void load( SmartPtr<ISharedObject> data )
        {
            auto pool = fb::make_ptr<Pool<T>>();
            pool->setNextSize( m_nextSize );
            setPool( pool );
        }

        void unload( SmartPtr<ISharedObject> data )
        {
#if FB_USE_MEMORY_POOL
            if( auto pool = getPool() )
            {
                if( auto p = getInstancesPtr() )
                {
                    auto &instances = *p;

#    if FB_TRACK_REFERENCES
                    if( !instances.empty() )
                    {
                        auto &tracker = ObjectTracker::instance();
                        tracker.dumpReport();
                    }
#    endif

                    for( auto instance : instances )
                    {
                        auto pObject = (T *)instance;
                        pObject->~T();

                        pool->free( instance );
                    }

                    setInstancesPtr( nullptr );
                }
            }

            setPool( nullptr );
#endif
        }

        void *createObject() override
        {
            auto object = new T;
            return object;
        }

        void *createObjectFromPool() override
        {
#if FB_USE_MEMORY_POOL
#    ifdef FB_USE_BOOST
            //FB_ASSERT( Memory::CheckHeap() );

            if( auto pool = getPool() )
            {
                auto object = new( pool->malloc() ) T;
                if( !object )
                {
                    std::cout << "Out of memory." << std::endl;
                    std::terminate();
                }

                //FB_ASSERT( Memory::CheckHeap() );

                object->setSharedObjectListener( &m_listener );

                object->setPoolElement( true );
                object->setCreatorData( this );

                addInstance( object );

                FB_ASSERT( object->isPoolElement() );
                return object;
            }

            return nullptr;
#    else
            auto ptr = new T();
            if( !ptr )
            {
                std::cout << "Out of memory." << std::endl;
                std::terminate();
            }

            return ptr;
#    endif
#else
            auto ptr = new T();
            if( !ptr )
            {
                std::cout << "Out of memory." << std::endl;
                std::terminate();
            }

            return ptr;
#endif
        }

        bool isObjectDerivedFromByInfo( u32 typeInfo ) const override
        {
            FB_ASSERT( typeInfo != 0 );

            auto objectTypeInfo = type::typeInfo();
            FB_ASSERT( objectTypeInfo != 0 );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            return typeManager->isDerived( objectTypeInfo, typeInfo );
        }

        void freeObject( void *object ) override
        {
#if FB_USE_MEMORY_POOL
#    ifdef FB_USE_BOOST
            auto pObject = (T *)object;
            pObject->~T();

            removeInstance( pObject );

            auto pool = getPool();
            pool->free( pObject );
#    endif
#endif
        }

        void *createArray( u32 numElements ) override
        {
            auto objects = new T[numElements];
            return objects;
        }

        String getObjectType() const override
        {
            return m_objectType;
        }

        void setObjectType( const String &type ) override
        {
            m_objectType = type;
        }

        hash64 getObjectTypeId() const override
        {
            return m_objectTypeId;
        }

        void setObjectTypeId( hash64 id ) override
        {
            m_objectTypeId = id;
        }

        u32 getObjectSize() const override
        {
            return sizeof( T );
        }

        void destroy_shared( T *pointer )
        {
#if FB_USE_MEMORY_POOL
            if( pointer )
            {
                pointer->~T();
                m_pool->free( pointer );
            }
#endif
        }

        String getTypeName() const override
        {
            return m_typeName;
        }

        void setTypeName( const String &typeName ) override
        {
            m_typeName = typeName;
        }

        u32 getDataSize() const
        {
            return m_nextSize;
        }

        void setDataSize( [[maybe_unused]] u32 size ) override
        {
#if FB_USE_MEMORY_POOL
            m_nextSize = size;

            auto pool = getPool();
            pool->setNextSize( size );

            m_instancePool.setGrowSize( size );
#endif
        }

        void allocate_data() override
        {
#if FB_USE_MEMORY_POOL
            // m_instancePool.allocate_data();
#endif
        }

        void *allocate() override
        {
            return nullptr;
        }

        void deallocate( [[maybe_unused]] void *ptr ) override
        {
        }

        SmartPtr<ISharedObject> createInstance() override
        {
#if FB_USE_MEMORY_POOL
            if( m_instancePool.getNumElements() > 0 )
            {
                auto ptr = m_instancePool.getInstance();
                if( ptr )
                {
                    return ptr;
                }
            }
#endif

            return SmartPtr<ISharedObject>( new T() );
        }

        void destroyInstance( SmartPtr<ISharedObject> instance ) override
        {
        }

        s32 getMemoryUsed() const override
        {
            auto instances = getInstances();
            return static_cast<s32>( getObjectSize() * instances.size() );
        }

        SmartPtr<Pool<T>> getPool() const
        {
            return m_pool;
        }

        void setPool( SmartPtr<Pool<T>> pool )
        {
            m_pool = pool;
        }

        Array<T *> getInstances() const
        {
            if( auto ptr = getInstancesPtr() )
            {
                RecursiveMutex::ScopedLock lock( m_instancesMutex );
                auto &instances = *ptr;
                return Array<T *>( instances.begin(), instances.end() );
            }

            return Array<T *>();
        }

        /** Gets the instantiate objects. */
        Array<SmartPtr<ISharedObject>> getInstanceObjects() const
        {
            if( auto ptr = getInstancesPtr() )
            {
                RecursiveMutex::ScopedLock lock( m_instancesMutex );
                auto &instances = *ptr;
                return Array<SmartPtr<ISharedObject>>( instances.begin(), instances.end() );
            }

            return Array<SmartPtr<ISharedObject>>();
        }

        SharedPtr<ConcurrentArray<T *>> getInstancesPtr() const
        {
            return m_instances;
        }

        void setInstancesPtr( SharedPtr<ConcurrentArray<T *>> ptr )
        {
            m_instances = ptr;
        }

        void addInstance( T *ptr )
        {
            auto instances = getInstancesPtr();
            if( !instances )
            {
                instances = fb::make_shared<ConcurrentArray<T *>>();
                setInstancesPtr( instances );
            }

            if( instances )
            {
                RecursiveMutex::ScopedLock lock( m_instancesMutex );
                instances->push_back( ptr );
            }
        }

        void removeInstance( T *ptr )
        {
            RecursiveMutex::ScopedLock lock( m_instancesMutex );

            auto instances = getInstances();
            auto it = std::find( instances.begin(), instances.end(), ptr );
            if( it != instances.end() )
            {
                instances.erase( it );

                auto pPtr = fb::make_shared<ConcurrentArray<T *>>( instances.begin(), instances.end() );
                setInstancesPtr( pPtr );
            }
        }

    protected:
        template <class B>
        class Listener : public ISharedObjectListener
        {
        public:
            Listener() = default;

            Listener( FactoryTemplate<B> *factory ) : m_factory( factory )
            {
            }

            ~Listener() override
            {
                m_factory = nullptr;
            }

            void loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                      LoadingState newState ) override
            {
            }

            bool destroy( void *ptr ) override
            {
                if( m_factory )
                {
                    m_factory->freeObject( ptr );
                    return true;
                }

                return false;
            }

            FactoryTemplate<B> *m_factory = nullptr;
        };

#if FB_USE_MEMORY_POOL
        AtomicSmartPtr<Pool<T>> m_pool;

        InstancePool<T> m_instancePool;
#endif

        Listener<T> m_listener;

        /// Instances
        AtomicSharedPtr<ConcurrentArray<T *>> m_instances;
        mutable RecursiveMutex m_instancesMutex;

        /// The type of object this factory creates
        String m_objectType;

        /// The name of the object type.
        String m_typeName;

        /// The id of the type of object this factory creates
        hash64 m_objectTypeId = 0;

        atomic_u32 m_instanceCount = 0;
        atomic_u32 m_nextSize = 4;
    };
}  // end namespace fb

#endif  // FactoryInstanceAdaptor_h__
