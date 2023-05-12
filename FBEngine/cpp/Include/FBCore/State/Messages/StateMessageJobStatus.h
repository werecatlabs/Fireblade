#ifndef StateMessageJobStatus_h__
#define StateMessageJobStatus_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //-------------------------------------------------
    class StateMessageJobStatus : public StateMessage
    {
    public:
        enum
        {
            JOB_STATUS_QUEUED,
            JOB_STATUS_START,
            JOB_STATUS_END,
            JOB_STATUS_DESTROY,

            JOB_STATUS_COUNT
        };

        StateMessageJobStatus();
        StateMessageJobStatus( u32 status );
        ~StateMessageJobStatus() override;

        SmartPtr<IJob> getJob() const;
        void setJob( SmartPtr<IJob> job );

        u32 getJobStatus() const;
        void setJobStatus( u32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<IJob> m_job;
        u32 m_jobStatus;
    };
}  // end namespace fb

#endif  // StateMessageJobStatus_h__
