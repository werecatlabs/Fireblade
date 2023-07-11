#ifndef ModifyPropertyCmd_h__
#define ModifyPropertyCmd_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class ModifyPropertyCmd : public Command
        {
        public:
            ModifyPropertyCmd();
            ~ModifyPropertyCmd() override;

            void undo() override;
            void redo() override;
            void execute() override;


        };

    }  // namespace editor
}  // namespace fb

#endif  // ModifyPropertyCmd_h__
