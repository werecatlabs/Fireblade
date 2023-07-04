#ifndef _REMOVE_ENTITY_BODY_CMD_H
#define _REMOVE_ENTITY_BODY_CMD_H

#include <GameEditorPrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Core/Properties.h>
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class RemoveResourceCmd : public Command
        {
        public:
            RemoveResourceCmd();
            ~RemoveResourceCmd();

            virtual void undo();
            virtual void redo();
            virtual void execute();

            String getFilePath() const;

            void setFilePath( const String &filePath );

        private:
            String m_filePath;
        };

    }  // namespace editor
}  // namespace fb

#endif  // _REMOVE_ENTITY_BODY_CMD_H
