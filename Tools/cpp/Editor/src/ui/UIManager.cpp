#include <GameEditorPCH.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include "commands/FileMenuCmd.h"
#include <ui/AboutDialog.h>
#include <ui/ActorWindow.h>
#include <ui/ApplicationFrame.h>
#include <ui/ProjectWindow.h>
#include <ui/TerrainWindow.h>
#include <ui/SceneWindow.h>
#include <ui/PropertiesWindow.h>
#include <ui/MeshImportWindow.h>
#include <ui/MaterialWindow.h>
#include "ui/ProfilerWindow.h"
#include <ui/ObjectWindow.h>
#include <ui/ObjectBrowserDialog.h>
#include <ui/ResourceDatabaseDialog.h>
#include <ui/InputManagerWindow.h>

#include "commands/ToggleEditorCamera.h"
#include "jobs/GenerateSkyboxMaterials.h"
#include "jobs/JobCreatePackage.h"
#include "jobs/OptimiseDatabasesJob.h"
#include "jobs/ProjectCleanJob.h"
#include "jobs/SetupMaterialJob.h"
#include "jobs/ReloadScriptsJob.h"
#include "jobs/PlaymodeJob.h"
#include "jobs/LeavePlaymodeJob.h"
#include "jobs/ImportUnityYaml.h"

namespace fb
{
    namespace editor
    {
        UIManager::UIManager()
        {
        }

        UIManager::~UIManager()
        {
            unload( nullptr );
        }

        void UIManager::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::NewProjectId ),
                                          "New Project", "Create a project",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::OpenProjectId ),
                                          "Open Project", "Open a project",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveProjectId ),
                                          "Save Project", "Save a project",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuSeparator( fileMenu );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::NewSceneId ),
                                          "New Scene", "New Scene", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::OpenSceneId ),
                                          "Open Scene", "Open Scene", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveId ), "Save Scene",
                                          "Save Scene", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveSceneAsId ),
                                          "Save Scene As", "Save Scene As",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::SaveAllId ), "Save All",
                                          "Save All", ui::IUIMenuItem::Type::Normal );
            //ApplicationUtil::addMenuSeparator( fileMenu );
            //ApplicationUtil::addMenuItem(
            //    fileMenu, static_cast<s32>( UIManager::WidgetId::GenerateCMakeProjectId ),
            //    "Generate CMake Project", "Generate CMake Project", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuSeparator( fileMenu );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::CreatePackageId ),
                                          "Create Package", "Create Package",
                                          ui::IUIMenuItem::Type::Normal );
            //ApplicationUtil::addMenuSeparator( fileMenu );

            //ApplicationUtil::addMenuItem(
            //    fileMenu, static_cast<s32>( UIManager::WidgetId::CreateUnityBindings ),
            //    "Create Unity Bindings", "Create Unity Bindings", ui::IUIMenuItem::Type::Normal );
            //ApplicationUtil::addMenuSeparator( fileMenu );

            //auto recentFilesMenu = ui->addElementByType<ui::IUIMenu>();
            //recentFilesMenu->setLabel( "Recent Files" );
            //ApplicationUtil::addMenuItem( recentFilesMenu,
            //                              static_cast<s32>( UIManager::WidgetId::LoadProceduralSceneId ),
            //                              "File", "File", ui::IUIMenuItem::Type::Normal );
            //fileMenu->addMenuItem( recentFilesMenu );

            //auto recentProjectsMenu = ui->addElementByType<ui::IUIMenu>();
            //recentProjectsMenu->setLabel( "Recent Projects" );
            //ApplicationUtil::addMenuItem( recentProjectsMenu,
            //                              static_cast<s32>( UIManager::WidgetId::LoadProceduralSceneId ),
            //                              "File", "File", ui::IUIMenuItem::Type::Normal );
            //fileMenu->addMenuItem( recentProjectsMenu );

            ApplicationUtil::addMenuSeparator( fileMenu );
            ApplicationUtil::addMenuItem( fileMenu, static_cast<s32>( WidgetId::Exit ), "Exit",
                                          "Quit this program", ui::IUIMenuItem::Type::Normal );

            menuBar->addMenu( fileMenu );

            auto editMenu = ui->addElementByType<ui::IUIMenu>();
            editMenu->setLabel( "Edit" );
            ApplicationUtil::addMenuItem( editMenu, static_cast<s32>( WidgetId::UndoId ), "Undo",
                                          "Undo command", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( editMenu, static_cast<s32>( WidgetId::RedoId ), "Redo",
                                          "Redo command", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuSeparator( editMenu );
            menuBar->addMenu( editMenu );

            auto assetsMenu = ui->addElementByType<ui::IUIMenu>();
            assetsMenu->setLabel( "Assets" );
            ApplicationUtil::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetImportId ),
                                          "Import", "Import", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetReimportId ),
                                          "Reimport", "Reimport", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( assetsMenu, static_cast<s32>( WidgetId::AssetDatabaseBuildId ),
                                          "Build Database", "Build Database",
                                          ui::IUIMenuItem::Type::Normal );

            menuBar->addMenu( assetsMenu );

            auto utilMenu = ui->addElementByType<ui::IUIMenu>();
            utilMenu->setLabel( "Util" );

            auto utilImportMenu = ui->addElementByType<ui::IUIMenu>();
            utilImportMenu->setLabel( "Import" );
            ApplicationUtil::addMenuItem(
                utilImportMenu, static_cast<s32>( WidgetId::ImportJsonSceneId ), "Import Json Scene",
                "Import Json Scene", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem(
                utilImportMenu, static_cast<s32>( WidgetId::ImportUnityYamlId ), "Import Unity Yaml",
                "Import Json Scene", ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilImportMenu );

            auto utilDebugMenu = ui->addElementByType<ui::IUIMenu>();
            utilDebugMenu->setLabel( "Debug" );
            ApplicationUtil::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::ShowAllOverlaysId ),
                                          "showAllOverlays", "showAllOverlays",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::HideAllOverlaysId ),
                                          "hideAllOverlays", "hideAllOverlays",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem(
                utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayTestId ),
                "createOverlayPanelTest", "createOverlayPanelTest", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem(
                utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayTextTestId ),
                "createOverlayTextTest", "createOverlayTextTest", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem(
                utilDebugMenu, static_cast<s32>( WidgetId::CreateOverlayButtonTestId ),
                "createOverlayButtonTest", "createOverlayButtonTest", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( utilDebugMenu, static_cast<s32>( WidgetId::CreateBoxTestId ),
                                          "createBoxTest", "createBoxTest",
                                          ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilDebugMenu );

            auto utilProceduralMenu = ui->addElementByType<ui::IUIMenu>();
            utilProceduralMenu->setLabel( "Procedural" );
            ApplicationUtil::addMenuItem(
                utilProceduralMenu, static_cast<s32>( WidgetId::CreateProceduralTestId ),
                "Create Procedural Test", "Create Procedural Test", ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilProceduralMenu );

            auto utilSceneMenu = ui->addElementByType<ui::IUIMenu>();
            utilSceneMenu->setLabel( "Scene" );
            ApplicationUtil::addMenuItem(
                utilSceneMenu, static_cast<s32>( WidgetId::CreateProceduralTestId ),
                "Create Procedural Test", "Create Procedural Test", ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilSceneMenu );

            auto utilDatabaseMenu = ui->addElementByType<ui::IUIMenu>();
            utilDatabaseMenu->setLabel( "Database" );
            ApplicationUtil::addMenuItem(
                utilDatabaseMenu, static_cast<s32>( WidgetId::OptimiseDatabasesId ),
                "Optimise Databases", "Optimise Databases", ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilDatabaseMenu );

            auto utilMaterialMenu = ui->addElementByType<ui::IUIMenu>();
            utilMaterialMenu->setLabel( "Materials" );
            ApplicationUtil::addMenuItem( utilMaterialMenu,
                                          static_cast<s32>( WidgetId::GenerateSkyboxMaterialsId ),
                                          "Generate Skybox Materials", "Generate Skybox Materials",
                                          ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem(
                utilMaterialMenu, static_cast<s32>( WidgetId::SetupMaterialsId ), "Setup Materials",
                "Setup Materials", ui::IUIMenuItem::Type::Normal );
            utilMenu->addMenuItem( utilMaterialMenu );

            auto utilProjectMenu = ui->addElementByType<ui::IUIMenu>();
            utilProjectMenu->setLabel( "Project" );

            ApplicationUtil::addMenuItem( utilProjectMenu, static_cast<s32>( WidgetId::CleanProjectId ),
                                          "Clean", "Clean", ui::IUIMenuItem::Type::Normal );

            utilMenu->addMenuItem( utilProjectMenu );

            menuBar->addMenu( utilMenu );

            auto proceduralMenu = ui->addElementByType<ui::IUIMenu>();
            proceduralMenu->setLabel( "Procedural" );
            ApplicationUtil::addMenuItem(
                proceduralMenu, static_cast<s32>( WidgetId::LoadProceduralSceneId ),
                "Load Procedural Scene", "Load Procedural Scene", ui::IUIMenuItem::Type::Normal );
            menuBar->addMenu( proceduralMenu );

            auto windowMenu = ui->addElementByType<ui::IUIMenu>();
            windowMenu->setLabel( "Window" );
            ApplicationUtil::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ComponentsId ),
                                          "Components", "Components", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ResourcesId ),
                                          "Resources", "Resources", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( windowMenu, static_cast<s32>( WidgetId::InputWindowId ),
                                          "Input", "Input", ui::IUIMenuItem::Type::Normal );
            ApplicationUtil::addMenuItem( windowMenu, static_cast<s32>( WidgetId::ProfilerWindowId ),
                                          "Profiler", "Profiler", ui::IUIMenuItem::Type::Normal );

            menuBar->addMenu( windowMenu );

            auto helpMenu = ui->addElementByType<ui::IUIMenu>();
            helpMenu->setLabel( "Help" );
            ApplicationUtil::addMenuItem( helpMenu, static_cast<s32>( WidgetId::AboutId ), "About",
                                          "About", ui::IUIMenuItem::Type::Normal );
            menuBar->addMenu( helpMenu );

            auto menubarListener = fb::make_ptr<CUIMenuBarListener>();
            menubarListener->setOwner( this );
            m_menubarListener = menubarListener;

            menuBar->addObjectListener( m_menubarListener );

            auto toolbar = ui->addElementByType<ui::IUIToolbar>();

            auto toolbarListener = fb::make_ptr<ToolbarListener>();
            toolbarListener->setOwner( this );
            toolbar->addObjectListener( toolbarListener );

            auto toolbarPlayButton = ui->addElementByType<ui::IUIButton>();
            toolbarPlayButton->setElementId( static_cast<s32>( WidgetId::RunId ) );
            toolbarPlayButton->setLabel( "Play" );
            toolbar->addChild( toolbarPlayButton );

            auto toolbarStopButton = ui->addElementByType<ui::IUIButton>();
            toolbarStopButton->setElementId( static_cast<s32>( WidgetId::StopId ) );
            toolbarStopButton->setLabel( "Stop" );
            toolbar->addChild( toolbarStopButton );

            auto toolbarStatsButton = ui->addElementByType<ui::IUIButton>();
            toolbarStatsButton->setElementId( static_cast<s32>( WidgetId::StatsId ) );
            toolbarStatsButton->setLabel( "Stats" );
            toolbar->addChild( toolbarStatsButton );

            auto reloadScriptsButton = ui->addElementByType<ui::IUIButton>();
            reloadScriptsButton->setElementId( static_cast<s32>( WidgetId::ReloadScriptsId ) );
            reloadScriptsButton->setLabel( "Reload" );
            toolbar->addChild( reloadScriptsButton );

            auto toggleEditorCameraButton = ui->addElementByType<ui::IUIButton>();
            toggleEditorCameraButton->setElementId( static_cast<s32>( WidgetId::ToggleEditorCameraId ) );
            toggleEditorCameraButton->setLabel( "Editor Camera" );
            toolbar->addChild( toggleEditorCameraButton );

            auto application = ui->getApplication();
            if( application )
            {
                application->setMenubar( menuBar );
                application->setToolbar( toolbar );
            }

            auto sceneWindow = fb::make_ptr<SceneWindow>( nullptr );
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

            m_updateSelectionJob = fb::make_ptr<UpdateSelectionJob>();

            if( auto stateContext = applicationManager->getStateObject() )
            {
                auto eventListener = fb::make_ptr<EventListener>();
                eventListener->setOwner( this );
                stateContext->addEventListener( eventListener );

                m_eventListener = eventListener;
            }
        }

        void UIManager::unload( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
            if( m_projectWindow )
            {
                m_projectWindow->update( t, dt );
            }

            if( m_frameStatistics )
            {
                m_frameStatistics->update();
            }
        }

        String UIManager::saveEntity( const String &fileName )
        {
            // StringW defaultFileName = StringUtil::getWString(fileName);

            // wxFileDialog dialog(m_appFrame,
            //	_T("Save Entity"),
            //	wxEmptyString,
            //	defaultFileName.c_str(),
            //	_T("Entity (*.entity)|*.entity"),
            //	wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

            // dialog.SetFilterIndex(0);

            // if (dialog.ShowModal() == wxID_OK)
            //{
            //	return String(dialog.GetPath().c_str());
            // }

            return StringUtil::EmptyString;
        }

        String UIManager::saveScript( const String &fileName )
        {
            // StringW defaultFileName = StringUtil::getWString(fileName);

            // wxFileDialog dialog(m_appFrame,
            //	_T("Save Script"),
            //	wxEmptyString,
            //	defaultFileName.c_str(),
            //	_T("Lua Script (*.lua)|*.lua"),
            //	wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

            // dialog.SetFilterIndex(0);

            // if (dialog.ShowModal() == wxID_OK)
            //{
            //	return String(dialog.GetPath().c_str());
            // }

            return StringUtil::EmptyString;
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

        SmartPtr<SceneWindow> UIManager::getSceneWindow() const
        {
            return m_sceneWindow;
        }

        void UIManager::setSceneWindow( SmartPtr<SceneWindow> sceneWindow )
        {
            m_sceneWindow = sceneWindow;
        }

        SmartPtr<ActorWindow> UIManager::getActorWindow() const
        {
            return m_actorWindow;
        }

        void UIManager::updateSelection()
        {
            if( m_updateSelectionJob->isFinished() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto jobQueue = applicationManager->getJobQueue();

                //jobQueue->queueJob( m_updateSelectionJob );
                m_updateSelectionJob->execute();
            }
        }

        void UIManager::updateActorSelection()
        {
            auto propertiesWindow = getPropertiesWindow();
            if( propertiesWindow )
            {
                propertiesWindow->updateSelection();
            }
        }

        void UIManager::updateComponentSelection()
        {
            auto propertiesWindow = getPropertiesWindow();
            if( propertiesWindow )
            {
                propertiesWindow->updateSelection();
            }

            if( auto objectWindow = getObjectWindow() )
            {
                objectWindow->updateSelection();
            }

            auto applicationManager = core::IApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();

            // auto selection = selectionManager->getSelected();
            // for (auto object : selection)
            //{
            //	if (object)
            //	{
            //		auto actor = fb::dynamic_pointer_cast<IActor>(object);
            //		auto tranform = fb::dynamic_pointer_cast<ITransform>(object);
            //		auto component = fb::dynamic_pointer_cast<scene::IComponent>(object);
            //		auto resource = fb::dynamic_pointer_cast<IResource>(object);

            //		if (resource)
            //		{
            //			int stop = 0;
            //			stop = 0;
            //		}
            //	}
            //}
        }

        PropertiesWindow *UIManager::getPropertiesWindow() const
        {
            return m_propertiesWindow;
        }

        void UIManager::setPropertiesWindow( PropertiesWindow *val )
        {
            m_propertiesWindow = val;
        }

        ApplicationFrame *UIManager::getApplicationFrame() const
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

        TerrainWindow *UIManager::getTerrainWindow() const
        {
            return m_terrainWindow;
        }

        void UIManager::setTerrainWindow( TerrainWindow *val )
        {
            m_terrainWindow = val;
        }

        void UIManager::showComponentEditWindow()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto selectionManager = applicationManager->getSelectionManager();

            // auto selection = selectionManager->getSelected();
            // for (auto object : selection)
            //{
            //	if (object)
            //	{
            //		auto actor = fb::dynamic_pointer_cast<IActor>(object);
            //		auto transform = fb::dynamic_pointer_cast<ITransform>(object);
            //		auto component = fb::dynamic_pointer_cast<scene::IComponent>(object);
            //		auto resource = fb::dynamic_pointer_cast<IResource>(object);
            //		auto fileSelection = fb::dynamic_pointer_cast<FileSelection>(object);

            //		if (component)
            //		{
            //			//if (component->isExactly<scene::MaterialComponent>())
            //			//{
            //			//	if (m_materialWindow)
            //			//	{
            //			//		m_materialWindow->show();
            //			//	}
            //			//}
            //		}
            //		else if (fileSelection)
            //		{
            //			hideComponentEditWindows();

            //			//auto filePath = fileSelection->getFilePath();
            //			//auto ext = Path::getFileExtension(filePath);
            //			//if (ext == ".fbx" || ext == ".FBX")
            //			//{
            //			//	if (m_meshImportWindow)
            //			//	{
            //			//		m_meshImportWindow->show();
            //			//	}
            //			//}
            //		}
            //	}
            //}
        }

        void UIManager::hideComponentEditWindows() const
        {
            // if (m_terrainWindow)
            //{
            //	m_terrainWindow->hide();
            // }

            // if (m_meshImportWindow)
            //{
            //	m_meshImportWindow->hide();
            // }

            // if (m_foliageWindow)
            //{
            //	m_foliageWindow->hide();
            // }

            // if (m_roadWindow)
            //{
            //	m_roadWindow->hide();
            // }

            // if (m_textureWindow)
            //{
            //	m_textureWindow->hide();
            // }

            // if (m_materialWindow)
            //{
            //	m_materialWindow->hide();
            // }
        }

        MeshImportWindow *UIManager::getMeshImportWindow() const
        {
            return m_meshImportWindow;
        }

        void UIManager::setMeshImportWindow( MeshImportWindow *val )
        {
            m_meshImportWindow = val;
        }

        FoliageWindow *UIManager::getFoliageWindow() const
        {
            return m_foliageWindow;
        }

        void UIManager::setFoliageWindow( FoliageWindow *val )
        {
            m_foliageWindow = val;
        }

        RoadFrame *UIManager::getRoadWindow() const
        {
            return m_roadWindow;
        }

        void UIManager::setRoadWindow( RoadFrame *val )
        {
            m_roadWindow = val;
        }

        HoudiniWindow *UIManager::getHoudiniWindow() const
        {
            return m_houdiniWindow;
        }

        void UIManager::setHoudiniWindow( HoudiniWindow *val )
        {
            m_houdiniWindow = val;
        }

        RenderWindow *UIManager::getRenderWindow() const
        {
            return m_renderWindow;
        }

        void UIManager::setRenderWindow( RenderWindow *val )
        {
            m_renderWindow = val;
        }

        RenderWindow *UIManager::getGameWindow() const
        {
            return m_gameWindow;
        }

        void UIManager::setGameWindow( RenderWindow *val )
        {
            m_gameWindow = val;
        }

        FileWindow *UIManager::getFileWindow() const
        {
            return m_fileWindow;
        }

        void UIManager::setFileWindow( FileWindow *val )
        {
            m_fileWindow = val;
        }

        SmartPtr<ProjectWindow> UIManager::getProjectWindow() const
        {
            return m_projectWindow;
        }

        void UIManager::setProjectWindow( SmartPtr<ProjectWindow> val )
        {
            m_projectWindow = val;
        }

        //
        // Array<ui::wxViewWindow*> UIManager::getWindows() const
        //{
        //	return m_windows;
        // }

        //
        // void UIManager::setWindows(const Array<ui::wxViewWindow*>& val)
        //{
        //	m_windows = val;
        // }

        //
        // void UIManager::addWindow(ui::wxViewWindow* window)
        //{
        //	m_windows.push_back(window);
        // }

        TextureWindow *UIManager::getTextureWindow() const
        {
            return m_textureWindow;
        }

        void UIManager::setTextureWindow( TextureWindow *textureWindow )
        {
            m_textureWindow = textureWindow;
        }

        MaterialWindow *UIManager::getMaterialWindow() const
        {
            return m_materialWindow;
        }

        void UIManager::setMaterialWindow( MaterialWindow *materialWindow )
        {
            m_materialWindow = materialWindow;
        }

        SmartPtr<ObjectWindow> UIManager::getObjectWindow() const
        {
            return m_objectWindow;
        }

        void UIManager::setObjectWindow( SmartPtr<ObjectWindow> objectWindow )
        {
            m_objectWindow = objectWindow;
        }

        SmartPtr<ObjectBrowserDialog> UIManager::getObjectBrowserDialog() const
        {
            return m_objectBrowserDialog;
        }

        void UIManager::setObjectBrowserDialog( SmartPtr<ObjectBrowserDialog> objectBrowserDialog )
        {
            m_objectBrowserDialog = objectBrowserDialog;
        }

        SmartPtr<ResourceDatabaseDialog> UIManager::getResourceDatabaseDialog() const
        {
            return m_resourceDatabaseDialog;
        }

        void UIManager::setResourceDatabaseDialog(
            SmartPtr<ResourceDatabaseDialog> resourceDatabaseDialog )
        {
            m_resourceDatabaseDialog = resourceDatabaseDialog;
        }

        SmartPtr<AboutDialog> UIManager::getAboutDialog() const
        {
            return m_aboutDialog;
        }

        UIManager::CUIMenuBarListener::CUIMenuBarListener()
        {
        }

        UIManager::CUIMenuBarListener::~CUIMenuBarListener()
        {
        }

        Parameter UIManager::CUIMenuBarListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto fileSystem = applicationManager->getFileSystem();

            auto editorManager = EditorManager::getSingletonPtr();

            auto resourceDatabase = applicationManager->getResourceDatabase();
            auto factoryManager = applicationManager->getFactoryManager();
            auto jobQueue = applicationManager->getJobQueue();

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
                        auto filePath = project->getProjectFilePath();
                        if( !StringUtil::isNullOrEmpty( filePath ) )
                        {
                            project->setProjectPath( filePath );
                            project->save( filePath );
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

                    auto sceneManager = applicationManager->getSceneManager();
                    FB_ASSERT( sceneManager );

                    if( auto scene = sceneManager->getCurrentScene() )
                    {
                        scene->setName( "Untitled" );
                        scene->setFilePath( String() );
                        scene->clear();
                    }

                    ApplicationUtil::createDefaultSky();
                    ApplicationUtil::createDirectionalLight();

                    uiManager->rebuildSceneTree();
                }
                break;
                case WidgetId::SaveId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto fileSystem = applicationManager->getFileSystem();
                    FB_ASSERT( fileSystem );

                    auto editorManager = EditorManager::getSingletonPtr();
                    FB_ASSERT( editorManager );

                    auto uiManager = editorManager->getUI();
                    FB_ASSERT( uiManager );

                    auto sceneManager = applicationManager->getSceneManager();
                    FB_ASSERT( sceneManager );

                    if( auto scene = sceneManager->getCurrentScene() )
                    {
                        auto filePath = scene->getFilePath();
                        if( fileSystem->isExistingFile( filePath ) )
                        {
                            scene->saveScene();
                        }
                        else
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
                                    scene->saveScene( filePath );
                                }
                            }
                        }
                    }
                }
                break;
                case WidgetId::SaveSceneAsId:
                {
                    auto editorManager = EditorManager::getSingletonPtr();
                    FB_ASSERT( editorManager );

                    auto uiManager = editorManager->getUI();
                    FB_ASSERT( uiManager );

                    //if( auto fileBrowser = uiManager->getFileBrowser() )
                    //{
                    //    auto elementId = static_cast<s32>( WidgetId::OpenSaveAsSceneDialog );
                    //    fileBrowser->setElementId( elementId );
                    //    fileBrowser->setDialogMode( ui::IUIFileBrowser::DialogMode::Save );
                    //    fileBrowser->setFileExtension( ".fbscene" );
                    //    fileBrowser->show();
                    //}
                }
                break;
                case WidgetId::SaveAllId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
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
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto jobQueue = applicationManager->getJobQueue();
                    FB_ASSERT( jobQueue );

                    auto job = fb::make_ptr<JobCreatePackage>();

                    auto dst = L"C:/dev/FirebladeRuntime/";
                    job->setDst( dst );

                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::AssetImportId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto resourceDatabase = applicationManager->getResourceDatabase();
                    FB_ASSERT( resourceDatabase );

                    resourceDatabase->importAssets();
                }
                break;
                case WidgetId::AssetReimportId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto resourceDatabase = applicationManager->getResourceDatabase();
                    FB_ASSERT( resourceDatabase );

                    resourceDatabase->reimportAssets();
                }
                break;
                case WidgetId::AssetDatabaseBuildId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto resourceDatabase = applicationManager->getResourceDatabase();
                    FB_ASSERT( resourceDatabase );

                    resourceDatabase->build();
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

                                auto applicationManager = core::IApplicationManager::instance();
                                FB_ASSERT( applicationManager );

                                auto project = editorManager->getProject();
                                FB_ASSERT( project );

                                auto uiManager = editorManager->getUI();
                                FB_ASSERT( uiManager );

                                auto fileSystem = applicationManager->getFileSystem();
                                FB_ASSERT( fileSystem );

                                auto sceneData = ApplicationUtil::importScene( filePath );

                                auto sceneManager = applicationManager->getSceneManager();
                                //auto scene = sceneManager->getCurrentScene();
                                //if( scene )
                                //{
                                //    scene->fromData( sceneData );
                                //}

                                uiManager->rebuildSceneTree();
                            }
                        }
                    }
                }
                break;
                case WidgetId::ImportUnityYamlId:
                {
                    auto job = fb::make_ptr<ImportUnityYaml>();
                    jobQueue->queueJob( job );
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
                case WidgetId::GenerateSkyboxMaterialsId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto job = fb::make_ptr<GenerateSkyboxMaterials>();
                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::CreateBoxTestId:
                {
                    auto sceneManager = applicationManager->getSceneManager();

                    auto cube = ApplicationUtil::createDefaultCube( true );
                    auto ground = ApplicationUtil::createDefaultGround();

                    uiManager->rebuildSceneTree();
                }
                break;
                case WidgetId::CleanProjectId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto jobQueue = applicationManager->getJobQueue();

                    auto job = fb::make_ptr<ProjectCleanJob>();
                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::SetupMaterialsId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto jobQueue = applicationManager->getJobQueue();

                    auto job = fb::make_ptr<SetupMaterialJob>();
                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::OptimiseDatabasesId:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto jobQueue = applicationManager->getJobQueue();

                    auto job = fb::make_ptr<OptimiseDatabasesJob>();
                    jobQueue->queueJob( job );
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
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto commandManager = applicationManager->getCommandManager();

                    auto cmd = fb::make_ptr<FileMenuCmd>();
                    cmd->setItemId( widgetID );
                    commandManager->addCommand( cmd );
                }
                break;
                }
            }

            return Parameter();
        }

        UIManager *UIManager::CUIMenuBarListener::getOwner() const
        {
            return m_owner;
        }

        void UIManager::CUIMenuBarListener::setOwner( UIManager *owner )
        {
            m_owner = owner;
        }

        void UIManager::setAboutDialog( SmartPtr<AboutDialog> aboutDialog )
        {
            m_aboutDialog = aboutDialog;
        }

        SmartPtr<InputManagerWindow> UIManager::getInputManagerWindow() const
        {
            return m_inputManagerWindow;
        }

        void UIManager::setInputManagerWindow( SmartPtr<InputManagerWindow> inputManagerWindow )
        {
            m_inputManagerWindow = inputManagerWindow;
        }

        SmartPtr<ProfilerWindow> UIManager::getProfilerWindow() const
        {
            return m_profilerWindow;
        }

        void UIManager::setProfilerWindow( SmartPtr<ProfilerWindow> profilerWindow )
        {
            m_profilerWindow = profilerWindow;
        }

        bool UIManager::isValid() const
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

        Parameter UIManager::ToolbarListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                           const Array<Parameter> &arguments,
                                                           SmartPtr<ISharedObject> sender,
                                                           SmartPtr<ISharedObject> object,
                                                           SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto taskManager = applicationManager->getTaskManager();
            auto jobQueue = applicationManager->getJobQueue();
            auto commandManager = applicationManager->getCommandManager();

            if( eventValue == IEvent::handleSelection )
            {
                auto editorManager = EditorManager::getSingletonPtr();
                FB_ASSERT( editorManager );

                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
                auto iElementId = element->getElementId();

                auto id = static_cast<WidgetId>( iElementId );
                switch( id )
                {
                case WidgetId::RunId:
                {
                    auto job = fb::make_ptr<PlaymodeJob>();
                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::StopId:
                {
                    auto job = fb::make_ptr<LeavePlaymodeJob>();
                    jobQueue->queueJob( job );
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
                    jobQueue->queueJob( job );
                }
                break;
                case WidgetId::ToggleEditorCameraId:
                {
                    auto cmd = fb::make_ptr<ToggleEditorCamera>();
                    commandManager->addCommand( cmd );
                }
                break;
                default:
                {
                }
                break;
                }
            }

            return Parameter();
        }

        UIManager *UIManager::ToolbarListener::getOwner() const
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

            ui->showComponentEditWindow();
        }

        Parameter UIManager::EventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                         const Array<Parameter> &arguments,
                                                         SmartPtr<ISharedObject> sender,
                                                         SmartPtr<ISharedObject> object,
                                                         SmartPtr<IEvent> event )
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

            return Parameter();
        }

        UIManager *UIManager::EventListener::getOwner() const
        {
            return m_owner;
        }

        void UIManager::EventListener::setOwner( UIManager *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
