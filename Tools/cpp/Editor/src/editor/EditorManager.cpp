#include <GameEditorPCH.h>
#include "editor/EditorManager.h"
#include "ui/ProjectWindow.h"
#include "editor/Project.h"
#include <ui/UIManager.h>

#include <FBCore/FBCore.h>

namespace fb::editor
{

    SmartPtr<EditorManager> EditorManager::m_singleton = nullptr;

    EditorManager::EditorManager() :
        m_editTerrain( false ),
        m_editFoliage( false ),
        m_fileSaved( true ),
        m_enablePhysics( false )
    {
    }

    EditorManager::~EditorManager()
    {
        unload( nullptr );
    }

    void EditorManager::unload( SmartPtr<ISharedObject> data )
    {
        if( auto ui = getUI() )
        {
            ui->unload( nullptr );
            setUI( nullptr );
        }

        if( m_translateManipulator )
        {
            m_translateManipulator->unload( data );
            m_translateManipulator = nullptr;
        }

        if( m_rotateManipulator )
        {
            m_rotateManipulator->unload( data );
            m_rotateManipulator = nullptr;
        }

        if( m_scaleManipulator )
        {
            m_scaleManipulator->unload( data );
            m_scaleManipulator = nullptr;
        }
    }

    void EditorManager::update( time_interval t, time_interval dt )
    {
        if( m_ui )
        {
            m_ui->update( t, dt );
        }

        //if( m_sceneViewManager )
        //{
        //    m_sceneViewManager->update( t, dt );
        //}
    }

    void EditorManager::importAssets()
    {
    }

    void EditorManager::loadProject( const String &filePath )
    {
        FB_ASSERT( Path::isExistingFile( filePath ) );

        if( Path::isExistingFile( filePath ) )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            timer->setTimeSinceLevelLoad( 0.0 );

            auto editorManager = getSingletonPtr();
            auto project = editorManager->getProject();
            FB_ASSERT( project );

            project->setPath( filePath );

            auto uiManager = editorManager->getUI();
            FB_ASSERT( uiManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto path = Path::getFilePath( filePath );
            path = StringUtil::cleanupPath( path );

            applicationManager->setProjectPath( path );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            fileSystem->addFolder( path, true );

            auto cachePath = path + "/Cache/";
            fileSystem->addFolder( cachePath, true );
            applicationManager->setCachePath( cachePath );

            auto settingsCachePath = path + "/SettingsCache/";
            fileSystem->addFolder( settingsCachePath, true );
            applicationManager->setSettingsPath( settingsCachePath );

            project->loadFromFile( filePath );

            if( auto editorSettings = applicationManager->getEditorSettings() )
            {
                editorSettings->setProperty( "Project Path", filePath );
            }

            uiManager->rebuildSceneTree();

            auto projectWindow = uiManager->getProjectWindow();
            if( projectWindow )
            {
                projectWindow->buildTree();
            }
        }
        else
        {
            FB_LOG_ERROR( "EditorManager::loadProject: Project file not found" );
            MessageBoxUtil::show( "Project file not found" );
        }
    }

    auto EditorManager::getProjectPath() const -> String
    {
        return m_projectPath;
    }

    void EditorManager::setProjectPath( const String &path )
    {
        m_projectPath = path;
    }

    auto EditorManager::getCachePath() const -> String
    {
        return m_cachePath;
    }

    void EditorManager::setCachePath( const String &path )
    {
        m_cachePath = path;
    }

    auto EditorManager::getProjectManager() const -> SmartPtr<ProjectManager>
    {
        return m_projectManager;
    }

    void EditorManager::setProjectManager( SmartPtr<ProjectManager> projectManager )
    {
        m_projectManager = projectManager;
    }

    auto EditorManager::getProject() const -> SmartPtr<Project>
    {
        return m_project;
    }

    void EditorManager::setProject( SmartPtr<Project> val )
    {
        m_project = val;
    }

    auto EditorManager::getUI() const -> SmartPtr<UIManager>
    {
        return m_ui;
    }

    void EditorManager::setUI( SmartPtr<UIManager> ui )
    {
        m_ui = ui;
    }

    auto EditorManager::getDecalCursor() const -> SmartPtr<render::IDecalCursor>
    {
        return m_decalCursor;
    }

    void EditorManager::setDecalCursor( SmartPtr<render::IDecalCursor> val )
    {
        m_decalCursor = val;
    }

    auto EditorManager::getEditTerrain() const -> bool
    {
        return m_editTerrain;
    }

    void EditorManager::setEditTerrain( bool val )
    {
        m_editTerrain = val;
    }

    auto EditorManager::getEditFoliage() const -> bool
    {
        return m_editFoliage;
    }

    void EditorManager::setEditFoliage( bool val )
    {
        m_editFoliage = val;
    }

    auto EditorManager::getFileSaved() const -> bool
    {
        return m_fileSaved;
    }

    void EditorManager::setFileSaved( bool val )
    {
        m_fileSaved = val;
    }

    void EditorManager::previewAsset( const String &path )
    {
        try
        {
            if( ApplicationUtil::isSupportedMesh( path ) )
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto sceneManager = applicationManager->getSceneManager();
                auto scene = sceneManager->getCurrentScene();
                auto prefabManager = applicationManager->getPrefabManager();

                auto resource = prefabManager->loadPrefab( path );
                if( resource )
                {
                    auto actor = resource->createActor();
                    if( actor )
                    {
                        scene->addActor( actor );
                        scene->registerAllUpdates( actor );
                    }
                }

                auto uiManager = getUI();
                if( uiManager )
                {
                    uiManager->rebuildSceneTree();
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto EditorManager::getSingletonPtr() -> SmartPtr<EditorManager>
    {
        return m_singleton;
    }

    void EditorManager::setSingletonPtr( SmartPtr<EditorManager> editorManager )
    {
        m_singleton = editorManager;
    }

    auto EditorManager::getTranslateManipulator() const -> SmartPtr<TranslateManipulator>
    {
        return m_translateManipulator;
    }

    void EditorManager::setTranslateManipulator( SmartPtr<TranslateManipulator> val )
    {
        m_translateManipulator = val;
    }

    auto EditorManager::getRotateManipulator() const -> SmartPtr<RotateManipulator>
    {
        return m_rotateManipulator;
    }

    void EditorManager::setRotateManipulator( SmartPtr<RotateManipulator> val )
    {
        m_rotateManipulator = val;
    }

    auto EditorManager::getScaleManipulator() const -> SmartPtr<ScaleManipulator>
    {
        return m_scaleManipulator;
    }

    void EditorManager::setScaleManipulator( SmartPtr<ScaleManipulator> val )
    {
        m_scaleManipulator = val;
    }

}  // namespace fb::editor
