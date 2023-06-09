#include <GameEditorPCH.h>
#include <commands/AddResourceCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {
        AddResourceCmd::AddResourceCmd()
        {
        }

        AddResourceCmd::~AddResourceCmd()
        {
        }

        void AddResourceCmd::redo()
        {
        }

        void AddResourceCmd::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto materialManager = graphicsSystem->getMaterialManager();

                auto resourceDatabase = applicationManager->getResourceDatabase();

                auto editorManager = EditorManager::getSingletonPtr();
                auto projectManager = editorManager->getProjectManager();

                auto ui = editorManager->getUI();

                auto filePath = getFilePath();

                SmartPtr<IResource> resource;
                SmartPtr<Properties> data;
                String dataStr;

                auto resourceType = getResourceType();
                switch( resourceType )
                {
                case ResourceType::Script:
                {
                    resource = nullptr;
                    data = nullptr;
                    dataStr = "";
                }
                break;
                case ResourceType::Material:
                {
                    resource = ApplicationUtil::createDefaultMaterial();
                    FB_ASSERT( resource );

                    data = resource->toData();
                    FB_ASSERT( data );

                    dataStr = DataUtil::toString( data.get(), true );
                    FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );
                }
                break;
                case ResourceType::Scene:
                {
                }
                break;
                case ResourceType::Director:
                {
                    auto director = fb::make_ptr<TerrainDirector>();
                    FB_ASSERT( director );

                    director->load( nullptr );

                    data = director->toData();
                    FB_ASSERT( data );
                    
                    dataStr = DataUtil::toString( data.get(), true );
                    FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );
                }
                break;
                };

                if( !fileSystem->isExistingFile( filePath ) )
                {
                    fileSystem->writeAllText( filePath, dataStr );
                }
                else
                {
                    auto fileIndex = 0;
                    auto maxRetries = 1000;

                    auto path = Path::getFilePath( filePath );
                    auto fileName = Path::getFileNameWithoutExtension( filePath );
                    auto fileExt = Path::getFileExtension( filePath );

                    auto newFileName = fileName + StringUtil::toString( fileIndex ) + fileExt;
                    auto newFilePath = Path::lexically_normal( path, newFileName );
                    while( fileSystem->isExistingFile( newFilePath ) && fileIndex < maxRetries )
                    {
                        ++fileIndex;

                        newFileName = fileName + StringUtil::toString( fileIndex ) + fileExt;
                        newFilePath = Path::lexically_normal( path, newFileName );
                    }

                    fileSystem->writeAllText( newFilePath, dataStr );
                }

                resourceDatabase->addResource( resource );

                auto refreshPath = Path::getFilePath( filePath );
                fileSystem->refreshPath( refreshPath, true );

                ui->rebuildResourceTree();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void AddResourceCmd::undo()
        {
        }

        String AddResourceCmd::getFilePath() const
        {
            return m_filePath;
        }

        void AddResourceCmd::setFilePath( const String &filePath )
        {
            m_filePath = filePath;
        }

        AddResourceCmd::ResourceType AddResourceCmd::getResourceType() const
        {
            return m_resourceType;
        }

        void AddResourceCmd::setResourceType( ResourceType resourceType )
        {
            m_resourceType = resourceType;
        }

        SmartPtr<ISharedObject> AddResourceCmd::getResource() const
        {
            return m_resource;
        }

        void AddResourceCmd::setResource( SmartPtr<ISharedObject> resource )
        {
            m_resource = resource;
        }
    }  // end namespace editor
}  // end namespace fb
