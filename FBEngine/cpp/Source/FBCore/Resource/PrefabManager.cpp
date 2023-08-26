#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/PrefabManager.h>
#include <FBCore/Resource/Prefab.h>
#include <FBCore/Scene/Actor.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        SmartPtr<IActor> PrefabManager::createInstance( SmartPtr<IActor> prefab )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        /*
        SmartPtr<IActor> CPrefabManager::loadActor( data::actor_data &data,
                                                    SmartPtr<IActor> parent )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto actor = sceneManager->createActor();
                FB_ASSERT( actor );

                auto actorData = factoryManager->make_ptr<Data<data::actor_data>>();
                FB_ASSERT( actorData );

                //actorData->setData( &data );
                //actor->fromData( actorData );

                return actor;
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }
        */

        SmartPtr<IActor> PrefabManager::loadActor( SmartPtr<Properties> data, SmartPtr<IActor> parent )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<IPrefab> PrefabManager::loadPrefab( const String &filePath )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto uuid = StringUtil::getUUID();

            auto prefabResource = create( uuid );
            auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

            auto ext = Path::getFileExtension( filePath );
            if( ext == ".fbx" || ext == ".FBX" )
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

        SmartPtr<IResource> PrefabManager::create( const String &uuid )
        {
            auto prefab = fb::make_ptr<Prefab>();

            if( auto handle = prefab->getHandle() )
            {
                handle->setUUID( uuid );
            }

            m_prefabs.push_back( prefab );
            return prefab;
        }

        SmartPtr<IResource> PrefabManager::create( const String &uuid, const String &name )
        {
            auto prefab = fb::make_ptr<Prefab>();

            if( auto handle = prefab->getHandle() )
            {
                handle->setUUID( uuid );
                handle->setName( name );
            }

            m_prefabs.push_back( prefab );
            return prefab;
        }

        void PrefabManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
        }

        SmartPtr<IResource> PrefabManager::loadFromFile( const String &filePath )
        {
            auto uuid = StringUtil::getUUID();

            auto prefabResource = create( uuid );
            auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

            return prefab;
        }

        SmartPtr<IResource> PrefabManager::load( const String &name )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto uuid = StringUtil::getUUID();

                auto prefabResource = create( uuid );
                auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

                auto ext = Path::getFileExtension( name );
                if( ext == ".fbx" || ext == ".FBX" )
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

        SmartPtr<IResource> PrefabManager::getByName( const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> PrefabManager::getById( const String &uuid )
        {
            return nullptr;
        }

        void PrefabManager::_getObject( void **ppObject ) const
        {
        }

        Pair<SmartPtr<IResource>, bool> PrefabManager::createOrRetrieve( const String &uuid,
                                                                         const String &path,
                                                                         const String &type )
        {
            if( auto prefabResource = getById( uuid ) )
            {
                return std::pair<SmartPtr<IResource>, bool>( prefabResource, false );
            }

            auto prefab = create( uuid, path );
            FB_ASSERT( prefab );

            return Pair<SmartPtr<IResource>, bool>( prefab, true );
        }

        Pair<SmartPtr<IResource>, bool> PrefabManager::createOrRetrieve( const String &path )
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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = prefab->toData();
            auto dataStr = DataUtil::toString( data.get() );

            fileSystem->writeAllText( filePath, dataStr );
        }
    }  // namespace scene
}  // end namespace fb
