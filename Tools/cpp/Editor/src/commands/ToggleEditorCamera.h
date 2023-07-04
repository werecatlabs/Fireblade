#ifndef ToggleEditorCamera_h__
#define ToggleEditorCamera_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {
        class ToggleEditorCamera : public Command
        {
        public:
            ToggleEditorCamera();
            ~ToggleEditorCamera() override;

            void undo() override;
            void redo() override;
            void execute() override;
        };

    }  // namespace editor

}  // namespace fb

#endif  // ToggleEditorCamera_h__
