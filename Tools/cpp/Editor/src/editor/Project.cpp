#include <GameEditorPCH.h>
#include <editor/Project.h>
#include <editor/EditorManager.h>
#include <jobs/LoadPluginJob.h>
#include <jobs/UnloadPluginJob.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
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
        m_graphicsSettingsDirector = fb::make_ptr<scene::GraphicsSettingsDirector>();
    }

    Project::~Project()
    {
        unload( nullptr );
    }

    void Project::load( SmartPtr<ISharedObject> data )
    {
    }

    void Project::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Project::create( const String &path )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();

            auto uiManager = editorManager->getUI();
            FB_ASSERT( uiManager );

            auto projectDirectory = StringUtil::cleanupPath( path );
            setPath( projectDirectory );

            auto defaultData = fb::static_pointer_cast<Properties>( getDefaultData() );
            auto dataStr = DataUtil::toString( defaultData.get(), true );

            auto filePath = path + "/" + "project.fbproject";
            fileSystem->writeAllText( filePath, dataStr );

            Path::createDirectories( path + "/Assets" );

            auto cachePath = path + "/Cache";
            Path::createDirectories( cachePath );

            auto enginePath = path + "/Engine";
            Path::createDirectories( enginePath );

            auto pluginPath = path + "/Plugin";
            Path::createDirectories( pluginPath );

            auto pluginHeader = getPluginHeader();
            auto pluginSource = getPluginSource();

            Path::writeAllText( pluginPath + "/Plugin.h", pluginHeader );
            Path::writeAllText( pluginPath + "/Plugin.cpp", pluginSource );

            uiManager->rebuildResourceTree();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Project::loadFromFile( const String &filePath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto jobQueue = applicationManager->getJobQueue();

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto scriptManager = applicationManager->getScriptManager();

        auto projectPath = StringUtil::cleanupPath( filePath );
        auto path = Path::getFilePath( projectPath );

        auto projectDirectory = StringUtil::cleanupPath( path );
        setPath( projectDirectory );

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

            auto projectPluginFileName = String( "Plugin.dll" );

            Array<String> pluginPaths;
            pluginPaths.reserve( 4 );

#if _DEBUG
            pluginPaths.push_back( projectDirectory + "/bin/windows/Debug/" + projectPluginFileName );
#else
            pluginPaths.push_back( projectDirectory + "/bin/windows/RelWithDebInfo/" +
                                   projectPluginFileName );
#endif
            //pluginPaths.push_back( projectDirectory + "/Assets/Plugins/x64/RelWithDebInfo/FBApplication.dll" );

            for( auto pluginPath : pluginPaths )
            {
                auto job = fb::make_ptr<LoadPluginJob>();
                job->setPluginPath( pluginPath );
                jobQueue->addJob( job );
                job->wait();
            }

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

    void Project::saveToFile( const String &filePath )
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto path = Path::getFilePath( filePath );
            setPath( path );

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
            saveToFile( filePath );
        }
    }

    String Project::getLabel() const
    {
        return m_label;
    }

    void Project::setLabel( const String &label )
    {
        m_label = label;
    }

    String Project::getPath() const
    {
        return m_projectDirectory;
    }

    void Project::setPath( const String &projectDirectory )
    {
        m_projectDirectory = projectDirectory;
    }

    String Project::getWorkingDirectory() const
    {
        return m_workingDirectory;
    }

    void Project::setWorkingDirectory( const String &workingDirectory )
    {
        m_workingDirectory = workingDirectory;
    }

    String Project::getApplicationFilePath() const
    {
        return m_applicationFilePath;
    }

    void Project::setApplicationFilePath( const String &applicationFilePath )
    {
        m_applicationFilePath = applicationFilePath;
    }

    Array<String> Project::getPaths() const
    {
        return m_paths;
    }

    void Project::setPaths( const Array<String> &paths )
    {
        m_paths = paths;
    }

    void Project::addPath( const String &path )
    {
        m_paths.push_back( path );
    }

    void Project::setProperties( SmartPtr<Properties> properties )
    {
        Resource<IProject>::setProperties( properties );

        properties->getPropertyValue( "scriptsPath", m_scriptsPath );
        properties->getPropertyValue( "entitiesPath", m_entitiesPath );
        properties->getPropertyValue( "version", m_version );
    }

    SmartPtr<Properties> Project::getProperties() const
    {
        auto properties = Resource<IProject>::getProperties();

        properties->setProperty( "scriptsPath", m_scriptsPath );
        properties->setProperty( "entitiesPath", m_entitiesPath );
        properties->setProperty( "version", m_version );

        return properties;
    }

    Array<SmartPtr<ISharedObject>> Project::getChildObjects() const
    {
        Array<SmartPtr<ISharedObject>> childObjects;
        childObjects.reserve( 12 );

        childObjects.push_back( m_graphicsSettingsDirector );

        return childObjects;
    }

    String Project::getSelectedProjectPath() const
    {
        return m_selectedProjectPath;
    }

    void Project::setSelectedProjectPath( const String &selectedProjectPath )
    {
        m_selectedProjectPath = selectedProjectPath;
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

    void Project::setCurrentScenePath( const String &currentScenePath )
    {
        m_currentScenePath = currentScenePath;
    }

    SmartPtr<ISharedObject> Project::getDefaultData() const
    {
        auto properties = fb::make_ptr<Properties>();

        auto uuid = StringUtil::getUUID();
        auto productName = String( "Untitled" );
        auto companyName = String( "Untitled" );
        auto currentScenePath = String();

        properties->setProperty( "projectVersion", "1.0.0" );
        properties->setProperty( "uuid", uuid );
        properties->setProperty( "productName", productName );
        properties->setProperty( "companyName", companyName );
        properties->setProperty( "currentScenePath", currentScenePath );

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
            auto applicationManager = core::ApplicationManager::instance();
            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto sceneManager = applicationManager->getSceneManager();

            auto properties = fb::static_pointer_cast<Properties>( data );
            auto currentScenePath = properties->getProperty( "currentScenePath" );

            auto cleanCurrentScenePath = StringUtil::cleanupPath( currentScenePath );
            setCurrentScenePath( cleanCurrentScenePath );

            sceneManager->loadScene( cleanCurrentScenePath );

            //resourceDatabase->refresh();
            sceneManager->edit();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Project::compile()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();
        auto processManager = applicationManager->getProcessManager();

        auto editorManager = EditorManager::getSingletonPtr();
        auto project = editorManager->getProject();

        auto plugin = project->getPlugin();

        auto pluginManager = applicationManager->getPluginManager();

        auto unloadPluginJob = fb::make_ptr<UnloadPluginJob>();
        unloadPluginJob->setPlugin( plugin );
        unloadPluginJob->execute();

        auto projectManager = fb::make_ptr<ProjectManager>();

        auto workingDirectory = Path::getWorkingDirectory();
        auto projectFolder = applicationManager->getProjectPath();
        auto cacheFolder = applicationManager->getCachePath();
        auto scriptProjectFolder = projectFolder + "/project";
        auto projectName = String( "Plugin" );  //Path::getLeaf( projectFolder );

        if( fileSystem->isExistingFolder( scriptProjectFolder ) )
        {
            fileSystem->deleteFilesFromPath( scriptProjectFolder );
        }
        else
        {
            fileSystem->createDirectories( scriptProjectFolder );
        }

        if( !fileSystem->isExistingFile( projectFolder + "/CMakeLists.txt" ) )
        {
            projectManager->generateProject();
        }

        Path::setWorkingDirectory( scriptProjectFolder );

        auto projectFolderW = StringUtil::toUTF8to16( projectFolder );

        Array<StringW> args;
        args.emplace_back( L"-T v143" );
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

        auto loadPluginJob = fb::make_ptr<LoadPluginJob>();
        loadPluginJob->setPluginPath( dllPath );
        loadPluginJob->execute();

        auto scriptManager = applicationManager->getScriptManager();
        if( scriptManager )
        {
            scriptManager->reload( nullptr );
        }
    }

    String Project::getPluginHeader() const
    {
        return "#ifndef Plugin_h__\n"
               "#define Plugin_h__\n"
               "\n"
               "#include \"FBCore/Interface/Memory/ISharedObject.h\"\n"
               "\n"
               "#if defined( __CYGWIN32__ )\n"
               "#    define UNITY_INTERFACE_API __stdcall\n"
               "#    define UNITY_INTERFACE_EXPORT __declspec( dllexport )\n"
               "#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( "
               "_WIN64 ) || \\\n"
               "    defined( WINAPI_FAMILY )\n"
               "#    define UNITY_INTERFACE_API __stdcall\n"
               "#    define UNITY_INTERFACE_EXPORT __declspec( dllexport )\n"
               "#elif defined( __MACH__ ) || defined( __ANDROID__ ) || defined( __linux__ )\n"
               "#    define UNITY_INTERFACE_API\n"
               "#    define UNITY_INTERFACE_EXPORT\n"
               "#else\n"
               "#    define UNITY_INTERFACE_API\n"
               "#    define UNITY_INTERFACE_EXPORT\n"
               "#endif\n"
               "\n"
               "namespace fb\n"
               "{\n"
               "    class Plugin : public ISharedObject\n"
               "    {\n"
               "    public:\n"
               "        Plugin();\n"
               "        ~Plugin() override;\n"
               "\n"
               "        void load( SmartPtr<ISharedObject> data ) override;\n"
               "\n"
               "        void unload( SmartPtr<ISharedObject> data ) override;\n"
               "\n"
               "        core::ApplicationManager *getApplicationManager() const;\n"
               "        void setApplicationManager( core::ApplicationManager *applicationManager "
               ");\n"
               "\n"
               "        static Plugin *instance();\n"
               "\n"
               "    protected:\n"
               "        core::ApplicationManager *m_applicationManager;\n"
               "\n"
               "        static Plugin *msPlugin;\n"
               "    };\n"
               "} // namespace fb\n"
               "\n"
               "#endif  // Plugin_h__";
    }

    String Project::getPluginSource() const
    {
        return "#include \"Plugin.h\"\n"
               "#include \"FBCore/FBCore.h\"\n"
               "#include \"FBCore/Core/Pool.h\"\n"
               "//#include \"BoxStack.h\"\n"
               "\n"
               "#if defined FB_PLATFORM_WIN32\n"
               "#    include <windows.h>\n"
               "#endif\n"
               "\n"
               "\n"
               "\n"
               "#if defined FB_PLATFORM_WIN32\n"
               "\n"
               "int WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID )\n"
               "{\n"
               "    return 1;\n"
               "}\n"
               "\n"
               "#endif\n"
               "\n"
               "namespace fb\n"
               "{\n"
               "    Plugin *Plugin::msPlugin = nullptr;\n"
               "\n"
               "    Plugin::Plugin()\n"
               "    {\n"
               "        msPlugin = this;\n"
               "    }\n"
               "\n"
               "    Plugin::~Plugin()\n"
               "    {\n"
               "    }\n"
               "\n"
               "    void Plugin::load( SmartPtr<ISharedObject> data )\n"
               "    {\n"
               "        //FactoryUtil::addFactory<BoxStack>();\n"
               "\n"
               "        std::cout << \"Plugin::load\" << std::endl;\n"
               "    }\n"
               "\n"
               "    void Plugin::unload( SmartPtr<ISharedObject> data )\n"
               "    {\n"
               "        auto applicationManager = getApplicationManager();\n"
               "        std::cout << \"Plugin::unload\" << std::endl;\n"
               "    }\n"
               "\n"
               "    core::ApplicationManager *Plugin::getApplicationManager() const\n"
               "    {\n"
               "        return m_applicationManager;\n"
               "    }\n"
               "\n"
               "    void Plugin::setApplicationManager( core::ApplicationManager "
               "*applicationManager )\n"
               "    {\n"
               "        m_applicationManager = applicationManager;\n"
               "    }\n"
               "\n"
               "    Plugin *Plugin::instance()\n"
               "    {\n"
               "        return msPlugin;\n"
               "    }\n"
               "} // namespace fb\n"
               "\n"
               "extern \"C\" {\n"
               "\n"
               "void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API\n"
               "loadPlugin( fb::core::ApplicationManager *applicationManager )\n"
               "{\n"
               "    using namespace fb;\n"
               "\n"
               "    core::ApplicationManager::setInstance( applicationManager );\n"
               "\n"
               "    auto typeManager = applicationManager->getTypeManager();\n"
               "    TypeManager::setInstance( typeManager );\n"
               "\n"
               "    auto plugin = new Plugin;\n"
               "    plugin->setApplicationManager( applicationManager );\n"
               "    plugin->load( nullptr );\n"
               "}\n"
               "\n"
               "void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API\n"
               "unloadPlugin( fb::core::ApplicationManager *applicationManager )\n"
               "{\n"
               "    using namespace fb;\n"
               "\n"
               "    auto plugin = Plugin::instance();\n"
               "    plugin->unload( nullptr );\n"
               "    delete plugin;\n"
               "}\n"
               "}";
    }

    SmartPtr<IPlugin> Project::getPlugin() const
    {
        return m_plugin;
    }

    void Project::setPlugin( SmartPtr<IPlugin> plugin )
    {
        m_plugin = plugin;
    }

    Array<String> Project::getScriptFilePaths() const
    {
        return {};
    }

    void Project::setScriptFilePaths( const Array<String> &val )
    {
    }

    Array<String> Project::getResourceFolders() const
    {
        return {};
    }

    void Project::setResourceFolders( const Array<String> &val )
    {
    }

    String Project::getApplicationType() const
    {
        return "";
    }

    void Project::setApplicationType( const String &val )
    {
    }

    bool Project::isArchive() const
    {
        return m_archive;
    }

    void Project::setArchive( bool archive )
    {
        m_archive = archive;
    }

    bool Project::isDirty() const
    {
        return m_dirty;
    }

    void Project::setDirty( bool dirty )
    {
        m_dirty = dirty;
    }

    void Project::setMediaPaths( const Array<String> &mediaPaths )
    {
        m_mediaPaths = mediaPaths;
    }

    Array<String> Project::getMediaPaths() const
    {
        return m_mediaPaths;
    }
}  // namespace fb::editor
