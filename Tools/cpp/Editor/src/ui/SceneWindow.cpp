#include <GameEditorPCH.h>
#include <ui/SceneWindow.h>
#include <editor/Project.h>
#include <editor/EditorManager.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <commands/DragDropActorCmd.h>
#include <commands/AddActorCmd.h>
#include <commands/RemoveSelectionCmd.h>
#include "commands/PromptCmd.h"
#include "jobs/SceneDropJob.h"
#include <FBCore/FBCore.h>

enum
{
    TREE_ITEM_STATE_NOT_FOUND = 0,
    TREE_ITEM_STATE_EXPANDED = 1,
    TREE_ITEM_STATE_NOT_EXPANDED = 2
};

namespace fb::editor
{
    using namespace scene;

    SceneWindow::SceneWindow() = default;

    SceneWindow::SceneWindow( SmartPtr<ui::IUIWindow> parent )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto listener = fb::make_ptr<SceneWindowListener>();
            listener->setOwner( this );
            m_menuListener = listener;

            auto window = ui->addElementByType<ui::IUIWindow>();
            window->setLabel( "Scene" );
            window->setHasBorder( true );
            setParentWindow( window );

            if( parent )
            {
                parent->addChild( window );
            }

            window->addObjectListener( m_menuListener );

            auto sceneWindow = ui->addElementByType<ui::IUIWindow>();
            sceneWindow->setLabel( "SceneWindow" );
            sceneWindow->setHasBorder( true );
            m_sceneWindow = sceneWindow;
            sceneWindow->addObjectListener( m_menuListener );
            window->addChild( sceneWindow );

            auto inputText = ui->addElementByType<ui::IUITextEntry>();
            sceneWindow->addChild( inputText );
            m_inputText = inputText;

            auto promptListener = fb::make_ptr<PromptListener>();
            promptListener->setOwner( this );
            m_inputText->addObjectListener( promptListener );
            m_promptListener = promptListener;

            auto treeCtrl = ui->addElementByType<ui::IUITreeCtrl>();
            treeCtrl->setMultiSelect( true );
            sceneWindow->addChild( treeCtrl );
            m_tree = treeCtrl;

            auto dragSource = fb::make_ptr<DragSource>();
            dragSource->setOwner( this );
            m_tree->setDragSource( dragSource );

            auto dropTarget = fb::make_ptr<DropTarget>();
            dropTarget->setOwner( this );
            window->setDropTarget( dropTarget );

            m_tree->setDropTarget( dropTarget );

            auto treeListener = fb::make_ptr<TreeCtrlListener>();
            treeListener->setOwner( this );
            m_treeListener = treeListener;

            m_tree->addObjectListener( treeListener );

            m_applicationMenu = ui->addElementByType<ui::IUIMenu>();
            m_applicationMenu->setLabel( "Scene" );

            m_applicationAddMenu = ui->addElementByType<ui::IUIMenu>();
            m_applicationAddMenu->setLabel( "Add" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CAMERA ), "Camera",
                               "Camera" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CAR ), "Car", "Car" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CUBE ), "Cube",
                               "Cube" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CUBE_MESH ),
                               "Cube Mesh", "Cube Mesh" );
            Util::addMenuSeparator( m_applicationAddMenu );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CUBEMAP ), "Cubemap",
                               "Cubemap" );

            Util::addMenuSeparator( m_applicationAddMenu );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_DIRECTIONAL_LIGHT ),
                               "Directional Light", "Directional Light" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_POINT_LIGHT ),
                               "Point Light", "Point Light" );
            Util::addMenuSeparator( m_applicationAddMenu );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_PARTICLESYSTEM ),
                               "Particle System", "Particle System" );

            Util::addMenuItem( m_applicationAddMenu,
                               static_cast<s32>( MenuId::ADD_PARTICLESYSTEM_SMOKE ),
                               "Particle System Smoke", "Particle System Smoke" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_PARTICLESYSTEM_SAND ),
                               "Particle System Sand", "Particle System Sand" );

            Util::addMenuSeparator( m_applicationAddMenu );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_PLANE ), "Plane",
                               "Plane" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_PHYSICS_CUBE ),
                               "Physics Cube", "Physics Cube" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_SKYBOX ), "Skybox",
                               "Skybox" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_NEW_TERRAIN ),
                               "Terrain", "Terrain" );
            Util::addMenuSeparator( m_applicationAddMenu );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_BUTTON ), "Button",
                               "Button" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CANVAS ), "Canvas",
                               "Canvas" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CHECKBOX ),
                               "Checkbox", "Checkbox" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_PANEL ), "Panel",
                               "Panel" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_SLIDER ), "Slider",
                               "Slider" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_TEXT ), "Text",
                               "Text" );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_TOGGLE_BUTTON ),
                               "Toggle Button", "Toggle Button" );

            m_applicationMenu->addMenuItem( m_applicationAddMenu );

            Util::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::ADD_NEW_ENTITY ),
                               "Add Actor", "Add Actor" );
            Util::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::SCENE_REMOVE_ACTOR ),
                               "Remove", "Remove" );

            m_applicationMenu->addObjectListener( m_menuListener );

            sceneWindow->setContextMenu( m_applicationMenu );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SceneWindow::~SceneWindow()
    {
        unload( nullptr );
    }

    void SceneWindow::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();

            auto applicationEventListener = fb::make_ptr<ApplicationEventListener>();
            applicationEventListener->setOwner( this );
            m_applicationEventListener = applicationEventListener;

            applicationManager->addObjectListener( applicationEventListener );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SceneWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( getLoadingState() == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                if( m_applicationEventListener )
                {
                    applicationManager->removeObjectListener( m_applicationEventListener );
                    m_applicationEventListener->unload( nullptr );
                    m_applicationEventListener = nullptr;
                }

                if( m_window )
                {
                    ui->removeElement( m_window );
                    m_window = nullptr;
                }

                if( m_sceneWindow )
                {
                    ui->removeElement( m_sceneWindow );
                    m_sceneWindow = nullptr;
                }

                if( m_tree )
                {
                    if( m_treeListener )
                    {
                        m_tree->removeObjectListener( m_treeListener );
                        m_treeListener = nullptr;
                    }

                    ui->removeElement( m_tree );
                    m_tree = nullptr;
                }

                if( m_applicationAddMenu )
                {
                    ui->removeElement( m_applicationAddMenu );
                    m_applicationAddMenu = nullptr;
                }

                if( m_applicationMenu )
                {
                    ui->removeElement( m_applicationMenu );
                    m_applicationMenu = nullptr;
                }

                m_menuListener = nullptr;

                m_selectedObject = nullptr;
                m_selectedEntity = nullptr;

                if( auto parentWindow = getParentWindow() )
                {
                    parentWindow->setContextMenu( nullptr );
                    ui->removeElement( parentWindow );
                    setParentWindow( nullptr );
                }

                for( auto data : m_dataArray )
                {
                    data->unload( nullptr );
                }

                m_dataArray.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SceneWindow::buildTree()
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_buildTreeMutex );

            FB_ASSERT( m_tree );

            saveTreeState();

            if( m_tree )
            {
                m_tree->clear();
            }

            for( auto data : m_dataArray )
            {
                data->unload( nullptr );
            }

            m_dataArray.clear();

            auto applicationManager = core::ApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            auto editorManager = EditorManager::getSingletonPtr();

            auto sceneManager = applicationManager->getSceneManager();
            auto currentScene = sceneManager->getCurrentScene();
            auto project = editorManager->getProject();

            if( currentScene )
            {
                auto sceneName = currentScene->getLabel();

                auto rootNode = m_tree->addRoot();
                if( rootNode )
                {
                    Util::setText( rootNode, sceneName );
                    rootNode->setExpanded( true );

                    auto treeNodeData = factoryManager->make_ptr<ProjectTreeData>(
                        "scene", "scene", currentScene, currentScene );
                    rootNode->setNodeUserData( treeNodeData );

                    auto actors = currentScene->getActors();
                    for( auto actor : actors )
                    {
                        addActorToTree( actor, rootNode );
                    }

                    auto projectRoot = m_tree->addNode();
                    FB_ASSERT( projectRoot );
                    addObjectToTree( project, projectRoot );
                    Util::setText( projectRoot, "Project" );
                    rootNode->addChild( projectRoot );

                    auto engineRoot = m_tree->addNode();
                    FB_ASSERT( engineRoot );
                    Util::setText( engineRoot, "Engine" );
                    rootNode->addChild( engineRoot );

                    auto children = applicationManager->getChildObjects();
                    for( auto child : children )
                    {
                        addObjectToTree( child, engineRoot );
                    }

                    m_tree->expand( rootNode );
                }
            }

            restoreTreeState();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SceneWindow::addObjectToTree( SmartPtr<ISharedObject> object,
                                       SmartPtr<ui::IUITreeNode> parentNode )
    {
        if( object )
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            auto handle = object->getHandle();
            auto name = handle->getName();

            auto node = m_tree->addNode();
            FB_ASSERT( node );
            Util::setText( node, name );
            parentNode->addChild( node );

            auto data = factoryManager->make_ptr<ProjectTreeData>( name, name, object, object );
            node->setNodeUserData( data );

            auto children = object->getChildObjects();
            for( auto child : children )
            {
                addObjectToTree( child, node );
            }
        }
    }

    void SceneWindow::addActorToTree( SmartPtr<IActor> actor, SmartPtr<ui::IUITreeNode> parentNode )
    {
        try
        {
            if( actor )
            {
                auto actorName = actor->getName();
                if( StringUtil::isNullOrEmpty( actorName ) )
                {
                    actorName = "Untitled";
                }

                auto applicationManager = core::ApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                auto node = m_tree->addNode();
                FB_ASSERT( node );
                Util::setText( node, actorName );

                auto data = factoryManager->make_ptr<ProjectTreeData>( "actor", "actor", actor, actor );
                node->setNodeUserData( data );

                if( parentNode )
                {
                    parentNode->addChild( node );
                }

                if( auto p = actor->getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        if( child )
                        {
                            addActorToTree( child, node );
                        }
                    }
                }

                m_dataArray.emplace_back( data );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SceneWindow::handleWindowClicked()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();
        selectionManager->clearSelection();
    }

    void SceneWindow::handleTreeSelectionChanged( SmartPtr<ui::IUITreeNode> node )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto inputManager = applicationManager->getInputDeviceManager();

        auto timer = applicationManager->getTimer();
        FB_ASSERT( timer );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto data = node->getNodeUserData();
        if( data )
        {
            auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( data );
            auto ownerType = projectTreeData->getOwnerType();
            if( ownerType == "actor" || ownerType == "scene" )
            {
                auto object = projectTreeData->getObjectData();
                auto pObject = object->getSharedFromThis<ISharedObject>();

                if( inputManager->isKeyPressed( KeyCodes::KEY_LCONTROL ) )
                {
                    selectionManager->addSelectedObject( pObject );
                }
                else
                {
                    selectionManager->clearSelection();
                    selectionManager->addSelectedObject( pObject );
                }
            }
            else
            {
                auto object = projectTreeData->getObjectData();

                if( inputManager->isKeyPressed( KeyCodes::KEY_LSHIFT ) )
                {
                    selectionManager->addSelectedObject( object );
                }
                else
                {
                    selectionManager->clearSelection();
                    selectionManager->addSelectedObject( object );
                }
            }
        }

        auto uiManager = editorManager->getUI();
        if( uiManager )
        {
            uiManager->updateSelection();
        }

        static const auto hashType = StringUtil::getHash( "focus_selection" );
        auto stateContext = applicationManager->getStateContext();
        stateContext->triggerEvent( IEvent::Type::UI, hashType, Array<Parameter>(), this, nullptr,
                                    nullptr );

        m_nodeSelectTime = timer->now();
    }

    void SceneWindow::saveTreeState()
    {
        RecursiveMutex::ScopedLock lock( m_treeStateMutex );

        // clear map
        auto treeState = fb::make_shared<std::map<String, bool>>();
        setTreeState( treeState );

        if( auto tree = getTree() )
        {
            if( auto root = tree->getRoot() )
            {
                if( root->isDerived<ui::IUITreeNode>() )
                {
                    saveItemState( nullptr, root );
                }
            }
        }
    }

    void SceneWindow::saveItemState( SmartPtr<ui::IUITreeNode> parent, SmartPtr<ui::IUITreeNode> node )
    {
        // make item name
        auto itemName = String();
        if( auto nodeData = fb::static_pointer_cast<ProjectTreeData>( node->getNodeUserData() ) )
        {
            auto ownerData = nodeData->getOwnerData();
            if( ownerData->isDerived<IActor>() )
            {
                auto actor = fb::static_pointer_cast<IActor>( ownerData );
                if( auto handle = actor->getHandle() )
                {
                    auto uuid = handle->getUUID();
                    itemName = uuid;
                }
            }
        }

        // get expanded state
        bool isExpanded = false;
        if( auto p = node->getChildren() )
        {
            auto &children = *p;
            if( !children.empty() )
            {
                isExpanded = node->isExpanded();
            }
        }

        // add item to map
        if( auto p = getTreeState() )
        {
            auto &treeState = *p;
            treeState.insert( std::map<String, bool>::value_type( itemName, isExpanded ) );
        }

        if( auto p = node->getChildren() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child->isDerived<ui::IUITreeNode>() )
                {
                    saveItemState( node, child );
                }
            }
        }
    }

    void SceneWindow::restoreTreeState()
    {
        RecursiveMutex::ScopedLock lock( m_treeStateMutex );

        if( auto tree = getTree() )
        {
            if( auto root = tree->getRoot() )
            {
                restoreItemState( nullptr, root, false );
            }
        }
    }

    void SceneWindow::restoreItemState( SmartPtr<ui::IUITreeNode> parent, SmartPtr<ui::IUITreeNode> node,
                                        bool parentWasNew )
    {
        //// make item name
        auto itemName = String();

        if( auto nodeData = fb::dynamic_pointer_cast<ProjectTreeData>( node->getNodeUserData() ) )
        {
            auto ownerData = nodeData->getOwnerData();
            if( ownerData->isDerived<IActor>() )
            {
                auto actor = fb::static_pointer_cast<IActor>( ownerData );
                if( auto handle = actor->getHandle() )
                {
                    auto uuid = handle->getUUID();
                    itemName = uuid;
                }
            }
        }

        // get item state from map
        auto state = getItemState( itemName );

        auto isExpanded = false;
        auto showItem = false;

        if( state != TREE_ITEM_STATE_NOT_FOUND )
        {
            isExpanded = ( state == TREE_ITEM_STATE_EXPANDED ) ? true : false;
            parentWasNew = false;
        }
        else
        {
            showItem = parentWasNew ? false : true;
            parentWasNew = true;
        }

        // set item state
        if( isExpanded )
        {
            node->setExpanded( true );
        }

        if( auto p = node->getChildren() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child->isDerived<ui::IUITreeNode>() )
                {
                    restoreItemState( node, child, parentWasNew );
                }
            }
        }
    }

    auto SceneWindow::getItemState( const String &itemName ) const -> s32
    {
        if( auto p = getTreeState() )
        {
            auto &treeState = *p;
            auto it = treeState.find( itemName );
            if( it != treeState.end() )
            {
                auto value = it->second;
                return value ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
            }
        }

        return TREE_ITEM_STATE_NOT_FOUND;
    }

    auto SceneWindow::getSelectedObject() const -> SmartPtr<ISharedObject>
    {
        return m_selectedObject;
    }

    void SceneWindow::setSelectedObject( SmartPtr<ISharedObject> selectedObject )
    {
        m_selectedObject = selectedObject;
    }

    void SceneWindow::deselectAll()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();
        FB_ASSERT( timer );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        if( ( m_nodeSelectTime + 0.2 ) < timer->now() )
        {
            m_tree->clearSelectTreeNodes();

            selectionManager->clearSelection();
        }
    }

    SceneWindow::TreeCtrlListener::TreeCtrlListener() = default;

    SceneWindow::TreeCtrlListener::~TreeCtrlListener() = default;

    auto SceneWindow::TreeCtrlListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                     const Array<Parameter> &arguments,
                                                     SmartPtr<ISharedObject> sender,
                                                     SmartPtr<ISharedObject> object,
                                                     SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleTreeSelectionActivated )
        {
            if( sender == m_owner->m_tree )
            {
                auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );
                m_owner->handleTreeSelectionChanged( node );
            }
        }
        else if( eventValue == IEvent::handleTreeSelectionRelease )
        {
            auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );
            m_owner->handleTreeSelectionChanged( node );
        }
        else if( eventValue == IEvent::handleTreeNodeDoubleClicked )
        {
            auto applicationManager = core::ApplicationManager::instance();

            auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );

            static const auto hashType = StringUtil::getHash( "focus_selection" );
            applicationManager->triggerEvent( IEvent::Type::UI, hashType, Array<Parameter>(), this, this,
                                              nullptr );
        }

        return {};
    }

    auto SceneWindow::TreeCtrlListener::getOwner() const -> SceneWindow *
    {
        return m_owner;
    }

    void SceneWindow::TreeCtrlListener::setOwner( SceneWindow *owner )
    {
        m_owner = owner;
    }

    auto SceneWindow::SceneWindowListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                        const Array<Parameter> &arguments,
                                                        SmartPtr<ISharedObject> sender,
                                                        SmartPtr<ISharedObject> object,
                                                        SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto commandManager = applicationManager->getCommandManager();

        if( eventValue == IEvent::handleSelection )
        {
            auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
            auto menuId = static_cast<MenuId>( element->getElementId() );

            switch( menuId )
            {
            case MenuId::ADD_NEW_ENTITY:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Actor );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_SKYBOX:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Skybox );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_NEW_TERRAIN:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Terrain );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CAMERA:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Camera );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CAR:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Car );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PARTICLESYSTEM:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::ParticleSystem );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PARTICLESYSTEM_SMOKE:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::ParticleSystemSmoke );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PARTICLESYSTEM_SAND:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::ParticleSystemSand );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PLANE:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Plane );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CUBE:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Cube );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CUBE_MESH:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::CubeMesh );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CUBEMAP:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Cubemap );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PHYSICS_CUBE:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::PhysicsCube );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_DIRECTIONAL_LIGHT:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::DirectionalLight );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_POINT_LIGHT:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::PointLight );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_BUTTON:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Button );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_SIMPLE_BUTTON:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Button );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CANVAS:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Canvas );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_CHECKBOX:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Checkbox );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_PANEL:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Panel );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_SLIDER:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Slider );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_TEXT:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::Text );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::ADD_TOGGLE_BUTTON:
            {
                auto cmd = fb::make_ptr<AddActorCmd>();
                cmd->setActorType( AddActorCmd::ActorType::ToggleButton );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::SCENE_REMOVE_ACTOR:
            {
                auto cmd = fb::make_ptr<RemoveSelectionCmd>();
                commandManager->addCommand( cmd );
            }
            break;
            default:
            {
            }
            }
        }
        else if( eventValue == IEvent::handleMouseClicked )
        {
            m_owner->deselectAll();
        }

        return {};
    }

    auto SceneWindow::SceneWindowListener::getOwner() const -> SceneWindow *
    {
        return m_owner;
    }

    void SceneWindow::SceneWindowListener::setOwner( SceneWindow *val )
    {
        m_owner = val;
    }

    auto SceneWindow::DropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                               const Array<Parameter> &arguments,
                                               SmartPtr<ISharedObject> sender,
                                               SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( eventValue == IEvent::handleDrop )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto jobQueue = applicationManager->getJobQueue();

                const auto &text = arguments[0].str;
                auto siblingIndex = arguments[1].getS32();

                auto owner = getOwner();
                auto tree = owner->m_tree;

                auto dropJob = fb::make_ptr<SceneDropJob>();

                dropJob->setOwner( owner );
                dropJob->setData( text );
                dropJob->setSiblingIndex( siblingIndex );
                dropJob->setTree( tree );
                dropJob->setSender( sender );
                jobQueue->addJob( dropJob );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        return {};
    }

    auto SceneWindow::DropTarget::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> src,
                                              SmartPtr<ui::IUIElement> dst, const String &data ) -> bool
    {
        return false;
    }

    auto SceneWindow::DropTarget::getOwner() const -> SceneWindow *
    {
        return m_owner;
    }

    void SceneWindow::DropTarget::setOwner( SceneWindow *owner )
    {
        m_owner = owner;
    }

    auto SceneWindow::DragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                               const Array<Parameter> &arguments,
                                               SmartPtr<ISharedObject> sender,
                                               SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( eventValue == IEvent::handleDrag )
        {
            auto dataStr = handleDrag( Vector2I::zero(), sender );
            return Parameter( dataStr );
        }

        return {};
    }

    auto SceneWindow::DragSource::handleDrag( const Vector2I &position,
                                              SmartPtr<ui::IUIElement> element ) -> String
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        if( element->isDerived<ui::IUITreeNode>() )
        {
            auto treeNode = fb::static_pointer_cast<ui::IUITreeNode>( element );
            auto text = Util::getText( treeNode );

            auto userData = treeNode->getNodeUserData();
            if( userData )
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );
                FB_ASSERT( projectTreeData );

                auto data = fb::make_ptr<Properties>();

                data->setProperty( "sourceId", treeNode->getTreeNodeId() );

                auto treeData = fb::static_pointer_cast<ProjectTreeData>( treeNode->getNodeUserData() );
                auto actor = treeData->getObjectData();

                if( actor )
                {
                    if( auto handle = actor->getHandle() )
                    {
                        auto uuid = handle->getUUID();
                        data->setProperty( "actorId", handle->getInstanceId() );
                        data->setProperty( "actorUUID", uuid );
                    }
                }

                return DataUtil::toString( data.get(), true );
            }
        }

        return "";
    }

    auto SceneWindow::DragSource::getOwner() const -> SceneWindow *
    {
        return m_owner;
    }

    void SceneWindow::DragSource::setOwner( SceneWindow *owner )
    {
        m_owner = owner;
    }

    auto SceneWindow::isValid() const -> bool
    {
        bool valid = true;
        return valid;
    }

    auto SceneWindow::getTree() const -> SmartPtr<ui::IUITreeCtrl>
    {
        return m_tree;
    }

    void SceneWindow::setTree( SmartPtr<ui::IUITreeCtrl> tree )
    {
        m_tree = tree;
    }

    auto SceneWindow::getTreeState() const -> SharedPtr<std::map<String, bool>>
    {
        return m_treeState;
    }

    void SceneWindow::setTreeState( SharedPtr<std::map<String, bool>> treeState )
    {
        m_treeState = treeState;
    }

    auto SceneWindow::getDragDropActorCmd() const -> SmartPtr<ICommand>
    {
        return m_dragDropActorCmd;
    }

    void SceneWindow::setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd )
    {
        m_dragDropActorCmd = dragDropActorCmd;
    }

    SceneWindow::PromptListener::PromptListener() = default;

    SceneWindow::PromptListener::~PromptListener() = default;

    auto SceneWindow::PromptListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                   const Array<Parameter> &arguments,
                                                   SmartPtr<ISharedObject> sender,
                                                   SmartPtr<ISharedObject> object,
                                                   SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handlePropertyChanged )
        {
            auto str = arguments[0].str;

            if( auto owner = getOwner() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto commandManager = applicationManager->getCommandManager();

                auto promptCmd = fb::make_ptr<PromptCmd>();
                promptCmd->setPrompt( str );
                commandManager->addCommand( promptCmd );
            }
        }

        return {};
    }

    auto SceneWindow::PromptListener::getOwner() const -> SceneWindow *
    {
        return m_owner;
    }

    void SceneWindow::PromptListener::setOwner( SceneWindow *owner )
    {
        m_owner = owner;
    }

    SceneWindow::ApplicationEventListener::ApplicationEventListener() = default;

    SceneWindow::ApplicationEventListener::~ApplicationEventListener() = default;

    void SceneWindow::ApplicationEventListener::unload( SmartPtr<ISharedObject> data )
    {
        m_owner = nullptr;
        setLoadingState( LoadingState::Unloaded );
    }

    auto SceneWindow::ApplicationEventListener::handleEvent( IEvent::Type eventType,
                                                             hash_type eventValue,
                                                             const Array<Parameter> &arguments,
                                                             SmartPtr<ISharedObject> sender,
                                                             SmartPtr<ISharedObject> object,
                                                             SmartPtr<IEvent> event ) -> Parameter
    {
        auto task = Thread::getCurrentTask();
        if( task == Thread::Task::Primary )
        {
            if( auto owner = getOwner() )
            {
                if( eventValue == IEvent::addActor )
                {
                    owner->buildTree();
                }
            }
        }

        return {};
    }

    auto SceneWindow::ApplicationEventListener::getOwner() const -> SmartPtr<SceneWindow>
    {
        return m_owner;
    }

    void SceneWindow::ApplicationEventListener::setOwner( SmartPtr<SceneWindow> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
