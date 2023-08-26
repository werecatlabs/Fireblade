#ifndef RunCommandJob_h__
#define RunCommandJob_h__

#include <FBCore/System/Job.h>

namespace fb
{
    class RunCommandJob : public Job
    {
    public:
        RunCommandJob();
        ~RunCommandJob() override;

        void execute() override;

        SmartPtr<ICommand> getCommand() const;
        void setCommand( SmartPtr<ICommand> command );

    protected:
        AtomicSmartPtr<ICommand> m_command;
    };
}  // end namespace fb

#endif  // RunCommandJob_h__
