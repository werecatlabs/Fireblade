#ifndef InstancePool_h__
#define InstancePool_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Thread/Threading.h>

namespace fb
{

    /** A pool class to manage object instances. */
    template <class T>
    class InstancePool : public ISharedObject
    {
    public:
        /** Class to handle object destruction. */
        class Listener : public ISharedObjectListener
        {
        public:
            Listener() = default;
            explicit Listener( InstancePool *owner );
            Listener( const Listener &other );
            ~Listener() override;

            bool destroy( void *ptr ) override;

            InstancePool *getOwner() const
            {
                return m_owner;
            }

            void setOwner( InstancePool *owner )
            {
                m_owner = owner;
            }

            RawPtr<InstancePool> m_owner;
        };

        InstancePool();
        explicit InstancePool( u32 numElements );
        ~InstancePool() override;

        void allocate_data();

        /** Gets an object. */
        SmartPtr<T> getInstance();

        /** Frees an object. */
        void freeInstance( SmartPtr<T> instance );

        /** Frees all the objects. */
        void freeAll();

        s32 getGrowSize() const;
        void setGrowSize( s32 val );

        s32 getMaxSize() const;
        void setMaxSize( s32 val );

        size_t getNumFreeElements() const;

        size_t getNumElements() const;

        ConcurrentArray<RawPtr<T>> &getElements();

        const ConcurrentArray<RawPtr<T>> &getElements() const;

    protected:
        AtomicSharedPtr<ConcurrentArray<T *>> m_pool;
        AtomicSharedPtr<Listener> m_listener;

        ConcurrentArray<RawPtr<T>> m_elements;
        ConcurrentQueue<SmartPtr<T>> m_freeElements;

        atomic_s32 m_growSize = 0;
        atomic_s32 m_maxSize = 128;
        atomic_bool m_isShuttingDown = false;

        // for thread safety
        mutable RecursiveMutex m_mutex;
    };

    template <class T>
    InstancePool<T>::InstancePool()
    {
        auto listener = fb::make_shared<Listener>();
        listener->setOwner( this );
        m_listener = listener;

        m_pool = fb::make_shared<ConcurrentArray<T *>>();
    }

    template <class T>
    InstancePool<T>::InstancePool( u32 numElements ) :
        m_growSize( numElements ),
        m_maxSize( -1 ),
        m_isShuttingDown( false )
    {
        auto listener = fb::make_shared<Listener>();
        listener->setOwner( this );
        m_listener = listener;

        m_pool = fb::make_shared<ConcurrentArray<T *>>();
    }

    template <class T>
    InstancePool<T>::~InstancePool()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pPool = m_pool.load();
        auto &pool = *pPool;

        for( auto &p : pool )
        {
            delete[] p;
        }

        pool.clear();

        m_isShuttingDown = true;
        m_listener = nullptr;
        freeAll();
    }

    template <class T>
    void InstancePool<T>::allocate_data()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( m_growSize > 0 )
        {
            if( m_maxSize == -1 )
            {
                auto pool = new T[m_growSize];

                for( u32 i = 0; i < static_cast<u32>( m_growSize ); ++i )
                {
                    auto ptr = &pool[i];
                    auto newInstance = SmartPtr<T>( ptr );
                    if( newInstance )
                    {
                        auto listener = m_listener.load();
                        auto pListener = listener.get();

                        newInstance->setSharedObjectListener( pListener );

                        m_elements.push_back( newInstance.get() );
                        FB_ASSERT( m_elements.size() < 1e5 );

                        FB_ASSERT( newInstance );
                        m_freeElements.push( newInstance );
                    }
                }

                auto pPool = m_pool.load();
                pPool->push_back( pool );
            }
            else if( m_maxSize >= 0 )
            {
                if( m_elements.size() < m_maxSize )
                {
                    auto pool = new T[m_growSize];

                    for( u32 i = 0; i < static_cast<u32>( m_growSize ); ++i )
                    {
                        auto ptr = &pool[i];
                        auto newInstance = SmartPtr<T>( ptr );

                        auto listener = m_listener.load();
                        auto pListener = listener.get();

                        newInstance->setSharedObjectListener( pListener );

                        m_elements.push_back( newInstance.get() );

                        FB_ASSERT( newInstance );
                        m_freeElements.push( newInstance );
                    }

                    auto pPool = m_pool.load();
                    pPool->push_back( pool );
                }
            }
        }
    }

    template <class T>
    SmartPtr<T> InstancePool<T>::getInstance()
    {
        if( m_freeElements.empty() )
        {
            allocate_data();
        }

        if( !m_freeElements.empty() )
        {
            SmartPtr<T> instance;
            while( !m_freeElements.try_pop( instance ) )
            {
                FB_ASSERT( instance );
                return instance;
            }
        }

        return nullptr;
    }

    template <class T>
    void InstancePool<T>::freeInstance( SmartPtr<T> instance )
    {
        if( !m_isShuttingDown )
        {
            FB_ASSERT( instance );
            m_freeElements.push( instance );
        }
    }

    template <class T>
    s32 InstancePool<T>::getGrowSize() const
    {
        return m_growSize;
    }

    template <class T>
    void InstancePool<T>::setGrowSize( s32 val )
    {
        m_growSize = val;

        if( m_growSize > m_maxSize )
        {
            m_maxSize = m_growSize.load();
        }
    }

    template <class T>
    s32 InstancePool<T>::getMaxSize() const
    {
        return m_maxSize;
    }

    template <class T>
    void InstancePool<T>::setMaxSize( s32 val )
    {
        m_maxSize = val;
    }

    template <class T>
    size_t InstancePool<T>::getNumFreeElements() const
    {
        return static_cast<size_t>( m_freeElements.unsafe_size() );
    }

    template <class T>
    size_t InstancePool<T>::getNumElements() const
    {
        return static_cast<size_t>( m_elements.size() );
    }

    template <class T>
    ConcurrentArray<RawPtr<T>> &InstancePool<T>::getElements()
    {
        return m_elements;
    }

    template <class T>
    const ConcurrentArray<RawPtr<T>> &InstancePool<T>::getElements() const
    {
        return m_elements;
    }

    template <class T>
    void InstancePool<T>::freeAll()
    {
        SmartPtr<T> instance;
        while( m_freeElements.try_pop( instance ) )
        {
        }
    }

    template <class T>
    InstancePool<T>::Listener::Listener( InstancePool *owner ) : m_owner( owner )
    {
    }

    template <class T>
    InstancePool<T>::Listener::Listener( const Listener &other ) : m_owner( other.m_owner )
    {
    }

    template <class T>
    InstancePool<T>::Listener::~Listener()
    {
    }

    template <class T>
    bool InstancePool<T>::Listener::destroy( void *ptr )
    {
        if( !m_owner->m_isShuttingDown )
        {
            RawPtr<T> instancePtr = static_cast<T *>( ptr );
            instancePtr->addReference();

            SmartPtr<T> instance = static_cast<T *>( ptr );
            m_owner->freeInstance( instance );

            return true;
        }

        return false;
    }

}  // end namespace fb

#endif  // InstancePool_h__
