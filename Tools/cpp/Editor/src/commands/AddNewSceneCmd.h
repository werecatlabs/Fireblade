#ifndef AddNewSceneCmd_h__
#define AddNewSceneCmd_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Properties.h>

namespace fb
{
    namespace editor
    {

        class AddNewSceneCmd : public ICommand
        {
        public:
            AddNewSceneCmd( Properties properties );
            ~AddNewSceneCmd();

            void undo();
            void redo();
            void execute();

        protected:
            Properties m_properties;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // AddNewSceneCmd_h__
