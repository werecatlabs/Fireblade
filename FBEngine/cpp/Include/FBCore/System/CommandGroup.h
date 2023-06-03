#ifndef _CommandGroup_H
#define _CommandGroup_H

#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    
    // Executes group of commands as a single command
    class CommandGroup : public SharedObject<ICommand>
    {
    public:
        CommandGroup();
        ~CommandGroup() override;

        void undo() override;
        void redo() override;
        void execute() override;

    private:
    };

}  // end namespace fb

#endif
