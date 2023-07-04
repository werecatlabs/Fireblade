#include <GameEditorPCH.h>
#include "ui/MaterialWindow.h"
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/ProjectTreeData.h>
#include <ui/PropertiesWindow.h>
#include <ui/SceneWindow.h>
#include <ui/UIManager.h>
#include <GameEditorTypes.h>
#include <ui/ObjectBrowserDialog.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <FBGraphics/FBGraphics.h>

namespace fb
{
    namespace editor
    {
        MaterialWindow::MaterialWindow( SmartPtr<ui::IUIWindow> parent )
        {
            auto className = String( "MaterialEditor" );
            setClassName( className );

            setParent( parent );
        }

        MaterialWindow::~MaterialWindow()
        {
            unload( nullptr );
        }

        void MaterialWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                BaseWindow::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                auto ui = applicationManager->getUI();

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                setParentWindow( parentWindow );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto debugWindow = ui->addElementByType<ui::IUIWindow>();
                setDebugWindow( debugWindow );

                if( parent )
                {
                    parent->addChild( debugWindow );
                }

                m_dropdown = ui->addElementByType<ui::IUIDropdown>();
                FB_ASSERT( m_dropdown );

                auto dropdownListener = fb::make_ptr<DropdownListener>();
                dropdownListener->setOwner( this );
                m_dropdown->addObjectListener( dropdownListener );

                auto materialTypes = render::GraphicsUtil::getMaterialTypes();
                m_dropdown->setOptions( materialTypes );

                debugWindow->addChild( m_dropdown );

                auto treeCtrl = ui->addElementByType<ui::IUITreeCtrl>();
                debugWindow->addChild( treeCtrl );
                m_tree = treeCtrl;

                auto treeListener = fb::make_ptr<TreeCtrlListener>();
                treeListener->setOwner( this );
                m_treeListener = treeListener;

                m_tree->addObjectListener( treeListener );

                m_propertiesWindow = fb::make_ptr<PropertiesWindow>();
                m_propertiesWindow->setParent( debugWindow );
                m_propertiesWindow->load( data );

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

        void MaterialWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                if( m_dropdown )
                {
                    ui->removeElement( m_dropdown );
                    m_dropdown = nullptr;
                }

                if( m_tree )
                {
                    m_tree->removeObjectListener( m_treeListener );

                    ui->removeElement( m_tree );
                    m_tree = nullptr;
                }

                if( m_treeListener )
                {
                    m_treeListener->unload( nullptr );
                    m_treeListener = nullptr;
                }

                if( m_propertiesWindow )
                {
                    m_propertiesWindow->unload( nullptr );
                    m_propertiesWindow = nullptr;
                }

                if( auto parentWindow = getParentWindow() )
                {
                    ui->removeElement( parentWindow );
                    setParentWindow( nullptr );
                }

                for( auto data : m_dataArray )
                {
                    data->unload( nullptr );
                }

                m_dataArray.clear();

                BaseWindow::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void MaterialWindow::updateSelection()
        {
            buildTree();
            BaseWindow::updateSelection();
        }

        SmartPtr<render::IMaterial> MaterialWindow::getMaterial() const
        {
            return m_material;
        }

        void MaterialWindow::setMaterial( SmartPtr<render::IMaterial> material )
        {
            m_material = material;
        }

        SmartPtr<ui::IUITreeCtrl> MaterialWindow::getTree() const
        {
            return m_tree;
        }

        void MaterialWindow::setTree( SmartPtr<ui::IUITreeCtrl> tree )
        {
            m_tree = tree;
        }

        void MaterialWindow::buildTree()
        {
            try
            {
                auto tree = getTree();
                FB_ASSERT( tree );

                if( tree )
                {
                    tree->clear();
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );
                FB_ASSERT( graphicsSystem->isValid() );

                auto materialManager = graphicsSystem->getMaterialManager();
                auto meshManager = applicationManager->getMeshManager();

                FB_ASSERT( materialManager );
                FB_ASSERT( meshManager );

                auto editorManager = EditorManager::getSingletonPtr();
                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto currentScene = sceneManager->getCurrentScene();
                FB_ASSERT( currentScene );

                if( currentScene )
                {
                    auto project = editorManager->getProject();
                    FB_ASSERT( project );

                    auto sceneName = currentScene->getName();

                    auto selection = selectionManager->getSelection();
                    if( selection.size() == 1 )
                    {
                        auto selectedObject = selection.front();

                        if( selectedObject->isDerived<FileSelection>() )
                        {
                            auto fileSelection =
                                fb::dynamic_pointer_cast<FileSelection>( selectedObject );

                            auto filePath = fileSelection->getFilePath();
                            auto ext = Path::getFileExtension( filePath );

                            auto fileInfo = fileSelection->getFileInfo();

                            static const auto materialExt = String( ".mat" );

                            if( ext == materialExt )
                            {
                                auto graphicsSystem = applicationManager->getGraphicsSystem();
                                auto materialManager = graphicsSystem->getMaterialManager();

                                auto material =
                                    resourceDatabase->loadResourceByType<render::IMaterial>( filePath );
                                if( !material )
                                {
                                    auto materialFilePath = fileInfo.absolutePath;
                                    material = resourceDatabase->loadResourceByType<render::IMaterial>(
                                        materialFilePath );
                                }

                                if( material )
                                {
                                    material->load( nullptr );

                                    setMaterial( material );

                                    auto materialType = material->getMaterialType();
                                    m_dropdown->setSelectedOption( static_cast<u32>( materialType ) );

                                    auto rootNode = tree->addRoot();
                                    FB_ASSERT( rootNode );
                                    rootNode->setExpanded( true );

                                    addMaterialToTree( material, rootNode );
                                }
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

        void MaterialWindow::addMaterialToTree( SmartPtr<render::IMaterial> material,
                                                SmartPtr<ui::IUITreeNode> node )
        {
            try
            {
                auto editorManager = EditorManager::getSingletonPtr();

                auto project = editorManager->getProject();
                FB_ASSERT( project );

                auto data = fb::make_ptr<ProjectTreeData>( "material", "material", material, material );

                auto materialHandle = material->getHandle();
                auto actorName = materialHandle->getName();
                if( StringUtil::isNullOrEmpty( actorName ) )
                {
                    actorName = "Untitled";
                }

                auto treeNode = m_tree->addNode();
                treeNode->setExpanded( true );

                FB_ASSERT( treeNode );
                ApplicationUtil::setText( treeNode, actorName );

                treeNode->setNodeUserData( data );

                if( node )
                {
                    node->addChild( treeNode );
                }

                addObjectToTree( material, treeNode );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void MaterialWindow::addObjectToTree( SmartPtr<ISharedObject> object,
                                              SmartPtr<ui::IUITreeNode> node )
        {
            try
            {
                if( object )
                {
                    auto editorManager = EditorManager::getSingletonPtr();

                    auto project = editorManager->getProject();
                    FB_ASSERT( project );

                    auto data = fb::make_ptr<ProjectTreeData>( "object", "object", object, object );

                    auto typeinfo = object->getTypeInfo();
                    FB_ASSERT( typeinfo != 0 );

                    auto typeManager = TypeManager::instance();
                    FB_ASSERT( typeManager );

                    auto className = typeManager->getName( typeinfo );
                    if( StringUtil::isNullOrEmpty( className ) )
                    {
                        className = "Untitled";
                    }

                    if( object->isDerived<IStateContext>() )
                    {
                        className = String( "StateObject" );
                    }
                    else if( object->isDerived<IStateListener>() )
                    {
                        className = String( "StateListener" );
                    }
                    else if( object->isDerived<render::IGraphicsScene>() )
                    {
                        className = String( "SceneManager" );
                    }
                    else if( object->isDerived<render::ISceneNode>() )
                    {
                        className = String( "SceneNode" );
                    }
                    else if( object->isDerived<render::IGraphicsMesh>() )
                    {
                        className = String( "Mesh" );
                    }
                    else if( object->isDerived<render::IMaterial>() )
                    {
                        className = String( "Material" );
                    }
                    else if( object->isDerived<render::IMaterialTechnique>() )
                    {
                        className = String( "Technique" );
                    }
                    else if( object->isDerived<render::IMaterialPass>() )
                    {
                        className = String( "Pass" );
                    }
                    else if( object->isDerived<render::IMaterialTexture>() )
                    {
                        auto textureHandle = object->getHandle();
                        className = textureHandle->getName();
                    }

                    auto treeNode = m_tree->addNode();

                    FB_ASSERT( treeNode );
                    ApplicationUtil::setText( treeNode, className );

                    treeNode->setNodeUserData( data );

                    if( node )
                    {
                        node->addChild( treeNode );
                    }

                    auto children = object->getChildObjects();
                    for( auto child : children )
                    {
                        if( child )
                        {
                            addObjectToTree( child, treeNode );
                        }
                    }

                    if( !children.empty() )
                    {
                        treeNode->setExpanded( true );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void MaterialWindow::handleDropdownSelection()
        {
            auto materialType =
                static_cast<render::IMaterial::MaterialType>( m_dropdown->getSelectedOption() );

            if( auto material = getMaterial() )
            {
                material->setMaterialType( materialType );
                material->save();
            }
        }

        MaterialWindow::TreeCtrlListener::TreeCtrlListener()
        {
        }

        MaterialWindow::TreeCtrlListener::~TreeCtrlListener()
        {
        }

        Parameter MaterialWindow::TreeCtrlListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::handleTreeSelectionActivated )
            {
                if( auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( object ) )
                {
                    auto data = fb::static_pointer_cast<ProjectTreeData>( node->getNodeUserData() );
                    if( data )
                    {
                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto selectionManager = applicationManager->getSelectionManager();
                        FB_ASSERT( selectionManager );

                        selectionManager->clearSelection();

                        auto component = data->getObjectData();
                        selectionManager->addSelectedObject( component );

                        m_owner->m_propertiesWindow->updateSelection();
                    }
                }
            }

            return Parameter();
        }

        MaterialWindow *MaterialWindow::TreeCtrlListener::getOwner() const
        {
            return m_owner;
        }

        void MaterialWindow::TreeCtrlListener::setOwner( MaterialWindow *owner )
        {
            m_owner = owner;
        }

        Parameter MaterialWindow::DropdownListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::handleSelection )
            {
                FB_ASSERT( m_owner );
                m_owner->handleDropdownSelection();
            }

            return Parameter();
        }

        MaterialWindow *MaterialWindow::DropdownListener::getOwner() const
        {
            return m_owner;
        }

        void MaterialWindow::DropdownListener::setOwner( MaterialWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
