#include <GameEditorPCH.h>
#include "TerrainWindow.h"
#include <FBCore/FBCore.h>

#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <ui/ResourceDatabaseDialog.h>

namespace fb
{
    namespace editor
    {
        TerrainWindow::TerrainWindow()
        {
            auto className = String( "TerrainEditor" );
            setClassName( className );
        }

        TerrainWindow::~TerrainWindow()
        {
        }

        void TerrainWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                EditorWindow::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                FB_ASSERT( parentWindow );

                setParentWindow( parentWindow );
                parentWindow->setLabel( "TerrainWindowChild" );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto debugWindow = ui->addElementByType<ui::IUIWindow>();
                setDebugWindow( debugWindow );
                debugWindow->setVisible( false, false );

                if( parent )
                {
                    parent->addChild( debugWindow );
                }

                auto terrainEditor = ui->addElementByType<ui::IUITerrainEditor>();
                debugWindow->addChild( terrainEditor );
                m_terrainEditor = terrainEditor;

                auto elementListener = fb::make_ptr<UIElementListener>();
                elementListener->setOwner( this );
                m_terrainEditorListener = elementListener;

                terrainEditor->addObjectListener( elementListener );

                if( auto invoker = getInvoker() )
                {
                    invoker->callObjectMember( "load" );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void TerrainWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );
                EditorWindow::unload( data );
                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ui::IUITerrainEditor> TerrainWindow::getTerrainEditor() const
        {
            return m_terrainEditor;
        }

        void TerrainWindow::setTerrainEditor( SmartPtr<ui::IUITerrainEditor> terrainEditor )
        {
            m_terrainEditor = terrainEditor;
        }

        void TerrainWindow::updateSelection()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto selectionManager = applicationManager->getSelectionManager();

                auto selection = selectionManager->getSelection();
                for( auto object : selection )
                {
                    if( object )
                    {
                        if( object->isDerived<scene::TerrainSystem>() )
                        {
                            auto terrain = fb::dynamic_pointer_cast<scene::TerrainSystem>( object );

                            if( auto terrainEditor = getTerrainEditor() )
                            {
                                terrainEditor->setTerrain( terrain );
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void TerrainWindow::loadImages()
        {
        }

        void TerrainWindow::addComboBoxBitmap()
        {
        }

        void TerrainWindow::OnRaiseBtnToggled()
        {
        }

        void TerrainWindow::OnLowerBtnToggled()
        {
        }

        void TerrainWindow::OnMinimumBtnToggled()
        {
        }

        void TerrainWindow::OnMaximumBtnToggled()
        {
        }

        void TerrainWindow::OnsetHeightBtnToggled()
        {
        }

        void TerrainWindow::OnPaintBtnToggled()
        {
        }

        void TerrainWindow::OnEraseBtnToggled()
        {
        }

        void TerrainWindow::OnBlendBtnToggled()
        {
        }

        void TerrainWindow::OnAddTextureBtn()
        {
        }

        void TerrainWindow::OnComboBox()
        {
        }

        void TerrainWindow::OnPropertyGridChangeTool()
        {
        }

        void TerrainWindow::populateToolProperties()
        {
        }

        Parameter TerrainWindow::UIElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto ui = editorManager->getUI();
            FB_ASSERT( ui );

            if( eventValue == ui::IUITerrainEditor::selectTerrainTextureHash )
            {
                auto resourceType = IResource::ResourceType::Texture;

                if( auto resourceDatabaseDialog = ui->getResourceDatabaseDialog() )
                {
                    resourceDatabaseDialog->setResourceType( resourceType );

                    //if( auto selected = getSelected() )
                    //{
                    if( arguments.size() >= 1 )
                    {
                        auto selected = arguments[0].object;
                        resourceDatabaseDialog->setCurrentObject( selected );
                    }
                    //}

                    resourceDatabaseDialog->setPropertyName( "baseTexture" );
                    resourceDatabaseDialog->setWindowVisible( true );
                    resourceDatabaseDialog->populate();
                }
            }

            return Parameter();
        }

        TerrainWindow *TerrainWindow::UIElementListener::getOwner() const
        {
            return m_owner;
        }

        void TerrainWindow::UIElementListener::setOwner( TerrainWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
