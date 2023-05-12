#ifndef IJob_h__
#define IJob_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>

namespace fb
{

    /**
     * @brief Interface for a job class, providing support for managing job execution, listeners, and states.
     * Inherits from ISharedObject.
     */
    class IJob : public ISharedObject
    {
    public:
        /**
         * @brief Enum class representing the different states a job can be in.
         */
        enum class JobState
        {
            Ready,
            Queue,
            Executing,
            Finish,

            Count
        };

        /**
         * @brief Enum class representing the priority levels of a job.
         */
        enum class JobPriority
        {
            High,
            Medium,
            Low,

            Count
        };

        /**
         * @brief Destroy the IJob object.
         */
        ~IJob() override = default;

        /**
         * @brief Get the affinity mask for the job, determining on which cores it can execute.
         *
         * @return s32 The affinity mask.
         */
        virtual s32 getAffinity() const = 0;

        /**
         * @brief Set the affinity mask for the job, determining on which cores it can execute.
         *
         * @param affinity The desired affinity mask.
         */
        virtual void setAffinity( s32 affinity ) = 0;

        /**
         * @brief Execute the job. This function is used to execute from different threads.
         * The implementation is assumed to be thread-safe.
         */
        virtual void execute() = 0;

        /**
         * @brief Start the coroutine for the job.
         */
        virtual void coroutine_execute() = 0;

        /**
         * @brief Execute a single step of the coroutine.
         *
         * @param rYield A reference to the IObjectYield smart pointer for managing coroutine execution.
         */
        virtual void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) = 0;

        /**
         * @brief Check if the job has listeners.
         *
         * @return bool True if the job has listeners, false otherwise.
         */
        virtual bool hasListeners() const = 0;

        /**
         * @brief Get the job listeners as an array.
         *
         * @return Array<SmartPtr<IJobListener>> An array of the job listeners.
         */
        virtual Array<SmartPtr<IJobListener>> getListeners() const = 0;

        /**
         * @brief Set the job listeners.
         *
         * @param listeners An array of the job listeners.
         */
        virtual void setListeners( const Array<SmartPtr<IJobListener>> &listeners ) = 0;

        /**
         * @brief Add a listener to the job.
         *
         * @param listener A smart pointer to the IJobListener to be added.
         */
        virtual void addListener( SmartPtr<IJobListener> listener ) = 0;

        /**
         * @brief Get the current state of the job.
         *
         * @return JobState The current state of the job.
         */
        virtual JobState getState() const = 0;

        /**
         * @brief Set the state of the job.
         *
         * @param state The desired state for the job.
         */
        virtual void setState( JobState state ) = 0;

        /**
         * @brief Get the job progress.
         *
         * @return u32 The current progress of the job.
         */
        virtual u32 getProgress() const = 0;

        /**
         * @brief Set the job progress.
         *
         * @param progress The desired progress for the job.
         */
        virtual void setProgress( u32 progress ) = 0;

        /** Gets the job priority. */
        virtual s32 getPriority() const = 0;

        /** Sets the job priority. */
        virtual void setPriority( s32 priority ) = 0;

        /** Used to know if to run on the primary thread. */
        virtual bool isPrimary() const = 0;

        /** Used to know if to run on the primary thread. */
        virtual void setPrimary( bool primary ) = 0;

        /** To know if the job is finished. */
        virtual bool isFinished() const = 0;

        /** To know if the job is finished. */
        virtual void setFinished( bool finished ) = 0;

        /** Called to wait until the job is finished. */
        virtual bool wait() = 0;

        /** Called to wait until the job is finished. */
        virtual bool wait( f64 maxWaitTime ) = 0;

        /** Gets a value to know if this job should execute as a coroutine. */
        virtual bool isCoroutine() const = 0;

        /** Sets a value to know if this job should execute as a coroutine. */
        virtual void setCoroutine( bool coroutine ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IJob_h__
