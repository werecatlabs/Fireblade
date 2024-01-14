#ifndef FactoryTemplate_h__
#define FactoryTemplate_h__

#include <FBCore/System/Factory.h>
#include <FBCore/Memory/PoolAllocator.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/InstancePool.h>
#include <FBCore/Core/Pool.h>

namespace fb
{
    /** A factory to class to create objects.
     */
    template <class T>
    class FactoryTemplate : public Factory
    {
    public:
        // The type of object this factory creates.
        using type = T;

        /** Constructor */
        FactoryTemplate();

        /** Constructor
        @param objectType The type of object this factory creates.
        @param objectTypeId The id of the type of object this factory creates.
        */
        FactoryTemplate( const String &objectType, hash64 objectTypeId );

        /** Destructor */
        ~FactoryTemplate() override;

        /** @copydoc Factory::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc Factory::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setGrowSize( u32 size );

        void allocatePoolData();

        /** Create an object. */
        void *createObject() override;

        /** Free an object. */
        void freeObject( void *object ) override;

        /** @copydoc Factory::isObjectDerivedFromByInfo */
        bool isObjectDerivedFromByInfo( u32 typeInfo ) const override;

        void setDataSize( [[maybe_unused]] u32 size );

        SmartPtr<Pool<T>> getPool() const;

        void setPool( SmartPtr<Pool<T>> pool );

        Array<T *> getInstances() const;

        SharedPtr<ConcurrentArray<T *>> getInstancesPtr() const;

        void setInstancesPtr( SharedPtr<ConcurrentArray<T *>> ptr );

        void addInstance( T *ptr );

        void removeInstance( T *ptr );

        FB_CLASS_REGISTER_TEMPLATE_DECL( FactoryTemplate, T );

    protected:
        AtomicSmartPtr<Pool<T>> m_pool;

        InstancePool<T> m_instancePool;

        /// Instances
        AtomicSharedPtr<ConcurrentArray<T *>> m_instances;
        mutable RecursiveMutex m_instancesMutex;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, FactoryTemplate, T, Factory );

    template <class T>
    FactoryTemplate<T>::FactoryTemplate()
    {
        m_objectSize = sizeof( T );
    }

    template <class T>
    FactoryTemplate<T>::FactoryTemplate( const String &objectType, hash64 objectTypeId )
    {
        setObjectType( objectType );
        setObjectTypeId( objectTypeId );

        auto objectSize = sizeof( T );
        setObjectSize( objectSize );
    }

    template <class T>
    FactoryTemplate<T>::~FactoryTemplate()
    {
        unload( nullptr );
    }

    template <class T>
    void FactoryTemplate<T>::load( SmartPtr<ISharedObject> data )
    {
        auto pool = fb::make_ptr<Pool<T>>();
        pool->setNextSize( m_nextSize );
        setPool( pool );
    }

    template <class T>
    void FactoryTemplate<T>::unload( SmartPtr<ISharedObject> data )
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

    template <class T>
    void FactoryTemplate<T>::setGrowSize( u32 size )
    {
        Factory::setGrowSize( size );

        if( auto pool = getPool() )
        {
            pool->setNextSize( size );
        }
    }

    template <class T>
    void FactoryTemplate<T>::allocatePoolData()
    {
        if( auto pool = getPool() )
        {
            pool->allocateData();
        }
    }

    template <class T>
    void *FactoryTemplate<T>::createObject()
    {
#if FB_USE_MEMORY_POOL
#    ifdef FB_USE_BOOST
        //FB_ASSERT( Memory::CheckHeap() );

        if( auto pool = getPool() )
        {
            if( pool->getSize() > 0 )
            {
                auto object = new( pool->malloc() ) T;
                if( !object )
                {
                    std::cout << "Out of memory." << std::endl;
                    std::terminate();
                }

                //FB_ASSERT( Memory::CheckHeap() );

                object->setSharedObjectListener( m_listener );
                object->setPoolElement( true );
                object->setCreatorData( this );

                addInstance( object );

                FB_ASSERT( object->isPoolElement() );
                return object;
            }
        }

        auto ptr = Factory::createObject();
        if( !ptr )
        {
            std::cout << "Out of memory." << std::endl;
            std::terminate();
        }

        auto object = new( ptr ) T;
        object->setSharedObjectListener( m_listener );
        return object;
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

    template <class T>
    void FactoryTemplate<T>::freeObject( void *object )
    {
        auto *pObject = (T *)object;
        pObject->~T();

        if( pObject->isPoolElement() )
        {
            removeInstance( pObject );

            if( auto pool = getPool() )
            {
                pool->free( pObject );
            }
        }
        else
        {
            Factory::freeObject( object );
        }
    }

    template <class T>
    bool FactoryTemplate<T>::isObjectDerivedFromByInfo( u32 typeInfo ) const
    {
        FB_ASSERT( typeInfo != 0 );

        auto objectTypeInfo = type::typeInfo();
        FB_ASSERT( objectTypeInfo != 0 );

        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        return typeManager->isDerived( objectTypeInfo, typeInfo );
    }

    template <class T>
    void FactoryTemplate<T>::setDataSize( [[maybe_unused]] u32 size )
    {
#if FB_USE_MEMORY_POOL
        m_nextSize = size;

        auto pool = getPool();
        pool->setNextSize( size );
        pool->allocateData();

        m_instancePool.setGrowSize( size );
#endif
    }

    template <class T>
    SmartPtr<Pool<T>> FactoryTemplate<T>::getPool() const
    {
        return m_pool;
    }

    template <class T>
    void FactoryTemplate<T>::setPool( SmartPtr<Pool<T>> pool )
    {
        m_pool = pool;
    }

    template <class T>
    Array<T *> FactoryTemplate<T>::getInstances() const
    {
        if( auto ptr = getInstancesPtr() )
        {
            RecursiveMutex::ScopedLock lock( m_instancesMutex );
            auto &instances = *ptr;
            return Array<T *>( instances.begin(), instances.end() );
        }

        return Array<T *>();
    }

    template <class T>
    SharedPtr<ConcurrentArray<T *>> FactoryTemplate<T>::getInstancesPtr() const
    {
        return m_instances;
    }

    template <class T>
    void FactoryTemplate<T>::setInstancesPtr( SharedPtr<ConcurrentArray<T *>> ptr )
    {
        m_instances = ptr;
    }

    template <class T>
    void FactoryTemplate<T>::addInstance( T *ptr )
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

    template <class T>
    void FactoryTemplate<T>::removeInstance( T *ptr )
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

}  // end namespace fb

#endif  // FactoryTemplate_h__
