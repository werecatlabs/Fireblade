#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobQueue.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    JobQueue::JobQueue() = default;

    JobQueue::~JobQueue() = default;

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
                    job->setState( IJob::State::Executing );
                    job->execute();
                    job->setState( IJob::State::Finish );
                }
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto threadPool = applicationManager->getThreadPool() )
            {
                if( threadPool->getNumThreads() == 0 )
                {
                    if( m_jobs.try_pop( job ) )
                    {
                        if( job )
                        {
                            job->setState( IJob::State::Executing );
                            job->execute();
                            job->setState( IJob::State::Finish );
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
                        job->setState( IJob::State::Executing );
                        job->execute();
                        job->setState( IJob::State::Finish );
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
                    job->setState( IJob::State::Executing );
                    job->execute();
                    job->setState( IJob::State::Finish );
                }
            }
        }
        };

        Thread::setCurrentTask( currentTask );
    }

    void JobQueue::addJob( SmartPtr<IJob> job )
    {
        FB_ASSERT( job->getState() != IJob::State::Queue );
        job->setState( IJob::State::Queue );

        if( job->isPrimary() )
        {
            m_primaryJobs.push( job );
        }
        else
        {
            m_jobs.push( job );
        }
    }

    void JobQueue::addJob( SmartPtr<IJob> job, Thread::Task task )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto pTask = taskManager->getTask( task );
        if( pTask )
        {
            pTask->addJob( job );
        }
    }

    void JobQueue::addJobAllTasks( SmartPtr<IJob> job )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto taskManager = applicationManager->getTaskManager() )
        {
            for( size_t i = 0; i < static_cast<size_t>( Thread::Task::Count ); ++i )
            {
                auto task = static_cast<Thread::Task>( i );
                auto pTask = taskManager->getTask( task );
                if( pTask )
                {
                    pTask->addJob( job );
                }
            }
        }
    }

    auto JobQueue::isRunning() const -> bool
    {
        return m_isRunning;
    }

    void JobQueue::setRunning( bool running )
    {
        m_isRunning = running;
    }

    auto JobQueue::getRate() const -> f32
    {
        return m_rate;
    }

    void JobQueue::setRate( f32 rate )
    {
        m_rate = rate;
    }

    auto JobQueue::getUseAffinity() const -> bool
    {
        return m_useAffinity;
    }

    void JobQueue::setUseAffinity( bool affinity )
    {
        m_useAffinity = affinity;
    }

    auto JobQueue::isProcessing( s32 id ) const -> bool
    {
        return m_isProcessing;
    }
}  // end namespace fb
