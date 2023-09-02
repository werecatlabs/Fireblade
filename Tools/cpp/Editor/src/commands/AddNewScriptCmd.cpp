#include <GameEditorPCH.h>
#include <commands/AddNewScriptCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <FBCore/FBCore.h>
#include <stdio.h>
#include <fstream>

using std::endl;
using std::ofstream;

namespace fb
{
    namespace editor
    {

        AddNewScriptCmd::AddNewScriptCmd()
        {
        }

        AddNewScriptCmd::AddNewScriptCmd( const Properties &properties )
        {
            m_properties = properties;

            m_properties.getPropertyValue( "filePath", m_filePath );
            m_properties.getPropertyValue( "fileName", m_fileName );
        }

        AddNewScriptCmd::~AddNewScriptCmd()
        {
        }

        void AddNewScriptCmd::redo()
        {
        }

        void AddNewScriptCmd::execute()
        {
            // auto filePath = m_filePath + String("/") + m_fileName;

            // std::ofstream scriptStream;
            // scriptStream.open( m_filePath.c_str() );
            // scriptStream.close();

            // EditorManager* appRoot = EditorManager::getSingletonPtr();
            // SmartPtr<Project> project = appRoot->getProject();

            // m_scriptTemplate = SmartPtr<ScriptTemplate>(new ScriptTemplate);
            // m_scriptTemplate->setProperties(m_properties);

            // auto applicationManager = core::IApplicationManager::instance();
            // auto& fileSystem = applicationManager->getFileSystem();
            // auto stream = fileSystem->open(m_scriptTemplate->getFileName(),
            // m_scriptTemplate->getFilePath()); if(stream)
            //{
            //	String data = stream->getAsString();
            //	m_scriptTemplate->setData(data);
            // }

            // project->addScriptTemplate(m_scriptTemplate);

            ScriptGenerator scriptGenerator;

            auto path = getPath();
            auto fileName = getFileName();
            scriptGenerator.createCPlusPlusScript( path, fileName );
        }

        void AddNewScriptCmd::undo()
        {
            String filePath;
            m_properties.getPropertyValue( "filePath", filePath );

            if( remove( filePath.c_str() ) != 0 )
            {
            }
        }

        String AddNewScriptCmd::getPath() const
        {
            return m_filePath;
        }

        void AddNewScriptCmd::setPath( const String &val )
        {
            m_filePath = val;
        }

        String AddNewScriptCmd::getFileName() const
        {
            return m_fileName;
        }

        void AddNewScriptCmd::setFileName( const String &val )
        {
            m_fileName = val;
        }

    }  // end namespace editor
}  // end namespace fb
