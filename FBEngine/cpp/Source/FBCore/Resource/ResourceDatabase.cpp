#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/ResourceDatabase.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/Scene/Directors/MaterialResourceDirector.h>
#include <FBCore/Scene/Directors/MeshResourceDirector.h>
#include <FBCore/Scene/Directors/TextureResourceDirector.h>
#include <FBCore/Scene/Directors/SceneLightingDirector.h>
#include <FBCore/Scene/Directors/SoundResourceDirector.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Interface/Database/IDatabase.h>
#include <FBCore/Interface/Database/IDatabaseManager.h>
#include <FBCore/Interface/Database/IDatabaseQuery.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IFont.h>
#include <FBCore/Interface/Graphics/IFontManager.h>
#include <FBCore/Interface/Graphics/IMaterialManager.h>
#include <FBCore/Interface/Graphics/IMeshConverter.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Graphics/ITextureManager.h>
#include <FBCore/Interface/IO/IFolderExplorer.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/Resource/IResourceManager.h>
#include <FBCore/Interface/Resource/IMeshManager.h>
#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Interface/Scene/IPrefab.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Interface/Sound/ISoundManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/IO/FileSystem.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Core/Path.h>
#include <FBCore/ApplicationUtil.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ResourceDatabase, IResourceDatabase );
    FB_CLASS_REGISTER_DERIVED( fb, ResourceDatabase::ImportFileJob, Job );

    atomic_s32 ResourceDatabase::m_numJobs = 0;

    ResourceDatabase::ResourceDatabase() = default;

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

            auto textureTypes = Array<String>( { ".png", ".jpg", ".jpeg", ".dds", ".tif", ".tga" } );
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
            auto applicationManager = core::ApplicationManager::instance();
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
                        objects.emplace_back( actor );

                        auto components = actor->getComponents();
                        for( auto component : components )
                        {
                            assetDatabaseManager->addResourceEntry( component );
                            objects.emplace_back( component );
                        }

                        auto children = actor->getAllChildren();
                        for( auto child : children )
                        {
                            assetDatabaseManager->addResourceEntry( child );
                            objects.emplace_back( child );

                            auto childComponents = child->getComponents();
                            for( auto childComponent : childComponents )
                            {
                                assetDatabaseManager->addResourceEntry( childComponent );
                                objects.emplace_back( childComponent );
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
        auto applicationManager = core::ApplicationManager::instance();

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

    void ResourceDatabase::optimise()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        assetDatabaseManager->optimise();
    }

    void ResourceDatabase::clean()
    {
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        assetDatabaseManager->executeQuery(
            "DELETE FROM resources WHERE type = 'Material' AND path = '';" );
        assetDatabaseManager->executeQuery(
            "DELETE FROM resources WHERE type = 'Texture' AND path = '';" );
    }

    void ResourceDatabase::deleteCache()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto cachePath = applicationManager->getCachePath();
        auto folderListing = fileSystem->getFolderListing( cachePath );
        auto &files = folderListing->getFiles();
        for( auto &file : files )
        {
            auto ext = Path::getFileExtension( file );
            if( ext == ".fbmeshbin" || ext == ".pxtrianglemesh" )
            {
                fileSystem->deleteFile( file );
            }
        }
    }

    auto ResourceDatabase::hasResource( SmartPtr<IResource> resource ) -> bool
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
        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        assetDatabaseManager->removeResourceEntryFromPath( path );
    }

    auto ResourceDatabase::findResource( u32 type, const String &path ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        const auto MATERIAL_TYPE = render::IMaterial::typeInfo();
        const auto TEXTURE_TYPE = render::ITexture::typeInfo();

        if( type == MATERIAL_TYPE )
        {
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto materialManager = graphicsSystem->getMaterialManager();

            return materialManager->getByName( path );
        }
        else if( type == TEXTURE_TYPE )
        {
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto textureManager = graphicsSystem->getTextureManager();

            return textureManager->getByName( path );
        }

        return nullptr;
    }

    auto ResourceDatabase::cloneResource( u32 type, SmartPtr<IResource> resource, const String &path )
        -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
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
            auto textureManager = graphicsSystem->getTextureManager();
            FB_ASSERT( textureManager );

            if( textureManager )
            {
                auto clonedTexture = textureManager->cloneTexture( resource, path );
                FB_ASSERT( clonedTexture );

                return clonedTexture;
            }
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

                auto applicationManager = core::ApplicationManager::instance();
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

    void ResourceDatabase::importFolder( const String &path )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto folderListing = fileSystem->getFolderListing( path );
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

    void ResourceDatabase::importFile( const String &filePath )
    {
        while( ResourceDatabase::m_numJobs >= 8192 )
        {
            Thread::sleep( 0.1 );
        }

        auto applicationManager = core::ApplicationManager::instance();
        auto jobQueue = applicationManager->getJobQueue();
        auto factoryManager = applicationManager->getFactoryManager();

        auto job = factoryManager->make_ptr<ImportFileJob>();
        job->setFilePath( filePath );

        if( jobQueue )
        {
            jobQueue->addJob( job );
        }
        else
        {
            job->execute();
        }
    }

    void ResourceDatabase::importCache()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        clean();

        auto projectPath = applicationManager->getProjectPath();
        if( StringUtil::isNullOrEmpty( projectPath ) )
        {
            projectPath = Path::getWorkingDirectory();
        }

        auto cachePath = applicationManager->getCachePath();

        std::cout << "Importing: " << cachePath << std::endl;

        importFolder( cachePath );

        std::cout << "Finished Importing: " << cachePath << std::endl;
    }

    void ResourceDatabase::importAssets()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            clean();

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto cachePath = applicationManager->getCachePath();
            importFolder( cachePath );

            auto projectAssetsPath = projectPath + "/Assets";
            auto folderListing = fileSystem->getFolderListing( projectAssetsPath );
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
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            clean();

            auto cachePath = applicationManager->getCachePath();
            fileSystem->deleteFilesFromPath( cachePath );

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

    void ResourceDatabase::calculateDependencies()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto resources = getResources();
        for( auto resource : resources )
        {
            auto dependencies = resource->getDependencies();
        }
    }

    auto ResourceDatabase::getResourceData() const -> Array<SmartPtr<scene::IDirector>>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            Array<SmartPtr<scene::IDirector>> resources;
            resources.reserve( 1024 );

            auto referenceObjects = getReferenceObjects();
            resources.insert( resources.end(), referenceObjects.begin(), referenceObjects.end() );

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

        return {};
    }

    auto ResourceDatabase::getResources() const -> Array<SmartPtr<IResource>>
    {
        Array<SmartPtr<IResource>> resources;
        resources.reserve( 1024 );

        auto objects = getSceneObjects();
        for( auto object : objects )
        {
            resources.emplace_back( object );
        }

        auto applicationManager = core::ApplicationManager::instance();
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
        auto applicationManager = core::ApplicationManager::instance();
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

    auto ResourceDatabase::loadResource( hash64 id ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
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

    auto ResourceDatabase::loadResource( const String &path ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
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
        auto meshManager = applicationManager->getMeshManager();
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

        auto sql = static_cast<String>( "select * from 'resources' where path='" + path + "'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query, true );
            }
        }

        sql = static_cast<String>( "SELECT * FROM 'resources' WHERE path LIKE '%' || '" + path +
                                   "' || '%'" );
        query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query, true );
            }
        }

        auto ext = Path::getFileExtension( path );
        if( ext == ".mat" )
        {
            auto materialResult = materialManager->createOrRetrieve( path );
            assetDatabaseManager->addResourceEntry( materialResult.first );
            return materialResult.first;
        }
        if( ApplicationUtil::isSupportedMesh( path ) )
        {
            auto meshResult = meshManager->loadFromFile( path );
            assetDatabaseManager->addResourceEntry( meshResult );
            return meshResult;
        }
        if( ApplicationUtil::isSupportedTexture( path ) )
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

    auto ResourceDatabase::loadDirector( const String &path ) -> SmartPtr<scene::IDirector>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto dataStr = fileSystem->readAllText( path );
        auto data = fb::make_ptr<Properties>();
        DataUtil::parse<Properties>( dataStr, data.get() );

        auto director = fb::make_ptr<scene::Director>();
        director->setFilePath( path );
        director->setProperties( data );

        return nullptr;
    }

    auto ResourceDatabase::loadDirectorFromResourcePath( const String &filePath )
        -> SmartPtr<scene::IDirector>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto projectPath = applicationManager->getProjectPath();
        auto settingsCachePath = applicationManager->getSettingsPath();
        auto filePathHash = StringUtil::getHash64( filePath );
        auto fileDataPath = settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
        fileDataPath = Path::getRelativePath( projectPath, fileDataPath );

        auto dataStr = fileSystem->readAllText( fileDataPath );
        auto data = fb::make_ptr<Properties>();
        DataUtil::parse<Properties>( dataStr, data.get() );

        auto director = fb::make_ptr<scene::Director>();
        director->setFilePath( fileDataPath );
        director->setProperties( data );

        return director;
    }

    auto ResourceDatabase::loadDirectorFromResourcePath( const String &filePath, u32 type )
        -> SmartPtr<scene::IDirector>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto projectPath = applicationManager->getProjectPath();
        auto settingsCachePath = applicationManager->getSettingsPath();
        auto filePathHash = StringUtil::getHash64( filePath );
        auto fileDataPath = settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
        auto relativeFileDataPath = Path::getRelativePath( projectPath, fileDataPath );

        auto data = fb::make_ptr<Properties>();

        if( fileSystem->isExistingFile( relativeFileDataPath ) )
        {
            auto dataStr = fileSystem->readAllText( relativeFileDataPath );
            DataUtil::parse<Properties>( dataStr, data.get() );
        }

        if( scene::MeshResourceDirector::typeInfo() == type )
        {
            auto director = fb::make_ptr<scene::MeshResourceDirector>();
            director->setResourcePath( filePath );
            director->setFilePath( fileDataPath );
            director->setProperties( data );

            return director;
        }
        else if( scene::SoundResourceDirector::typeInfo() == type )
        {
            auto director = fb::make_ptr<scene::SoundResourceDirector>();
            director->setResourcePath( filePath );
            director->setFilePath( fileDataPath );
            director->setProperties( data );

            return director;
        }
        else if( scene::TextureResourceDirector::typeInfo() == type )
        {
            auto director = fb::make_ptr<scene::TextureResourceDirector>();
            director->setResourcePath( filePath );
            director->setFilePath( fileDataPath );
            director->setProperties( data );

            return director;
        }

        auto director = fb::make_ptr<scene::Director>();
        director->setFilePath( fileDataPath );
        director->setProperties( data );

        return director;
    }

    auto ResourceDatabase::loadResourceById( const String &uuid ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
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

        auto sql = static_cast<String>( "select * from 'resources' where uuid='" + uuid + "'" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                return createOrRetrieveResource( query, true );
            }
        }

        return nullptr;
    }

    auto ResourceDatabase::createOrRetrieveResource( SmartPtr<IDatabaseQuery> query, bool bLoadResource )
        -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();

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

            auto resource = materialResult.first;
            if( bLoadResource )
            {
                graphicsSystem->loadObject( resource );
            }

            return materialResult.first;
        }
        if( StringUtil::contains( type, "MeshResource" ) )
        {
            auto meshManager = applicationManager->getMeshManager();
            auto meshResult = meshManager->createOrRetrieve( uuid, path, type );

            auto resource = meshResult.first;
            if( bLoadResource )
            {
                graphicsSystem->loadObject( resource );
            }

            return meshResult.first;
        }
        if( type == "Texture" )
        {
            auto textureResult = textureManager->createOrRetrieve( uuid, path, type );

            auto resource = textureResult.first;
            if( bLoadResource )
            {
                graphicsSystem->loadObject( resource );
            }

            return textureResult.first;
        }
        else
        {
            auto object = factoryManager->createObjectFromType<IResource>( type );
            if( object )
            {
                auto handle = object->getHandle();
                if( handle )
                {
                    handle->setUUID( uuid );
                }

                object->setFilePath( path );
                object->loadFromFile( path );

                return object;
            }
        }

        return nullptr;
    }

    auto ResourceDatabase::getDatabaseManager() const -> SmartPtr<IDatabaseManager>
    {
        return m_databaseManager;
    }

    void ResourceDatabase::setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager )
    {
        m_databaseManager = databaseManager;
    }

    auto ResourceDatabase::createOrRetrieveFromDirector( hash_type type, const String &path,
                                                         SmartPtr<scene::IDirector> director )
        -> Pair<SmartPtr<IResource>, bool>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto assetDatabaseManager =
                fb::static_pointer_cast<AssetDatabaseManager>( getDatabaseManager() );

            static const auto fontType = render::IFont::typeInfo();
            static const auto materialType = render::IMaterial::typeInfo();

            Pair<SmartPtr<IResource>, bool> result;

            if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
            {
                auto materialManager = graphicsSystem->getMaterialManager();
                auto meshManager = applicationManager->getMeshManager();
                auto fontManager = graphicsSystem->getFontManager();

                static const auto materialType = render::IMaterial::typeInfo();
                static const auto meshResourceType = IMeshResource::typeInfo();

                String uuid;

                auto director = fb::dynamic_pointer_cast<scene::ResourceDirector>(
                    assetDatabaseManager->getResourceEntryFromPath( path ) );
                if( director )
                {
                    uuid = director->getResourceUUID();
                }

                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                if( typeManager->isDerived( static_cast<u32>( type ), materialType ) )
                {
                    if( materialManager )
                    {
                        result = materialManager->createOrRetrieve( uuid, path, "" );
                    }
                }

                if( typeManager->isDerived( static_cast<u32>( type ), meshResourceType ) )
                {
                    if( meshManager )
                    {
                        result = meshManager->createOrRetrieve( uuid, path, "" );
                    }
                }

                if( typeManager->isDerived( static_cast<u32>( type ), fontType ) )
                {
                    result = fontManager->createOrRetrieve( uuid, path, "" );
                }

                return result;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return {};
    }

    auto ResourceDatabase::createOrRetrieve( hash_type type, const String &path )
        -> Pair<SmartPtr<IResource>, bool>
    {
        try
        {
            if( StringUtil::isNullOrEmpty( path ) )
            {
                return {};
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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto assetDatabaseManager =
                fb::static_pointer_cast<AssetDatabaseManager>( getDatabaseManager() );
            auto director = assetDatabaseManager->getResourceEntryFromPath( path );

            auto result = createOrRetrieveFromDirector( type, path, director );

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

    auto ResourceDatabase::createOrRetrieve( const String &path ) -> Pair<SmartPtr<IResource>, bool>
    {
        try
        {
            if( StringUtil::isNullOrEmpty( path ) )
            {
                return {};
            }

            FB_ASSERT( !StringUtil::isNullOrEmpty( path ) );

            // Check if the resource already exists in the database.
            for( auto &[hash, map] : m_resourceMap )
            {
                auto resIt = map.find( path );
                if( resIt != map.end() )
                {
                    // The resource already exists in the database.
                    return Pair<SmartPtr<IResource>, bool>( resIt->second.lock(), false );
                }
            }

            static const auto materialType = render::IMaterial::typeInfo();
            static const auto meshResourceType = IMeshResource::typeInfo();
            static const auto textureResourceType = render::ITexture::typeInfo();

            u32 type = 0;

            if( ApplicationUtil::isSupportedMesh( path ) )
            {
                type = meshResourceType;
            }
            else if( Path::getFileExtension( path ) == ".mat" )
            {
                type = materialType;
            }
            else if( ApplicationUtil::isSupportedTexture( path ) )
            {
                type = textureResourceType;
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto assetDatabaseManager =
                fb::static_pointer_cast<AssetDatabaseManager>( getDatabaseManager() );
            auto director = assetDatabaseManager->getResourceEntryFromPath( path );

            auto result = createOrRetrieveFromDirector( type, path, director );

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

    auto ResourceDatabase::getObject( const String &uuid ) -> SmartPtr<ISharedObject>
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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();

            auto databaseManager = getDatabaseManager();
            auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );

            auto cachePath = applicationManager->getCachePath();
            assetDatabaseManager->loadFromFile( cachePath + "asset.db" );

            auto sql = static_cast<String>( "select * from 'resources' where uuid='" + uuid + "'" );
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
                    else
                    {
                        auto resource = factoryManager->createObjectFromType<IResource>( type );
                        if( resource )
                        {
                            auto handle = resource->getHandle();
                            if( handle )
                            {
                                handle->setUUID( uuid );
                            }

                            resource->loadFromFile( path );

                            return resource;
                        }
                    }

                    query->nextRow();
                }
            }
        }

        return nullptr;
    }

    auto ResourceDatabase::loadAssetNode( SmartPtr<IDatabaseManager> db, SmartPtr<Properties> parent,
                                          hash64 id ) -> SmartPtr<Properties>
    {
        auto applicationManager = core::ApplicationManager::instance();

        auto properties = fb::make_ptr<Properties>();
        parent->addChild( properties );

        auto sAssetId = StringUtil::toString( id );
        String sql = "select * from configured_actors where id = '" + sAssetId + "'";

        if( auto query = db->executeQuery( sql ) )
        {
            while( !query->eof() )
            {
                auto numFields = query->getNumFields();
                for( size_t i = 0; i < numFields; ++i )
                {
                    auto name = query->getFieldName( static_cast<u32>( i ) );
                    auto value = query->getFieldValue( static_cast<u32>( i ) );

                    properties->setProperty( name, value );
                }

                query->nextRow();
            }
        }

        String parentSql = "select * from configured_actors where parent_id = '" + sAssetId + "'";
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

    auto ResourceDatabase::loadAssetProperties( hash64 id ) -> SmartPtr<Properties>
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto databaseManager = fb::make_ptr<AssetDatabaseManager>();

        auto mediaPath = applicationManager->getMediaPath();
        //databaseManager->loadFromFile( mediaPath + "/AssetDatabase.db" );
        databaseManager->loadFromFile( "C:/dev/fireblade/Bin/Media/AssetDatabase.db" );

        auto properties = fb::make_ptr<Properties>();
        properties->setName( "asset" );
        properties->setProperty( String( "asset_type" ), String( "model" ) );

        loadAssetNode( databaseManager, properties, id );

        auto sAssetId = StringUtil::toString( id );
        String sql = "select * from attribs where actor_id = '" + sAssetId + "'";

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
        objects.emplace_back( actor );

        if( auto transform = actor->getTransform() )
        {
            objects.emplace_back( transform );
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

    auto ResourceDatabase::getReferenceObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();

        Array<SmartPtr<ISharedObject>> objects;
        objects.reserve( 4096 );

        auto projectPath = applicationManager->getProjectPath();
        if( StringUtil::isNullOrEmpty( projectPath ) )
        {
            projectPath = Path::getWorkingDirectory();
        }

        auto databaseManager = getDatabaseManager();
        FB_ASSERT( databaseManager );

        auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
        FB_ASSERT( assetDatabaseManager );

        auto cachePath = applicationManager->getMediaPath();

        auto databasePath = cachePath + "/AssetDatabase.db";
        assetDatabaseManager->loadFromFile( databasePath );

        auto sql = String( "select * from 'configured_actors' where parent_id=1" );
        auto query = assetDatabaseManager->executeQuery( sql );
        if( query )
        {
            while( !query->eof() )
            {
                auto id = query->getFieldValue( "id" );
                auto uuid = query->getFieldValue( "uuid" );
                auto path = query->getFieldValue( "path" );
                auto type = query->getFieldValue( "type" );
                auto name = query->getFieldValue( "name" );

                path = "reference/" + name;  // test

                //if( StringUtil::isNullOrEmpty( path ) )
                //{
                //    query->nextRow();
                //    continue;
                //}

                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = id;
                }

                auto resourceDirector = factoryManager->make_object<scene::IDirector>();
                resourceDirector->load( nullptr );

                if( auto handle = resourceDirector->getHandle() )
                {
                    handle->setName( name );
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

                objects.emplace_back( resourceDirector );

                query->nextRow();
            }
        }

        return objects;
    }

    auto ResourceDatabase::getSceneObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto applicationManager = core::ApplicationManager::instance();
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

    auto ResourceDatabase::getObjectByFileId( const String &fileId ) const -> SmartPtr<ISharedObject>
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

    auto ResourceDatabase::getInstancesPtr() const -> SharedPtr<Array<SmartPtr<IResource>>>
    {
        return m_instances;
    }

    void ResourceDatabase::setInstancesPtr( SharedPtr<Array<SmartPtr<IResource>>> instances )
    {
        m_instances = instances;
    }

    auto ResourceDatabase::getTextureTypes() const -> Array<String>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_textureTypes;
    }

    void ResourceDatabase::setTextureTypes( const Array<String> &textureTypes )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_textureTypes = textureTypes;
    }

    auto ResourceDatabase::getAudioTypes() const -> Array<String>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_audioTypes;
    }

    void ResourceDatabase::setAudioTypes( const Array<String> &audioTypes )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_audioTypes = audioTypes;
    }

    ResourceDatabase::ImportFileJob::ImportFileJob()
    {
        ResourceDatabase::m_numJobs++;
    }

    ResourceDatabase::ImportFileJob::~ImportFileJob()
    {
        ResourceDatabase::m_numJobs--;
    }

    void ResourceDatabase::ImportFileJob::execute()
    {
        try
        {
            auto filePath = getFilePath();

            // FB_LOG("Importing File: " + filePath);

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            // FB_ASSERT( sceneManager );

            // auto assetDatabaseManager = getDatabaseManager();
            // FB_ASSERT( assetDatabaseManager );

            auto databaseManager = resourceDatabase->getDatabaseManager();
            FB_ASSERT( databaseManager );

            auto assetDatabaseManager = fb::static_pointer_cast<AssetDatabaseManager>( databaseManager );
            FB_ASSERT( assetDatabaseManager );

            auto cachePath = applicationManager->getCachePath();

            auto databasePath = cachePath + "/" + "asset.db";
            databasePath = StringUtil::cleanupPath( databasePath );

            assetDatabaseManager->loadFromFile( databasePath );

            auto basePath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( basePath ) )
            {
                basePath = Path::getWorkingDirectory();
            }

            auto data = String( "" );

            auto fileExt = Path::getFileExtension( filePath );
            fileExt = StringUtil::make_lower( fileExt );

            if( fileExt == ".fbmeshbin" )
            {
                auto meshManager = applicationManager->getMeshManager();
                auto mesh = meshManager->loadFromFile( filePath );
                if( mesh )
                {
                    if( assetDatabaseManager->hasResourceEntry( mesh ) )
                    {
                        assetDatabaseManager->updateResourceEntry( mesh );
                    }
                    else
                    {
                        assetDatabaseManager->addResourceEntry( mesh );
                    }
                }
            }
            else if( ApplicationUtil::isSupportedMesh( filePath ) )
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
            else if( fileExt == ".mat" )
            {
                if( graphicsSystem )
                {
                    auto materialManager = graphicsSystem->getMaterialManager();
                    auto material = materialManager->loadFromFile( filePath );
                    if( material )
                    {
                        if( assetDatabaseManager->hasResourceEntry( material ) )
                        {
                            assetDatabaseManager->updateResourceEntry( material );
                        }
                        else
                        {
                            assetDatabaseManager->addResourceEntry( material );
                        }
                    }
                }
            }
            else if( ApplicationUtil::isSupportedTexture( filePath ) )
            {
                if( graphicsSystem )
                {
                    auto textureManager = graphicsSystem->getTextureManager();
                    auto texture = textureManager->loadFromFile( filePath );
                    if( texture )
                    {
                        if( assetDatabaseManager->hasResourceEntry( texture ) )
                        {
                            assetDatabaseManager->updateResourceEntry( texture );
                        }
                        else
                        {
                            assetDatabaseManager->addResourceEntry( texture );
                        }
                    }
                }
            }
            else if( fileExt == ".lightingpreset" )
            {
                auto lightingpreset = fb::make_ptr<scene::SceneLightingDirector>();
                if( lightingpreset )
                {
                    auto handle = lightingpreset->getHandle();
                    auto uuid = handle->getUUID();
                    if( StringUtil::isNullOrEmpty( uuid ) )
                    {
                        uuid = StringUtil::getUUID();
                        handle->setUUID( uuid );
                    }

                    lightingpreset->setFilePath( filePath );

                    auto dataStr = fileSystem->readAllText( filePath );

                    auto properties = fb::make_ptr<Properties>();
                    DataUtil::parse( dataStr, properties.get() );

                    lightingpreset->setProperties( properties );

                    if( assetDatabaseManager->hasResourceEntry( lightingpreset ) )
                    {
                        assetDatabaseManager->updateResourceEntry( lightingpreset );
                    }
                    else
                    {
                        assetDatabaseManager->addResourceEntry( lightingpreset );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ResourceDatabase::ImportFileJob::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

    String ResourceDatabase::ImportFileJob::getFilePath() const
    {
        return m_filePath;
    }

}  // end namespace fb
