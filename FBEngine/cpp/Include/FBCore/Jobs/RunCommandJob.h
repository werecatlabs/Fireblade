#ifndef RunCommandJob_h__
#define RunCommandJob_h__

#include <FBCore/System/Job.h>

namespace fb
{

    /** A job that is used to run a command. */
    class RunCommandJob : public Job
    {
    public:
        /** Constructor */
        RunCommandJob();

        /** Destructor */
        ~RunCommandJob() override;

        /** @copydoc Job::execute */
        void execute() override;

        /** Get the command to run. */
        SmartPtr<ICommand> getCommand() const;

        /** Set the command to run. */
        void setCommand( SmartPtr<ICommand> command );

    protected:
        // The command to run.
        SmartPtr<ICommand> m_command;
    };

}  // end namespace fb

#endif  // RunCommandJob_h__
