#include <GameEditorPCH.h>
#include <ui/SceneWindow.h>
#include <editor/Project.h>
#include <editor/EditorManager.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <commands/DragDropActorCmd.h>
#include <commands/AddActorCmd.h>
#include <commands/RemoveSelectionCmd.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

#include "jobs/SceneDropJob.h"

#define TREE_ITEM_STATE_NOT_FOUND 0
#define TREE_ITEM_STATE_EXPANDED 1
#define TREE_ITEM_STATE_NOT_EXPANDED 2

namespace fb
{
    namespace editor
    {
        using namespace scene;

        SceneWindow::SceneWindow()
        {
        }

        SceneWindow::SceneWindow( SmartPtr<ui::IUIWindow> parent )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto listener = fb::make_ptr<SceneWindowListener>();
                listener->setOwner( this );
                m_menuListener = listener;

                auto window = ui->addElementByType<ui::IUIWindow>();
                window->setLabel( "Scene" );
                window->setHasBorder( true );
                m_window = window;

                if(parent)
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

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>(MenuId::ADD_CAMERA), "Camera", "Camera" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>(MenuId::ADD_CAR),
                                              "Car", "Car" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>(MenuId::ADD_CUBE),
                                              "Cube", "Cube" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_CUBE_MESH), "Cube Mesh",
                                              "Cube Mesh" );
                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_CUBEMAP), "Cubemap",
                                              "Cubemap" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_DIRECTIONAL_LIGHT),
                                              "Directional Light", "Directional Light" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_POINT_LIGHT), "Point Light",
                                              "Point Light" );
                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PARTICLESYSTEM),
                                              "Particle System", "Particle System" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PARTICLESYSTEM_SMOKE),
                                              "Particle System Smoke", "Particle System Smoke" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PARTICLESYSTEM_SAND),
                                              "Particle System Sand", "Particle System Sand" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PLANE), "Plane", "Plane" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PHYSICS_CUBE),
                                              "Physics Cube", "Physics Cube" );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>(MenuId::ADD_SKYBOX), "Skybox", "Skybox" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_NEW_TERRAIN), "Terrain",
                                              "Terrain" );
                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>(MenuId::ADD_BUTTON), "Button", "Button" );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>(MenuId::ADD_CANVAS), "Canvas", "Canvas" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>(MenuId::ADD_PANEL), "Panel", "Panel" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>(MenuId::ADD_TEXT),
                                              "Text", "Text" );

                m_applicationMenu->addMenuItem( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationMenu,
                                              static_cast<s32>(MenuId::ADD_NEW_ENTITY), "Add Actor",
                                              "Add Actor" );
                ApplicationUtil::addMenuItem( m_applicationMenu,
                                              static_cast<s32>(MenuId::SCENE_REMOVE_ACTOR), "Remove",
                                              "Remove" );

                m_applicationMenu->addObjectListener( m_menuListener );

                sceneWindow->setContextMenu( m_applicationMenu );
            }
            catch(std::exception &e)
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

                setLoadingState( LoadingState::Loaded );
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if(getLoadingState() == LoadingState::Loaded)
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto ui = applicationManager->getUI();
                    FB_ASSERT( ui );

                    if(m_window)
                    {
                        ui->removeElement( m_window );
                        m_window = nullptr;
                    }

                    if(m_sceneWindow)
                    {
                        ui->removeElement( m_sceneWindow );
                        m_sceneWindow = nullptr;
                    }

                    if(m_tree)
                    {
                        if(m_treeListener)
                        {
                            m_tree->removeObjectListener( m_treeListener );
                            m_treeListener = nullptr;
                        }

                        ui->removeElement( m_tree );
                        m_tree = nullptr;
                    }

                    if(m_applicationAddMenu)
                    {
                        ui->removeElement( m_applicationAddMenu );
                        m_applicationAddMenu = nullptr;
                    }

                    if(m_applicationMenu)
                    {
                        ui->removeElement( m_applicationMenu );
                        m_applicationMenu = nullptr;
                    }

                    m_menuListener = nullptr;

                    m_selectedObject = nullptr;
                    m_selectedEntity = nullptr;

                    if(auto parentWindow = getParentWindow())
                    {
                        parentWindow->setContextMenu( nullptr );
                        ui->removeElement( parentWindow );
                        setParentWindow( nullptr );
                    }

                    for(auto data : m_dataArray)
                    {
                        data->unload( nullptr );
                    }

                    m_dataArray.clear();

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch(std::exception &e)
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

                if(m_tree)
                {
                    m_tree->clear();
                }

                for(auto data : m_dataArray)
                {
                    data->unload( nullptr );
                }

                m_dataArray.clear();

                auto applicationManager = core::IApplicationManager::instance();
                auto editorManager = EditorManager::getSingletonPtr();

                auto sceneManager = applicationManager->getSceneManager();
                auto currentScene = sceneManager->getCurrentScene();
                auto project = editorManager->getProject();

                if(currentScene)
                {
                    auto sceneName = currentScene->getName();

                    auto rootNode = m_tree->addRoot();
                    if(rootNode)
                    {
                        ApplicationUtil::setText( rootNode, sceneName );
                        rootNode->setExpanded( true );

                        auto actors = currentScene->getActors();
                        for(auto actor : actors)
                        {
                            addActorToTree( actor, rootNode );
                        }

                        m_tree->expand( rootNode );
                    }
                }

                restoreTreeState();
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneWindow::addActorToTree( SmartPtr<IActor> actor, SmartPtr<ui::IUITreeNode> parentNode )
        {
            try
            {
                if(actor)
                {
                    auto actorName = actor->getName();
                    if(StringUtil::isNullOrEmpty( actorName ))
                    {
                        actorName = "Untitled";
                    }

                    auto applicationManager = core::IApplicationManager::instance();
                    auto factoryManager = applicationManager->getFactoryManager();

                    auto node = m_tree->addNode();
                    FB_ASSERT( node );
                    ApplicationUtil::setText( node, actorName );

                    auto data =
                        factoryManager->make_ptr<ProjectTreeData>( "actor", "actor", actor, actor );
                    node->setNodeUserData( data );

                    if(parentNode)
                    {
                        parentNode->addChild( node );
                    }

                    if(auto p = actor->getChildrenPtr())
                    {
                        auto &children = *p;
                        for(auto child : children)
                        {
                            if(child)
                            {
                                addActorToTree( child, node );
                            }
                        }
                    }

                    m_dataArray.push_back( data );
                }
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneWindow::handleWindowClicked()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();
            selectionManager->clearSelection();
        }

        void SceneWindow::handleTreeSelectionChanged( SmartPtr<ui::IUITreeNode> node )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto data = node->getNodeUserData();
            if(data)
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( data );
                auto ownerType = projectTreeData->getOwnerType();
                if(ownerType == "actor")
                {
                    auto object = projectTreeData->getObjectData();
                    auto pObject = object->getSharedFromThis<ISharedObject>();

                    selectionManager->clearSelection();
                    selectionManager->addSelectedObject( pObject );
                }
            }

            auto uiManager = editorManager->getUI();
            if(uiManager)
            {
                uiManager->updateSelection();
            }

            static const auto hashType = StringUtil::getHash( "focus_selection" );
            auto stateObject = applicationManager->getStateObject();
            stateObject->triggerEvent( IEvent::Type::UI, hashType, Array<Parameter>(), this, nullptr,
                                       nullptr );

            m_nodeSelectTime = timer->now();
        }

        void SceneWindow::saveTreeState()
        {
            RecursiveMutex::ScopedLock lock( m_treeStateMutex );

            // clear map
            auto treeState = fb::make_shared<std::map<String, bool>>();
            setTreeState( treeState );

            if(auto tree = getTree())
            {
                if(auto root = tree->getRoot())
                {
                    if(root->isDerived<ui::IUITreeNode>())
                    {
                        saveItemState( nullptr, root );
                    }
                }
            }
        }

        void SceneWindow::saveItemState( SmartPtr<ui::IUITreeNode> parent,
                                         SmartPtr<ui::IUITreeNode> node )
        {
            // make item name
            auto itemName = String();
            if(auto nodeData = fb::static_pointer_cast<ProjectTreeData>( node->getNodeUserData() ))
            {
                auto ownerData = nodeData->getOwnerData();
                if(ownerData->isDerived<IActor>())
                {
                    auto actor = fb::static_pointer_cast<IActor>( ownerData );
                    if(auto handle = actor->getHandle())
                    {
                        auto uuid = handle->getUUID();
                        itemName = uuid;
                    }
                }
            }

            // get expanded state
            bool isExpanded = false;
            if(auto p = node->getChildren())
            {
                auto &children = *p;
                if(!children.empty())
                {
                    isExpanded = node->isExpanded();
                }
            }

            // add item to map
            if(auto p = getTreeState())
            {
                auto &treeState = *p;
                treeState.insert( std::map<String, bool>::value_type( itemName, isExpanded ) );
            }

            if(auto p = node->getChildren())
            {
                auto &children = *p;
                for(auto &child : children)
                {
                    if(child->isDerived<ui::IUITreeNode>())
                    {
                        saveItemState( node, child );
                    }
                }
            }
        }

        void SceneWindow::restoreTreeState()
        {
            RecursiveMutex::ScopedLock lock( m_treeStateMutex );

            if(auto tree = getTree())
            {
                if(auto root = tree->getRoot())
                {
                    restoreItemState( nullptr, root, false );
                }
            }
        }

        void SceneWindow::restoreItemState( SmartPtr<ui::IUITreeNode> parent,
                                            SmartPtr<ui::IUITreeNode> node, bool parentWasNew )
        {
            //// make item name
            auto itemName = String();

            if(auto nodeData = fb::dynamic_pointer_cast<ProjectTreeData>( node->getNodeUserData() ))
            {
                auto ownerData = nodeData->getOwnerData();
                if(ownerData->isDerived<IActor>())
                {
                    auto actor = fb::static_pointer_cast<IActor>( ownerData );
                    if(auto handle = actor->getHandle())
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

            if(state != TREE_ITEM_STATE_NOT_FOUND)
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
            if(isExpanded)
            {
                node->setExpanded( true );
            }

            if(auto p = node->getChildren())
            {
                auto &children = *p;
                for(auto &child : children)
                {
                    if(child->isDerived<ui::IUITreeNode>())
                    {
                        restoreItemState( node, child, parentWasNew );
                    }
                }
            }
        }

        s32 SceneWindow::getItemState( const String &itemName ) const
        {
            if(auto p = getTreeState())
            {
                auto &treeState = *p;
                auto it = treeState.find( itemName );
                if(it != treeState.end())
                {
                    auto value = it->second;
                    return value ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
                }
            }

            return TREE_ITEM_STATE_NOT_FOUND;
        }

        SmartPtr<ISharedObject> SceneWindow::getSelectedObject() const
        {
            return m_selectedObject;
        }

        void SceneWindow::setSelectedObject( SmartPtr<ISharedObject> val )
        {
            m_selectedObject = val;
        }

        void SceneWindow::deselectAll()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            if(( m_nodeSelectTime + 0.2 ) < timer->now())
            {
                m_tree->clearSelectTreeNodes();

                selectionManager->clearSelection();
            }
        }

        SceneWindow::TreeCtrlListener::TreeCtrlListener()
        {
        }

        SceneWindow::TreeCtrlListener::~TreeCtrlListener()
        {
        }

        Parameter SceneWindow::TreeCtrlListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if(eventValue == IEvent::handleTreeSelectionActivated)
            {
                auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );
                m_owner->handleTreeSelectionChanged( node );
            }
            else if(eventValue == IEvent::handleTreeNodeDoubleClicked)
            {
                auto applicationManager = core::IApplicationManager::instance();

                auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );

                static const auto hashType = StringUtil::getHash( "focus_selection" );
                applicationManager->triggerEvent( IEvent::Type::UI, hashType, Array<Parameter>(), this,
                                                  this, nullptr );
            }

            return Parameter();
        }

        SceneWindow *SceneWindow::TreeCtrlListener::getOwner() const
        {
            return m_owner;
        }

        void SceneWindow::TreeCtrlListener::setOwner( SceneWindow *owner )
        {
            m_owner = owner;
        }

        Parameter SceneWindow::SceneWindowListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();

            if(eventValue == IEvent::handleSelection)
            {
                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
                auto menuId = static_cast<MenuId>(element->getElementId());

                switch(menuId)
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
                case MenuId::ADD_PANEL:
                {
                    auto cmd = fb::make_ptr<AddActorCmd>();
                    cmd->setActorType( AddActorCmd::ActorType::Panel );
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
            else if(eventValue == IEvent::handleMouseClicked)
            {
                m_owner->deselectAll();
            }

            return Parameter();
        }

        SceneWindow *SceneWindow::SceneWindowListener::getOwner() const
        {
            return m_owner;
        }

        void SceneWindow::SceneWindowListener::setOwner( SceneWindow *val )
        {
            m_owner = val;
        }

        Parameter SceneWindow::DropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                        const Array<Parameter> &arguments,
                                                        SmartPtr<ISharedObject> sender,
                                                        SmartPtr<ISharedObject> object,
                                                        SmartPtr<IEvent> event )
        {
            if(eventValue == IEvent::handleDrop)
            {
                try
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    auto jobQueue = applicationManager->getJobQueue();

                    const auto &text = arguments[0].str;

                    auto owner = getOwner();
                    auto tree = owner->m_tree;

                    auto dropJob = fb::make_ptr<SceneDropJob>();

                    dropJob->setOwner( owner );
                    dropJob->setData( text );
                    dropJob->setTree( tree );
                    dropJob->setSender( sender );
                    jobQueue->addJob( dropJob );
                }
                catch(std::exception &e)
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            return Parameter();
        }

        bool SceneWindow::DropTarget::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> src,
                                                  SmartPtr<ui::IUIElement> dst, const String &data )
        {
            return false;
        }

        SceneWindow *SceneWindow::DropTarget::getOwner() const
        {
            return m_owner;
        }

        void SceneWindow::DropTarget::setOwner( SceneWindow *owner )
        {
            m_owner = owner;
        }

        Parameter SceneWindow::DragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                        const Array<Parameter> &arguments,
                                                        SmartPtr<ISharedObject> sender,
                                                        SmartPtr<ISharedObject> object,
                                                        SmartPtr<IEvent> event )
        {
            if(eventValue == IEvent::handleDrag)
            {
                auto dataStr = handleDrag( Vector2I::zero(), sender );
                return Parameter( dataStr );
            }

            return Parameter();
        }

        String SceneWindow::DragSource::handleDrag( const Vector2I &position,
                                                    SmartPtr<ui::IUIElement> element )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();

            if(element->isDerived<ui::IUITreeNode>())
            {
                auto treeNode = fb::static_pointer_cast<ui::IUITreeNode>( element );
                auto text = ApplicationUtil::getText( treeNode );

                auto userData = treeNode->getNodeUserData();
                if(userData)
                {
                    auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );
                    FB_ASSERT( projectTreeData );

                    auto data = fb::make_ptr<Properties>();

                    data->setProperty( "sourceId", treeNode->getTreeNodeId() );

                    auto treeData =
                        fb::static_pointer_cast<ProjectTreeData>( treeNode->getNodeUserData() );
                    auto actor = treeData->getObjectData();

                    if(actor)
                    {
                        if(auto handle = actor->getHandle())
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

        SceneWindow *SceneWindow::DragSource::getOwner() const
        {
            return m_owner;
        }

        void SceneWindow::DragSource::setOwner( SceneWindow *owner )
        {
            m_owner = owner;
        }

        bool SceneWindow::isValid() const
        {
            bool valid = true;
            return valid;
        }

        SmartPtr<ui::IUITreeCtrl> SceneWindow::getTree() const
        {
            return m_tree;
        }

        void SceneWindow::setTree( SmartPtr<ui::IUITreeCtrl> tree )
        {
            m_tree = tree;
        }

        SharedPtr<std::map<String, bool>> SceneWindow::getTreeState() const
        {
            return m_treeState;
        }

        void SceneWindow::setTreeState( SharedPtr<std::map<String, bool>> treeState )
        {
            m_treeState = treeState;
        }

        SmartPtr<ICommand> SceneWindow::getDragDropActorCmd() const
        {
            return m_dragDropActorCmd;
        }

        void SceneWindow::setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd )
        {
            m_dragDropActorCmd = dragDropActorCmd;
        }
    } // end namespace editor
}     // end namespace fb
