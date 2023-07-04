#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobYield.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace core
    {
        JobYield::JobYield( SmartPtr<IJob> job ) : m_job( job )
        {
        }

        JobYield::JobYield( SmartPtr<IObjectYield> &jobYield )

        {
        }

        void JobYield::stop()
        {
            m_job->setState( Job::JobState::Finish );
        }
    }  // namespace core
}  // namespace fb
