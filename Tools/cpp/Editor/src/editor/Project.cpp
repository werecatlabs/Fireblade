#include <GameEditorPCH.h>
#include <editor/Project.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBApplication/FBApplication.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        const String Project::DEFAULT_PROJECT_NAME = "Project";
        const String Project::DEFAULT_MEDIA_PATH = "../Media/";
        const String Project::DEFAULT_SCRIPTS_PATH = "../Media/Scripts/";
        const String Project::DEFAULT_ENTITIES_PATH = "../Media/Scripts/Entities/";
        const String Project::DEFAULT_VERSION = "1.0.0";

        Project::Project() :
            m_label( StringUtil::EmptyString ),
            m_projectDirectory( StringUtil::EmptyString ),
            m_projectPath( StringUtil::EmptyString ),
            m_applicationFilePath( StringUtil::EmptyString ),
            m_scriptsPath( StringUtil::EmptyString ),
            m_entitiesPath( StringUtil::EmptyString ),
            m_version( StringUtil::EmptyString )
        {
            // try
            //{
            //	m_label = DEFAULT_PROJECT_NAME;
            //	m_scriptsPath = DEFAULT_SCRIPTS_PATH;
            //	m_entitiesPath = DEFAULT_ENTITIES_PATH;
            //	m_version = DEFAULT_VERSION;

            //	setCameraManagerTemplate(SmartPtr<CameraManagerTemplate>(new CameraManagerTemplate));

            //	setResourcesetTemplate(SmartPtr<ResourceSetTemplate>(new ResourceSetTemplate));

            //	setApplicationTemplate(SmartPtr<ApplicationTemplate>(new ApplicationTemplate));
            //	addCoreComponents();

            //	addDefaultTasks();
            //	addDefaultScript();
            //}
            // catch (std::exception& e)
            //{
            //	wxMessageBox(e.what());
            //}
        }

        Project::~Project()
        {
        }

        void Project::create( const String &path )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto editorManager = EditorManager::getSingletonPtr();

                auto uiManager = editorManager->getUI();
                FB_ASSERT( uiManager );

                auto projectDirectory = StringUtil::cleanupPath( path );
                setProjectDirectory( projectDirectory );

                auto defaultData = fb::static_pointer_cast<Properties>( getDefaultData() );
                auto dataStr = DataUtil::toString( defaultData.get(), true );

                auto filePath = path + "/" + "project.fbproject";
                fileSystem->writeAllText( filePath, dataStr );

                uiManager->rebuildResourceTree();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Project::load( const String &filePath )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto scriptManager = applicationManager->getScriptManager();

            auto projectPath = StringUtil::cleanupPath( filePath );
            auto path = Path::getFilePath( projectPath );

            auto projectDirectory = StringUtil::cleanupPath( projectPath );
            setProjectDirectory( projectDirectory );

            try
            {
                String scriptExt;

#if FB_ENABLE_LUA
                scriptExt = ".lua";
#elif FB_ENABLE_PYTHON
                scriptExt = ".py";
#endif

                auto scripts = fileSystem->getFileNamesWithExtension( scriptExt );
                scriptManager->loadScripts( scripts );

                setFilePath( projectPath );

                auto dataStr = fileSystem->readAllText( projectPath );
                auto data = fb::make_ptr<Properties>();
                DataUtil::parse( dataStr, data.get() );

                fromData( data );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Project::save( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto path = Path::getFilePath( filePath );
                setProjectDirectory( path );

                auto pData = fb::static_pointer_cast<Properties>( toData() );
                auto dataStr = DataUtil::toString( pData.get(), true );

                fileSystem->writeAllText( filePath, dataStr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Project::save()
        {
            auto filePath = getFilePath();
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            if( !StringUtil::isNullOrEmpty( filePath ) )
            {
                save( filePath );
            }
        }

        String Project::getLabel() const
        {
            return m_label;
        }

        void Project::setLabel( const String &val )
        {
            m_label = val;
        }

        String Project::getProjectDirectory() const
        {
            return m_projectDirectory;
        }

        void Project::setProjectDirectory( const String &val )
        {
            m_projectDirectory = val;
        }

        String Project::getWorkingDirectory() const
        {
            return m_projectDirectory;
        }

        void Project::setWorkingDirectory( const String &val )
        {
            m_projectDirectory = val;
        }

        String Project::getApplicationFilePath() const
        {
            return m_applicationFilePath;
        }

        void Project::setApplicationFilePath( const String &val )
        {
            m_applicationFilePath = val;
        }

        String Project::getEntitiesPath() const
        {
            return m_entitiesPath;
        }

        void Project::setEntitiesPath( const String &val )
        {
            m_entitiesPath = val;
        }

        Array<String> Project::getPaths() const
        {
            return m_paths;
        }

        void Project::setPaths( Array<String> val )
        {
            m_paths = val;
        }

        void Project::addPath( const String &val )
        {
            m_paths.push_back( val );
        }

        void Project::setProperties( const Properties &properties )
        {
            // properties.getPropertyValue("name", m_label);
            // properties.getPropertyValue("application", m_applicationFilePath);

            // m_mediaPaths.clear();

            // String value;
            // properties.getPropertyValue("resourcePaths", value);
            // StringUtil::parseArray(value, m_mediaPaths);
            // for(u32 i=0; i<m_mediaPaths.size(); ++i)
            //{
            //	String mediaPath = m_mediaPaths[i];

            //	if(!StringUtil::isPathRelative(m_projectDirectory, mediaPath))
            //		m_mediaPaths[i] = Path::getRelativePath( m_projectDirectory, mediaPath );
            //}

            // auto applicationManager = core::IApplicationManager::instance();
            // SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();
            // for(u32 i=0; i<m_mediaPaths.size(); ++i)
            //{
            //	try
            //	{
            //		String fullPath = Path::getFullPath(m_projectDirectory, m_mediaPaths[i]);
            //		fileSystem->addFileArchive(fullPath, true, true, FB_ARCH_TYPE_FOLDER);
            //	}
            //	catch (std::exception& e)
            //	{
            //		LOG_MESSAGE("Application", "Error creating relative path.");
            //	}
            //	catch (...)
            //	{
            //		LOG_MESSAGE("Application", "Error creating relative path.");
            //	}
            // }

            // SmartPtr<IGraphicsSystem> gfxSystem = engine->getGraphicsSystem();
            // gfxSystem->loadResources();

            properties.getPropertyValue( "scriptsPath", m_scriptsPath );
            properties.getPropertyValue( "entitiesPath", m_entitiesPath );
            properties.getPropertyValue( "version", m_version );
        }

        void Project::getProperties( Properties &properties ) const
        {
            properties.addProperty( Property( "name", m_label, "Name", "string" ) );
            properties.addProperty(
                Property( "application", m_applicationFilePath, "Application", "file" ) );

            String value = StringUtil::toString( m_mediaPaths );
            properties.addProperty( Property( "resourcePaths", value, "Resource Paths", "folders" ) );

            // properties.addProperty(Property("scriptsPath", m_scriptsPath, "Scripts Path", "string"));
            // properties.addProperty(Property("entitiesPath", m_entitiesPath, "Entities Path",
            // "string"));
            properties.addProperty( Property( "version", m_version, "Version", "string" ) );
        }

        const String &Project::getEditableType() const
        {
            static String editableType = "Project";
            return editableType;
        }

        void Project::setDirty( bool dirty )
        {
        }

        String Project::getSelectedProjectPath() const
        {
            return m_selectedProjectPath;
        }

        void Project::setSelectedProjectPath( const String &val )
        {
            m_selectedProjectPath = val;
        }

        void Project::applyDefaults()
        {
        }

        SmartPtr<ISharedObject> Project::getOwner() const
        {
            return nullptr;
        }

        void Project::getOwner( SmartPtr<ISharedObject> owner )
        {
        }

        String Project::getCurrentScenePath() const
        {
            return m_currentScenePath;
        }

        void Project::setCurrentScenePath( const String &val )
        {
            m_currentScenePath = val;
        }

        SmartPtr<ISharedObject> Project::getDefaultData() const
        {
            auto properties = fb::make_ptr<Properties>();

            auto uuid = StringUtil::getUUID();
            auto productName = String( "Untitled" );
            auto companyName = String( "Untitled" );

            properties->setProperty( "projectVersion", "1.0.0" );
            properties->setProperty( "uuid", uuid );
            properties->setProperty( "productName", productName );
            properties->setProperty( "companyName", companyName );
            properties->setProperty( "currentScenePath", getCurrentScenePath() );

            return properties;
        }

        SmartPtr<ISharedObject> Project::toData() const
        {
            auto properties = fb::make_ptr<Properties>();

            properties->setProperty( "projectVersion", "1.0.0" );
            properties->setProperty( "uuid", m_uuid );
            properties->setProperty( "productName", m_productName );
            properties->setProperty( "companyName", m_companyName );
            properties->setProperty( "currentScenePath", getCurrentScenePath() );

            return properties;
        }

        void Project::fromData( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();

                auto sceneManager = applicationManager->getSceneManager();

                auto properties = fb::static_pointer_cast<Properties>( data );
                auto currentScenePath = properties->getProperty( "currentScenePath" );

                auto cleanCurrentScenePath = StringUtil::cleanupPath( currentScenePath );
                setCurrentScenePath( cleanCurrentScenePath );

                sceneManager->loadScene( cleanCurrentScenePath );

                sceneManager->edit();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Project::compile()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto fileSystem = applicationManager->getFileSystem();
            auto processManager = applicationManager->getProcessManager();

            auto projectManager = fb::make_ptr<fb::ProjectManager>();

            auto workingDirectory = Path::getWorkingDirectory();
            auto projectFolder = applicationManager->getProjectPath();
            auto cacheFolder = applicationManager->getCachePath();
            auto scriptProjectFolder = cacheFolder + "/project";
            auto projectName = Path::getLeaf( projectFolder );

            fileSystem->deleteFilesFromPath( scriptProjectFolder );

            projectManager->generateCMakeProject();

            fileSystem->createDirectories( scriptProjectFolder );
            Path::setWorkingDirectory( scriptProjectFolder );

            auto projectFolderW = StringUtil::toUTF8to16( projectFolder );

            Array<StringW> args;
            args.push_back( L"-T v142" );
            args.push_back( projectFolderW );

            processManager->shellExecute( L"cmake", args );

            auto buildCommandStr =
                L"\"C:/Program Files/Microsoft Visual "
                L"Studio/2022/Preview/Msbuild/Current/Bin/amd64/msbuild.exe\"";

            Array<StringW> buildArgs;
            buildArgs.push_back( StringUtil::toUTF8to16( projectName + ".sln" ) );

            processManager->shellExecute( buildCommandStr, buildArgs );

            Path::setWorkingDirectory( workingDirectory );

            applicationManager->setProjectLibraryName( projectName );

            auto numRetries = 0;
            auto dllPath = applicationManager->getProjectLibraryPath();
            while( !fileSystem->isExistingFile( dllPath ) && numRetries++ > 100 )
            {
                Thread::sleep( 1.0 );
            }

            auto scriptManager = applicationManager->getScriptManager();
            if( scriptManager )
            {
                scriptManager->reload( 0 );
            }
        }

        bool Project::isDirty() const
        {
            return false;
        }

        void Project::setMediaPaths( Array<String> val )
        {
            m_mediaPaths = val;
        }

        Array<String> Project::getMediaPaths() const
        {
            return m_mediaPaths;
        }

        void Project::setProjectPath( String val )
        {
            m_projectPath = val;
        }

        String Project::getProjectFilePath() const
        {
            return m_projectPath;
        }

    }  // end namespace editor
}  // end namespace fb
