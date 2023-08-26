#include <FBCore/FBCorePCH.h>
#include <FBCore/System/RunCommandJob.h>
#include <FBCore/Interface/System/ICommand.h>

namespace fb
{
    RunCommandJob::RunCommandJob()
    {
    }

    RunCommandJob::~RunCommandJob()
    {
    }

    void RunCommandJob::execute()
    {
        if( auto command = getCommand() )
        {
            command->execute();
        }
    }

    SmartPtr<ICommand> RunCommandJob::getCommand() const
    {
        return m_command;
    }

    void RunCommandJob::setCommand( SmartPtr<ICommand> command )
    {
        m_command = command;
    }
}  // end namespace fb
