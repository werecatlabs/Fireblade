#ifndef FlagSetJob_h__
#define FlagSetJob_h__

#include <FBCore/System/Job.h>

namespace fb
{

    class FlagSetJob : public Job
    {
    public:
        FlagSetJob();
        ~FlagSetJob() override;

        void execute() override;

        void setFlag( u32 flag );
        u32 getFlag() const;

    protected:
        u32 m_flag = 0;
    };

}  // namespace fb

#endif  // FlagSetJob_h__
