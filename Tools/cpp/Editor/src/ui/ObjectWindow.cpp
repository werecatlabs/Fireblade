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
#include "GameEditorTypes.h"

namespace fb::editor
{
    ObjectWindow::ObjectWindow() = default;

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
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto editorManager = EditorManager::getSingletonPtr();
            auto editorUI = editorManager->getUI();

            auto parent = getParent();

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            parentWindow->setLabel( "Object" );
            setParentWindow( parentWindow );

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

            auto propertiesWindow = fb::make_ptr<PropertiesWindow>();
            propertiesWindow->setParent( parentWindow );
            propertiesWindow->load( data );
            m_propertiesWindow = propertiesWindow;

            //terrainWindow->setWindowVisible( true );

            m_actorWindow->setWindowVisible( false );
            m_materialWindow->setWindowVisible( false );
            m_fileViewWindow->setWindowVisible( false );
            m_resourceWindow->setWindowVisible( false );
            m_propertiesWindow->setWindowVisible( false );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ObjectWindow::reload( SmartPtr<ISharedObject> data )
    {
        if( m_actorWindow )
        {
            m_actorWindow->reload( data );
        }

        if( m_materialWindow )
        {
            m_materialWindow->reload( data );
        }

        if( m_fileViewWindow )
        {
            m_fileViewWindow->reload( data );
        }

        if( m_resourceWindow )
        {
            m_resourceWindow->reload( data );
        }

        EditorWindow::reload( data );
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

    auto ObjectWindow::getWindow() const -> SmartPtr<ui::IUIWindow>
    {
        return m_window;
    }

    void ObjectWindow::setWindow( SmartPtr<ui::IUIWindow> window )
    {
        m_window = window;
    }

    void ObjectWindow::updateSelection()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto selection = selectionManager->getSelection();
        if( !selection.empty() )
        {
            auto selected = selection.front();

            if( selected->isDerived<scene::IActor>() )
            {
                m_objectType = ObjectType::Actor;
            }

            else if( selected->isDerived<scene::TerrainSystem>() )
            {
                m_objectType = ObjectType::Terrain;
            }
            else if( selected->isDerived<FileSelection>() )
            {
                auto fileSelection = fb::static_pointer_cast<FileSelection>( selected );
                auto filePath = fileSelection->getFilePath();
                auto fileExt = Path::getFileExtension( filePath );
                fileExt = StringUtil::make_lower( fileExt );

                m_objectType = ObjectType::Resource;

                if( fileExt == ".mat" )
                {
                    m_resourceType = ObjectType::Material;
                    //m_resourceType = ObjectWindow::ObjectType::Resource;
                }
                else if( ApplicationUtil::isSupportedMesh( filePath ) )
                {
                    m_resourceType = ObjectType::Mesh;
                }
                else if( ApplicationUtil::isSupportedTexture( filePath ) )
                {
                    m_resourceType = ObjectType::Texture;
                }
                else if( ApplicationUtil::isSupportedSound( filePath ) )
                {
                    m_resourceType = ObjectType::Sound;
                }
                else if( fileExt == ".resource" )
                {
                    m_resourceType = ObjectType::Resource;
                }
                else
                {
                    m_resourceType = ObjectType::FileUnknown;
                    m_objectType = ObjectType::FileUnknown;
                }
            }
            else if( selected->isDerived<IResource>() )
            {
                m_resourceType = ObjectType::Resource;
                m_objectType = ObjectType::Resource;
            }
            else if( selected->isDerived<ISharedObject>() )
            {
                m_resourceType = ObjectType::SharedObject;
                m_objectType = ObjectType::SharedObject;
            }
        }

        switch( m_objectType )
        {
        case ObjectType::None:
        {
        }
        break;
        case ObjectType::Actor:
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
        case ObjectType::Resource:
        {
            switch( m_resourceType )
            {
            case ObjectType::None:
            {
            }
            break;
            case ObjectType::Actor:
            {
            }
            break;
            case ObjectType::SharedObject:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( true );
                }

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

                if( m_propertiesWindow )
                {
                    m_propertiesWindow->updateSelection();
                }
            }
            break;
            case ObjectType::Resource:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            case ObjectType::FileUnknown:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            case ObjectType::Mesh:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            case ObjectType::Material:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            case ObjectType::MaterialNode:
            {
            }
            break;
            case ObjectType::Terrain:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            case ObjectType::Sound:
            case ObjectType::Texture:
            {
                if( m_propertiesWindow )
                {
                    m_propertiesWindow->setWindowVisible( false );
                }

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
            default:
            {
            }
            break;
            }
        }
        break;
        case ObjectType::Mesh:
        {
        }
        break;
        case ObjectType::Material:
        {
        }
        break;
        case ObjectType::MaterialNode:
        {
        }
        break;
        default:
        {
        }
        break;
        }
    }
}  // namespace fb::editor
