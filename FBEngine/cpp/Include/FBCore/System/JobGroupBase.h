#ifndef JobGroupBase_h__
#define JobGroupBase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IJobGroup.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    
    class JobGroupBase : public SharedObject<IJobGroup>
    {
    public:
        JobGroupBase();
        ~JobGroupBase() override;

        void update() override;

        virtual bool getInterupt() const;
        virtual void setInterupt( bool interupt );

        JobState getState() const override;
        void setState( JobState state ) override;

        u32 getProgress() const override;
        void setProgress( u32 val ) override;

        s32 getPriority() const;
        void setPriority( s32 val ) override;

        bool isPrimary() const override;
        void setPrimary( bool val ) override;

        bool isFinished() const override;
        void setFinished( bool finished ) override;

        bool wait() override;
        bool wait( f64 maxWaitTime ) override;

        s32 getAffinity() const override;
        void setAffinity( s32 val ) override;

        void execute() override;

        void coroutine_execute() override;
        void coroutine_execute_step( SmartPtr<IObjectYield> &rYield ) override;

        bool isCoroutine() const override;
        void setCoroutine( bool val ) override;

    protected:
        Atomic<JobState> m_state = JobState::Ready;
        atomic_u32 m_progress = 0;
        atomic_u32 m_priority = 0;
        atomic_bool m_isPrimary = false;
        atomic_bool m_isFinished = false;
    };
}  // end namespace fb

#endif  // JobGroupBase_h__
