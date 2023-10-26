#ifndef FileMenuCmd_h__
#define FileMenuCmd_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/ICommand.h>
#include "ui/UIManager.h"
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        /**
         * @brief Represents a command related to file menu operations.
         *
         * This class is derived from the base Command class and provides specific
         * implementations for undo, redo, and execute operations related to file menu
         * commands.
         */
        class FileMenuCmd : public Command
        {
        public:
            /**
             * @brief Default constructor for the FileMenuCmd class.
             */
            FileMenuCmd();

            /**
             * @brief Destructor for the FileMenuCmd class.
             */
            ~FileMenuCmd() override;

            /**
             * @brief Reverts the effect of the executed file menu command.
             *
             * This method is used to undo the action performed by the execute() method.
             */
            void undo() override;

            /**
             * @brief Reapplies the previously undone file menu command.
             *
             * This method is used to redo the action that was undone using the undo() method.
             */
            void redo() override;

            /**
             * @brief Executes the file menu command.
             *
             * This method performs the main action associated with the file menu command.
             */
            void execute() override;

            /**
             * @brief Gets the unique identifier of the associated UI menu item.
             *
             * @return The unique identifier (WidgetId) of the UI menu item.
             */
            UIManager::WidgetId getItemId() const;

            /**
             * @brief Sets the unique identifier of the associated UI menu item.
             *
             * @param itemId The unique identifier (WidgetId) to set for the UI menu item.
             */
            void setItemId( UIManager::WidgetId itemId );

        protected:
            /**
             * @brief The unique identifier of the associated UI menu item.
             *
             * This member variable stores the unique identifier of the UI menu item
             * associated with the file menu command.
             */
            UIManager::WidgetId m_itemId = UIManager::WidgetId::None;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // AddEntityCmd_h__
