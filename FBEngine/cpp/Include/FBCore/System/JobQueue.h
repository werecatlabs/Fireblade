#ifndef JobQueueStandard_h__
#define JobQueueStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Base/ConcurrentQueue.h>

namespace fb
{
    
    class JobQueue : public CSharedObject<IJobQueue>
    {
    public:
        JobQueue();
        ~JobQueue() override;

        void update() override;

        void queueJob( SmartPtr<IJob> job ) override;

        void queueJob( SmartPtr<IJob> job, Thread::Task task ) override;

        void queueJobAll( SmartPtr<IJob> job ) override;

        bool isRunning() const override;

        void setRunning( bool running ) override;

        f32 getRate() const override;

        void setRate( f32 rate ) override;

        void addListener( SmartPtr<IJobQueueListener> listener ) override;

        void removeListener( SmartPtr<IJobQueueListener> listener ) override;

        bool getUseAffinity() const override;

        void setUseAffinity( bool affinity ) override;

        bool isProcessing( s32 id ) const;

    protected:
        ConcurrentQueue<SmartPtr<IJob>> m_primaryJobs;
        ConcurrentQueue<SmartPtr<IJob>> m_jobs;
        Array<SmartPtr<IJobQueueListener>> m_listeners;
        atomic_f32 m_rate = 1.0f / 15.0f;
        atomic_bool m_useAffinity = false;
        atomic_bool m_isProcessing = false;
        atomic_bool m_isRunning = false;
    };
}  // end namespace fb

#endif  // JobQueueStandard_h__
