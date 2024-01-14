#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/FlagSetJob.h>

namespace fb
{

    FlagSetJob::FlagSetJob() = default;

    FlagSetJob::~FlagSetJob() = default;

    void FlagSetJob::execute()
    {
    }

    void FlagSetJob::setFlag( u32 flag )
    {
        m_flag = flag;
    }

    auto FlagSetJob::getFlag() const -> u32
    {
        return m_flag;
    }

}  // namespace fb
