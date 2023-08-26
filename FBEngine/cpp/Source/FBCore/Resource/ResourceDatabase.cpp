#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/ResourceDatabase.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ResourceDatabase, IResourceDatabase );

    ResourceDatabase::ResourceDatabase()
    {
    }

    ResourceDatabase::~ResourceDatabase()
    {
        unload( nullptr );
    }

    void ResourceDatabase::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto databaseManager = fb::make_ptr<AssetDatabaseManager>();
            setDatabaseManager( databaseManager );

            auto textureTypes = Array<String>( { ".png", ".jpg", ".jpeg", ".dds", ".tif" } );
            setTextureTypes( textureTypes );

            auto audioTypes = Array<String>( { ".wav", ".mp3", ".ogg" } );
            setAudioTypes( audioTypes );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ResourceDatabase::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_resourceMap.clear();
                setDatabaseManager( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ResourceDatabase::build()
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

            auto databasePath = cachePath + "/" + "asset.db";
            databasePath = StringUtil::cleanupPath( databasePath );

            assetDatabaseManager->loadFromFile( databasePath );

            auto sceneManager = applicationManager->getSceneManager();
            if( sceneManager )
            {
                auto scene = sceneManager->getCurrentScene();
                FB_ASSERT( scene );

                Array<SmartPtr<ISharedObject>> objects;

                auto actors = scene->getActors();
                for( auto actor : actors )
                {
                    if( !assetDatabaseManager->hasResourceEntry( actor ) )
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
                        auto path = file.filePath;

                        auto pMaterial = materialManager->createOrRetrieve( path );
                        auto material = fb::static_pointer_cast<render::IMaterial>( pMaterial.first );
                        if( material )
                        {
                            if( !assetDatabaseManager->hasResourceEntry( material ) )
                            {
                                assetDatabaseManager->addResourceEntry( material );
                            }
                        }
                    }
                }
            }

            auto textureTypes = getTextureTypes();
            for( auto textureType : textureTypes )
            {
                auto textureFiles = fileSystem->getFilesWithExtension( textureType );
                if( !textureFiles.empty() )
                {
                    if( graphicsSystem )
                    {
                        auto textureManager = graphicsSystem->getTextureManager();

                        for( auto &file : textureFiles )
                        {
                            auto path = file.filePath;

                            auto textureResult = textureManager->createOrRetrieve( path );
                            auto texture =
                                fb::static_pointer_cast<render::ITexture>( textureResult.first );
                            if( texture )
                            {
                                if( !assetDatabaseManager->hasResourceEntry( texture ) )
                                {
                                    assetDatabaseManager->addResourceEntry( texture );
                                }
                            }
                        }
                    }
                }
            }

            auto audioTypes = getAudioTypes();
            for( auto audioType : audioTypes )
            {
                auto audioFiles = fileSystem->getFilesWithExtension( audioType );
                if( !audioFiles.empty() )
                {
                    auto audioManager = applicationManager->getSoundManager();
                    if( audioManager )
                    {
                        for( auto &file : audioFiles )
                        {
                            auto path = file.filePath;

                            auto audioResult = audioManager->createOrRetrieve( path );
                            auto audio = fb::static_pointer_cast<ISound>( audioResult.first );
                            if( audio )
                            {
                                if( !assetDatabaseManager->hasResourceEntry( audio ) )
                                {
                                    assetDatabaseManager->addResourceEntry( audio );
                                }
                            }
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

    void ResourceDatabase::refresh()
    {
        auto applicationManager = core::IApplicationManager::instance();

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto cachePath = applicationManager->getCachePath();

        auto databasePath = cachePath + "/" + "asset.db";
        databasePath = StringUtil::cleanupPath( databasePath );

        assetDatabaseManager->loadFromFile( databasePath );

        auto sceneManager = applicationManager->getSceneManager();
        if( sceneManager )
        {
            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );

            auto objects = getSceneObjects();
            for( auto object : objects )
            {
                if( !assetDatabaseManager->hasResourceEntry( object ) )
                {
                    assetDatabaseManager->addResourceEntry( object );
                }
            }

            m_objects = objects;
        }
    }

    bool ResourceDatabase::hasResource( SmartPtr<IResource> resource )
    {
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        return assetDatabaseManager->hasResourceEntry( resource );
    }

    void ResourceDatabase::addResource( SmartPtr<IResource> resource )
    {
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        assetDatabaseManager->addResourceEntry( resource );
    }

    void ResourceDatabase::removeResource( SmartPtr<IResource> resource )
    {
        if( resource )
        {
            auto databaseManager = getDatabaseManager();
            FB_ASSERT( databaseManager );

            auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
            FB_ASSERT( assetDatabaseManager );

            assetDatabaseManager->removeResourceEntry( resource );
        }
    }

    void ResourceDatabase::removeResourceFromPath( const String &path )
    {
    }

    SmartPtr<IResource> ResourceDatabase::findResource( u32 type, const String &path )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto typeManager = TypeManager::instance();

        const auto MATERIAL_TYPE = render::IMaterial::typeInfo();
        const auto TEXTURE_TYPE = render::ITexture::typeInfo();

        if( type == MATERIAL_TYPE )
        {
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto materialManager = graphicsSystem->getMaterialManager();

            return materialManager->getByName( path );
        }

        return nullptr;
    }

    SmartPtr<IResource> ResourceDatabase::cloneResource( u32 type, SmartPtr<IResource> resource,
                                                         const String &path )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto materialManager = graphicsSystem->getMaterialManager();
        FB_ASSERT( materialManager );

        auto renderLock = taskManager->lockTask( Thread::Task::Render );

        const auto MATERIAL_TYPE = render::IMaterial::typeInfo();
        const auto TEXTURE_TYPE = render::ITexture::typeInfo();

        if( type == MATERIAL_TYPE )
        {
            if( materialManager )
            {
                auto clonedMaterial = materialManager->cloneMaterial( resource, path );
                FB_ASSERT( clonedMaterial );

                return clonedMaterial;
            }
        }
        else if( type == TEXTURE_TYPE )
        {
        }

        return nullptr;
    }

    void ResourceDatabase::importFolder( SmartPtr<IFolderExplorer> folderListing )
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

    void ResourceDatabase::importFile( const String &filePath )
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
                            FB_ASSERT( converter );

                            if( converter )
                            {
                                converter->writeMesh( actor );
                            }
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

    void ResourceDatabase::importAssets()
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

    void ResourceDatabase::reimportAssets()
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

    Array<SmartPtr<scene::IDirector>> ResourceDatabase::getResourceData() const
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            Array<SmartPtr<scene::IDirector>> resources;
            resources.reserve( 1024 );

            auto objects = getSceneObjects();
            for( auto object : objects )
            {
                auto id = String();
                auto uuid = String();
                auto path = String();
                auto type = String();

                if( auto handle = object->getHandle() )
                {
                    id = handle->getId();
                    uuid = handle->getUUID();
                }

                if( object->isDerived<scene::IActor>() )
                {
                    type = "Actor";
                }
                else if( object->isDerived<scene::IComponent>() )
                {
                    type = "Component";

                    auto component = fb::static_pointer_cast<scene::IComponent>( object );
                    if( auto actor = component->getActor() )
                    {
                        auto actorHandle = actor->getHandle();
                        path = actorHandle->getName() + "_" +
                               typeManager->getName( component->getTypeInfo() );
                    }
                }

                auto resourceDirector = factoryManager->make_object<scene::IDirector>();
                resourceDirector->load( nullptr );

                if( auto handle = resourceDirector->getHandle() )
                {
                    handle->setName( path );

                    if( StringUtil::isNullOrEmpty( uuid ) )
                    {
                        uuid = StringUtil::getUUID();
                    }

                    handle->setUUID( uuid );
                }

                auto directorProperties = resourceDirector->getProperties();
                directorProperties->setProperty( "type", type );

                resources.push_back( resourceDirector );
            }

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

            auto databasePath = cachePath + "asset.db";
            assetDatabaseManager->loadFromFile( databasePath );

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

                    auto resourceDirector = factoryManager->make_object<scene::IDirector>();
                    resourceDirector->load( nullptr );

                    if( auto handle = resourceDirector->getHandle() )
                    {
                        handle->setName( path );
                        handle->setUUID( uuid );
                    }

                    auto directorProperties = resourceDirector->getProperties();

                    auto numFields = query->getNumFields();

                    for( size_t i = 0; i < numFields; ++i )
                    {
                        auto fieldName = query->getFieldName( static_cast<u32>( i ) );
                        auto fieldValue = query->getFieldValue( static_cast<u32>( i ) );

                        directorProperties->setProperty( fieldName, fieldValue );
                    }

                    resources.push_back( resourceDirector );

                    query->nextRow();
                }
            }

            return resources;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Array<SmartPtr<scene::IDirector>>();
    }

    Array<SmartPtr<IResource>> ResourceDatabase::getResources() const
    {
        Array<SmartPtr<IResource>> resources;
        resources.reserve( 1024 );

        auto objects = getSceneObjects();
        for( auto object : objects )
        {
            resources.push_back( object );
        }

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

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

        auto databasePath = cachePath + "asset.db";
        assetDatabaseManager->loadFromFile( databasePath );

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

    void ResourceDatabase::createActor( SmartPtr<scene::IActor> parent, SmartPtr<Properties> properties )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto actor = sceneManager->createActor();
        FB_ASSERT( actor );

        parent->addChild( actor );

        auto name = properties->getProperty( "name" );
        actor->setName( name );

        auto children = properties->getChildren();
        for( auto child : children )
        {
            createActor( actor, child );
        }
    }

    SmartPtr<IResource> ResourceDatabase::loadResource( hash64 id )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto prefabManager = applicationManager->getPrefabManager();

        auto materialManager = graphicsSystem->getMaterialManager();
        auto textureManager = graphicsSystem->getTextureManager();

        auto properties = loadAssetProperties( id );

        auto uuid = properties->getProperty( "uuid" );
        if( StringUtil::isNullOrEmpty( uuid ) )
        {
            uuid = StringUtil::getUUID();
        }

        auto type = properties->getProperty( "asset_type" );
        if( type == "material" )
        {
        }
        else if( type == "texture" )
        {
        }
        else if( type == "prefab" )
        {
            auto result = prefabManager->createOrRetrieve( uuid, "", type );
            return result.first;
        }
        else if( type == "scene" )
        {
        }
        else if( type == "shader" )
        {
        }
        else if( type == "mesh" )
        {
        }
        else if( type == "model" )
        {
            //auto result = prefabManager->createOrRetrieve( uuid, "", type );
            //return result.first;

            auto actor = sceneManager->createActor();
            createActor( actor, properties );

            SmartPtr<scene::IPrefab> prefab = prefabManager->create( uuid );
            prefab->setActor( actor );
            return prefab;
        }
        else if( type == "animation" )
        {
        }
        else if( type == "audio" )
        {
        }
        else if( type == "font" )
        {
        }
        else if( type == "script" )
        {
        }
        else if( type == "script_class" )
        {
        }
        else if( type == "script_function" )
        {
        }
        else if( type == "script_module" )
        {
        }
        else if( type == "script_property" )
        {
        }
        else if( type == "script_variable" )
        {
        }
        else if( type == "shader" )
        {
        }
        else if( type == "shader_program" )
        {
        }
        else if( type == "shader_variable" )
        {
        }
        else if( type == "shader_variable_block" )
        {
        }
        else if( type == "shader_variable_sampler" )
        {
        }
        else if( type == "shader_variable_texture" )
        {
        }
        else if( type == "shader_variable_uniform" )
        {
        }
        else if( type == "vehicle" )
        {
        }
        else
        {
        }

        return nullptr;
    }

    SmartPtr<IResource> ResourceDatabase::loadResource( const String &path )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        if( !graphicsSystem )
        {
            return nullptr;
        }

        auto materialManager = graphicsSystem->getMaterialManager();

        auto textureManager = graphicsSystem->getTextureManager();

        auto filePathLower = StringUtil::make_lower( path );
        auto fileName = Path::getFileName( path );
        auto fileNameLower = StringUtil::make_lower( fileName );

        auto id = StringUtil::getHash( path );
        auto idLowerCase = StringUtil::getHash( filePathLower );
        auto idFileName = StringUtil::getHash( fileName );
        auto idLowerCaseFileName = StringUtil::getHash( fileNameLower );

        auto cachePath = applicationManager->getCachePath();
        assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

        auto sql = String( "select * from 'resources' where path='" + path + "'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query );
            }
        }

        sql = String( "SELECT * FROM 'resources' WHERE path LIKE '%' || '" + path + "' || '%'" );
        query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query );
            }
        }

        auto ext = Path::getFileExtension( path );
        if( ext == ".mat" )
        {
            auto materialResult = materialManager->createOrRetrieve( path );
            assetDatabaseManager->addResourceEntry( materialResult.first );
            return materialResult.first;
        }
        if( ext == ".jpg" || ext == ".jpeg" || ext == ".tiff" || ext == ".png" )
        {
            auto textureResult = textureManager->createOrRetrieve( path );
            assetDatabaseManager->addResourceEntry( textureResult.first );
            return textureResult.first;
        }
        if( ext == ".resource" )
        {
            //auto jsonStr = fileSystem->readAllText( path );

            //auto pData = fb::make_ptr<Data<data::properties>>();
            //auto data = pData->getDataAsType<data::properties>();
            //DataUtil::parse( jsonStr, data );

            //auto director = fb::make_ptr<scene::CDirector>();
            //director->fromData( pData );

            //assetDatabaseManager->addResourceEntry( director );
            //return director;
        }

        return nullptr;
    }

    SmartPtr<IResource> ResourceDatabase::loadResourceById( const String &uuid )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto materialManager = graphicsSystem->getMaterialManager();

        auto textureManager = graphicsSystem->getTextureManager();

        auto cachePath = applicationManager->getCachePath();
        assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

        auto sql = String( "select * from 'resources' where uuid='" + uuid + "'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query );
            }
        }

        return nullptr;
    }

    SmartPtr<IResource> ResourceDatabase::createOrRetrieveResource( SmartPtr<IDatabaseQuery> query )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto materialManager = graphicsSystem->getMaterialManager();

        auto textureManager = graphicsSystem->getTextureManager();

        auto id = query->getFieldValue( "id" );
        auto uuid = query->getFieldValue( "uuid" );
        auto path = query->getFieldValue( "path" );
        auto type = query->getFieldValue( "type" );

        if( type == "Material" )
        {
            auto materialResult = materialManager->createOrRetrieve( uuid, path, type );
            return materialResult.first;
        }
        if( type == "Texture" )
        {
            auto textureResult = textureManager->createOrRetrieve( uuid, path, type );
            return textureResult.first;
        }

        return nullptr;
    }

    SmartPtr<IDatabaseManager> ResourceDatabase::getDatabaseManager() const
    {
        return m_databaseManager;
    }

    void ResourceDatabase::setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager )
    {
        m_databaseManager = databaseManager;
    }

    Pair<SmartPtr<IResource>, bool> ResourceDatabase::createOrRetrieveFromDirector(
        hash_type type, const String &path, SmartPtr<scene::IDirector> director )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto materialManager = graphicsSystem->getMaterialManager();
            FB_ASSERT( materialManager );

            auto fontManager = graphicsSystem->getFontManager();
            FB_ASSERT( fontManager );

            static const auto fontType = render::IFont::typeInfo();
            static const auto materialType = render::IMaterial::typeInfo();

            auto typeManager = TypeManager::instance();
            if( typeManager->isDerived( static_cast<u32>( type ), fontType ) )
            {
                return fontManager->createOrRetrieve( path );
            }
            if( typeManager->isDerived( static_cast<u32>( type ), materialType ) )
            {
                return materialManager->createOrRetrieve( path );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>();
    }

    Pair<SmartPtr<IResource>, bool> ResourceDatabase::createOrRetrieve( hash_type type,
                                                                        const String &path )
    {
        try
        {
            if( StringUtil::isNullOrEmpty( path ) )
            {
                return Pair<SmartPtr<IResource>, bool>();
            }

            FB_ASSERT( type != 0 );
            FB_ASSERT( !StringUtil::isNullOrEmpty( path ) );

            // Check if the resource already exists in the database.
            auto it = m_resourceMap.find( type );
            if( it != m_resourceMap.end() )
            {
                auto resIt = it->second.find( path );
                if( resIt != it->second.end() )
                {
                    // The resource already exists in the database.
                    return Pair<SmartPtr<IResource>, bool>( resIt->second.lock(), false );
                }
            }

            Pair<SmartPtr<IResource>, bool> result;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
            {
                auto materialManager = graphicsSystem->getMaterialManager();
                auto meshManager = applicationManager->getMeshManager();

                static const auto materialType = render::IMaterial::typeInfo();
                static const auto meshResourceType = IMeshResource::typeInfo();

                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                if( typeManager->isDerived( static_cast<u32>( type ), materialType ) )
                {
                    if( materialManager )
                    {
                        result = materialManager->createOrRetrieve( path );
                    }
                }

                if( typeManager->isDerived( static_cast<u32>( type ), meshResourceType ) )
                {
                    if( meshManager )
                    {
                        result = meshManager->createOrRetrieve( path );
                    }
                }
            }

            // The resource does not exist in the database, so create a new one.
            m_resourceMap[type][path] = result.first;

            return result;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    Pair<SmartPtr<IResource>, bool> ResourceDatabase::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    SmartPtr<ISharedObject> ResourceDatabase::getObject( const String &uuid )
    {
        if( !StringUtil::isNullOrEmpty( uuid ) )
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
        }

        return nullptr;
    }

    SmartPtr<Properties> ResourceDatabase::loadAssetNode( SmartPtr<IDatabaseManager> db,
                                                          SmartPtr<Properties> parent, hash64 id )
    {
        auto applicationManager = core::IApplicationManager::instance();

        auto properties = fb::make_ptr<Properties>();
        parent->addChild( properties );

        auto sAssetId = StringUtil::toString( id );
        String sql = "select * from configured_models where id = '" + sAssetId + "'";

        if( auto query = db->executeQuery( sql ) )
        {
            while( !query->eof() )
            {
                auto numFields = query->getNumFields();
                for( size_t i = 0; i < numFields; ++i )
                {
                    auto name = query->getFieldName( (u32)i );
                    auto value = query->getFieldValue( (u32)i );

                    properties->setProperty( name, value );
                }

                query->nextRow();
            }
        }

        String parentSql = "select * from configured_models where parent_id = '" + sAssetId + "'";
        if( auto query = db->executeQuery( parentSql ) )
        {
            while( !query->eof() )
            {
                auto id = query->getFieldValue( "id" );
                auto iId = StringUtil::parseInt( id );
                loadAssetNode( db, properties, iId );
                query->nextRow();
            }
        }

        return properties;
    }

    SmartPtr<Properties> ResourceDatabase::loadAssetProperties( hash64 id )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto databaseManager = fb::make_ptr<AssetDatabaseManager>();

        auto mediaPath = applicationManager->getMediaPath();
        //databaseManager->loadFromFile( mediaPath + "/AssetDatabase.db" );
        databaseManager->loadFromFile( "C:/dev/fireblade/Bin/Media/AssetDatabase.db" );

        auto properties = fb::make_ptr<Properties>();
        properties->setName( "asset" );
        properties->setProperty( String( "asset_type" ), String( "model" ) );

        loadAssetNode( databaseManager, properties, id );

        auto sAssetId = StringUtil::toString( id );
        String sql = "select * from attribs where model_id = '" + sAssetId + "'";

        if( auto query = databaseManager->executeQuery( sql ) )
        {
            while( query->eof() )
            {
                auto name = query->getFieldValue( "title" );
                auto value = query->getFieldValue( "value" );

                properties->setProperty( name, value );

                query->nextRow();
            }
        }

        return properties;
    }

    void ResourceDatabase::getSceneObjects( SmartPtr<scene::IActor> actor,
                                            Array<SmartPtr<ISharedObject>> &objects ) const
    {
        objects.push_back( actor );

        if( auto transform = actor->getTransform() )
        {
            objects.push_back( transform );
        }

        if( auto p = actor->getComponentsPtr() )
        {
            auto &components = *p;
            objects.insert( objects.end(), components.begin(), components.end() );
        }

        if( auto p = actor->getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child )
                {
                    getSceneObjects( child, objects );
                }
            }
        }
    }

    Array<SmartPtr<ISharedObject>> ResourceDatabase::getSceneObjects() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        Array<SmartPtr<ISharedObject>> objects;
        objects.reserve( 4096 * 8 );

        if( auto sceneManager = applicationManager->getSceneManager() )
        {
            if( auto scene = sceneManager->getCurrentScene() )
            {
                auto actors = scene->getActors();
                for( auto actor : actors )
                {
                    if( actor )
                    {
                        getSceneObjects( actor, objects );
                    }
                }
            }
        }

        return objects;
    }

    SmartPtr<ISharedObject> ResourceDatabase::getObjectByFileId( const String &fileId ) const
    {
        for( auto &object : m_objects )
        {
            if( auto handle = object->getHandle() )
            {
                if( handle->getFileId() == fileId )
                {
                    return object;
                }
            }
        }

        return nullptr;
    }

    SharedPtr<Array<SmartPtr<IResource>>> ResourceDatabase::getInstancesPtr() const
    {
        return m_instances;
    }

    void ResourceDatabase::setInstancesPtr( SharedPtr<Array<SmartPtr<IResource>>> instances )
    {
        m_instances = instances;
    }

    Array<String> ResourceDatabase::getTextureTypes() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_textureTypes;
    }

    void ResourceDatabase::setTextureTypes( const Array<String> &textureTypes )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_textureTypes = textureTypes;
    }

    Array<String> ResourceDatabase::getAudioTypes() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_audioTypes;
    }

    void ResourceDatabase::setAudioTypes( const Array<String> &audioTypes )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_audioTypes = audioTypes;
    }

}  // end namespace fb
