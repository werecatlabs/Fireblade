#ifndef JobQueueTBB_h__
#define JobQueueTBB_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IJobQueue.h>


#if FB_USE_TBB

#    include <tbb/concurrent_queue.h>
#    include <boost/thread.hpp>

namespace fb
{
    
    class JobQueueTBB : public IJobQueue
    {
    public:
        JobQueueTBB();
        ~JobQueueTBB() override;

        void update() override;

        void addJob( SmartPtr<IJob> job ) override;

        void addJob( SmartPtr<IJob> job, Thread::Task task ) override;

        void addJobAllTasks( SmartPtr<IJob> job ) override;

        bool isRunning() const override;
        void setRunning( bool val ) override;

        f32 getRate() const override;
        void setRate( f32 val ) override;

        bool getUseAffinity() const override;
        void setUseAffinity( bool val ) override;
        bool isProcessing( s32 id ) const;

    protected:
        struct WorkerThread
        {
            WorkerThread( JobQueueTBB *jobQueue );

            void operator()();

            SmartPtr<JobQueueTBB> m_jobQueue;
        };

        /// Threads.
        Array<boost::thread *> m_threads;

        /// Jobs typedef.
        using Jobs = tbb::concurrent_queue<SmartPtr<IJob>>;

        /// Thread job queue.
        Jobs m_jobQueue;

        /// Primary thread job queue.
        Jobs m_primaryJobQueue;

        /// Thread update rate.
        f32 m_updateRate;
    };
}  // end namespace fb

#endif

#endif  // JobQueueTBB_h__
