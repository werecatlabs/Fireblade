#ifndef RunCommandJob_h__
#define RunCommandJob_h__

#include <FBCore/FBCorePrerequisites.h>
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
        SmartPtr<ICommand> m_command;
    };
}  // end namespace fb

#endif  // RunCommandJob_h__
