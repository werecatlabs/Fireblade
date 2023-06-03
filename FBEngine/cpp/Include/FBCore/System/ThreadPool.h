#ifndef ThreadPoolStandard_h__
#define ThreadPoolStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Core/FSMListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/System/WorkerThread.h>

namespace fb
{

    /** Standard thread pool implementation. */
    class ThreadPool : public SharedObject<IThreadPool>
    {
    public:
        /** Constructor */
        ThreadPool();

        /** Destructor */
        ~ThreadPool() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ThreadPool::addWorkerThread */
        SmartPtr<IWorkerThread> addWorkerThread() override;

        /** @copydoc ThreadPool::getThread */
        SmartPtr<IWorkerThread> getThread( u32 index ) override;

        /** @copydoc ThreadPool::getNumThreads */
        u32 getNumThreads() const override;

        /** @copydoc ThreadPool::setNumThreads */
        void setNumThreads( u32 numThreads ) override;

        /** @copydoc ThreadPool::getState */
        State getState() const override;

        /** @copydoc ThreadPool::setState */
        void setState( State state ) override;

        /** @copydoc ThreadPool::stop */
        void stop();

        bool isValid() const override;

        FB_CLASS_REGISTER_DECL;

    private:
        class ThreadPoolFSMListener : public FSMListener
        {
        public:
            ThreadPoolFSMListener() = default;
            ~ThreadPoolFSMListener() override = default;

            IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

            ThreadPool *getOwner() const;
            void setOwner( ThreadPool *owner );

        private:
            ThreadPool *m_owner = nullptr;
        };

        SmartPtr<IFSM> getFSM() const;
        void setFSM( SmartPtr<IFSM> fsm );

        SmartPtr<IFSMListener> getFSMListener() const;
        void setFSMListener( SmartPtr<IFSMListener> listener );

        IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType );

        AtomicSmartPtr<IFSM> m_fsm;
        AtomicSmartPtr<IFSMListener> m_fsmListener;

        atomic_u32 m_numThreads = 0;

        Array<SmartPtr<IWorkerThread>> m_workerThreads;
        Array<IWorkerThread::State> m_states;
        Array<std::thread *> m_threads;
        Array<f64> m_targetFPS;
        Array<Thread::ThreadId> m_threadId;
        Array<u32> m_queueLengthMilliseconds;
        Array<u32> m_reserveFlags;

        mutable RecursiveMutex m_mutex;  
    };
}  // end namespace fb

#endif  // ThreadPoolStandard_h__
