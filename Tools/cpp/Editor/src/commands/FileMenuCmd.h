#ifndef FileMenuCmd_h__
#define FileMenuCmd_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/System/ICommand.h>
#include "ui/UIManager.h"
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {
        class FileMenuCmd : public Command
        {
        public:
            FileMenuCmd();
            ~FileMenuCmd() override;

            void undo() override;
            void redo() override;
            void execute() override;

            UIManager::WidgetId getItemId() const;
            void setItemId( UIManager::WidgetId val );

        protected:
            UIManager::WidgetId m_itemId = UIManager::WidgetId::None;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AddEntityCmd_h__
