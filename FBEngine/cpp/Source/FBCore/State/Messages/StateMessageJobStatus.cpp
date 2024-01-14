#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageJobStatus.h"
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageJobStatus, StateMessage );

    //-------------------------------------------------
    StateMessageJobStatus::StateMessageJobStatus() : m_jobStatus( 0 )
    {
    }

    //-------------------------------------------------
    StateMessageJobStatus::StateMessageJobStatus( u32 status ) : m_jobStatus( status )
    {
    }

    //-------------------------------------------------
    StateMessageJobStatus::~StateMessageJobStatus() = default;

    //-------------------------------------------------
    auto StateMessageJobStatus::getJob() const -> SmartPtr<IJob>
    {
        return m_job;
    }

    //-------------------------------------------------
    void StateMessageJobStatus::setJob( SmartPtr<IJob> job )
    {
        m_job = job;
    }

    //-------------------------------------------------
    auto StateMessageJobStatus::getJobStatus() const -> u32
    {
        return m_jobStatus;
    }

    //-------------------------------------------------
    void StateMessageJobStatus::setJobStatus( u32 val )
    {
        m_jobStatus = val;
    }
}  // end namespace fb
