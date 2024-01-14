#include <GameEditorPCH.h>
#include "RemoveResourceCmd.h"
#include "editor/EditorManager.h"
#include "editor/Project.h"

#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    RemoveResourceCmd::RemoveResourceCmd() = default;

    RemoveResourceCmd::~RemoveResourceCmd() = default;

    void RemoveResourceCmd::undo()
    {
    }

    void RemoveResourceCmd::redo()
    {
    }

    void RemoveResourceCmd::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto resourceDatabase = applicationManager->getResourceDatabase();
        auto fileSystem = applicationManager->getFileSystem();

        auto path = getFilePath();
        auto folder = Path::isFolder( path );
        if( folder )
        {
            auto files = fileSystem->getFiles( path );
            for( auto file : files )
            {
                auto resource = resourceDatabase->loadResource( file );
                if( resource )
                {
                    resourceDatabase->removeResource( resource );
                }
            }

            Path::deleteFolder( path );
        }
        else
        {
            auto resource = resourceDatabase->loadResource( path );
            resourceDatabase->removeResource( resource );
            fileSystem->deleteFile( m_filePath );
        }

        auto editorManager = editor::EditorManager::getSingletonPtr();
        editorManager->getUI()->rebuildResourceTree();
    }

    auto RemoveResourceCmd::getFilePath() const -> String
    {
        return m_filePath;
    }

    void RemoveResourceCmd::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

}  // namespace fb::editor
