#include <GameEditorPCH.h>
#include <ui/UIManager.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <commands/FileMenuCmd.h>
#include <commands/ToggleEditorCamera.h>
#include <jobs/AssetDatabaseBuildJob.h>
#include <jobs/AssetImportJob.h>
#include <jobs/CopyEngineFilesJob.h>
#include <jobs/CreatePluginCodeJob.h>
#include <jobs/GenerateSkybox.h>
#include <jobs/JobCreatePackage.h>
#include <jobs/OptimiseDatabasesJob.h>
#include <jobs/ProjectCleanJob.h>
#include <jobs/SetupMaterialJob.h>
#include <jobs/ReloadScriptsJob.h>
#include <jobs/PlaymodeJob.h>
#include <jobs/LeavePlaymodeJob.h>
#include <jobs/ImportUnityYaml.h>
#include <jobs/LoadPluginJob.h>
#include <jobs/SaveSceneJob.h>
#include <jobs/UnloadPluginJob.h>
#include <ui/AboutDialog.h>
#include <ui/ActorWindow.h>
#include <ui/ProjectWindow.h>
#include <ui/TerrainWindow.h>
#include <ui/SceneWindow.h>
#include <ui/PropertiesWindow.h>
#include <ui/MaterialWindow.h>
#include "ui/ProfilerWindow.h"
#include <ui/ObjectWindow.h>
#include <ui/ObjectBrowserDialog.h>
#include <ui/ResourceDatabaseDialog.h>
#include <ui/InputManagerWindow.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    UIManager::UIManager() = default;

    UIManager::~UIManager()
    {
        unload( nullptr );
    }

    void UIManager::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        m_frameStatistics = factoryManager->make_ptr<FrameStatistics>();
        //m_frameStatistics->load( nullptr );
        m_frameStatistics->setVisible( false );
        graphicsSystem->loadObject( m_frameStatistics, true );

        auto menuBar = ui->addElementByType<ui::IUIMenubar>();

        auto fileMenu = ui->addElementByType<ui::IUIMenu>();
        fileMenu->setLabel( "File" );

        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::NewProjectId ), "New Project",
                           "Create a project", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::OpenProjectId ), "Open Project",
                           "Open a project", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveProjectId ), "Save Project",
                           "Save a project", ui::IUIMenuItem::Type::Normal );
        Util::addMenuSeparator( fileMenu );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::NewSceneId ), "New Scene", "New Scene",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::OpenSceneId ), "Open Scene",
                           "Open Scene", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveId ), "Save Scene", "Save Scene",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveSceneAsId ), "Save Scene As",
                           "Save Scene As", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveAllId ), "Save All", "Save All",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuSeparator( fileMenu );
        Util::addMenuItem( fileMenu, static_cast<s32>( UIManager::WidgetId::GenerateCMakeProjectId ),
                           "Generate CMake Project", "Generate CMake Project",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( fileMenu, static_cast<s32>( UIManager::WidgetId::CompileId ), "Compile",
                           "Compile", ui::IUIMenuItem::Type::Normal );
        Util::addMenuSeparator( fileMenu );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::CreatePackageId ), "Create Package",
                           "Create Package", ui::IUIMenuItem::Type::Normal );
        //Util::addMenuSeparator( fileMenu );

        //Util::addMenuItem(
        //    fileMenu, static_cast<s32>( UIManager::WidgetId::CreateUnityBindings ),
        //    "Create Unity Bindings", "Create Unity Bindings", ui::IUIMenuItem::Type::Normal );
        //Util::addMenuSeparator( fileMenu );

        //auto recentFilesMenu = ui->addElementByType<ui::IUIMenu>();
        //recentFilesMenu->setLabel( "Recent Files" );
        //Util::addMenuItem( recentFilesMenu,
        //                              static_cast<s32>( UIManager::WidgetId::LoadProceduralSceneId ),
        //                              "File", "File", ui::IUIMenuItem::Type::Normal );
        //fileMenu->addMenuItem( recentFilesMenu );

        //auto recentProjectsMenu = ui->addElementByType<ui::IUIMenu>();
        //recentProjectsMenu->setLabel( "Recent Projects" );
        //Util::addMenuItem( recentProjectsMenu,
        //                              static_cast<s32>( UIManager::WidgetId::LoadProceduralSceneId ),
        //                              "File", "File", ui::IUIMenuItem::Type::Normal );
        //fileMenu->addMenuItem( recentProjectsMenu );

        Util::addMenuSeparator( fileMenu );
        Util::addMenuItem( fileMenu, static_cast<s32>( WidgetId::Exit ), "Exit", "Quit this program",
                           ui::IUIMenuItem::Type::Normal );

        menuBar->addMenu( fileMenu );

        auto editMenu = ui->addElementByType<ui::IUIMenu>();
        editMenu->setLabel( "Edit" );
        Util::addMenuItem( editMenu, static_cast<s32>( WidgetId::UndoId ), "Undo", "Undo command",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( editMenu, static_cast<s32>( WidgetId::RedoId ), "Redo", "Redo command",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuSeparator( editMenu );
        menuBar->addMenu( editMenu );

        auto assetsMenu = ui->addElementByType<ui::IUIMenu>();
        assetsMenu->setLabel( "Assets" );
        Util::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetImportId ), "Import", "Import",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetReimportId ), "Reimport",
                           "Reimport", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetDatabaseBuildId ),
                           "Build Database", "Build Database", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetDatabaseImportCacheId ),
                           "Import Cache", "Import Cache", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetDatabaseDeleteCacheId ),
                           "Delete Cache", "Delete Cache", ui::IUIMenuItem::Type::Normal );
        menuBar->addMenu( assetsMenu );

        auto utilMenu = ui->addElementByType<ui::IUIMenu>();
        utilMenu->setLabel( "Util" );

        auto utilImportMenu = ui->addElementByType<ui::IUIMenu>();
        utilImportMenu->setLabel( "Import" );
        Util::addMenuItem( utilImportMenu, static_cast<s32>( WidgetId::ImportJsonSceneId ),
                           "Import Json Scene", "Import Json Scene", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilImportMenu, static_cast<s32>( WidgetId::ImportUnityYamlId ),
                           "Import Unity Yaml", "Import Json Scene", ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilImportMenu );

        auto utilDebugMenu = ui->addElementByType<ui::IUIMenu>();
        utilDebugMenu->setLabel( "Debug" );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::ShowAllOverlaysId ),
                           "showAllOverlays", "showAllOverlays", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::HideAllOverlaysId ),
                           "hideAllOverlays", "hideAllOverlays", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayTestId ),
                           "createOverlayPanelTest", "createOverlayPanelTest",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayTextTestId ),
                           "createOverlayTextTest", "createOverlayTextTest",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayButtonTestId ),
                           "createOverlayButtonTest", "createOverlayButtonTest",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::CreateBoxTestId ), "createBoxTest",
                           "createBoxTest", ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilDebugMenu );

        auto utilProceduralMenu = ui->addElementByType<ui::IUIMenu>();
        utilProceduralMenu->setLabel( "Procedural" );
        Util::addMenuItem( utilProceduralMenu, static_cast<s32>( WidgetId::CreateProceduralTestId ),
                           "Create Procedural Test", "Create Procedural Test",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilProceduralMenu, static_cast<s32>( WidgetId::LoadProceduralSceneId ),
                           "Load Procedural Scene", "Load Procedural Scene",
                           ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilProceduralMenu );

        auto utilSceneMenu = ui->addElementByType<ui::IUIMenu>();
        utilSceneMenu->setLabel( "Scene" );
        Util::addMenuItem( utilSceneMenu, static_cast<s32>( WidgetId::CreateProceduralTestId ),
                           "Create Procedural Test", "Create Procedural Test",
                           ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilSceneMenu );

        auto utilDatabaseMenu = ui->addElementByType<ui::IUIMenu>();
        utilDatabaseMenu->setLabel( "Database" );
        Util::addMenuItem( utilDatabaseMenu, static_cast<s32>( WidgetId::OptimiseDatabasesId ),
                           "Optimise Databases", "Optimise Databases", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilDatabaseMenu, static_cast<s32>( WidgetId::CreateAssetFromDatabasesId ),
                           "Create asset", "Create asset", ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilDatabaseMenu );

        auto utilMaterialMenu = ui->addElementByType<ui::IUIMenu>();
        utilMaterialMenu->setLabel( "Materials" );
        Util::addMenuItem( utilMaterialMenu, static_cast<s32>( WidgetId::GenerateSkyboxMaterialsId ),
                           "Generate Skybox Materials", "Generate Skybox Materials",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( utilMaterialMenu, static_cast<s32>( WidgetId::SetupMaterialsId ),
                           "Setup Materials", "Setup Materials", ui::IUIMenuItem::Type::Normal );
        utilMenu->addMenuItem( utilMaterialMenu );

        auto utilPhysicsMenu = ui->addElementByType<ui::IUIMenu>();
        utilPhysicsMenu->setLabel( "Physics" );

        //Util::addMenuItem( utilPhysicsMenu, static_cast<s32>( WidgetId::CreateRigidBodies ),
        //                   "Create Rigid Bodies", "Create Rigid Bodies",
        //                   ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilPhysicsMenu, static_cast<s32>( WidgetId::CreateRigidStaticMeshId ),
                           "Create Rigid Static", "Create Rigid Static", ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilPhysicsMenu, static_cast<s32>( WidgetId::CreateRigidDynamicMeshId ),
                           "Create Rigid Dynamic", "Create Rigid Dynamic",
                           ui::IUIMenuItem::Type::Normal );

        utilMenu->addMenuItem( utilPhysicsMenu );

        auto utilProjectMenu = ui->addElementByType<ui::IUIMenu>();
        utilProjectMenu->setLabel( "Project" );

        Util::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::CreatePluginCodeId ),
                           "Create Plugin Code", "Create Plugin Code", ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::LoadPluginId ), "Load Plugin",
                           "Load Plugin", ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::UnloadPluginId ),
                           "Unload Plugin", "Unload Plugin", ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::CopyEngineFilesId ),
                           "Copy Engine Files", "Copy Engine Files", ui::IUIMenuItem::Type::Normal );

        Util::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::CleanProjectId ), "Clean",
                           "Clean", ui::IUIMenuItem::Type::Normal );

        utilMenu->addMenuItem( utilProjectMenu );

        menuBar->addMenu( utilMenu );

        //auto proceduralMenu = ui->addElementByType<ui::IUIMenu>();
        //proceduralMenu->setLabel( "Procedural" );
        //Util::addMenuItem( proceduralMenu, static_cast<s32>( WidgetId::LoadProceduralSceneId ),
        //                   "Load Procedural Scene", "Load Procedural Scene",
        //                   ui::IUIMenuItem::Type::Normal );
        //menuBar->addMenu( proceduralMenu );

        auto windowMenu = ui->addElementByType<ui::IUIMenu>();
        windowMenu->setLabel( "Window" );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ComponentsId ), "Components",
                           "Components", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ResourcesId ), "Resources",
                           "Resources", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::InputWindowId ), "Input", "Input",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ProfilerWindowId ), "Profiler",
                           "Profiler", ui::IUIMenuItem::Type::Normal );
        Util::addMenuSeparator( windowMenu );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ObjectWindowId ), "Object", "Object",
                           ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ProjectWindowId ), "Project",
                           "Project", ui::IUIMenuItem::Type::Normal );
        Util::addMenuItem( windowMenu, static_cast<s32>( WidgetId::SceneWindowId ), "Scene", "Scene",
                           ui::IUIMenuItem::Type::Normal );

        menuBar->addMenu( windowMenu );

        auto helpMenu = ui->addElementByType<ui::IUIMenu>();
        helpMenu->setLabel( "Help" );
        Util::addMenuItem( helpMenu, static_cast<s32>( WidgetId::AboutId ), "About", "About",
                           ui::IUIMenuItem::Type::Normal );
        menuBar->addMenu( helpMenu );

        auto menubarListener = fb::make_ptr<MenuBarListener>();
        menubarListener->setOwner( this );
        m_menubarListener = menubarListener;

        menuBar->addObjectListener( m_menubarListener );

        auto toolbar = ui->addElementByType<ui::IUIToolbar>();

        auto toolbarListener = fb::make_ptr<ToolbarListener>();
        toolbarListener->setOwner( this );
        toolbar->addObjectListener( toolbarListener );

        auto toolbarPlayButton = ui->addElementByType<ui::IUILabelTogglePair>();
        toolbarPlayButton->setElementId( static_cast<s32>( WidgetId::RunId ) );
        toolbarPlayButton->setLabel( "Play" );
        toolbar->addChild( toolbarPlayButton );
        toolbarPlayButton->setValue( false );
        m_playmodeToggle = toolbarPlayButton;

        //auto toolbarStopButton = ui->addElementByType<ui::IUIButton>();
        //toolbarStopButton->setElementId( static_cast<s32>( WidgetId::StopId ) );
        //toolbarStopButton->setLabel( "Stop" );
        //toolbar->addChild( toolbarStopButton );

        auto toggleEditorCameraButton = ui->addElementByType<ui::IUILabelTogglePair>();
        toggleEditorCameraButton->setElementId( static_cast<s32>( WidgetId::ToggleEditorCameraId ) );
        toggleEditorCameraButton->setLabel( "Editor Camera" );
        toolbar->addChild( toggleEditorCameraButton );
        toggleEditorCameraButton->setValue( true );
        m_editorCameraToggle = toggleEditorCameraButton;

        auto toolbarStatsButton = ui->addElementByType<ui::IUIButton>();
        toolbarStatsButton->setElementId( static_cast<s32>( WidgetId::StatsId ) );
        toolbarStatsButton->setLabel( "Stats" );
        toolbar->addChild( toolbarStatsButton );

        auto reloadScriptsButton = ui->addElementByType<ui::IUIButton>();
        reloadScriptsButton->setElementId( static_cast<s32>( WidgetId::ReloadScriptsId ) );
        reloadScriptsButton->setLabel( "Reload" );
        toolbar->addChild( reloadScriptsButton );

        auto toggleTranslateButton = ui->addElementByType<ui::IUIButton>();
        toggleTranslateButton->setElementId( static_cast<s32>( WidgetId::TranslateEditorCameraId ) );
        toggleTranslateButton->setLabel( "Translate" );
        toolbar->addChild( toggleTranslateButton );

        auto toggleRotateButton = ui->addElementByType<ui::IUIButton>();
        toggleRotateButton->setElementId( static_cast<s32>( WidgetId::RotateEditorCameraId ) );
        toggleRotateButton->setLabel( "Rotate" );
        toolbar->addChild( toggleRotateButton );

        auto toggleScaleButton = ui->addElementByType<ui::IUIButton>();
        toggleScaleButton->setElementId( static_cast<s32>( WidgetId::ScaleEditorCameraId ) );
        toggleScaleButton->setLabel( "Scale" );
        toolbar->addChild( toggleScaleButton );

        auto application = ui->getApplication();
        if( application )
        {
            application->setMenubar( menuBar );
            application->setToolbar( toolbar );
        }

        auto sceneWindow = fb::make_ptr<SceneWindow>( nullptr );
        sceneWindow->load( nullptr );
        setSceneWindow( sceneWindow );

        auto objectWindow = fb::make_ptr<ObjectWindow>();
        objectWindow->load( nullptr );
        setObjectWindow( objectWindow );

        auto projectWindow = fb::make_ptr<ProjectWindow>();
        projectWindow->load( nullptr );
        setProjectWindow( projectWindow );

        auto objectBrowserDialog = fb::make_ptr<ObjectBrowserDialog>();
        objectBrowserDialog->load( nullptr );
        setObjectBrowserDialog( objectBrowserDialog );
        objectBrowserDialog->setWindowVisible( false );

        auto resourceDatabaseDialog = fb::make_ptr<ResourceDatabaseDialog>();
        resourceDatabaseDialog->load( nullptr );
        setResourceDatabaseDialog( resourceDatabaseDialog );
        resourceDatabaseDialog->setWindowVisible( false );

        auto inputManagerWindow = fb::make_ptr<InputManagerWindow>();
        inputManagerWindow->load( nullptr );
        setInputManagerWindow( inputManagerWindow );
        inputManagerWindow->setWindowVisible( false );

        auto profilerWindow = fb::make_ptr<ProfilerWindow>();
        profilerWindow->load( nullptr );
        setProfilerWindow( profilerWindow );
        profilerWindow->setWindowVisible( false );

        auto aboutDialog = fb::make_ptr<AboutDialog>();
        aboutDialog->load( nullptr );
        setAboutDialog( aboutDialog );
        aboutDialog->setWindowVisible( false );

        //auto addDialog = fb::make_ptr<BaseWindow>();
        //addDialog->load( nullptr );
        //setProjectWindow( addDialog );

        m_updateSelectionJob = fb::make_ptr<UpdateSelectionJob>();

        if( auto stateContext = applicationManager->getStateContext() )
        {
            auto eventListener = fb::make_ptr<EventListener>();
            eventListener->setOwner( this );
            stateContext->addEventListener( eventListener );

            m_eventListener = eventListener;
        }
    }

    void UIManager::unload( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        if( m_frameStatistics )
        {
            m_frameStatistics->unload( nullptr );
            m_frameStatistics = nullptr;
        }

        if( auto resourceDatabaseDialog = getResourceDatabaseDialog() )
        {
            resourceDatabaseDialog->unload( nullptr );
            setResourceDatabaseDialog( nullptr );
        }

        if( auto sceneWindow = getSceneWindow() )
        {
            sceneWindow->unload( nullptr );
            setSceneWindow( nullptr );
        }

        if( auto propertiesWindow = getPropertiesWindow() )
        {
            propertiesWindow->unload( nullptr );
            setPropertiesWindow( nullptr );
        }

        if( auto projectWindow = getProjectWindow() )
        {
            projectWindow->unload( nullptr );
            setProjectWindow( nullptr );
        }

        if( auto objectBrowserDialog = getObjectBrowserDialog() )
        {
            objectBrowserDialog->unload( nullptr );
            setObjectBrowserDialog( nullptr );
        }

        if( auto objectWindow = getObjectWindow() )
        {
            objectWindow->unload( nullptr );
            setObjectWindow( nullptr );
        }

        auto application = ui->getApplication();
        if( application )
        {
            application->unload( nullptr );
        }

        if( auto actorWindow = getActorWindow() )
        {
            actorWindow->unload( nullptr );
            setActorWindow( nullptr );
        }
    }

    void UIManager::update( time_interval t, time_interval dt )
    {
        if( m_frameStatistics )
        {
            m_frameStatistics->update();
        }
    }

    void UIManager::rebuildResourceTree()
    {
        if( auto projectWindow = getProjectWindow() )
        {
            projectWindow->buildTree();
        }
    }

    void UIManager::rebuildSceneTree()
    {
        try
        {
            if( auto sceneWindow = getSceneWindow() )
            {
                sceneWindow->buildTree();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void UIManager::rebuildActorTree()
    {
        try
        {
            auto actorWindow = getActorWindow();
            if( actorWindow )
            {
                actorWindow->buildTree();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto UIManager::getSceneWindow() const -> SmartPtr<SceneWindow>
    {
        return m_sceneWindow;
    }

    void UIManager::setSceneWindow( SmartPtr<SceneWindow> sceneWindow )
    {
        m_sceneWindow = sceneWindow;
    }

    auto UIManager::getActorWindow() const -> SmartPtr<ActorWindow>
    {
        return m_actorWindow;
    }

    void UIManager::updateSelection()
    {
        if( m_updateSelectionJob->isFinished() )
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto jobQueue = applicationManager->getJobQueue();

            //jobQueue->queueJob( m_updateSelectionJob );
            m_updateSelectionJob->execute();
        }
    }

    void UIManager::updateActorSelection()
    {
        if( auto propertiesWindow = getPropertiesWindow() )
        {
            propertiesWindow->updateSelection();
        }
    }

    void UIManager::updateComponentSelection()
    {
        if( auto propertiesWindow = getPropertiesWindow() )
        {
            propertiesWindow->updateSelection();
        }

        if( auto objectWindow = getObjectWindow() )
        {
            objectWindow->updateSelection();
        }
    }

    auto UIManager::getPropertiesWindow() const -> PropertiesWindow *
    {
        return m_propertiesWindow;
    }

    void UIManager::setPropertiesWindow( PropertiesWindow *val )
    {
        m_propertiesWindow = val;
    }

    auto UIManager::getApplicationFrame() const -> ApplicationFrame *
    {
        return m_appFrame;
    }

    void UIManager::setApplicationFrame( ApplicationFrame *val )
    {
        m_appFrame = val;
    }

    void UIManager::setActorWindow( SmartPtr<ActorWindow> actorWindow )
    {
        m_actorWindow = actorWindow;
    }

    auto UIManager::getTerrainWindow() const -> TerrainWindow *
    {
        return m_terrainWindow;
    }

    void UIManager::setTerrainWindow( TerrainWindow *val )
    {
        m_terrainWindow = val;
    }

    auto UIManager::getFoliageWindow() const -> FoliageWindow *
    {
        return m_foliageWindow;
    }

    void UIManager::setFoliageWindow( FoliageWindow *val )
    {
        m_foliageWindow = val;
    }

    auto UIManager::getRoadWindow() const -> RoadFrame *
    {
        return m_roadWindow;
    }

    void UIManager::setRoadWindow( RoadFrame *val )
    {
        m_roadWindow = val;
    }

    auto UIManager::getHoudiniWindow() const -> HoudiniWindow *
    {
        return m_houdiniWindow;
    }

    void UIManager::setHoudiniWindow( HoudiniWindow *val )
    {
        m_houdiniWindow = val;
    }

    auto UIManager::getRenderWindow() const -> RenderWindow *
    {
        return m_renderWindow;
    }

    void UIManager::setRenderWindow( RenderWindow *val )
    {
        m_renderWindow = val;
    }

    auto UIManager::getGameWindow() const -> RenderWindow *
    {
        return m_gameWindow;
    }

    void UIManager::setGameWindow( RenderWindow *val )
    {
        m_gameWindow = val;
    }

    auto UIManager::getFileWindow() const -> FileWindow *
    {
        return m_fileWindow;
    }

    void UIManager::setFileWindow( FileWindow *val )
    {
        m_fileWindow = val;
    }

    auto UIManager::getProjectWindow() const -> SmartPtr<ProjectWindow>
    {
        return m_projectWindow;
    }

    void UIManager::setProjectWindow( SmartPtr<ProjectWindow> val )
    {
        m_projectWindow = val;
    }

    auto UIManager::getTextureWindow() const -> TextureWindow *
    {
        return m_textureWindow;
    }

    void UIManager::setTextureWindow( TextureWindow *textureWindow )
    {
        m_textureWindow = textureWindow;
    }

    auto UIManager::getMaterialWindow() const -> MaterialWindow *
    {
        return m_materialWindow;
    }

    void UIManager::setMaterialWindow( MaterialWindow *materialWindow )
    {
        m_materialWindow = materialWindow;
    }

    auto UIManager::getObjectWindow() const -> SmartPtr<ObjectWindow>
    {
        return m_objectWindow;
    }

    void UIManager::setObjectWindow( SmartPtr<ObjectWindow> objectWindow )
    {
        m_objectWindow = objectWindow;
    }

    auto UIManager::getObjectBrowserDialog() const -> SmartPtr<ObjectBrowserDialog>
    {
        return m_objectBrowserDialog;
    }

    void UIManager::setObjectBrowserDialog( SmartPtr<ObjectBrowserDialog> objectBrowserDialog )
    {
        m_objectBrowserDialog = objectBrowserDialog;
    }

    auto UIManager::getResourceDatabaseDialog() const -> SmartPtr<ResourceDatabaseDialog>
    {
        return m_resourceDatabaseDialog;
    }

    void UIManager::setResourceDatabaseDialog( SmartPtr<ResourceDatabaseDialog> resourceDatabaseDialog )
    {
        m_resourceDatabaseDialog = resourceDatabaseDialog;
    }

    auto UIManager::getAboutDialog() const -> SmartPtr<AboutDialog>
    {
        return m_aboutDialog;
    }

    UIManager::MenuBarListener::MenuBarListener() = default;

    UIManager::MenuBarListener::~MenuBarListener() = default;

    auto UIManager::MenuBarListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto application = applicationManager->getApplication();

        auto editorManager = EditorManager::getSingletonPtr();

        auto resourceDatabase = applicationManager->getResourceDatabase();
        auto factoryManager = applicationManager->getFactoryManager();
        auto jobQueue = applicationManager->getJobQueue();
        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto uiManager = editorManager->getUI();
        FB_ASSERT( uiManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto commandManager = applicationManager->getCommandManager();

        if( eventValue == IEvent::handleSelection )
        {
            auto element = fb::static_pointer_cast<ui::IUIElement>( object );

            auto widgetID = static_cast<WidgetId>( element->getElementId() );
            switch( widgetID )
            {
            case WidgetId::SaveProjectId:
            {
                if( auto project = editorManager->getProject() )
                {
                    auto filePath = project->getFilePath();
                    if( !StringUtil::isNullOrEmpty( filePath ) )
                    {
                        project->saveToFile( filePath );
                    }
                    else
                    {
                        FB_LOG_ERROR( "Project file path empty" );
                    }
                }
                else
                {
                    FB_LOG_ERROR( "Project null" );
                }
            }
            break;
            case WidgetId::NewSceneId:
            {
                auto renderLock = taskManager->lockTask( Thread::Task::Render );
                auto physicsLock = taskManager->lockTask( Thread::Task::Physics );
                auto applicationLock = taskManager->lockTask( Thread::Task::Application );

                auto editorManager = EditorManager::getSingletonPtr();
                FB_ASSERT( editorManager );

                auto uiManager = editorManager->getUI();
                FB_ASSERT( uiManager );

                scene->setName( "Untitled" );
                scene->setFilePath( String() );
                scene->clear();

                application->createDefaultSky();
                application->createDirectionalLight();

                uiManager->rebuildSceneTree();
            }
            break;
            case WidgetId::SaveId:
            {
                auto job = fb::make_ptr<SaveSceneJob>();
                job->setPrimary( true );  // fix for macos
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::SaveSceneAsId:
            {
                auto job = fb::make_ptr<SaveSceneJob>();
                job->setPrimary( true );  // fix for macos
                job->setSaveAs( true );
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::SaveAllId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto editorManager = EditorManager::getSingletonPtr();
                FB_ASSERT( editorManager );

                auto uiManager = editorManager->getUI();
                FB_ASSERT( uiManager );

                auto sceneManager = applicationManager->getSceneManager();

                if( auto scene = sceneManager->getCurrentScene() )
                {
                    scene->saveScene();
                }

                if( auto project = editorManager->getProject() )
                {
                    project->save();
                }
            }
            break;
            case WidgetId::CreatePackageId:
            {
                if( auto fileDialog = fileSystem->openFileDialog() )
                {
                    fileDialog->setDialogMode( INativeFileDialog::DialogMode::Select );
                    fileDialog->setFileExtension( ".*" );

                    auto result = fileDialog->openDialog();
                    if( result == INativeFileDialog::Result::Dialog_Okay )
                    {
                        auto path = fileDialog->getFilePath();

                        auto job = fb::make_ptr<JobCreatePackage>();

                        auto dst = StringUtil::toUTF8to16( path );
                        job->setDst( dst );

                        jobQueue->addJob( job );
                    }
                }
            }
            break;
            case WidgetId::AssetImportId:
            {
                auto job = fb::make_ptr<AssetImportJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::AssetReimportId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                FB_ASSERT( resourceDatabase );

                resourceDatabase->reimportAssets();
            }
            break;
            case WidgetId::AssetDatabaseBuildId:
            {
                auto job = fb::make_ptr<AssetDatabaseBuildJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::AssetDatabaseImportCacheId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                FB_ASSERT( resourceDatabase );

                resourceDatabase->importCache();
            }
            break;
            case WidgetId::AssetDatabaseDeleteCacheId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                FB_ASSERT( resourceDatabase );

                resourceDatabase->deleteCache();
            }
            break;
            case WidgetId::ImportJsonSceneId:
            {
                if( auto fileDialog = fileSystem->openFileDialog() )
                {
                    auto projectPath = Path::getWorkingDirectory();
                    if( !fileSystem->isExistingFolder( projectPath ) )
                    {
                        projectPath = "";
                    }

                    fileDialog->setDialogMode( INativeFileDialog::DialogMode::Open );
                    fileDialog->setFileExtension( ".json" );
                    fileDialog->setFilePath( projectPath );

                    auto result = fileDialog->openDialog();
                    if( result == INativeFileDialog::Result::Dialog_Okay )
                    {
                        auto filePath = fileDialog->getFilePath();
                        if( !StringUtil::isNullOrEmpty( filePath ) )
                        {
                            filePath = StringUtil::cleanupPath( filePath );

                            auto applicationManager = core::ApplicationManager::instance();
                            FB_ASSERT( applicationManager );

                            auto project = editorManager->getProject();
                            FB_ASSERT( project );

                            auto uiManager = editorManager->getUI();
                            FB_ASSERT( uiManager );

                            auto fileSystem = applicationManager->getFileSystem();
                            FB_ASSERT( fileSystem );

                            auto sceneData = application->importScene( filePath );

                            auto sceneManager = applicationManager->getSceneManager();
                            auto scene = sceneManager->getCurrentScene();
                            if( scene )
                            {
                                scene->fromData( sceneData );
                            }

                            uiManager->rebuildSceneTree();
                        }
                    }
                }
            }
            break;
            case WidgetId::ImportUnityYamlId:
            {
                auto job = fb::make_ptr<ImportUnityYaml>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::UndoId:
            {
                auto cmd = commandManager->getPreviousCommand();
                if( cmd != nullptr )
                {
                    cmd->undo();
                }
            }
            break;
            case WidgetId::RedoId:
            {
                auto cmd = commandManager->getNextCommand();
                if( cmd != nullptr )
                {
                    cmd->redo();
                }
            }
            break;
            case WidgetId::ComponentsId:
            {
                auto owner = getOwner();
                auto window = owner->getObjectBrowserDialog();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::ResourcesId:
            {
                auto owner = getOwner();
                auto window = owner->getResourceDatabaseDialog();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::InputWindowId:
            {
                auto owner = getOwner();
                auto window = owner->getInputManagerWindow();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::ProfilerWindowId:
            {
                auto owner = getOwner();
                auto window = owner->getProfilerWindow();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::ObjectWindowId:
            {
                auto owner = getOwner();
                auto window = owner->getObjectWindow();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::ProjectWindowId:
            {
                auto owner = getOwner();
                auto window = owner->getProjectWindow();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::SceneWindowId:
            {
                auto owner = getOwner();
                auto window = owner->getSceneWindow();
                window->setWindowVisible( true );
            }
            break;
            case WidgetId::GenerateSkyboxMaterialsId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto job = fb::make_ptr<GenerateSkybox>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::CreateBoxTestId:
            {
                auto sceneManager = applicationManager->getSceneManager();

                auto cube = application->createDefaultCube( true );
                auto ground = application->createDefaultGround();

                uiManager->rebuildSceneTree();
            }
            break;
            case WidgetId::CreatePluginCodeId:
            {
                auto job = fb::make_ptr<CreatePluginCodeJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::LoadPluginId:
            {
                auto pluginFileName = String( "Plugin.dll" );
                auto pluginPath = applicationManager->getProjectPath() + "/bin/windows/RelWithDebInfo/" +
                                  pluginFileName;

                auto job = fb::make_ptr<LoadPluginJob>();
                job->setPluginPath( pluginPath );
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::UnloadPluginId:
            {
                auto job = fb::make_ptr<UnloadPluginJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::CopyEngineFilesId:
            {
                auto job = fb::make_ptr<CopyEngineFilesJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::CleanProjectId:
            {
                auto job = fb::make_ptr<ProjectCleanJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::SetupMaterialsId:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto jobQueue = applicationManager->getJobQueue();

                auto job = fb::make_ptr<SetupMaterialJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::OptimiseDatabasesId:
            {
                auto job = fb::make_ptr<OptimiseDatabasesJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::CreateAssetFromDatabasesId:
            {
                SmartPtr<scene::IPrefab> prefab = resourceDatabase->loadResource( 3 );
                if( prefab )
                {
                    auto actor = prefab->createActor();
                    scene->addActor( actor );
                }

                uiManager->rebuildSceneTree();
            }
            break;
            case WidgetId::AboutId:
            {
                if( auto aboutDialog = uiManager->getAboutDialog() )
                {
                    aboutDialog->setWindowVisible( true );
                }
            }
            break;
            default:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto commandManager = applicationManager->getCommandManager();

                auto cmd = fb::make_ptr<FileMenuCmd>();
                cmd->setItemId( widgetID );
                commandManager->addCommand( cmd );
            }
            break;
            }
        }

        return {};
    }

    auto UIManager::MenuBarListener::getOwner() const -> UIManager *
    {
        return m_owner;
    }

    void UIManager::MenuBarListener::setOwner( UIManager *owner )
    {
        m_owner = owner;
    }

    void UIManager::setAboutDialog( SmartPtr<AboutDialog> aboutDialog )
    {
        m_aboutDialog = aboutDialog;
    }

    auto UIManager::getInputManagerWindow() const -> SmartPtr<InputManagerWindow>
    {
        return m_inputManagerWindow;
    }

    void UIManager::setInputManagerWindow( SmartPtr<InputManagerWindow> inputManagerWindow )
    {
        m_inputManagerWindow = inputManagerWindow;
    }

    auto UIManager::getProfilerWindow() const -> SmartPtr<ProfilerWindow>
    {
        return m_profilerWindow;
    }

    void UIManager::setProfilerWindow( SmartPtr<ProfilerWindow> profilerWindow )
    {
        m_profilerWindow = profilerWindow;
    }

    auto UIManager::isValid() const -> bool
    {
        bool valid = true;

        if( m_projectWindow )
        {
            valid = valid && m_projectWindow->isValid();
        }

        if( m_projectWindow )
        {
            valid = valid && m_projectWindow->isValid();
        }

        if( m_sceneWindow )
        {
            valid = valid && m_sceneWindow->isValid();
        }

        return valid;
    }

    auto UIManager::ToolbarListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto taskManager = applicationManager->getTaskManager();
        auto jobQueue = applicationManager->getJobQueue();
        auto commandManager = applicationManager->getCommandManager();

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
        auto iElementId = element->getElementId();

        auto id = static_cast<WidgetId>( iElementId );

        if( eventValue == IEvent::handleValueChanged )
        {
            switch( id )
            {
            case WidgetId::RunId:
            {
                auto toggled = m_owner->m_playmodeToggle->getValue();
                if( toggled )
                {
                    auto job = fb::make_ptr<PlaymodeJob>();
                    jobQueue->addJob( job );
                }
                else
                {
                    auto job = fb::make_ptr<LeavePlaymodeJob>();
                    jobQueue->addJob( job );
                }
            }
            break;
            case WidgetId::ToggleEditorCameraId:
            {
                auto toggled = m_owner->m_editorCameraToggle->getValue();
                auto cmd = fb::make_ptr<ToggleEditorCamera>();
                cmd->setToggleValue( toggled );
                commandManager->addCommand( cmd );
            }
            break;
            };
        }
        else if( eventValue == IEvent::handleSelection )
        {
            switch( id )
            {
            case WidgetId::RunId:
            {
                auto job = fb::make_ptr<PlaymodeJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::StopId:
            {
                auto job = fb::make_ptr<LeavePlaymodeJob>();
                jobQueue->addJob( job );
            }
            break;
            case WidgetId::StatsId:
            {
                auto frameStatistics = m_owner->m_frameStatistics;
                if( frameStatistics )
                {
                    auto visible = !frameStatistics->isVisible();
                    frameStatistics->setVisible( visible );
                }
            }
            break;
            case WidgetId::ReloadScriptsId:
            {
                auto job = fb::make_ptr<ReloadScriptsJob>();
                jobQueue->addJob( job );
            }
            break;

            case WidgetId::TranslateEditorCameraId:
            {
                auto translateManipulator = editorManager->getTranslateManipulator();
                if( translateManipulator )
                {
                    auto enabled = !translateManipulator->isEnabled();
                    translateManipulator->setEnabled( enabled );
                }
            }
            break;
            case WidgetId::RotateEditorCameraId:
            {
                auto rotateManipulator = editorManager->getRotateManipulator();
                if( rotateManipulator )
                {
                    auto enabled = !rotateManipulator->isEnabled();
                    rotateManipulator->setEnabled( enabled );
                }
            }
            break;
            case WidgetId::ScaleEditorCameraId:
            {
                auto scaleManipulator = editorManager->getScaleManipulator();
                if( scaleManipulator )
                {
                    auto enabled = !scaleManipulator->isEnabled();
                    scaleManipulator->setEnabled( enabled );
                }
            }
            break;
            default:
            {
            }
            break;
            }
        }

        return {};
    }

    auto UIManager::ToolbarListener::getOwner() const -> UIManager *
    {
        return m_owner;
    }

    void UIManager::ToolbarListener::setOwner( UIManager *owner )
    {
        m_owner = owner;
    }

    void UIManager::UpdateSelectionJob::execute()
    {
        auto editorManager = EditorManager::getSingletonPtr();
        auto ui = editorManager->getUI();

        auto objectWindow = ui->getObjectWindow();
        if( objectWindow )
        {
            objectWindow->updateSelection();
        }

        auto propertiesWindow = ui->getPropertiesWindow();
        if( propertiesWindow )
        {
            propertiesWindow->updateSelection();
        }

        auto terrainWindow = ui->getTerrainWindow();
        if( terrainWindow )
        {
            terrainWindow->updateSelection();
        }
    }

    auto UIManager::EventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                const Array<Parameter> &arguments,
                                                SmartPtr<ISharedObject> sender,
                                                SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( eventType == IEvent::Type::Loading )
        {
            if( eventValue == scene::ISceneManager::sceneLoadedHash )
            {
                if( auto owner = getOwner() )
                {
                    owner->rebuildSceneTree();
                }
            }
        }

        return {};
    }

    auto UIManager::EventListener::getOwner() const -> UIManager *
    {
        return m_owner;
    }

    void UIManager::EventListener::setOwner( UIManager *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
