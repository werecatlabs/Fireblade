#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobQueue.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/IJobQueueListener.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    JobQueue::JobQueue()
    {
    }

    JobQueue::~JobQueue()
    {
    }

    void JobQueue::update()
    {
        auto currentTask = Thread::getCurrentTask();
        Thread::setCurrentTask( Thread::Task::None );

        switch( auto threadId = Thread::getCurrentThreadId() )
        {
        case Thread::ThreadId::Primary:
        {
            SmartPtr<IJob> job;
            if( m_primaryJobs.try_pop( job ) )
            {
                if( job )
                {
                    job->setState( IJob::JobState::Executing );
                    job->execute();
                    job->setState( IJob::JobState::Finish );
                }
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto threadPool = applicationManager->getThreadPool() )
            {
                if( threadPool->getNumThreads() == 0 )
                {
                    if( m_jobs.try_pop( job ) )
                    {
                        if( job )
                        {
                            job->setState( IJob::JobState::Executing );
                            job->execute();
                            job->setState( IJob::JobState::Finish );
                        }
                    }
                }
            }
            else
            {
                if( m_jobs.try_pop( job ) )
                {
                    if( job )
                    {
                        job->setState( IJob::JobState::Executing );
                        job->execute();
                        job->setState( IJob::JobState::Finish );
                    }
                }
            }
        }
        break;
        default:
        {
            SmartPtr<IJob> job;
            if( m_jobs.try_pop( job ) )
            {
                if( job )
                {
                    job->setState( IJob::JobState::Executing );
                    job->execute();
                    job->setState( IJob::JobState::Finish );
                }
            }
        }
        };

        Thread::setCurrentTask( currentTask );
    }

    void JobQueue::queueJob( SmartPtr<IJob> job )
    {
        if( job->isPrimary() )
        {
            m_primaryJobs.push( job );
        }
        else
        {
            m_jobs.push( job );
        }
    }

    void JobQueue::queueJob( SmartPtr<IJob> job, Thread::Task task )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto pTask = taskManager->getTask( task );
        if( pTask )
        {
            pTask->queueJob( job );
        }
    }

    void JobQueue::queueJobAll( SmartPtr<IJob> job )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto taskManager = applicationManager->getTaskManager() )
        {
            for( size_t i = 0; i < static_cast<size_t>( Thread::Task::Count ); ++i )
            {
                auto task = static_cast<Thread::Task>( i );
                auto pTask = taskManager->getTask( task );
                if( pTask )
                {
                    pTask->queueJob( job );
                }
            }
        }
    }

    bool JobQueue::isRunning() const
    {
        return m_isRunning;
    }

    void JobQueue::setRunning( bool running )
    {
        m_isRunning = running;
    }

    f32 JobQueue::getRate() const
    {
        return m_rate;
    }

    void JobQueue::setRate( f32 rate )
    {
        m_rate = rate;
    }

    void JobQueue::addListener( SmartPtr<IJobQueueListener> listener )
    {
        m_listeners.push_back( listener );
    }

    void JobQueue::removeListener( SmartPtr<IJobQueueListener> listener )
    {
        auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
        if( it != m_listeners.end() )
        {
            m_listeners.erase( it );
        }
    }

    bool JobQueue::getUseAffinity() const
    {
        return m_useAffinity;
    }

    void JobQueue::setUseAffinity( bool affinity )
    {
        m_useAffinity = affinity;
    }

    bool JobQueue::isProcessing( s32 id ) const
    {
        return m_isProcessing;
    }
}  // end namespace fb
