#ifndef InstancePool_h__
#define InstancePool_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Thread/Threading.h>

namespace fb
{
    /** A pool class to manage object instances. */
    template <class T>
    class InstancePool : public CSharedObject<ISharedObject>
    {
    public:
        /** Class to handle object destruction. */
        class Listener : public ISharedObjectListener
        {
        public:
            Listener() = default;
            Listener( InstancePool *owner );
            Listener( const Listener &other );
            ~Listener() override;

            void loadingStateChanged(ISharedObject* sharedObject, LoadingState oldState, LoadingState newState) override
            {
                
            }

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
        InstancePool( u32 numElements );
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

        FB_MUTEX_MUTABLE( m_mutex );
    };
}  // end namespace fb

#include <FBCore/Base/InstancePool.inl>

#endif  // InstancePool_h__
