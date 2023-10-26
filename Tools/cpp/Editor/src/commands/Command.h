#ifndef Command_h__
#define Command_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    namespace editor
    {
        class Command : public ICommand
        {
        public:
            Command() = default;
            ~Command() override = default;

            void undo() override;

            void redo() override;

            void execute() override;

            State getState() const override;

            void setState( State state ) override;

        protected:
            mutable RecursiveMutex m_mutex;
            State m_commandState = State::Allocated;
        };
    }  // namespace editor
}  // namespace fb

#endif  // Command_h__
