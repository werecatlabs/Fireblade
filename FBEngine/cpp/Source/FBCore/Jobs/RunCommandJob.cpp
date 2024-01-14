#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/RunCommandJob.h>
#include <FBCore/Interface/System/ICommand.h>

namespace fb
{
    RunCommandJob::RunCommandJob() = default;

    RunCommandJob::~RunCommandJob() = default;

    void RunCommandJob::execute()
    {
        if( auto command = getCommand() )
        {
            command->execute();
        }
    }

    auto RunCommandJob::getCommand() const -> SmartPtr<ICommand>
    {
        return m_command;
    }

    void RunCommandJob::setCommand( SmartPtr<ICommand> command )
    {
        m_command = command;
    }
}  // end namespace fb
