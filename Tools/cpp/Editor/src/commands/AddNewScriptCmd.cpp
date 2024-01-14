#include <GameEditorPCH.h>
#include <commands/AddNewScriptCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <FBCore/FBCore.h>
#include <cstdio>
#include <fstream>

using std::endl;
using std::ofstream;

namespace fb::editor
{

    AddNewScriptCmd::AddNewScriptCmd() = default;

    AddNewScriptCmd::AddNewScriptCmd( const Properties &properties )
    {
        m_properties = properties;

        m_properties.getPropertyValue( "filePath", m_filePath );
        m_properties.getPropertyValue( "fileName", m_fileName );
    }

    AddNewScriptCmd::~AddNewScriptCmd() = default;

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

        // auto applicationManager = core::ApplicationManager::instance();
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
        scriptGenerator.createScript( ScriptGenerator::LanguageType::LUA, path );
    }

    void AddNewScriptCmd::undo()
    {
        String filePath;
        m_properties.getPropertyValue( "filePath", filePath );

        if( remove( filePath.c_str() ) != 0 )
        {
        }
    }

    auto AddNewScriptCmd::getPath() const -> String
    {
        return m_filePath;
    }

    void AddNewScriptCmd::setPath( const String &val )
    {
        m_filePath = val;
    }

    auto AddNewScriptCmd::getFileName() const -> String
    {
        return m_fileName;
    }

    void AddNewScriptCmd::setFileName( const String &val )
    {
        m_fileName = val;
    }

}  // namespace fb::editor
