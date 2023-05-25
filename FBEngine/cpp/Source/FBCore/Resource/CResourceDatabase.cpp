#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/CResourceDatabase.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/FBCore.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, CResourceDatabase, CSharedObject<IResourceDatabase> );

    CResourceDatabase::CResourceDatabase()
    {
    }

    CResourceDatabase::~CResourceDatabase()
    {
        unload( nullptr );
    }

    void CResourceDatabase::load( SmartPtr<ISharedObject> data )
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

    void CResourceDatabase::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_resourceMap.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceDatabase::build()
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

            auto textureTypes = Array<String>( { ".png", ".jpg", ".jpeg", ".dds", ".tif" } );
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
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    bool CResourceDatabase::hasResource( SmartPtr<IResource> resource )
    {
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        return assetDatabaseManager->hasResourceEntry( resource );
    }

    void CResourceDatabase::addResource( SmartPtr<IResource> resource )
    {
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        assetDatabaseManager->addResourceEntry( resource );
    }

    void CResourceDatabase::removeResource( SmartPtr<IResource> resource )
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

    void CResourceDatabase::removeResourceFromPath( const String &path )
    {
    }

    SmartPtr<IResource> CResourceDatabase::findResource( u32 type, const String &path )
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

            auto uiMaterial = materialManager->getByName( path );
            //FB_ASSERT( uiMaterial );

            return uiMaterial;
        }

        return nullptr;
    }

    SmartPtr<IResource> CResourceDatabase::cloneResource( u32 type, SmartPtr<IResource> resource,
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

        auto clonedMaterial = materialManager->cloneMaterial( resource, path );
        FB_ASSERT( clonedMaterial );

        //getInstancesPtr()->push_back( clonedMaterial );

        return clonedMaterial;
    }

    void CResourceDatabase::importFolder( SmartPtr<IFolderExplorer> folderListing )
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

    void CResourceDatabase::importFile( const String &filePath )
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

    void CResourceDatabase::importAssets()
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

    void CResourceDatabase::reimportAssets()
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

    Array<SmartPtr<scene::IDirector>> CResourceDatabase::getResourceData() const
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
                        auto fieldName = query->getFieldName( (u32)i );
                        auto fieldValue = query->getFieldValue( (u32)i );

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

    Array<SmartPtr<IResource>> CResourceDatabase::getResources() const
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

    SmartPtr<IResource> CResourceDatabase::loadResource( const String &path )
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
        else if( ext == ".jpg" || ext == ".jpeg" || ext == ".tiff" || ext == ".png" )
        {
            auto textureResult = textureManager->createOrRetrieve( path );
            assetDatabaseManager->addResourceEntry( textureResult.first );
            return textureResult.first;
        }
        else if( ext == ".resource" )
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

    SmartPtr<IResource> CResourceDatabase::loadResourceById( const String &uuid )
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

    SmartPtr<IResource> CResourceDatabase::createOrRetrieveResource( SmartPtr<IDatabaseQuery> query )
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
        else if( type == "Texture" )
        {
            auto textureResult = textureManager->createOrRetrieve( uuid, path, type );
            return textureResult.first;
        }

        return nullptr;
    }

    SmartPtr<IDatabaseManager> CResourceDatabase::getDatabaseManager() const
    {
        return m_databaseManager;
    }

    void CResourceDatabase::setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager )
    {
        m_databaseManager = databaseManager;
    }

    Pair<SmartPtr<IResource>, bool> CResourceDatabase::createOrRetrieveFromDirector(
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
            if( typeManager->isDerived( (u32)type, fontType ) )
            {
                return fontManager->createOrRetrieve( path );
            }
            else if( typeManager->isDerived( (u32)type, materialType ) )
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

    Pair<SmartPtr<IResource>, bool> CResourceDatabase::createOrRetrieve( hash_type type,
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

                if( typeManager->isDerived( (u32)type, materialType ) )
                {
                    if( materialManager )
                    {
                        result = materialManager->createOrRetrieve( path );
                    }
                }

                if( typeManager->isDerived( (u32)type, meshResourceType ) )
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

    Pair<SmartPtr<IResource>, bool> CResourceDatabase::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    SmartPtr<ISharedObject> CResourceDatabase::getObject( const String &uuid )
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
                    else if( type == "Texture" )
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

    Array<SmartPtr<ISharedObject>> CResourceDatabase::getSceneObjects() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto projectPath = applicationManager->getProjectPath();
        if( StringUtil::isNullOrEmpty( projectPath ) )
        {
            projectPath = Path::getWorkingDirectory();
        }

        Array<SmartPtr<ISharedObject>> objects;

        if( auto sceneManager = applicationManager->getSceneManager() )
        {
            if( auto scene = sceneManager->getCurrentScene() )
            {
                auto actors = scene->getActors();
                for( auto actor : actors )
                {
                    objects.push_back( actor );

                    auto components = actor->getComponents();
                    for( auto component : components )
                    {
                        objects.push_back( component );
                    }

                    if( auto p = actor->getChildrenPtr() )
                    {
                        auto &children = *p;
                        for( auto child : children )
                        {
                            if( child )
                            {
                                objects.push_back( child );

                                auto childComponents =
                                    child->getComponentsAndInChildren<scene::IComponent>();
                                for( auto childComponent : childComponents )
                                {
                                    objects.push_back( childComponent );
                                }
                            }
                        }
                    }
                }
            }
        }

        return objects;
    }

    void CResourceDatabase::addResourceListener( SmartPtr<IResourceListener> resourceListener )
    {
    }

    void CResourceDatabase::setResourceListeners(
        const Array<SmartPtr<IResourceListener>> &resourceListener )
    {
    }

    Array<SmartPtr<IResourceListener>> CResourceDatabase::getResourceListeners() const
    {
        return Array<SmartPtr<IResourceListener>>();
    }

    SharedPtr<Array<SmartPtr<IResource>>> CResourceDatabase::getInstancesPtr() const
    {
        return m_instances;
    }

    void CResourceDatabase::setInstancesPtr( SharedPtr<Array<SmartPtr<IResource>>> instances )
    {
        m_instances = instances;
    }

    void CResourceDatabase::BuildResourceDatabaseJob::execute()
    {
    }

    SmartPtr<CResourceDatabase> CResourceDatabase::BuildResourceDatabaseJob::getOwner() const
    {
        return m_owner;
    }

    void CResourceDatabase::BuildResourceDatabaseJob::setOwner( SmartPtr<CResourceDatabase> owner )
    {
        m_owner = owner;
    }
}  // end namespace fb
