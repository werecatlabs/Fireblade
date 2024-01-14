#include <FBCore/FBCorePCH.h>
#include <FBCore/System/WorkerThread.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ISystemSettings.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/System/TimerChrono.h>
#include <thread>

#include "FBCore/Memory/PointerUtil.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, WorkerThread, IWorkerThread );

    s32 WorkerThread::m_threadIdExt = static_cast<s32>( Thread::ThreadId::WorkerThread );

    WorkerThread::WorkerThread()
    {
        m_thread = nullptr;
    }

    WorkerThread::~WorkerThread()
    {
        unload( nullptr );
    }

    void WorkerThread::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

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
            if( isLoaded() )
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
            FB_ASSERT( isAlive() );

            auto pApplicationManager = core::ApplicationManager::instance();
            auto applicationManager = pApplicationManager.get();
            FB_ASSERT( applicationManager );

            auto pTimer = fb::make_ptr<TimerChrono>();
            auto timer = pTimer.get();
            FB_ASSERT( timer );

            while( applicationManager->isRunning() && !applicationManager->getQuit() )
            {
                switch( auto state = getState() )
                {
                case State::Start:
                {
                    auto pJobQueue = applicationManager->getJobQueue();
                    auto jobQueue = pJobQueue.get();
                    FB_ASSERT( jobQueue );

                    auto pTaskManager = applicationManager->getTaskManager();
                    auto taskManager = pTaskManager.get();
                    FB_ASSERT( taskManager );

                    Thread::setCurrentThreadId( m_workerThreadId );

                    while( applicationManager->isRunning() && !applicationManager->getQuit() &&
                           getState() != State::Stop )
                    {
                        setUpdating( true );

                        auto fps = getTargetFPS();
                        if( fps > 0.0 )
                        {
                            auto start = timer->now();

                            timer->update();

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
                    }
                }
                break;
                default:
                {
                    Thread::sleep( 1.0 );
                }
                break;
                }
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

            while( isUpdating() && updatingCount < maxCount )
            {
                Thread::yield();
                updatingCount++;
            }

            stopCount++;
        }
    }

    auto WorkerThread::isUpdating() const -> bool
    {
        return m_isUpdating;
    }

    void WorkerThread::setUpdating( bool updating )
    {
        m_isUpdating = updating;
    }

    auto WorkerThread::getThreadId() const -> Thread::ThreadId
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
    }

    auto WorkerThread::getThread() const -> std::thread *
    {
        return m_thread;
    }

    void WorkerThread::setThread( std::thread *thread )
    {
        m_thread = thread;
    }

    auto WorkerThread::getTargetFPS() const -> time_interval
    {
        FB_ASSERT( !Math<time_interval>::equals( m_targetFPS, 0.0 ) );
        FB_ASSERT( Math<time_interval>::isFinite( m_targetFPS ) );
        return m_targetFPS;
    }

    auto WorkerThread::getState() const -> IWorkerThread::State
    {
        return m_state;
    }

}  // end namespace fb
