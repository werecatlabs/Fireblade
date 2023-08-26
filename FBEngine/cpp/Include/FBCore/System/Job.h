#ifndef __CJob_h__
#define __CJob_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Memory/ObjectYield.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{

    class Job : public IJob
    {
    public:
        Job() = default;
        ~Job() override = default;

        /** @copydoc IJob::getState */
        JobState getState() const override;

        /** @copydoc IJob::setState */
        void setState( JobState state ) override;

        /** @copydoc IJob::getProgress */
        u32 getProgress() const override;

        /** @copydoc IJob::setProgress */
        void setProgress( u32 progress ) override;

        /** @copydoc IJob::getPriority */
        s32 getPriority() const;

        /** @copydoc IJob::setPriority */
        void setPriority( s32 priority );

        /** @copydoc IJob::isPrimary */
        bool isPrimary() const override;

        /** @copydoc IJob::setPrimary */
        void setPrimary( bool primary ) override;

        /** @copydoc IJob::isFinished */
        bool isFinished() const override;

        /** @copydoc IJob::setFinished */
        void setFinished( bool finished ) override;

        /** @copydoc IJob::wait */
        bool wait() override;

        /** @copydoc IJob::wait */
        bool wait( f64 maxWaitTime ) override;

        /** @copydoc IJob::getAffinity */
        s32 getAffinity() const override;

        /** @copydoc IJob::setAffinity */
        void setAffinity( s32 affinity ) override;

        /** @copydoc IJob::execute */
        void execute() override;

        /** @copydoc IJob::coroutine_execute */
        void coroutine_execute() override;

        /** @copydoc IJob::coroutine_execute_step */
        void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

        /** @copydoc IJob::isCoroutine */
        bool isCoroutine() const override;

        /** @copydoc IJob::setCoroutine */
        void setCoroutine( bool coroutine ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        atomic_u32 m_affinity = 0;
        Atomic<JobState> m_state = JobState::Ready;
        atomic_u32 m_progress = 0;
        atomic_u32 m_priority = 0;
        atomic_bool m_isPrimary = false;
        atomic_bool m_isFinished = false;
        atomic_bool m_isCoroutine = false;

        SmartPtr<IObjectYield> m_yieldObject;
    };
}  // end namespace fb

#endif  // JobBase_h__
