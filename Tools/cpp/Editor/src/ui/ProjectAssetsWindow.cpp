#include <GameEditorPCH.h>
#include <ui/ProjectAssetsWindow.h>
#include <commands/AddResourceCmd.h>
#include <commands/DragDropActorCmd.h>
#include <commands/AddActorCmd.h>
#include <commands/AddNewScriptCmd.h>
#include <commands/RemoveResourceCmd.h>
#include <ui/FileWindow.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <jobs/FileSelectedJob.h>
#include <FBCore/FBCore.h>

enum
{
    TREE_ITEM_STATE_NOT_FOUND = 0,
    TREE_ITEM_STATE_EXPANDED = 1,
    TREE_ITEM_STATE_NOT_EXPANDED = 2
};

namespace fb::editor
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

            auto applicationManager = core::ApplicationManager::instance();
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

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::AddMaterial ), "Material",
                               "Material" );

            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::AddScript ), "Script",
                               "Add a script" );

            Util::addMenuSeparator( m_applicationAddMenu );
            Util::addMenuItem( m_applicationAddMenu, static_cast<s32>( MenuId::AddTerrainDirector ),
                               "Terrain Resource", "Terrain Resource" );

            m_applicationMenu->addMenuItem( m_applicationAddMenu );

            Util::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::Remove ), "Remove",
                               "Remove" );
            Util::addMenuSeparator( m_applicationMenu );
            Util::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::Import ), "Import",
                               "Import" );
            Util::addMenuItem( m_applicationMenu, static_cast<s32>( MenuId::Reimport ), "Reimport",
                               "Reimport" );

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

                auto applicationManager = core::ApplicationManager::instance();
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

                EditorWindow::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ProjectAssetsWindow::addFolderToTree( SmartPtr<ui::IUITreeNode> parent,
                                               SmartPtr<IFolderExplorer> listing )
        -> SmartPtr<ui::IUITreeNode>
    {
        try
        {
            if( !listing )
            {
                return nullptr;
            }

            if( auto tree = getTree() )
            {
                auto &applicationManager = core::ApplicationManager::instance();
                if( !applicationManager->isRunning() )
                {
                    return nullptr;
                }

                if( applicationManager->getQuit() )
                {
                    return nullptr;
                }

                auto &factoryManager = applicationManager->getFactoryManager();

                auto &fileSystem = applicationManager->getFileSystem();

                auto folderPath = listing->getFolderName();

                auto editorManager = EditorManager::getSingletonPtr();
                auto project = editorManager->getProject();
                auto data =
                    factoryManager->make_ptr<ProjectTreeData>( "folder", folderPath, project, project );

                auto folderName = Path::getFileName( folderPath );

                if( auto node = tree->addNode() )
                {
                    Util::setText( node, folderName );

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
                        auto pathHash = StringUtil::getHash( filePath );

                        auto pFileData = factoryManager->make_ptr<Data<FileInfo>>();

                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( pathHash, fileInfo ) )
                        {
                            pFileData->setData( &fileInfo );
                        }

                        auto data = factoryManager->make_ptr<ProjectTreeData>( "file", filePath,
                                                                               pFileData, pFileData );

                        auto fileName = Path::getFileName( filePath );

                        if( auto fileNode = tree->addNode() )
                        {
                            Util::setText( fileNode, fileName );

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
        auto applicationManager = core::ApplicationManager::instance();
        auto jobQueue = applicationManager->getJobQueue();

        auto job = fb::make_ptr<BuildTreeJob>();
        job->setOwner( this );
        jobQueue->addJob( job );
    }

    void ProjectAssetsWindow::buildTree()
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( auto tree = getTree() )
            {
                tree->clear();

                auto applicationManager = core::ApplicationManager::instance();
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

                    auto sceneName = currentScene->getLabel();

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

    void ProjectAssetsWindow::handleTreeSelectionActivated( SmartPtr<ui::IUITreeNode> node )
    {
        auto applicationManager = core::ApplicationManager::instance();
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
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto jobQueue = applicationManager->getJobQueue();

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
                auto path = projectTreeData->getObjectType();

                auto job = fb::make_ptr<FileSelectedJob>();
                job->setFilePath( path );
                jobQueue->addJob( job );
            }
        }
    }

    void ProjectAssetsWindow::saveTreeState()
    {
    }

    void ProjectAssetsWindow::saveItemState()
    {
    }

    void ProjectAssetsWindow::restoreTreeState()
    {
    }

    auto ProjectAssetsWindow::getParentWindow() const -> SmartPtr<ui::IUIWindow>
    {
        return m_parentWindow;
    }

    void ProjectAssetsWindow::setParentWindow( SmartPtr<ui::IUIWindow> parentWindow )
    {
        m_parentWindow = parentWindow;
    }

    void ProjectAssetsWindow::restoreItemState()
    {
    }

    auto ProjectAssetsWindow::getSelectedPath() const -> String
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

    auto ProjectAssetsWindow::getItemState( String itemName ) -> int
    {
        std::map<String, bool>::iterator it;
        for( it = treeState.begin(); it != treeState.end(); ++it )
        {
            String name = ( *it ).first;
            if( name == itemName )
            {
                return ( *it ).second ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
            }
        }

        return TREE_ITEM_STATE_NOT_FOUND;
    }

    auto ProjectAssetsWindow::TreeCtrlListener::handleEvent( IEvent::Type eventType,
                                                             hash_type eventValue,
                                                             const Array<Parameter> &arguments,
                                                             SmartPtr<ISharedObject> sender,
                                                             SmartPtr<ISharedObject> object,
                                                             SmartPtr<IEvent> event ) -> Parameter
    {
        auto owner = getOwner();

        if( eventValue == IEvent::handleTreeSelectionRelease )
        {
            owner->m_selectedObject = object;

            auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( arguments[0].object );
            auto treeData = fb::static_pointer_cast<ProjectTreeData>( node->getNodeUserData() );
            if( treeData )
            {
                if( treeData->getOwnerType() != "folder" )
                {
                    m_owner->handleTreeSelectionActivated( node );
                }
            }
        }
        else if( eventValue == IEvent::handleTreeNodeDoubleClicked )
        {
            owner->m_selectedObject = object;

            auto node = fb::dynamic_pointer_cast<ui::IUITreeNode>( arguments[0].object );
            m_owner->handleTreeNodeDoubleClicked( node );
        }

        return {};
    }

    auto ProjectAssetsWindow::TreeCtrlListener::getOwner() const -> ProjectAssetsWindow *
    {
        return m_owner;
    }

    void ProjectAssetsWindow::TreeCtrlListener::setOwner( ProjectAssetsWindow *val )
    {
        m_owner = val;
    }

    auto ProjectAssetsWindow::WindowListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                           const Array<Parameter> &arguments,
                                                           SmartPtr<ISharedObject> sender,
                                                           SmartPtr<ISharedObject> object,
                                                           SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto commandManager = applicationManager->getCommandManager();
        FB_ASSERT( commandManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();

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
        case MenuId::AddScript:
        {
            auto filePath = owner->getSelectedPath();

            const auto materialName = String( "NewScript.lua" );

            auto cmd = fb::make_ptr<AddNewScriptCmd>();

            auto materialPath = filePath + "/" + materialName;
            cmd->setPath( materialPath );

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
        break;
        case MenuId::Import:
        {
            auto filePath = owner->getSelectedPath();
            if( Path::isFolder( filePath ) )
            {
                resourceDatabase->importFolder( filePath );
            }
            else
            {
                resourceDatabase->importFile( filePath );
            }
        }
        break;
        case MenuId::Reimport:
        {
            auto filePath = owner->getSelectedPath();
            if( Path::isFolder( filePath ) )
            {
                resourceDatabase->importFolder( filePath );
            }
            else
            {
                resourceDatabase->importFile( filePath );
            }
        }
        break;
        default:
        {
        }
        }

        return {};
    }

    auto ProjectAssetsWindow::WindowListener::getOwner() const -> ProjectAssetsWindow *
    {
        return m_owner;
    }

    void ProjectAssetsWindow::WindowListener::setOwner( ProjectAssetsWindow *val )
    {
        m_owner = val;
    }

    auto ProjectAssetsWindow::DragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                       const Array<Parameter> &arguments,
                                                       SmartPtr<ISharedObject> sender,
                                                       SmartPtr<ISharedObject> object,
                                                       SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleDrag )
        {
            auto str = handleDrag( Vector2I::zero(), sender );
            return Parameter( str );
        }

        return {};
    }

    auto ProjectAssetsWindow::DragSource::handleDrag( const Vector2I &position,
                                                      SmartPtr<ui::IUIElement> element ) -> String
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        if( element->isDerived<ui::IUITreeNode>() )
        {
            auto treeNode = fb::static_pointer_cast<ui::IUITreeNode>( element );
            auto text = Util::getText( treeNode );

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

    auto ProjectAssetsWindow::DragSource::getOwner() const -> ProjectAssetsWindow *
    {
        return m_owner;
    }

    void ProjectAssetsWindow::DragSource::setOwner( ProjectAssetsWindow *val )
    {
        m_owner = val;
    }

    auto ProjectAssetsWindow::isValid() const -> bool
    {
        bool valid = true;
        return valid;
    }

    auto ProjectAssetsWindow::getTree() const -> SmartPtr<ui::IUITreeCtrl>
    {
        return m_tree;
    }

    void ProjectAssetsWindow::setTree( SmartPtr<ui::IUITreeCtrl> tree )
    {
        m_tree = tree;
    }

    auto ProjectAssetsWindow::DropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                       const Array<Parameter> &arguments,
                                                       SmartPtr<ISharedObject> sender,
                                                       SmartPtr<ISharedObject> object,
                                                       SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleDrop )
        {
            Vector2I position;
            SmartPtr<ui::IUIElement> src;
            SmartPtr<ui::IUIElement> dst;
            String dataText = arguments[0].str;

            handleDrop( position, src, dst, dataText );
        }

        return {};
    }

    auto ProjectAssetsWindow::DropTarget::handleDrop( const Vector2I &position,
                                                      SmartPtr<ui::IUIElement> src,
                                                      SmartPtr<ui::IUIElement> dst, const String &text )
        -> bool
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto commandManager = applicationManager->getCommandManager();
            FB_ASSERT( commandManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sceneManager = applicationManager->getSceneManager();

            auto properties = fb::make_ptr<Properties>();
            auto dataStr = String( text.c_str() );

            if( !StringUtil::isNullOrEmpty( dataStr ) )
            {
                DataUtil::parse( dataStr, properties.get() );

                auto filePath = properties->getProperty( "filePath" );

                FileInfo fileInfo;
                if( fileSystem->findFileInfo( filePath, fileInfo ) )
                {
                    filePath = fileInfo.absolutePath;
                }

                auto owner = getOwner();
                auto tree = owner->m_tree;

                auto dragSrc = tree->getDragSourceElement();
                auto dropDst = tree->getDropDestinationElement();

                auto dropProperties = fb::make_ptr<Properties>();
                DataUtil::parse( text, dropProperties.get() );

                u32 actorId = 0;
                dropProperties->getPropertyValue( "actorId", actorId );

                if( auto actor = sceneManager->getActor( actorId ) )
                {
                    if( auto handle = actor->getHandle() )
                    {
                        auto name = handle->getName();

                        auto pActorData = actor->toData();
                        auto actorData = fb::static_pointer_cast<Properties>( pActorData );

                        auto path = applicationManager->getProjectPath();

                        auto treeDropDst = fb::static_pointer_cast<ui::IUITreeNode>( dropDst );
                        if( treeDropDst )
                        {
                            if( auto userData = treeDropDst->getNodeUserData() )
                            {
                                auto projectTreeData =
                                    fb::static_pointer_cast<ProjectTreeData>( userData );
                                if( projectTreeData )
                                {
                                    path = Path::getFilePath( projectTreeData->getObjectType() );
                                }
                            }
                        }

                        auto jsonStr = DataUtil::toString( actorData.get(), true );
                        fileSystem->writeAllText( path + "/" + name + ".prefab", jsonStr );
                        fileSystem->refreshAll( false );
                    }
                }

                owner->build();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return false;
    }

    auto ProjectAssetsWindow::DropTarget::getOwner() const -> ProjectAssetsWindow *
    {
        return m_owner;
    }

    void ProjectAssetsWindow::DropTarget::setOwner( ProjectAssetsWindow *owner )
    {
        m_owner = owner;
    }

    void ProjectAssetsWindow::BuildTreeJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto owner = getOwner() )
        {
            if( applicationManager->isRunning() && !applicationManager->getQuit() )
            {
                owner->buildTree();
            }
        }
    }

    auto ProjectAssetsWindow::BuildTreeJob::getOwner() const -> SmartPtr<ProjectAssetsWindow>
    {
        return m_owner;
    }

    void ProjectAssetsWindow::BuildTreeJob::setOwner( SmartPtr<ProjectAssetsWindow> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
