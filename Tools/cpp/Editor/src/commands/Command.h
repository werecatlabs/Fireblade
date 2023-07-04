#ifndef Command_h__
#define Command_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace editor
    {
        class Command : public SharedObject<ICommand>
        {
        public:
            Command();
            ~Command() override;

            void undo() override;

            void redo() override;

            void execute() override;

            State getState() const override;

            void setState( State state ) override;

        protected:
            State m_commandState = State::Allocated;
        };
    } // namespace editor
}     // namespace fb

#endif  // Command_h__
