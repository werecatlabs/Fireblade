#include <GameEditorPCH.h>
#include "editor/EditorManager.h"
#include "ui/ProjectWindow.h"
#include "editor/Project.h"
#include <ui/UIManager.h>
#include <FBApplication/FBApplication.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
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
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            timer->setTimeSinceLevelLoad( 0.0 );

            auto editorManager = getSingletonPtr();
            auto project = editorManager->getProject();
            FB_ASSERT( project );

            project->setProjectPath( filePath );

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
            applicationManager->setSettingsCachePath( settingsCachePath );

            auto relativeProjectPath = Path::getRelativePath( projectPath, filePath );
            project->load( relativeProjectPath );

            auto editorSettings = applicationManager->getEditorSettings();
            editorSettings->setProperty( "Project Path", filePath );

            uiManager->rebuildSceneTree();

            auto projectWindow = uiManager->getProjectWindow();
            if( projectWindow )
            {
                projectWindow->buildTree();
            }
        }

        String EditorManager::getProjectPath() const
        {
            return m_projectPath;
        }

        void EditorManager::setProjectPath( const String &path )
        {
            m_projectPath = path;
        }

        String EditorManager::getCachePath() const
        {
            return m_cachePath;
        }

        void EditorManager::setCachePath( const String &path )
        {
            m_cachePath = path;
        }

        SmartPtr<ProjectManager> EditorManager::getProjectManager() const
        {
            return m_projectManager;
        }

        void EditorManager::setProjectManager( SmartPtr<ProjectManager> projectManager )
        {
            m_projectManager = projectManager;
        }

        SmartPtr<Project> EditorManager::getProject() const
        {
            return m_project;
        }

        void EditorManager::setProject( SmartPtr<Project> val )
        {
            m_project = val;
        }

        SmartPtr<UIManager> EditorManager::getUI() const
        {
            return m_ui;
        }

        void EditorManager::setUI( SmartPtr<UIManager> ui )
        {
            m_ui = ui;
        }

        SmartPtr<render::IDecalCursor> EditorManager::getDecalCursor() const
        {
            return m_decalCursor;
        }

        void EditorManager::setDecalCursor( SmartPtr<render::IDecalCursor> val )
        {
            m_decalCursor = val;
        }

        bool EditorManager::getEditTerrain() const
        {
            return m_editTerrain;
        }

        void EditorManager::setEditTerrain( bool val )
        {
            m_editTerrain = val;
        }

        bool EditorManager::getEditFoliage() const
        {
            return m_editFoliage;
        }

        void EditorManager::setEditFoliage( bool val )
        {
            m_editFoliage = val;
        }

        bool EditorManager::getFileSaved() const
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
                auto ext = Path::getFileExtension( path );
                if( ext == ".fbx" || ext == ".FBX" || ext == ".hda" || ext == ".HDA" )
                {
                    auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<EditorManager> EditorManager::getSingletonPtr()
        {
            return m_singleton;
        }

        void EditorManager::setSingletonPtr( SmartPtr<EditorManager> editorManager )
        {
            m_singleton = editorManager;
        }

        SmartPtr<TranslateManipulator> EditorManager::getTranslateManipulator() const
        {
            return m_translateManipulator;
        }

        void EditorManager::setTranslateManipulator( SmartPtr<TranslateManipulator> val )
        {
            m_translateManipulator = val;
        }

        SmartPtr<RotateManipulator> EditorManager::getRotateManipulator() const
        {
            return m_rotateManipulator;
        }

        void EditorManager::setRotateManipulator( SmartPtr<RotateManipulator> val )
        {
            m_rotateManipulator = val;
        }

        SmartPtr<ScaleManipulator> EditorManager::getScaleManipulator() const
        {
            return m_scaleManipulator;
        }

        void EditorManager::setScaleManipulator( SmartPtr<ScaleManipulator> val )
        {
            m_scaleManipulator = val;
        }

    }  // end namespace editor
}  // end namespace fb
