#include <FBCore/FBCorePCH.h>
#include <FBCore/System/RunCommandJob.h>
#include <FBCore/FBCore.h>

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
        if( m_command )
        {
            m_command->execute();
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
