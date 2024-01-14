#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/PrefabManager.h>
#include <FBCore/Resource/Prefab.h>
#include <FBCore/Scene/Actor.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/ApplicationUtil.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb, PrefabManager, IPrefabManager );

    PrefabManager::PrefabManager() = default;

    PrefabManager::~PrefabManager() = default;

    auto PrefabManager::createInstance( SmartPtr<IActor> prefab ) -> SmartPtr<IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto newActor = sceneManager->createActor();
        auto prefabData = prefab->toData();
        newActor->fromData( prefabData );

        return newActor;
    }

    auto PrefabManager::loadActor( SmartPtr<Properties> data, SmartPtr<IActor> parent )
        -> SmartPtr<IActor>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            actor->setProperties( data );

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto PrefabManager::loadPrefab( const String &filePath ) -> SmartPtr<IPrefab>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto uuid = StringUtil::getUUID();

        auto prefabResource = create( uuid );
        auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

        auto ext = Path::getFileExtension( filePath );
        ext = StringUtil::make_lower( ext );

        if( ApplicationUtil::isSupportedMesh( filePath ) )
        {
            auto meshLoader = applicationManager->getMeshLoader();
            FB_ASSERT( meshLoader );

            if( meshLoader )
            {
                auto actor = meshLoader->loadActor( filePath );
                if( actor )
                {
                    prefab->setActor( actor );
                    return prefab;
                }
            }
        }
        else if( ext == ".hda" || ext == ".HDA" )
        {
        }
        else if( ext == ".fbmeshbin" || ext == ".FBMESHBIN" )
        {
            auto actor = sceneManager->createActor();
            auto meshComponent = actor->addComponent<Mesh>();
            if( meshComponent )
            {
                meshComponent->setMeshPath( filePath );
            }

            auto meshRenderer = actor->addComponent<MeshRenderer>();
            auto material = actor->addComponent<Material>();

            prefab->setActor( actor );
            return prefab;
        }
        else if( ext == ".prefab" )
        {
            auto jsonStr = fileSystem->readAllText( filePath );

            auto data = fb::make_ptr<Properties>();
            DataUtil::parse( jsonStr, data.get() );

            auto actor = loadActor( data, nullptr );
            prefab->setActor( actor );
            return prefab;
        }

        return nullptr;
    }

    auto PrefabManager::create( const String &uuid ) -> SmartPtr<IResource>
    {
        auto prefab = fb::make_ptr<Prefab>();

        if( auto handle = prefab->getHandle() )
        {
            handle->setUUID( uuid );
        }

        m_prefabs.emplace_back( prefab );
        return prefab;
    }

    auto PrefabManager::create( const String &uuid, const String &name ) -> SmartPtr<IResource>
    {
        auto prefab = fb::make_ptr<Prefab>();

        if( auto handle = prefab->getHandle() )
        {
            handle->setUUID( uuid );
            handle->setName( name );
        }

        m_prefabs.emplace_back( prefab );
        return prefab;
    }

    void PrefabManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    auto PrefabManager::loadFromFile( const String &filePath ) -> SmartPtr<IResource>
    {
        auto uuid = StringUtil::getUUID();

        auto prefabResource = create( uuid );
        auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

        return prefab;
    }

    auto PrefabManager::loadResource( const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto uuid = StringUtil::getUUID();

            auto prefabResource = create( uuid );
            auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

            auto ext = Path::getFileExtension( name );
            ext = StringUtil::make_lower( ext );

            if( ApplicationUtil::isSupportedMesh( name ) )
            {
                auto meshLoader = applicationManager->getMeshLoader();
                if( meshLoader )
                {
                    auto actor = meshLoader->loadActor( name );

                    prefab->setActor( actor );
                    return prefab;
                }
            }
            else if( ext == ".hda" || ext == ".HDA" )
            {
            }
            else if( ext == ".fbmeshbin" || ext == ".FBMESHBIN" )
            {
                auto sceneManager = applicationManager->getSceneManager();
                auto scene = sceneManager->getCurrentScene();

                auto actor = sceneManager->createActor();
                auto meshComponent = actor->addComponent<Mesh>();
                if( meshComponent )
                {
                    meshComponent->setMeshPath( name );
                }

                auto meshRenderer = actor->addComponent<MeshRenderer>();
                auto material = actor->addComponent<Material>();

                prefab->setActor( actor );
                return prefab;
            }
            else if( ext == ".prefab" )
            {
                auto jsonStr = fileSystem->readAllText( name );

                auto data = fb::make_ptr<Properties>();
                DataUtil::parse( jsonStr, data.get() );

                auto actor = loadActor( data, nullptr );
                actor->updateTransform();

                prefab->setActor( actor );
                prefab->setData( data );
                return prefab;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void PrefabManager::load( SmartPtr<ISharedObject> data )
    {
    }

    void PrefabManager::unload( SmartPtr<ISharedObject> data )
    {
        for( auto prefab : m_prefabs )
        {
            prefab->unload( nullptr );
        }

        m_prefabs.clear();
    }

    auto PrefabManager::getByName( const String &name ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto PrefabManager::getById( const String &uuid ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    void PrefabManager::_getObject( void **ppObject ) const
    {
    }

    auto PrefabManager::createOrRetrieve( const String &uuid, const String &path, const String &type )
        -> Pair<SmartPtr<IResource>, bool>
    {
        if( auto prefabResource = getById( uuid ) )
        {
            return std::pair<SmartPtr<IResource>, bool>( prefabResource, false );
        }

        auto prefab = create( uuid, path );
        FB_ASSERT( prefab );

        return Pair<SmartPtr<IResource>, bool>( prefab, true );
    }

    auto PrefabManager::createOrRetrieve( const String &path ) -> Pair<SmartPtr<IResource>, bool>
    {
        if( auto prefabResource = getByName( path ) )
        {
            return std::pair<SmartPtr<IResource>, bool>( prefabResource, false );
        }

        auto prefab = create( path );
        FB_ASSERT( prefab );

        return Pair<SmartPtr<IResource>, bool>( prefab, true );
    }

    void PrefabManager::savePrefab( const String &filePath, SmartPtr<IActor> prefab )
    {
        FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
        FB_ASSERT( prefab );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto data = prefab->toData();
        auto dataStr = DataUtil::toString( data.get() );

        fileSystem->writeAllText( filePath, dataStr );
    }
}  // namespace fb::scene
