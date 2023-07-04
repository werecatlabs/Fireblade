#include <GameEditorPCH.h>
#include <ui/ObjectWindow.h>
#include <ui/ActorWindow.h>
#include <ui/MaterialWindow.h>
#include <ui/PropertiesWindow.h>
#include <ui/FileViewWindow.h>
#include <ui/ResourceWindow.h>
#include <ui/TerrainWindow.h>
#include <ui/UIManager.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {

        ObjectWindow::ObjectWindow()
        {
        }

        ObjectWindow::ObjectWindow( SmartPtr<ui::IUIWindow> parent )
        {
            setParent( parent );
        }

        ObjectWindow::~ObjectWindow()
        {
            unload( nullptr );
        }

        void ObjectWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto editorManager = EditorManager::getSingletonPtr();
                auto editorUI = editorManager->getUI();

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                parentWindow->setLabel( "Object" );
                setWindow( parentWindow );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto actorWindow = fb::make_ptr<ActorWindow>( parentWindow );
                actorWindow->load( data );
                m_actorWindow = actorWindow;

                editorUI->setActorWindow( actorWindow );

                auto materialWindow = fb::make_ptr<MaterialWindow>( parentWindow );
                materialWindow->load( data );
                m_materialWindow = materialWindow;

                auto fileViewWindow = fb::make_ptr<FileViewWindow>( parentWindow );
                fileViewWindow->load( data );
                m_fileViewWindow = fileViewWindow;

                auto resourceWindow = fb::make_ptr<ResourceWindow>( parentWindow );
                resourceWindow->load( data );
                m_resourceWindow = resourceWindow;

                //terrainWindow->setWindowVisible( true );

                m_actorWindow->setWindowVisible( false );
                m_materialWindow->setWindowVisible( false );
                m_fileViewWindow->setWindowVisible( false );
                m_resourceWindow->setWindowVisible( false );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ObjectWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( m_actorWindow )
                {
                    m_actorWindow->unload( nullptr );
                    m_actorWindow = nullptr;
                }

                if( m_materialWindow )
                {
                    m_materialWindow->unload( nullptr );
                    m_materialWindow = nullptr;
                }

                if( m_fileViewWindow )
                {
                    m_fileViewWindow->unload( nullptr );
                    m_fileViewWindow = nullptr;
                }

                if( m_resourceWindow )
                {
                    m_resourceWindow->unload( nullptr );
                    m_resourceWindow = nullptr;
                }

                for( auto data : m_dataArray )
                {
                    data->unload( nullptr );
                }

                m_dataArray.clear();

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ui::IUIWindow> ObjectWindow::getWindow() const
        {
            return m_window;
        }

        void ObjectWindow::setWindow( SmartPtr<ui::IUIWindow> window )
        {
            m_window = window;
        }

        void ObjectWindow::updateSelection()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            if( !selection.empty() )
            {
                auto selected = selection.front();

                if( selected->isDerived<scene::IActor>() )
                {
                    m_objectType = ObjectWindow::ObjectType::Actor;
                }
                else if( selected->isDerived<scene::TerrainSystem>() )
                {
                    m_objectType = ObjectWindow::ObjectType::Terrain;
                }
                else if( selected->isDerived<FileSelection>() )
                {
                    auto fileSelection = fb::static_pointer_cast<FileSelection>( selected );
                    auto filePath = fileSelection->getFilePath();
                    auto fileExt = Path::getFileExtension( filePath );
                    fileExt = StringUtil::make_lower( fileExt );

                    m_objectType = ObjectWindow::ObjectType::Resource;

                    if( fileExt == ".mat" )
                    {
                        m_resourceType = ObjectWindow::ObjectType::Material;
                    }
                    else if( fileExt == ".fbx" )
                    {
                        m_resourceType = ObjectWindow::ObjectType::Mesh;
                    }
                    else if( fileExt == ".resource" )
                    {
                        m_resourceType = ObjectWindow::ObjectType::Resource;
                    }
                    else
                    {
                        m_resourceType = ObjectWindow::ObjectType::FileUnknown;
                        m_objectType = ObjectWindow::ObjectType::FileUnknown;
                    }
                }
            }

            switch( m_objectType )
            {
            case ObjectWindow::ObjectType::None:
            {
            }
            break;
            case ObjectWindow::ObjectType::Actor:
            {
                if( m_actorWindow )
                {
                    m_actorWindow->setWindowVisible( true );
                }

                if( m_materialWindow )
                {
                    m_materialWindow->setWindowVisible( false );
                }

                if( m_fileViewWindow )
                {
                    m_fileViewWindow->setWindowVisible( false );
                }

                if( m_resourceWindow )
                {
                    m_resourceWindow->setWindowVisible( false );
                }

                if( m_actorWindow )
                {
                    m_actorWindow->updateSelection();
                }
            }
            break;
            case ObjectWindow::ObjectType::Resource:
            {
                switch( m_resourceType )
                {
                case ObjectWindow::ObjectType::None:
                {
                }
                break;
                case ObjectWindow::ObjectType::Actor:
                {
                }
                break;
                case ObjectWindow::ObjectType::Resource:
                {
                    if( m_actorWindow )
                    {
                        m_actorWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->setWindowVisible( false );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->setWindowVisible( false );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->setWindowVisible( true );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->updateSelection();
                    }
                }
                break;
                case ObjectWindow::ObjectType::FileUnknown:
                {
                    if( m_actorWindow )
                    {
                        m_actorWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->setWindowVisible( false );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->setWindowVisible( true );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->setWindowVisible( false );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->updateSelection();
                    }
                }
                break;
                case ObjectWindow::ObjectType::Mesh:
                {
                    if( m_actorWindow )
                    {
                        m_actorWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->setWindowVisible( false );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->setWindowVisible( false );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->setWindowVisible( true );
                    }

                    if( m_actorWindow )
                    {
                        m_resourceWindow->updateSelection();
                    }
                }
                break;
                case ObjectWindow::ObjectType::Material:
                {
                    if( m_actorWindow )
                    {
                        m_actorWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->setWindowVisible( true );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->setWindowVisible( false );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->updateSelection();
                    }
                }
                break;
                case ObjectWindow::ObjectType::MaterialNode:
                {
                }
                break;
                case ObjectWindow::ObjectType::Terrain:
                {
                    if( m_actorWindow )
                    {
                        m_actorWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->setWindowVisible( false );
                    }

                    if( m_fileViewWindow )
                    {
                        m_fileViewWindow->setWindowVisible( false );
                    }

                    if( m_resourceWindow )
                    {
                        m_resourceWindow->setWindowVisible( false );
                    }

                    if( m_materialWindow )
                    {
                        m_materialWindow->updateSelection();
                    }
                }
                break;
                default:
                {
                }
                break;
                };
            }
            break;
            case ObjectWindow::ObjectType::Mesh:
            {
            }
            break;
            case ObjectWindow::ObjectType::Material:
            {
            }
            break;
            case ObjectWindow::ObjectType::MaterialNode:
            {
            }
            break;
            default:
            {
            }
            break;
            };
        }

    }  // end namespace editor
}  // end namespace fb
