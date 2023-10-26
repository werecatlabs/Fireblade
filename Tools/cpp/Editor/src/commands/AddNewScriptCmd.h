#ifndef AddNewScriptCmd_h__
#define AddNewScriptCmd_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Properties.h>
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class AddNewScriptCmd : public Command
        {
        public:
            AddNewScriptCmd();
            AddNewScriptCmd( const Properties &properties );
            ~AddNewScriptCmd();

            void redo();
            void execute();
            void undo();

            String getPath() const;
            void setPath( const String &val );

            String getFileName() const;
            void setFileName( const String &val );

        protected:
            Properties m_properties;
            String m_filePath;
            String m_fileName;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // AddNewScriptCmd_h__
