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
                    m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CAMERA ), "Camera", "Camera" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CAR ),
                                              "Car", "Car" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CUBE ),
                                              "Cube", "Cube" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_CUBEMAP ), "Cubemap",
                                              "Cubemap" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_DIRECTIONAL_LIGHT ),
                                              "Directional Light", "Directional Light" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_POINT_LIGHT ), "Point Light",
                                              "Point Light" );
                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PARTICLESYSTEM ),
                                              "Particle System", "Particle System" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PARTICLESYSTEM_SMOKE ),
                                              "Particle System Smoke", "Particle System Smoke" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PARTICLESYSTEM_SAND ),
                                              "Particle System Sand", "Particle System Sand" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PLANE ), "Plane", "Plane" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PHYSICS_CUBE ),
                                              "Physics Cube", "Physics Cube" );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>( MenuId::ADD_SKYBOX ), "Skybox", "Skybox" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_NEW_TERRAIN ), "Terrain",
                                              "Terrain" );
                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>( MenuId::ADD_BUTTON ), "Button", "Button" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_SIMPLE_BUTTON ),
                                              "Simple Button", "Simple Button" );

                ApplicationUtil::addMenuItem(
                    m_applicationAddMenu, static_cast<s32>( MenuId::ADD_CANVAS ), "Canvas", "Canvas" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::ADD_PANEL ), "Panel", "Panel" );
                ApplicationUtil::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::ADD_TEXT ),
                                              "Text", "Text" );

                m_applicationMenu->addMenuItem( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationMenu,
                                              static_cast<s32>( MenuId::ADD_NEW_ENTITY ), "Add Actor",
                                              "Add Actor" );
                ApplicationUtil::addMenuItem( m_applicationMenu,
                                              static_cast<s32>( MenuId::SCENE_REMOVE_ACTOR ), "Remove",
                                              "Remove" );

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

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto ui = applicationManager->getUI();
                    FB_ASSERT( ui );

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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto editorManager = EditorManager::getSingletonPtr();
                FB_ASSERT( editorManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto currentScene = sceneManager->getCurrentScene();
                FB_ASSERT( currentScene );

                if( currentScene )
                {
                    auto project = editorManager->getProject();
                    FB_ASSERT( project );

                    //auto appTemplate = project->getApplicationTemplate();

                    auto sceneName = currentScene->getName();

                    // ProjectTreeData data("project", "project", project, project);
                    // ProjectTreeData scriptsData(m_applicationMenu, "project", "filter", project,
                    // m_parentFilter);

                    auto rootNode = m_tree->addRoot();
                    if( rootNode )
                    {
                        ApplicationUtil::setText( rootNode, sceneName );
                        rootNode->setExpanded( true );

                        // sceneName.c_str(), -1, -1, new ProjectTreeData(data)

                        auto actors = currentScene->getActors();
                        for( auto actor : actors )
                        {
                            addActorToTree( actor, rootNode );
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

            // try
            //{
            //	FB_ASSERT(m_treeWX);
            //	m_treeWX->DeleteAllItems();

            //	auto applicationManager = core::IApplicationManager::instance();
            //	auto editorManager = EditorManager::getSingletonPtr();

            //	auto sceneManager = applicationManager->getSceneManager();
            //	FB_ASSERT(sceneManager);

            //	auto currentScene = sceneManager->getCurrentScene();
            //	FB_ASSERT(currentScene);

            //	if (currentScene)
            //	{
            //		auto project = editorManager->getProject();
            //		FB_ASSERT(project);

            //		auto appTemplate = project->getApplicationTemplate();

            //		auto sceneName = currentScene->getName();

            //		ProjectTreeData data("project", "project", project, project);
            //		ProjectTreeData scriptsData(m_applicationMenu, "project", "filter", project,
            // m_parentFilter);

            //		auto rootId = m_treeWX->AddRoot(sceneName.c_str(), -1, -1, new
            // ProjectTreeData(data));

            //		auto actors = currentScene->getActors();
            //		for (auto actor : actors)
            //		{
            //			addActorToTree(actor, rootId);
            //		}

            //		m_treeWX->Expand(rootId);
            //	}
            //}
            // catch (std::exception& e)
            //{
            //	wxMessageBox(e.what());
            //}
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

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto factoryManager = applicationManager->getFactoryManager();
                    FB_ASSERT( factoryManager );

                    auto node = m_tree->addNode();
                    FB_ASSERT( node );
                    ApplicationUtil::setText( node, actorName );

                    auto data =
                        factoryManager->make_ptr<ProjectTreeData>( "actor", "actor", actor, actor );
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

                    m_dataArray.push_back( data );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneWindow::handleWindowClicked()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();
            selectionManager->clearSelection();

            // m_tree->UnselectAll();
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
            if( data )
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( data );
                auto ownerType = projectTreeData->getOwnerType();
                if( ownerType == "actor" )
                {
                    auto object = projectTreeData->getObjectData();
                    auto pObject = object->getSharedFromThis<ISharedObject>();

                    selectionManager->clearSelection();
                    selectionManager->addSelectedObject( pObject );
                }
            }

            auto uiManager = editorManager->getUI();
            if( uiManager )
            {
                uiManager->updateSelection();
            }

            static const auto hashType = StringUtil::getHash( "focus_selection" );
            auto stateObject = applicationManager->getStateObject();
            stateObject->triggerEvent( IEvent::Type::UI, hashType, Array<Parameter>(), this, nullptr,
                                       nullptr );

            m_nodeSelectTime = timer->now();

            // try
            //{
            //	FB_ASSERT(m_treeWX);

            //	if(m_treeWX->HasFocus())
            //	{
            //		auto selectedId = event.GetItem ();
            //		if (!selectedId)
            //		{
            //			return;
            //		}

            //		if (selectedId.IsOk())
            //		{
            //			m_lastSelectedItem = selectedId;
            //		}

            //		auto applicationManager = core::IApplicationManager::instance();
            //		auto selectionManager = applicationManager->getSelectionManager();

            //		auto appRoot = EditorManager::getSingletonPtr();
            //		auto data = (ProjectTreeData*)m_treeWX->GetItemData(selectedId);
            //		if(data)
            //		{
            //			if(data->getOwnerType()==("entity"))
            //			{
            //				SmartPtr<EntityTemplate> entityTemplate = data->getOwnerData();
            //				appRoot->getProject()->setSelectedEntityTemplate(entityTemplate);
            //			}
            //			else if(data->getObjectType()==("mesh"))
            //			{
            //				SmartPtr<MeshTemplate> meshTemplate = data->getObjectData();

            //				SmartPtr<SceneViewManager> meshViewManager = appRoot->getSceneViewManager();
            //				meshViewManager->setMeshTemplate(meshTemplate);
            //			}
            //			else if (data->getObjectType() == "actor")
            //			{
            //				auto object = data->getObjectData();
            //				auto pObject = object->getSharedFromThis<ISharedObject>();
            //
            //				selectionManager->clearSelection();
            //				selectionManager->addSelectedObject(pObject);
            //			}
            //			else
            //			{
            //				selectionManager->clearSelection();
            //			}
            //		}
            //		else
            //		{
            //			selectionManager->clearSelection();
            //		}

            //		wxTreeItemId parentId = m_treeWX->GetItemParent(selectedId);
            //		if (!parentId)
            //			return;

            //		wxString parentType = m_treeWX->GetItemText(parentId);
            //		wxString componentName = m_treeWX->GetItemText(selectedId);

            //		EntityTreeItemSelectedPtr msg(new EntityTreeItemSelected);

            //		//msg->setComponentType(parentType.mb_str().data());
            //		//msg->setComponentName(componentName.mb_str().data());

            //		if(data)
            //		{
            //			if (m_selectedObject)
            //			{
            //				//m_selectedObject = (IEditableObject*)data->getObjectData();

            //				const String& editableType = m_selectedObject->getEditableType();
            //				if (editableType == ("EntityTemplate"))
            //				{
            //					m_selectedEntity = m_selectedObject;
            //				}

            //				msg->setSelectedObject(m_selectedObject);
            //			}
            //		}
            //		else
            //		{
            //			m_selectedObject = nullptr;
            //		}

            //		auto messageManager = appRoot->getMessageManager();
            //		if (messageManager)
            //		{
            //			messageManager->postMessage(msg);
            //		}

            //		auto uiManager = appRoot->getUI();
            //		if (uiManager)
            //		{
            //			uiManager->updateSelection();
            //		}
            //	}
            //}
            // catch (std::exception& e)
            //{
            //	wxMessageBox(e.what());
            //}
        }

        void SceneWindow::handleTreeDragStart()
        {
            // auto selectedId = event.GetItem();
            // if (!selectedId)
            //{
            //	return;
            // }

            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto selectionManager = applicationManager->getSelectionManager();
            // FB_ASSERT(selectionManager);

            // data::drag_drop_data data;

            // auto itemData = (ProjectTreeData*)m_treeWX->GetItemData(selectedId);
            // if (itemData)
            //{
            //	//auto ownerType = itemData->getOwnerType();
            //	//if (ownerType == "file")
            //	//{
            //	//	auto path = itemData->getObjectType();
            //	//	data.filePath = path;

            //	//	auto fileSelection = fb::make_ptr<FileSelection>();
            //	//	fileSelection->setFilePath(path);

            //	//	selectionManager->clearSelection();
            //	//	selectionManager->addSelectedObject(fileSelection);
            //	//}
            //}

            ////auto selection = selectionManager->getSelected();
            ////for (auto selected : selection)
            ////{
            ////	if (selected->isExactly<FileSelection>())
            ////	{
            ////		auto fileSelection = fb::static_pointer_cast<FileSelection>(selected);
            ////	}
            ////}

            // auto dataStr = DataUtil::toString(&data, true);

            // wxTextDataObject my_data(dataStr.c_str());

            // wxDropSource dragSource(m_treeWX);
            // dragSource.SetData(my_data);
            // wxDragResult result = dragSource.DoDragDrop(true);
        }

        void SceneWindow::OnActivateItem()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto selectionManager = applicationManager->getSelectionManager();

            // if(m_treeWX->HasFocus())
            //{
            //	wxTreeItemId selectedId = m_treeWX->GetSelection();
            //	if(!selectedId)
            //		return;

            //	wxTreeItemId parentId = m_treeWX->GetItemParent(selectedId);
            //	if(!parentId)
            //		return;

            //	wxString parentType = m_treeWX->GetItemText(parentId);
            //	wxString componentName = m_treeWX->GetItemText(selectedId);

            //	if(parentType.length() == 0 || componentName.length() == 0)
            //		return;

            //	ProjectTreeData* projectTreeData = (ProjectTreeData*)m_treeWX->GetItemData(selectedId);
            //	if(projectTreeData)
            //	{
            //		selectionManager->clearSelection();

            //		if (projectTreeData->getOwnerType() == "Actor" || projectTreeData->getOwnerType() ==
            //"actor")
            //		{
            //			auto actor = fb::static_pointer_cast<IActor>(projectTreeData->getOwnerData());
            //			selectionManager->addSelectedObject(actor);
            //		}

            //		EntityTreeItemActivatedPtr msg(new EntityTreeItemActivated);

            //		String componentType;

            //		String ext = Path::getFileExtension(componentName.mb_str().data());
            //		if(ext==(".lua"))
            //		{
            //			componentType = "Script";
            //		}
            //		else if(ext==(".gui"))
            //		{
            //			componentType = "GUI";

            //			//appRoot->getSceneViewManager()->addOverlay(componentName.mb_str().data());
            //		}
            //		else
            //		{
            //			componentType = parentType.mb_str().data();
            //		}

            //		msg->setComponentType(componentType);
            //		msg->setComponentName(componentName.mb_str().data());
            //		msg->setObject(projectTreeData->getObjectData());
            //		//appRoot->getMessageManager()->postMessage(msg);
            //	}
            //}
        }

        void SceneWindow::addNewActor()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            commandManager->addCommand( cmd );
        }

        void SceneWindow::removeActor()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();

            auto cmd = fb::make_ptr<RemoveSelectionCmd>();
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addSkybox()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Skybox );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addNewTerrain()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Terrain );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addDirectionalLight()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();

            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::DirectionalLight );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addPointLight()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();

            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::PointLight );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addCube()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Cube );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addCubemap()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Cubemap );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addPhysicsCube()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();
            auto project = editorManager->getProject();

            ApplicationUtil::createDefaultGround();

            auto uiManager = editorManager->getUI();
            uiManager->rebuildSceneTree();
        }

        void SceneWindow::addPlane()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();
            auto project = editorManager->getProject();

            ApplicationUtil::createDefaultPlane();

            auto uiManager = editorManager->getUI();
            uiManager->rebuildSceneTree();
        }

        void SceneWindow::addCamera()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();
            auto project = editorManager->getProject();

            ApplicationUtil::createCamera();

            auto uiManager = editorManager->getUI();
            uiManager->rebuildSceneTree();
        }

        void SceneWindow::addCar()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();
            auto project = editorManager->getProject();

            ApplicationUtil::createDefaultVehicle();

            auto uiManager = editorManager->getUI();
            uiManager->rebuildSceneTree();
        }

        void SceneWindow::addButton()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Button );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addSimpleButton()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::SimpleButton );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addCanvas()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Canvas );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addPanel()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Panel );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::addText()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto commandManager = applicationManager->getCommandManager();
            auto cmd = fb::make_ptr<AddActorCmd>();
            cmd->setActorType( AddActorCmd::ActorType::Text );
            commandManager->addCommand( cmd );
        }

        void SceneWindow::beginDrag()
        {
            //// need to explicitly allow drag
            // if (event.GetItem() != m_treeWX->GetRootItem())
            //{
            //	m_draggedItem = event.GetItem();

            //	wxPoint clientpt = event.GetPoint();
            //	wxPoint screenpt = m_treeWX->ClientToScreen(clientpt);

            //	event.Allow();
            //}
            // else
            //{
            //	wxLogMessage("OnBeginDrag: this item can't be dragged.");
            //}
        }

        void SceneWindow::endDrag()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto sceneManager = applicationManager->getSceneManager();
            // FB_ASSERT(applicationManager);

            // auto scene = sceneManager->getCurrentScene();
            // FB_ASSERT(scene);

            // auto itemSrc = m_draggedItem;
            // auto itemDst = event.GetItem();

            // if (itemSrc && itemDst)
            //{
            //	m_draggedItem = (wxTreeItemId)0l;

            //	//// where to copy the item?
            //	//if (itemDst.IsOk() && !m_tree->ItemHasChildren(itemDst))
            //	//{
            //	//	// copy to the parent then
            //	//	itemDst = m_tree->GetItemParent(itemDst);
            //	//}

            //	//if (!itemDst.IsOk())
            //	//{
            //	//	wxLogMessage("OnEndDrag: can't drop here.");

            //	//	return;
            //	//}

            //	wxString text = m_treeWX->GetItemText(itemSrc);
            //	//wxLogMessage("OnEndDrag: '%s' copied to '%s'.",
            //	//	text, GetItemText(itemDst));

            //	// just do append here - we could also insert it just before/after the item
            //	// on which it was dropped, but this requires slightly more work... we also
            //	// completely ignore the client data and icon of the old item but could
            //	// copy them as well.
            //	//
            //	// Finally, we only copy one item here but we might copy the entire tree if
            //	// we were dragging a folder.
            //	//int image = wxGetApp().ShowImages() ? TreeCtrlIcon_File : -1;
            //	wxTreeItemId id = m_treeWX->AppendItem(itemDst, text);

            //	//if (wxGetApp().ShowStates())
            //	//	SetItemState(id, GetItemState(itemSrc));

            //	auto srcData = (ProjectTreeData*)m_treeWX->GetItemData(itemSrc);
            //	auto dstData = (ProjectTreeData*)m_treeWX->GetItemData(itemDst);

            //	if (srcData && dstData)
            //	{
            //		auto objectTypeSrc = srcData->getObjectType();
            //		auto objectTypeDst = dstData->getObjectType();

            //		if (objectTypeSrc == "actor" || objectTypeDst == "actor")
            //		{
            //			auto objectSrc = srcData->getObjectData();
            //			auto pObjectSrc = objectSrc->getSharedFromThis<ISharedObject>();
            //			auto actorSrc = fb::static_pointer_cast<IActor>(pObjectSrc);

            //			auto objectDst = dstData->getObjectData();
            //			auto pObjectDst = objectDst->getSharedFromThis<ISharedObject>();
            //			auto actorDst = fb::static_pointer_cast<IActor>(pObjectDst);
            //			auto projectDst = fb::static_pointer_cast<Project>(pObjectDst);

            //			if (actorSrc && actorDst)
            //			{
            //				auto parent = actorSrc->getParent();
            //				if (parent != nullptr)
            //				{
            //					parent->removeChild(actorSrc);
            //				}
            //				else
            //				{
            //					scene->removeActor(actorSrc);
            //				}

            //				actorDst->addChild(actorSrc);
            //				this->buildTree();
            //			}
            //			else if (actorSrc && projectDst)
            //			{
            //				auto parent = actorSrc->getParent();
            //				if (parent != nullptr)
            //				{
            //					parent->removeChild(actorSrc);
            //					scene->addActor(actorSrc);
            //					this->buildTree();
            //				}
            //			}
            //		}
            //	}
            //}
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

        void SceneWindow::saveItemState( SmartPtr<ui::IUITreeNode> parent,
                                         SmartPtr<ui::IUITreeNode> node )
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

            //get selected state
            //if(m_treeWX->IsSelected(itemId))
            //m_newSelectedItem = itemId;

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

        void SceneWindow::restoreItemState( SmartPtr<ui::IUITreeNode> parent,
                                            SmartPtr<ui::IUITreeNode> node, bool parentWasNew )
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

            //// show item
            // if(showItem)
            //{
            //	m_newSelectedItem = itemId;
            // }

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

        s32 SceneWindow::getItemState( const String &itemName ) const
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

            if( ( m_nodeSelectTime + 0.2 ) < timer->now() )
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
            if( eventValue == IEvent::handleTreeSelectionActivated )
            {
                auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );
                m_owner->handleTreeSelectionChanged( node );
            }
            else if( eventValue == IEvent::handleTreeNodeDoubleClicked )
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
            if( eventValue == IEvent::handleSelection )
            {
                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
                auto menuId = static_cast<MenuId>( element->getElementId() );

                switch( menuId )
                {
                case MenuId::ADD_NEW_ENTITY:
                {
                    m_owner->addNewActor();
                }
                break;
                case MenuId::ADD_SKYBOX:
                {
                    m_owner->addSkybox();
                }
                break;
                case MenuId::ADD_NEW_TERRAIN:
                {
                    m_owner->addNewTerrain();
                }
                break;
                case MenuId::ADD_CAMERA:
                {
                    m_owner->addCamera();
                }
                break;
                case MenuId::ADD_CAR:
                {
                    m_owner->addCar();
                }
                break;
                case MenuId::ADD_PARTICLESYSTEM:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    auto commandManager = applicationManager->getCommandManager();
                    auto cmd = fb::make_ptr<AddActorCmd>();
                    cmd->setActorType( AddActorCmd::ActorType::ParticleSystem );
                    commandManager->addCommand( cmd );
                }
                break;
                case MenuId::ADD_PARTICLESYSTEM_SMOKE:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    auto commandManager = applicationManager->getCommandManager();
                    auto cmd = fb::make_ptr<AddActorCmd>();
                    cmd->setActorType( AddActorCmd::ActorType::ParticleSystemSmoke );
                    commandManager->addCommand( cmd );
                }
                break;
                case MenuId::ADD_PARTICLESYSTEM_SAND:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    auto commandManager = applicationManager->getCommandManager();
                    auto cmd = fb::make_ptr<AddActorCmd>();
                    cmd->setActorType( AddActorCmd::ActorType::ParticleSystemSand );
                    commandManager->addCommand( cmd );
                }
                break;
                case MenuId::ADD_PLANE:
                {
                    m_owner->addPlane();
                }
                break;
                case MenuId::ADD_CUBE:
                {
                    m_owner->addCube();
                }
                break;
                case MenuId::ADD_CUBEMAP:
                {
                    m_owner->addCubemap();
                }
                break;
                case MenuId::ADD_PHYSICS_CUBE:
                {
                    m_owner->addPhysicsCube();
                }
                break;
                case MenuId::ADD_DIRECTIONAL_LIGHT:
                {
                    m_owner->addDirectionalLight();
                }
                break;
                case MenuId::ADD_POINT_LIGHT:
                {
                    m_owner->addPointLight();
                }
                break;
                case MenuId::ADD_BUTTON:
                {
                    m_owner->addButton();
                }
                break;
                case MenuId::ADD_SIMPLE_BUTTON:
                {
                    m_owner->addSimpleButton();
                }
                break;
                case MenuId::ADD_CANVAS:
                {
                    m_owner->addCanvas();
                }
                break;
                case MenuId::ADD_PANEL:
                {
                    m_owner->addPanel();
                }
                break;
                case MenuId::ADD_TEXT:
                {
                    m_owner->addText();
                }
                break;
                case MenuId::SCENE_REMOVE_ACTOR:
                {
                    m_owner->removeActor();
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
            if( eventValue == IEvent::handleDrop )
            {
                try
                {
                    auto text = arguments[0].str;

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto commandManager = applicationManager->getCommandManager();
                    FB_ASSERT( commandManager );

                    auto fileSystem = applicationManager->getFileSystem();
                    FB_ASSERT( fileSystem );

                    auto properties = fb::make_ptr<Properties>();
                    auto dataStr = String( text.c_str() );

                    if( !StringUtil::isNullOrEmpty( dataStr ) )
                    {
                        DataUtil::parse( dataStr, properties.get() );

                        auto filePath = properties->getProperty( "filePath" );

                        auto owner = getOwner();
                        auto tree = owner->m_tree;

                        auto dragSrc = tree->getDragSourceElement();
                        auto dropDst = tree->getDropDestinationElement();

                        if( sender->isDerived<ui::IUIWindow>() )
                        {
                            if( !StringUtil::isNullOrEmpty( filePath ) )
                            {
                                auto cmd = fb::make_ptr<AddActorCmd>();
                                cmd->setActorType( AddActorCmd::ActorType::Actor );
                                cmd->setFilePath( filePath );
                                commandManager->addCommand( cmd );
                            }
                            else
                            {
                                if( !owner->m_dragDropActorCmd )
                                {
                                    auto cmd = fb::make_ptr<DragDropActorCmd>();
                                    cmd->setPosition( Vector2I::zero() );
                                    cmd->setSrc( sender );
                                    cmd->setDst( nullptr );
                                    cmd->setData( text );

                                    commandManager->addCommand( cmd );
                                }

                                owner->m_dragDropActorCmd = nullptr;
                            }
                        }
                        else if( dragSrc )
                        {
                            if( !commandManager->hasCommand( owner->m_dragDropActorCmd ) )
                            {
                                owner->m_dragDropActorCmd = nullptr;
                            }

                            if( !owner->m_dragDropActorCmd )
                            {
                                auto cmd = fb::make_ptr<DragDropActorCmd>();
                                cmd->setPosition( Vector2I::zero() );
                                cmd->setSrc( dragSrc );
                                cmd->setDst( dropDst );
                                cmd->setData( text );
                                owner->m_dragDropActorCmd = cmd;

                                commandManager->addCommand( cmd );
                            }
                        }
                        else if( !StringUtil::isNullOrEmpty( filePath ) )
                        {
                            auto cmd = fb::make_ptr<AddActorCmd>();
                            cmd->setActorType( AddActorCmd::ActorType::Actor );
                            cmd->setFilePath( filePath );
                            commandManager->addCommand( cmd );
                        }
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            return Parameter();
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
            return Parameter();
        }

        String SceneWindow::DragSource::handleDrag( const Vector2I &position,
                                                    SmartPtr<ui::IUIElement> element )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            if( element->isDerived<ui::IUITreeNode>() )
            {
                auto treeNode = fb::static_pointer_cast<ui::IUITreeNode>( element );
                auto text = ApplicationUtil::getText( treeNode );

                auto userData = treeNode->getNodeUserData();
                if( userData )
                {
                    auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );
                    FB_ASSERT( projectTreeData );

                    auto data = fb::make_ptr<Properties>();

                    data->setProperty( "sourceId", treeNode->getTreeNodeId() );

                    SmartPtr<ProjectTreeData> treeData = treeNode->getNodeUserData();
                    SmartPtr<IActor> actor = treeData->getObjectData();

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
    }  // end namespace editor
}  // end namespace fb
