#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobYield.h>
#include <FBCore/System/Job.h>
#include <utility>

namespace fb::core
{
    JobYield::JobYield( SmartPtr<IJob> job ) : m_job( std::move( job ) )
    {
    }

    JobYield::JobYield( SmartPtr<IObjectYield> &jobYield )
    {
    }

    void JobYield::stop()
    {
        m_job->setState( Job::State::Finish );
    }
}  // namespace fb::core
