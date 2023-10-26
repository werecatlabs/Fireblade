#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobQueueTBB.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Core/LogManager.h>

#if FB_USE_TBB

#    include <tbb/parallel_for_each.h>
#    include <tbb/task_scheduler_init.h>

namespace fb
{

    JobQueueTBB::JobQueueTBB() : m_updateRate( 1.0f / 500.0f )
    {
        u32 hardwareConcurrency = Thread::hardware_concurrency();
        for( size_t i = 0; i < hardwareConcurrency; ++i )
            m_threads.push_back( new boost::thread( WorkerThread( this ) ) );
    }

    JobQueueTBB::~JobQueueTBB()
    {
        for( u32 i = 0; i < m_threads.size(); ++i )
        {
            boost::thread *thread = m_threads[i];
            thread->join();
            FB_SAFE_DELETE( thread );
        }
    }

    void JobQueueTBB::update()
    {
        auto thread = Thread::getCurrentThreadId();
        switch( thread )
        {
        case Thread::ThreadId::Primary:
        {
            if( !m_primaryJobQueue.empty() )
            {
                SmartPtr<IJob> job;
                while( m_primaryJobQueue.try_pop( job ) )
                {
                    if( job )
                    {
                        job->execute();
                    }
                }
            }
        }
        break;
        default:
        {
            try
            {
                if( !m_jobQueue.empty() )
                {
                    SmartPtr<IJob> job;
                    while( m_jobQueue.try_pop( job ) )
                    {
                        job->setState( IJob::State::Executing );
                        job->execute();
                        job->setState( IJob::State::Finish );

                        Thread::yield();
                    }
                }
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        }
    }

    void JobQueueTBB::addJob( SmartPtr<IJob> job )
    {
        job->setState( IJob::State::Queue );

        if( !job->isPrimary() )
        {
            m_jobQueue.push( job );
        }
        else
        {
            m_primaryJobQueue.push( job );
        }

        auto size = m_jobQueue.unsafe_size();
        size = size;

        if( size > 100 )
        {
            size = size;
        }
    }

    void JobQueueTBB::addJob( SmartPtr<IJob> job, Thread::Task task )
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

    void JobQueueTBB::addJobAllTasks( SmartPtr<IJob> job )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

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

    bool JobQueueTBB::isRunning() const
    {
        return false;
    }

    void JobQueueTBB::setRunning( bool val )
    {
    }

    f32 JobQueueTBB::getRate() const
    {
        return 0.0f;
    }

    void JobQueueTBB::setRate( f32 val )
    {
    }

    bool JobQueueTBB::getUseAffinity() const
    {
        return false;
    }

    void JobQueueTBB::setUseAffinity( bool val )
    {
    }

    bool JobQueueTBB::isProcessing( s32 id ) const
    {
        return false;
    }

    JobQueueTBB::WorkerThread::WorkerThread( JobQueueTBB *jobQueue ) : m_jobQueue( jobQueue )
    {
    }

    void JobQueueTBB::WorkerThread::operator()()
    {
    }
}  // end namespace fb

#endif
