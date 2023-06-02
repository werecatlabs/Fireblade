#ifndef _WorkerThread_H_
#define _WorkerThread_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IWorkerThread.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Atomics/AtomicFloat.h>

#if FB_ENABLE_ASSERTS
#    include <FBCore/Math/Math.h>
#endif

namespace fb
{
    
    class WorkerThread : public CSharedObject<IWorkerThread>
    {
    public:
        /** Constructor */
        WorkerThread();

        /** Copy constructor */
        WorkerThread( const WorkerThread &other ) = delete;

        /** Destructor */
        ~WorkerThread() override;

        /** @copydoc IWorkerThread::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IWorkerThread::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IWorkerThread::run */
        void run() override;

        /** @copydoc IWorkerThread::getThreadId */
        Thread::ThreadId getThreadId() const override;

        /** @copydoc IWorkerThread::setThreadId */
        void setThreadId( Thread::ThreadId threadId ) override;

        /** @copydoc IWorkerThread::getTargetFPS */
        time_interval getTargetFPS() const override;

        /** @copydoc IWorkerThread::setTargetFPS */
        void setTargetFPS( time_interval framesPerSecond ) override;

        /** @copydoc IWorkerThread::getState */
        State getState() const override;

        /** @copydoc IWorkerThread::setState */
        void setState( State state ) override;

        /** @copydoc IWorkerThread::stop */
        void stop() override;

        /** @copydoc IWorkerThread::isUpdating */
        bool isUpdating() const override;

        /** @copydoc IWorkerThread::setUpdating */
        void setUpdating( bool updating ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        std::thread *getThread() const;

        void setThread( std::thread *thread );

        Atomic<std::thread *> m_thread;

        atomic_f64 m_targetFPS = 5000.0;
        Atomic<Thread::ThreadId> m_workerThreadId = Thread::ThreadId::Primary;
        Atomic<State> m_state = State::None;
        atomic_bool m_isUpdating = false;

        static s32 m_threadIdExt;
    };

    inline time_interval WorkerThread::getTargetFPS() const
    {
        FB_ASSERT( !Math<time_interval>::equals( m_targetFPS, 0.0 ) );
        FB_ASSERT( Math<time_interval>::isFinite( m_targetFPS ) );
        return m_targetFPS;
    }

    inline IWorkerThread::State WorkerThread::getState() const
    {
        return m_state;
    }

} // namespace fb

#endif
