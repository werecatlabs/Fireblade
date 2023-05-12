#include <FBCore/FBCorePCH.h>
#include <FBCore/System/WorkerThread.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ISystemManager.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/System/TimerChrono.h>
#include <thread>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, WorkerThread, CSharedObject<IWorkerThread> );

    s32 WorkerThread::m_threadIdExt = static_cast<s32>( Thread::ThreadId::WorkerThread );

    WorkerThread::WorkerThread()
    {
        m_thread = nullptr;
        //setGarbageCollected( false );
    }

    WorkerThread::~WorkerThread()
    {
        unload( nullptr );
    }

    void WorkerThread::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloaded );

            m_workerThreadId = static_cast<Thread::ThreadId>( m_threadIdExt++ );
            m_thread = new std::thread( &WorkerThread::run, this );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void WorkerThread::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( auto thread = getThread() )
                {
                    if( thread->joinable() )
                    {
                        thread->join();
                    }

                    delete thread;
                    setThread( nullptr );
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void WorkerThread::run()
    {
        try
        {
            auto pApplicationManager = core::IApplicationManager::instance();
            auto applicationManager = pApplicationManager.get();
            FB_ASSERT( applicationManager );

            auto state = getState();
            while( applicationManager->isRunning() )
            {
                switch( state )
                {
                case State::Start:
                {
                    auto pJobQueue = applicationManager->getJobQueue();
                    auto jobQueue = pJobQueue.get();
                    FB_ASSERT( jobQueue );

                    auto pTimer = fb::make_ptr<TimerChrono>();
                    auto timer = pTimer.get();
                    FB_ASSERT( timer );

                    auto pTaskManager = applicationManager->getTaskManager();
                    auto taskManager = pTaskManager.get();
                    FB_ASSERT( taskManager );

                    Thread::setCurrentThreadId( m_workerThreadId );

                    while( applicationManager->isRunning() && state != State::Stop )
                    {
                        setUpdating( true );

                        auto fps = getTargetFPS();
                        if( fps > 0.0 )
                        {
                            auto start = timer->now();

                            timer->update();
                            auto dt = timer->getDeltaTime();

                            jobQueue->update();
                            taskManager->update();

                            auto end = timer->now();
                            auto timeTaken = end - start;

                            auto fRate = 1.0 / fps;
                            auto sleepTime = fRate - timeTaken;
                            Thread::sleep( sleepTime );
                        }
                        else
                        {
                            Thread::sleep( 1.0 );
                        }

                        setUpdating( false );

                        state = getState();
                    }
                }
                break;
                default:
                {
                    Thread::sleep( 1.0 );
                }
                break;
                }

                state = getState();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void WorkerThread::setState( State state )
    {
        m_state = state;
    }

    void WorkerThread::stop()
    {
        auto stopCount = 0;
        auto updatingCount = 0;
        auto maxCount = 100;

        while( getState() != State::Stop && stopCount < maxCount )
        {
            setState( State::Stop );

            while( isUpdating() && updatingCount < maxCount)
            {
                Thread::yield();
                updatingCount++;
            }

            stopCount++;
        }
    }

    bool WorkerThread::isUpdating() const
    {
        return m_isUpdating;
    }

    void WorkerThread::setUpdating( bool updating )
    {
        m_isUpdating = updating;
    }

    Thread::ThreadId WorkerThread::getThreadId() const
    {
        return m_workerThreadId;
    }

    void WorkerThread::setThreadId( Thread::ThreadId threadId )
    {
        m_workerThreadId = threadId;
    }

    void WorkerThread::setTargetFPS( time_interval framesPerSecond )
    {
        m_targetFPS = framesPerSecond;

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pThreadPool = applicationManager->getThreadPool();
        auto threadPool = pThreadPool.get();
        FB_ASSERT( threadPool );
    }

    std::thread *WorkerThread::getThread() const
    {
        return m_thread;
    }

    void WorkerThread::setThread( std::thread *thread )
    {
        m_thread = thread;
    }

}  // end namespace fb
