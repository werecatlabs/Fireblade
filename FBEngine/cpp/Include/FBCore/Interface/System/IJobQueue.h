#ifndef IJobQueue_h__
#define IJobQueue_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    /** Interface for a job queue class. */
    class IJobQueue : public ISharedObject
    {
    public:
        /** Virtual destructor.
         */
        ~IJobQueue() override = default;

        /** Adds a job for processing.
        @param job The job to be queued.
        */
        virtual void addJob( SmartPtr<IJob> job ) = 0;

        /** Adds a job for processing.
        @param job The job to be queued.
        @param task The task that should process the job.
        */
        virtual void addJob( SmartPtr<IJob> job, Thread::Task task ) = 0;

        /** Adds a job for processing.
        @param job The job to be queued.
        */
        virtual void addJobAllTasks( SmartPtr<IJob> job ) = 0;

        /** Gets whether the job queue is running.
        @return A boolean to indicate whether the job queue is running.
        */
        virtual bool isRunning() const = 0;

        /** Sets whether the job queue is running.
        @param running A boolean to indicate whether the job queue is running.
        */
        virtual void setRunning( bool running ) = 0;

        /** Gets the update rate of the worker threads.
        @returns
            The rate of the update.
        */
        virtual f32 getRate() const = 0;

        /** Sets the update rate of the worker threads.
        @param rate The rate of the update.
        */
        virtual void setRate( f32 rate ) = 0;

        /** Gets if thread affinity should be used. */
        virtual bool getUseAffinity() const = 0;

        /** Sets if thread affinity should be used.
        @param affinity A boolean to know if the affinity should be used.
        */
        virtual void setUseAffinity( bool affinity ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IJobQueue_h__
