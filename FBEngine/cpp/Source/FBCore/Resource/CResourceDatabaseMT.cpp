#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/CResourceDatabaseMT.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/FBCore.h>

namespace fb
{

    CResourceDatabaseMT::CResourceDatabaseMT()
    {
    }

    CResourceDatabaseMT::~CResourceDatabaseMT()
    {
        unload( nullptr );
    }

    void CResourceDatabaseMT::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto databaseManager = fb::make_ptr<AssetDatabaseManager>();
            setDatabaseManager( databaseManager );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::build()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            // FB_ASSERT( graphicsSystem );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto databaseManager = getDatabaseManager();
            FB_ASSERT( databaseManager );

            auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
            FB_ASSERT( assetDatabaseManager );

            auto cachePath = applicationManager->getCachePath();
            assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

            assetDatabaseManager->clearDatabase();

            auto sceneManager = applicationManager->getSceneManager();
            if( sceneManager )
            {
                auto scene = sceneManager->getCurrentScene();
                FB_ASSERT( scene );

                Array<SmartPtr<ISharedObject>> objects;

                auto actors = scene->getActors();
                for( auto actor : actors )
                {
                    assetDatabaseManager->addResourceEntry( actor );
                    objects.push_back( actor );

                    auto components = actor->getComponents();
                    for( auto component : components )
                    {
                        assetDatabaseManager->addResourceEntry( component );
                        objects.push_back( component );
                    }

                    auto children = actor->getAllChildren();
                    for( auto child : children )
                    {
                        assetDatabaseManager->addResourceEntry( child );
                        objects.push_back( child );

                        auto childComponents = child->getComponents();
                        for( auto childComponent : childComponents )
                        {
                            assetDatabaseManager->addResourceEntry( childComponent );
                            objects.push_back( childComponent );
                        }
                    }
                }

                m_objects = objects;
            }

            auto materialFiles = fileSystem->getFilesWithExtension( ".mat" );
            if( !materialFiles.empty() )
            {
                if( graphicsSystem )
                {
                    auto materialManager = graphicsSystem->getMaterialManager();

                    for( auto &file : materialFiles )
                    {
                        auto path = String( file.filePath.c_str() );

#ifdef _DEBUG
                        if( StringUtil::contains( path, "Materials/BackgroundUI.mat" ) )
                        {
                            int stop = 0;
                            stop = 0;
                        }

                        if( StringUtil::contains( path, "Media/DefaultSkybox.mat" ) )
                        {
                            int stop = 0;
                            stop = 0;
                        }
#endif

                        auto material = materialManager->loadFromFile( path );

                        assetDatabaseManager->addResourceEntry( material );
                    }
                }
            }

            auto textureFiles = fileSystem->getFilesWithExtension( ".png" );
            if( !textureFiles.empty() )
            {
                if( graphicsSystem )
                {
                    auto textureManager = graphicsSystem->getTextureManager();

                    for( auto &file : textureFiles )
                    {
                        auto path = String( file.filePath.c_str() );
                        auto texture = textureManager->loadFromFile( path );
                        if( texture )
                        {
                            assetDatabaseManager->addResourceEntry( texture );
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::importFolder( SmartPtr<IFolderExplorer> folderListing )
    {
        try
        {
            if( folderListing )
            {
                FB_LOG( "Importing: " + folderListing->getFolderName() );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto files = folderListing->getFiles();
                for( auto file : files )
                {
                    auto relativePath = Path::getRelativePath( projectPath, file );
                    importFile( relativePath );
                }

                auto subFolders = folderListing->getSubFolders();
                for( auto subFolder : subFolders )
                {
                    try
                    {
                        importFolder( subFolder );
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::importFile( const String &filePath )
    {
        try
        {
            // FB_LOG("Importing File: " + filePath);

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            // FB_ASSERT( sceneManager );

            // auto assetDatabaseManager = getDatabaseManager();
            // FB_ASSERT( assetDatabaseManager );

            auto basePath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( basePath ) )
            {
                basePath = Path::getWorkingDirectory();
            }

            auto data = String( "" );

            auto extension = Path::getFileExtension( filePath );
            if( extension == ".fbx" || extension == ".FBX" )
            {
                //data::mesh_importer_data meshImportData;
                // meshImportData.path = filePath;
                // data = DataUtil::toString(&meshImportData);
            }
            else
            {
                //data::importer_data importData;
                //importData.path = filePath;
                //data = DataUtil::toString( &importData );
            }

            // if (db)
            //{
            //	auto sql = String("INSERT INTO `assets`(`id`,`name`,`data`) VALUES (NULL, '") + filePath
            //+ String("', '" + data + "');"); 	auto query = db->query(sql);

            //	db->unload(0);
            //	db = nullptr;
            //}

            if( extension == ".fbx" || extension == ".FBX" )
            {
                auto meshLoader = applicationManager->getMeshLoader();
                if( meshLoader )
                {
                    auto actor = meshLoader->loadActor( filePath );
                    if( actor )
                    {
                        if( graphicsSystem )
                        {
                            auto converter = graphicsSystem->getMeshConverter();
                            converter->writeMesh( actor );
                        }
                    }

                    if( sceneManager )
                    {
                        sceneManager->destroyActor( actor );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::importAssets()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto folderListing = fileSystem->getFolderListing( projectPath );
            if( folderListing )
            {
                importFolder( folderListing );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabaseMT::reimportAssets()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto cachePath = applicationManager->getCachePath();
            fileSystem->deleteFilesFromPath( cachePath );

            // auto databaseFilePath = cachePath + "/asset.db";
            // if (fileSystem->isExistingFile(databaseFilePath))
            //{
            //	fileSystem->deleteFile(databaseFilePath);
            // }
            //
            // auto db = getDatabase();
            // FB_ASSERT(db);

            // db->load(databaseFilePath, "");

            // auto sql = String("create table 'assets' (id INTEGER PRIMARY KEY, name TEXT, data
            // TEXT);"); db->query(sql);

            // db->unload(0);
            // db = nullptr;

            auto projectPath = applicationManager->getProjectPath();
            auto assetsPath = projectPath + "/Assets";
            auto folderListing = fileSystem->getFolderListing( assetsPath );
            if( folderListing )
            {
                importFolder( folderListing );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    Array<SmartPtr<IResource>> CResourceDatabaseMT::getResources() const
    {
        Array<SmartPtr<IResource>> resources;

        auto objects = getSceneObjects();
        for( auto object : objects )
        {
            resources.push_back( object );
        }

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto cachePath = applicationManager->getCachePath();
        assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

        auto sql = String( "select * from 'resources'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                auto id = query->getFieldValue( "id" );
                auto uuid = query->getFieldValue( "uuid" );
                auto path = query->getFieldValue( "path" );
                auto type = query->getFieldValue( "type" );

                if( StringUtil::isNullOrEmpty( path ) )
                {
                    query->nextRow();
                    continue;
                }

                if( type == "Material" )
                {
                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    auto materialManager = graphicsSystem->getMaterialManager();

                    auto materialResult = materialManager->createOrRetrieve( uuid, path, type );
                    if( materialResult.first )
                    {
                        resources.push_back( materialResult.first );
                    }
                }
                else if( type == "Texture" )
                {
                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    auto textureManager = graphicsSystem->getTextureManager();

                    auto textureResult = textureManager->createOrRetrieve( uuid, path, type );
                    if( textureResult.first )
                    {
                        resources.push_back( textureResult.first );
                    }
                }

                query->nextRow();
            }
        }

        return resources;
    }

    SmartPtr<IResource> CResourceDatabaseMT::loadResource( const String &path )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();

        auto resources = getResources();
        for( auto resource : resources )
        {
            auto fileId = resource->getFileSystemId();

            auto fileInfo = FileInfo();
            if( fileSystem->findFileInfo( fileId, fileInfo ) )
            {
                if( fileInfo.filePath.c_str() == path )
                {
                    return resource;
                }
            }
        }

        return nullptr;
    }

    SmartPtr<IDatabaseManager> CResourceDatabaseMT::getDatabaseManager() const
    {
        return m_databaseManager;
    }

    void CResourceDatabaseMT::setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager )
    {
        m_databaseManager = databaseManager;
    }

    Pair<SmartPtr<IResource>, bool> CResourceDatabaseMT::createOrRetrieve( hash_type type,
                                                                           const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    Pair<SmartPtr<IResource>, bool> CResourceDatabaseMT::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    SmartPtr<ISharedObject> CResourceDatabaseMT::getObject( const String &uuid )
    {
        auto objects = getSceneObjects();
        for( auto object : objects )
        {
            if( object )
            {
                auto handle = object->getHandle();
                auto objectUuid = handle->getUUID();

                if( uuid == objectUuid )
                {
                    return object;
                }
            }
        }

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );

        auto cachePath = applicationManager->getCachePath();
        assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

        auto sql = String( "select * from 'resources' where uuid='" + uuid + "'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                auto id = query->getFieldValue( "id" );
                auto uuid = query->getFieldValue( "uuid" );
                auto path = query->getFieldValue( "path" );
                auto type = query->getFieldValue( "type" );

                if( type == "Material" )
                {
                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    auto materialManager = graphicsSystem->getMaterialManager();

                    auto materialResult = materialManager->createOrRetrieve( uuid, path, type );
                    return materialResult.first;
                }
                if( type == "Texture" )
                {
                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    auto textureManager = graphicsSystem->getTextureManager();

                    auto textureResult = textureManager->createOrRetrieve( uuid, path, type );
                    return textureResult.first;
                }

                query->nextRow();
            }
        }

        return nullptr;
    }

    Array<SmartPtr<ISharedObject>> CResourceDatabaseMT::getSceneObjects() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto projectPath = applicationManager->getProjectPath();
        if( StringUtil::isNullOrEmpty( projectPath ) )
        {
            projectPath = Path::getWorkingDirectory();
        }

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        Array<SmartPtr<ISharedObject>> objects;

        auto actors = scene->getActors();
        for( auto actor : actors )
        {
            objects.push_back( actor );

            auto components = actor->getComponents();
            for( auto component : components )
            {
                objects.push_back( component );
            }

            auto children = actor->getAllChildren();
            for( auto child : children )
            {
                if( child )
                {
                    objects.push_back( child );

                    auto childComponents = child->getComponents();
                    for( auto childComponent : childComponents )
                    {
                        objects.push_back( childComponent );
                    }
                }
            }
        }

        return objects;
    }

    void CResourceDatabaseMT::addResourceListener( SmartPtr<IResourceListener> resourceListener )
    {
    }

    void CResourceDatabaseMT::setResourceListeners(
        const Array<SmartPtr<IResourceListener>> &resourceListener )
    {
    }

    Array<SmartPtr<IResourceListener>> CResourceDatabaseMT::getResourceListeners() const
    {
        return Array<SmartPtr<IResourceListener>>();
    }

    void CResourceDatabaseMT::BuildResourceDatabaseJob::execute()
    {
    }

    SmartPtr<CResourceDatabaseMT> CResourceDatabaseMT::BuildResourceDatabaseJob::getOwner() const
    {
        return m_owner;
    }

    void CResourceDatabaseMT::BuildResourceDatabaseJob::setOwner( SmartPtr<CResourceDatabaseMT> owner )
    {
        m_owner = owner;
    }
}  // end namespace fb
