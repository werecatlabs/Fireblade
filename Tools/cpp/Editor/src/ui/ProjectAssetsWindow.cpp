#include <GameEditorPCH.h>
#include <ui/ProjectAssetsWindow.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <ui/FileWindow.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <editor/EditorManager.h>
#include <commands/AddResourceCmd.h>
#include <commands/DragDropActorCmd.h>
#include <commands/AddActorCmd.h>
#include <editor/Project.h>

#include "commands/RemoveResourceCmd.h"

#define TREE_ITEM_STATE_NOT_FOUND 0
#define TREE_ITEM_STATE_EXPANDED 1
#define TREE_ITEM_STATE_NOT_EXPANDED 2

namespace fb
{
    namespace editor
    {
        ProjectAssetsWindow::ProjectAssetsWindow()
        {
            try
            {
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        ProjectAssetsWindow::~ProjectAssetsWindow()
        {
            try
            {
                unload( nullptr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ProjectAssetsWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                parentWindow->setLabel( "Assets" );
                parentWindow->setHasBorder( true );
                setParentWindow( parentWindow );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto treeCtrl = ui->addElementByType<ui::IUITreeCtrl>();
                parentWindow->addChild( treeCtrl );
                m_tree = treeCtrl;

                auto dragSource = fb::make_ptr<DragSource>();
                dragSource->setOwner( this );
                treeCtrl->setDragSource( dragSource );

                auto dropTarget = fb::make_ptr<DropTarget>();
                dropTarget->setOwner( this );
                parentWindow->setDropTarget( dropTarget );

                m_tree->setDropTarget( dropTarget );

                auto treeListener = fb::make_ptr<TreeCtrlListener>();
                treeListener->setOwner( this );
                m_treeListener = treeListener;

                m_tree->addObjectListener( treeListener );

                auto listener = fb::make_ptr<WindowListener>();
                listener->setOwner( this );
                m_menuListener = listener;

                m_applicationMenu = ui->addElementByType<ui::IUIMenu>();
                m_applicationMenu->setLabel( "Scene" );

                m_applicationAddMenu = ui->addElementByType<ui::IUIMenu>();
                m_applicationAddMenu->setLabel( "Add" );

                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::AddMaterial ), "Material",
                                              "Material" );

                ApplicationUtil::addMenuSeparator( m_applicationAddMenu );
                ApplicationUtil::addMenuItem( m_applicationAddMenu,
                                              static_cast<s32>( MenuId::AddTerrainDirector ),
                                              "Terrain Resource", "Terrain Resource" );

                m_applicationMenu->addMenuItem( m_applicationAddMenu );

                ApplicationUtil::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::Remove ),
                                              "Remove", "Remove" );

                parentWindow->setContextMenu( m_applicationMenu );

                m_applicationMenu->addObjectListener( m_menuListener );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ProjectAssetsWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                Atomic<LoadingState> unloadedState = LoadingState::Unloaded;

                auto &loadingState = getLoadingState();
                if( loadingState != unloadedState )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto ui = applicationManager->getUI();
                    FB_ASSERT( ui );

                    if( m_tree )
                    {
                        if( m_treeListener )
                        {
                            m_tree->removeObjectListener( m_treeListener );
                        }

                        ui->removeElement( m_tree );
                        m_tree = nullptr;
                    }

                    if( m_treeListener )
                    {
                        m_treeListener->unload( nullptr );
                        m_treeListener = nullptr;
                    }

                    if( m_applicationMenu )
                    {
                        ui->removeElement( m_applicationMenu );
                        m_applicationMenu = nullptr;
                    }

                    if( auto parentWindow = getParentWindow() )
                    {
                        parentWindow->setContextMenu( nullptr );
                        ui->removeElement( parentWindow );
                        setParentWindow( nullptr );
                    }

                    auto dataArray = getData();
                    for( auto data : dataArray )
                    {
                        data->unload( nullptr );
                    }

                    clearData();

                    BaseWindow::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ui::IUITreeNode> ProjectAssetsWindow::addFolderToTree(
            SmartPtr<ui::IUITreeNode> parent, SmartPtr<IFolderExplorer> listing )
        {
            try
            {
                if( !listing )
                {
                    return nullptr;
                }

                if( auto tree = getTree() )
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    if( !applicationManager->isRunning() )
                    {
                        return nullptr;
                    }

                    if( applicationManager->getQuit() )
                    {
                        return nullptr;
                    }

                    auto factoryManager = applicationManager->getFactoryManager();

                    auto fileSystem = applicationManager->getFileSystem();

                    auto folderPath = listing->getFolderName();

#if !FB_FINAL
                    if( StringUtil::contains( folderPath, "Scripts" ) )
                    {
                        int stop = 0;
                        stop = 0;
                    }
#endif

                    auto editorManager = EditorManager::getSingletonPtr();
                    auto project = editorManager->getProject();
                    auto data = fb::make_ptr<ProjectTreeData>( "folder", folderPath, project, project );

                    auto folderName = Path::getFileName( folderPath );

                    if( auto node = tree->addNode() )
                    {
                        ApplicationUtil::setText( node, folderName );

                        node->setNodeUserData( data );

                        if( parent )
                        {
                            parent->addChild( node );
                        }

                        auto projectFolder = applicationManager->getProjectPath();
                        if( StringUtil::isNullOrEmpty( projectFolder ) )
                        {
                            projectFolder = Path::getWorkingDirectory();
                        }

                        const auto &files = listing->getFiles();
                        for( const auto &file : files )
                        {
                            auto filePath = StringUtil::cleanupPath( file );
                            filePath = Path::getRelativePath( projectFolder, filePath );
                            //filePath = Path::lexically_relative( projectFolder, filePath );

                            auto pFileData = fb::make_ptr<Data<FileInfo>>();

                            FileInfo fileInfo;
                            if( fileSystem->findFileInfo( filePath, fileInfo ) )
                            {
                                pFileData->setData( &fileInfo );
                            }

                            auto data = factoryManager->make_ptr<ProjectTreeData>(
                                "file", filePath, pFileData, pFileData );

                            auto fileName = Path::getFileName( filePath );

                            if( auto fileNode = tree->addNode() )
                            {
                                ApplicationUtil::setText( fileNode, fileName );

                                fileNode->setNodeUserData( data );

                                node->addChild( fileNode );
                            }

                            addData( data );
                        }

                        auto subFolders = listing->getSubFolders();
                        for( auto &subFolder : subFolders )
                        {
                            addFolderToTree( node, subFolder );
                        }

                        return node;
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void ProjectAssetsWindow::build()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto jobQueue = applicationManager->getJobQueue();

            auto job = fb::make_ptr<BuildTreeJob>();
            job->setOwner( this );
            jobQueue->queueJob( job );
        }

        void ProjectAssetsWindow::buildTree()
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

                if( auto tree = getTree() )
                {
                    tree->clear();

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto fileSystem = applicationManager->getFileSystem();

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

                        auto rootNode = tree->addRoot();
                        FB_ASSERT( rootNode );
                        rootNode->setExpanded( true );

                        auto projectFolder = applicationManager->getProjectPath();
                        if( StringUtil::isNullOrEmpty( projectFolder ) )
                        {
                            projectFolder = Path::getWorkingDirectory();
                        }

                        auto assetsFolder = projectFolder + "/Assets";
                        if( !fileSystem->isExistingFolder( assetsFolder ) )
                        {
                            assetsFolder = projectFolder;
                        }

                        if( !StringUtil::isNullOrEmpty( assetsFolder ) )
                        {
                            if( auto folderListing = fileSystem->getFolderListing( assetsFolder ) )
                            {
                                auto node = addFolderToTree( rootNode, folderListing );
                                if( node )
                                {
                                    node->setExpanded( true );
                                }
                            }
                        }

                        tree->expand( rootNode );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ProjectAssetsWindow::handleTreeSelectionChanged()
        {
            // try
            //{
            //	//if(m_tree->HasFocus())
            //	{
            //		wxTreeItemId selectedId = event.GetItem();
            //		if (!selectedId)
            //		{
            //			return;
            //		}

            //		if (selectedId.IsOk())
            //		{
            //			m_lastSelectedItem = selectedId;
            //		}

            //		auto editorManager = EditorManager::getSingletonPtr();
            //		auto ui = editorManager->getUI();

            //		auto project = editorManager->getProject();
            //		auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
            //		if (data)
            //		{
            //			if (data->getOwnerType() == ("entity"))
            //			{
            //				SmartPtr<EntityTemplate> entityTemplate = data->getOwnerData();
            //				editorManager->getProject()->setSelectedEntityTemplate(entityTemplate);
            //			}
            //			else if (data->getOwnerType() == "folder")
            //			{
            //				auto path = data->getObjectType();
            //				project->setSelectedProjectPath(path);

            //				auto fileWindow = ui->getFileWindow();
            //				if (fileWindow)
            //				{
            //					fileWindow->buildTree();
            //				}
            //			}
            //			else if (data->getObjectType() == ("mesh"))
            //			{
            //				SmartPtr<MeshTemplate> meshTemplate = data->getObjectData();

            //				SmartPtr<SceneViewManager> meshViewManager =
            // editorManager->getSceneViewManager();
            // meshViewManager->setMeshTemplate(meshTemplate);
            //			}
            //			else
            //			{
            //				editorManager->getProject()->setSelectedEntityTemplate(nullptr);
            //			}
            //		}

            //		//wxTreeItemId parentId = m_tree->GetItemParent(selectedId);
            //		//if(!parentId)
            //		//	return;

            //		//wxString parentType = m_tree->GetItemText(parentId);
            //		//wxString componentName = m_tree->GetItemText(selectedId);

            //		EntityTreeItemSelectedPtr msg(new EntityTreeItemSelected);

            //		//msg->setComponentType(parentType.mb_str().data());
            //		//msg->setComponentName(componentName.mb_str().data());

            //		if (data)
            //		{
            //			m_selectedObject = data->getObjectData();

            //			const String& editableType = m_selectedObject->getEditableType();
            //			if (editableType == ("EntityTemplate"))
            //			{
            //				m_selectedEntity = m_selectedObject;
            //			}

            //			msg->setSelectedObject(m_selectedObject);
            //		}
            //		else
            //		{
            //			m_selectedObject = nullptr;
            //		}

            //		editorManager->getMessageManager()->postMessage(msg);
            //	}
            //}
            // catch (Exception& e)
            //{
            //	wxMessageBox(e.what());
            //}
            // catch (std::exception& e)
            //{
            //	wxMessageBox(e.what());
            //}
        }

        void ProjectAssetsWindow::handleTreeSelectionActivated( SmartPtr<ui::IUITreeNode> node )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto editorManager = EditorManager::getSingletonPtr();
            auto project = editorManager->getProject();

            auto data = node->getNodeUserData();
            if( data )
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( data );
                auto ownerType = projectTreeData->getOwnerType();
                if( ownerType == "file" )
                {
                    auto path = projectTreeData->getObjectType();

                    auto fileSelection = fb::make_ptr<FileSelection>();
                    fileSelection->setFilePath( path );

                    auto data = fb::static_pointer_cast<IData>( projectTreeData->getObjectData() );
                    if( data )
                    {
                        auto fileInfo = data->getDataAsType<FileInfo>();
                        if( fileInfo )
                        {
                            fileSelection->setFileInfo( *fileInfo );
                        }
                    }

                    selectionManager->clearSelection();
                    selectionManager->addSelectedObject( fileSelection );
                }
            }

            auto ui = editorManager->getUI();
            ui->updateSelection();
        }

        void ProjectAssetsWindow::handleTreeNodeDoubleClicked( SmartPtr<ui::IUITreeNode> node )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto taskManager = applicationManager->getTaskManager();
            FB_ASSERT( taskManager );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto project = editorManager->getProject();
            FB_ASSERT( project );

            auto uiManager = editorManager->getUI();
            FB_ASSERT( uiManager );

            auto data = node->getNodeUserData();
            if( data )
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( data );
                auto ownerType = projectTreeData->getOwnerType();
                if( ownerType == "file" )
                {
                    auto renderLock = taskManager->lockTask( Thread::Task::Render );
                    auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

                    auto path = projectTreeData->getObjectType();

                    auto projectPath = applicationManager->getProjectPath();
                    if( StringUtil::isNullOrEmpty( projectPath ) )
                    {
                        projectPath = Path::getWorkingDirectory();
                    }

                    auto scenePath = Path::lexically_normal( projectPath, path );
                    scenePath = StringUtil::cleanupPath( scenePath );
                    project->setCurrentScenePath( scenePath );

                    auto sceneManager = applicationManager->getSceneManager();
                    auto scene = sceneManager->getCurrentScene();
                    if( scene )
                    {
                        scene->clear();
                        scene->loadScene( scenePath );
                    }

                    sceneManager->edit();
                }
            }

            uiManager->rebuildSceneTree();
        }

        void ProjectAssetsWindow::OnContextMenu()
        {
            // wxTreeItemId itemId = event.GetItem();

            //// select the item so the context menu functions works fine
            // if (itemId.IsOk())
            //	m_tree->SelectItem(itemId);

            // ProjectTreeData* data = (ProjectTreeData*)m_tree->GetItemData(itemId);
            // if (data != NULL)
            //{
            //	wxMenu* menu = data->getContextMenu();
            //	if (menu)
            //		m_tree->PopupMenu(menu);
            //	else
            //		m_tree->PopupMenu(m_defaultMenu);
            // }
            // else
            //{
            //	m_tree->PopupMenu(m_defaultMenu);
            // }

            // event.Skip();
        }

        void ProjectAssetsWindow::OnActivateItem()
        {
            // if (m_tree->HasFocus())
            //{
            //	wxTreeItemId selectedId = m_tree->GetSelection();
            //	if (!selectedId)
            //		return;

            //	wxTreeItemId parentId = m_tree->GetItemParent(selectedId);
            //	if (!parentId)
            //		return;

            //	wxString parentType = m_tree->GetItemText(parentId);
            //	wxString componentName = m_tree->GetItemText(selectedId);

            //	if (parentType.length() == 0 || componentName.length() == 0)
            //		return;

            //	ProjectTreeData* projectTreeData = (ProjectTreeData*)m_tree->GetItemData(selectedId);
            //	if (projectTreeData)
            //	{
            //		auto appRoot = IApplicationManager::instance();
            //		EntityTreeItemActivatedPtr msg(new EntityTreeItemActivated);

            //		String componentType;

            //		String ext = Path::getFileExtension(componentName.mb_str().data());
            //		if (ext == (".lua"))
            //		{
            //			componentType = "Script";
            //		}
            //		else if (ext == (".gui"))
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

        void ProjectAssetsWindow::saveTreeState()
        {
            //// clear map
            // treeState.clear();

            //// reset selected item
            // m_newSelectedItem = NULL;

            // wxTreeItemId itemId = m_tree->GetRootItem();
            // String parent = "";
            // if (itemId)
            //	saveItemState(parent, itemId);
        }

        void ProjectAssetsWindow::saveItemState()
        {
            //// make item name
            // String itemName = String(m_tree->GetItemText(itemId));
            // if (parent != "")
            //	itemName = parent + "/" + itemName;

            //// get expanded state
            // bool isExpanded = false;
            // if (m_tree->ItemHasChildren(itemId))
            //	isExpanded = m_tree->IsExpanded(itemId);

            ////get selected state
            // if (m_tree->IsSelected(itemId))
            //	m_newSelectedItem = itemId;

            //// add item to map
            // treeState.insert(std::map<String, bool>::value_type(itemName, isExpanded));

            //// parse childes
            // wxTreeItemIdValue cookie;
            // wxTreeItemId childrenItem = m_tree->GetFirstChild(itemId, cookie);
            // while (childrenItem.IsOk())
            //{
            //	saveItemState(itemName, childrenItem);
            //	childrenItem = m_tree->GetNextChild(itemId, cookie);
            // }
        }

        void ProjectAssetsWindow::restoreTreeState()
        {
            // wxTreeItemId itemId = m_tree->GetRootItem();

            //// restore items
            // String parent = "";
            // if (itemId)
            //	restoreItemState(parent, itemId, false);

            // if (m_newSelectedItem)
            //{
            //	m_tree->EnsureVisible(m_newSelectedItem);
            //	m_tree->SelectItem(m_newSelectedItem);
            // }
        }

        SmartPtr<ui::IUIWindow> ProjectAssetsWindow::getParentWindow() const
        {
            return m_parentWindow;
        }

        void ProjectAssetsWindow::setParentWindow( SmartPtr<ui::IUIWindow> parentWindow )
        {
            m_parentWindow = parentWindow;
        }

        void ProjectAssetsWindow::restoreItemState()
        {
            //// make item name
            // String itemName = String(m_tree->GetItemText(itemId));
            // if (parent != "")
            //	itemName = parent + "/" + itemName;

            //// get item state from map
            // int state = getItemState(itemName);

            // bool isExpanded = false;
            // bool showItem = false;
            // if (state != TREE_ITEM_STATE_NOT_FOUND)
            //{
            //	isExpanded = (state == TREE_ITEM_STATE_EXPANDED) ? true : false;
            //	parentWasNew = false;
            // }
            // else
            //{
            //	showItem = parentWasNew ? false : true;
            //	parentWasNew = true;
            // }

            //// set item state
            // if (isExpanded)
            //	m_tree->Expand(itemId);

            //// show item
            // if (showItem)
            //{
            //	m_newSelectedItem = itemId;
            // }

            //// parse childes
            // wxTreeItemIdValue cookie;
            // wxTreeItemId childrenItem = m_tree->GetFirstChild(itemId, cookie);
            // while (childrenItem.IsOk())
            //{
            //	restoreItemState(itemName, childrenItem, parentWasNew);
            //	childrenItem = m_tree->GetNextChild(itemId, cookie);
            // }
        }

        String ProjectAssetsWindow::getSelectedPath() const
        {
            if( m_selectedObject )
            {
                auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( m_selectedObject );
                auto userData = node->getNodeUserData();
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );

                if( projectTreeData )
                {
                    return projectTreeData->getObjectType();
                }
            }

            return "";
        }

        int ProjectAssetsWindow::getItemState( String itemName )
        {
            std::map<String, bool>::iterator it;
            for( it = treeState.begin(); it != treeState.end(); ++it )
            {
                String name = ( *it ).first;
                if( name == itemName )
                    return ( *it ).second ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
            }

            return TREE_ITEM_STATE_NOT_FOUND;
        }

        ProjectAssetsWindow::TreeCtrlListener::TreeCtrlListener()
        {
        }

        ProjectAssetsWindow::TreeCtrlListener::~TreeCtrlListener()
        {
        }

        Parameter ProjectAssetsWindow::TreeCtrlListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto owner = getOwner();

            if( eventValue == IEvent::handleTreeSelectionActivated )
            {
                owner->m_selectedObject = object;

                auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( arguments[0].object );
                m_owner->handleTreeSelectionActivated( node );
            }
            else if( eventValue == IEvent::handleTreeNodeDoubleClicked )
            {
                owner->m_selectedObject = object;

                auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( arguments[0].object );
                m_owner->handleTreeNodeDoubleClicked( node );
            }

            return Parameter();
        }

        ProjectAssetsWindow *ProjectAssetsWindow::TreeCtrlListener::getOwner() const
        {
            return m_owner;
        }

        void ProjectAssetsWindow::TreeCtrlListener::setOwner( ProjectAssetsWindow *val )
        {
            m_owner = val;
        }

        Parameter ProjectAssetsWindow::WindowListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto commandManager = applicationManager->getCommandManager();
            FB_ASSERT( commandManager );

            auto owner = getOwner();

            auto element = fb::static_pointer_cast<ui::IUIElement>( object );
            auto elementId = element->getElementId();
            auto menuId = static_cast<MenuId>( elementId );
            switch( menuId )
            {
            case MenuId::AddMaterial:
            {
                auto filePath = owner->getSelectedPath();

                const auto materialName = String( "NewMaterial.mat" );

                auto cmd = fb::make_ptr<AddResourceCmd>();

                auto materialPath = filePath + "/" + materialName;
                cmd->setFilePath( materialPath );

                cmd->setResourceType( AddResourceCmd::ResourceType::Material );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::AddTerrainDirector:
            {
                auto filePath = owner->getSelectedPath();

                const auto materialName = String( "NewTerrain.resource" );

                auto cmd = fb::make_ptr<AddResourceCmd>();

                auto materialPath = filePath + "/" + materialName;
                cmd->setFilePath( materialPath );

                cmd->setResourceType( AddResourceCmd::ResourceType::Director );
                commandManager->addCommand( cmd );
            }
            break;
            case MenuId::Remove:
            {
                auto filePath = owner->getSelectedPath();
                auto cmd = fb::make_ptr<RemoveResourceCmd>();
                cmd->setFilePath( filePath );
                commandManager->addCommand( cmd );
            }
            default:
            {
            }
            }

            return Parameter();
        }

        ProjectAssetsWindow *ProjectAssetsWindow::WindowListener::getOwner() const
        {
            return m_owner;
        }

        void ProjectAssetsWindow::WindowListener::setOwner( ProjectAssetsWindow *val )
        {
            m_owner = val;
        }

        ProjectAssetsWindow::DragSource::DragSource()
        {
        }

        ProjectAssetsWindow::DragSource::~DragSource()
        {
        }

        Parameter ProjectAssetsWindow::DragSource::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        String ProjectAssetsWindow::DragSource::handleDrag( const Vector2I &position,
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
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );
                if( projectTreeData )
                {
                    auto properties = fb::make_ptr<Properties>();

                    auto ownerType = projectTreeData->getOwnerType();
                    if( ownerType == "file" )
                    {
                        auto path = projectTreeData->getObjectType();
                        properties->setProperty( "filePath", path );

                        auto fileSelection = fb::make_ptr<FileSelection>();
                        fileSelection->setFilePath( path );

                        auto data = fb::static_pointer_cast<IData>( projectTreeData->getObjectData() );
                        auto fileInfo = data->getDataAsType<FileInfo>();
                        fileSelection->setFileInfo( *fileInfo );

                        selectionManager->clearSelection();
                        selectionManager->addSelectedObject( fileSelection );
                    }

                    auto selection = selectionManager->getSelection();
                    for( auto selected : selection )
                    {
                        if( selected->isExactly<FileSelection>() )
                        {
                            auto fileSelection = fb::static_pointer_cast<FileSelection>( selected );
                        }
                    }

                    return DataUtil::toString( properties.get(), true );
                }
            }

            return String( "" );
        }

        ProjectAssetsWindow *ProjectAssetsWindow::DragSource::getOwner() const
        {
            return m_owner;
        }

        void ProjectAssetsWindow::DragSource::setOwner( ProjectAssetsWindow *val )
        {
            m_owner = val;
        }

        bool ProjectAssetsWindow::isValid() const
        {
            bool valid = true;
            return valid;
        }

        SmartPtr<ui::IUITreeCtrl> ProjectAssetsWindow::getTree() const
        {
            return m_tree;
        }

        void ProjectAssetsWindow::setTree( SmartPtr<ui::IUITreeCtrl> tree )
        {
            m_tree = tree;
        }

        Parameter ProjectAssetsWindow::DropTarget::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        bool ProjectAssetsWindow::DropTarget::handleDrop( const Vector2I &position,
                                                          SmartPtr<ui::IUIElement> src,
                                                          SmartPtr<ui::IUIElement> dst,
                                                          const String &text )
        {
            //try
            //{
            //    auto applicationManager = core::IApplicationManager::instance();
            //    FB_ASSERT( applicationManager );

            //    auto commandManager = applicationManager->getCommandManager();
            //    FB_ASSERT( commandManager );

            //    auto fileSystem = applicationManager->getFileSystem();
            //    FB_ASSERT( fileSystem );

            //    auto sceneManager = applicationManager->getSceneManager();

            //    data::drag_drop_data data;
            //    auto dataStr = String( text.c_str() );

            //    if( !StringUtil::isNullOrEmpty( dataStr ) )
            //    {
            //        DataUtil::parse( dataStr, &data );

            //        auto filePath = data.filePath;

            //        FileInfo fileInfo;
            //        if( fileSystem->findFileInfo( data.filePath, fileInfo ) )
            //        {
            //            filePath = fileInfo.absolutePath;
            //        }

            //        /*
            //    SmartPtr<IData> pFileData;

            //    if (src)
            //    {
            //        if (src->isDerived<ui::IUITreeNode>())
            //        {
            //            auto pTreeNode = fb::static_pointer_cast<ui::IUITreeNode>(src);
            //            pFileData = pTreeNode->getNodeUserData();
            //        }
            //    }

            //    auto fileInfo = pFileData->getDataAsType<FileInfo>();
            //    auto filePath = StringUtil::cleanupPath( fileInfo->absolutePath );
            //    if (StringUtil::isNullOrEmpty(filePath))
            //    {
            //        filePath = data.filePath;
            //    }
            //    */

            //        auto owner = getOwner();
            //        auto tree = owner->m_tree;

            //        auto dragSrc = tree->getDragSourceElement();
            //        auto dropDst = tree->getDropDestinationElement();

            //        //if( dragSrc && dropDst )
            //        {
            //            data::drag_drop_data data;
            //            DataUtil::parse( text, &data );

            //            if( auto actor = sceneManager->getActor( data.actorId ) )
            //            {
            //                if( auto handle = actor->getHandle() )
            //                {
            //                    auto name = handle->getName();

            //                    auto actorData = actor->toData();
            //                    auto pActorData = actorData->getDataAsType<data::actor_data>();

            //                    auto path = applicationManager->getProjectPath();

            //                    auto treeDropDst = fb::static_pointer_cast<ui::IUITreeNode>( dropDst );
            //                    if( auto userData = treeDropDst->getNodeUserData() )
            //                    {
            //                        auto projectTreeData =
            //                            fb::static_pointer_cast<ProjectTreeData>( userData );
            //                        if( projectTreeData )
            //                        {
            //                            path = Path::getFilePath( projectTreeData->getObjectType() );
            //                        }
            //                    }

            //                    auto jsonStr = DataUtil::toString( pActorData, true );
            //                    fileSystem->writeAllText( path + "/" + name + ".prefab", jsonStr );
            //                    fileSystem->refreshAll( false );
            //                }
            //            }

            //            //auto cmd = fb::make_ptr<DragDropActorCmd>();
            //            //cmd->setPosition( position );
            //            //cmd->setSrc( dragSrc );
            //            //cmd->setDst( dropDst );
            //            //cmd->setData( text );

            //            //commandManager->addCommand( cmd );
            //        }
            //        //else if( !StringUtil::isNullOrEmpty( filePath ) )
            //        //{
            //        //    auto cmd = fb::make_ptr<AddActorCmd>();
            //        //    cmd->setActorType( AddActorCmd::ActorType::Actor );
            //        //    cmd->setFilePath( filePath );
            //        //    commandManager->addCommand( cmd );
            //        //}

            //        owner->build();
            //    }
            //}
            //catch( std::exception &e )
            //{
            //    FB_LOG_EXCEPTION( e );
            //}

            return false;
        }

        ProjectAssetsWindow *ProjectAssetsWindow::DropTarget::getOwner() const
        {
            return m_owner;
        }

        void ProjectAssetsWindow::DropTarget::setOwner( ProjectAssetsWindow *val )
        {
            m_owner = val;
        }

        ProjectAssetsWindow::BuildTreeJob::BuildTreeJob()
        {
        }

        ProjectAssetsWindow::BuildTreeJob::~BuildTreeJob()
        {
        }

        void ProjectAssetsWindow::BuildTreeJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isRunning() && !applicationManager->getQuit() )
            {
                m_owner->buildTree();
            }
        }

        ProjectAssetsWindow *ProjectAssetsWindow::BuildTreeJob::getOwner() const
        {
            return m_owner;
        }

        void ProjectAssetsWindow::BuildTreeJob::setOwner( ProjectAssetsWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
