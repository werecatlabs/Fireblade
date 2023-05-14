#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/CPrefabManager.h>
#include <FBCore/Resource/CPrefab.h>
#include <FBCore/Scene/CActor.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        SmartPtr<IActor> CPrefabManager::createInstance( SmartPtr<IActor> prefab )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto newActor = sceneManager->createActor();
            //auto prefabData = prefab->toData();
            //newActor->fromData( prefabData );

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

        SmartPtr<IActor> CPrefabManager::loadActor( SmartPtr<IData> data,
                                                    SmartPtr<IActor> parent )
        {
            //auto pData = data->getDataAsType<data::actor_data>();
            //return loadActor( *pData, parent );

            return nullptr;
        }

        SmartPtr<IPrefab> CPrefabManager::loadPrefab( const String &filePath )
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
            if(ext == ".fbx" || ext == ".FBX")
            {
                auto meshLoader = applicationManager->getMeshLoader();
                FB_ASSERT( meshLoader );

                if(meshLoader)
                {
                    auto actor = meshLoader->loadActor( filePath );
                    if(actor)
                    {
                        prefab->setActor( actor );
                        return prefab;
                    }
                }
            }
            else if(ext == ".hda" || ext == ".HDA")
            {
            }
            else if(ext == ".fbmeshbin" || ext == ".FBMESHBIN")
            {
                auto actor = sceneManager->createActor();
                auto meshComponent = actor->addComponent<Mesh>();
                if(meshComponent)
                {
                    meshComponent->setMeshPath( filePath );
                }

                auto meshRenderer = actor->addComponent<MeshRenderer>();
                auto material = actor->addComponent<Material>();

                prefab->setActor( actor );
                return prefab;
            }
            else if(ext == ".prefab")
            {
                //auto jsonStr = fileSystem->readAllText( filePath );

                //data::actor_data data;
                //DataUtil::parse( jsonStr, &data );

                //auto actor = loadActor( data, nullptr );
                //actor->updateTransform();

                //prefab->setActor( actor );
                //return prefab;
            }

            return nullptr;
        }

        SmartPtr<IResource> CPrefabManager::create( const String &uuid )
        {
            auto prefab = fb::make_ptr<CPrefab>();

            if(auto handle = prefab->getHandle())
            {
                handle->setUUID( uuid );
            }

            m_prefabs.push_back( prefab );
            return prefab;
        }

        SmartPtr<IResource> CPrefabManager::create( const String &uuid, const String &name )
        {
            auto prefab = fb::make_ptr<CPrefab>();

            if(auto handle = prefab->getHandle())
            {
                handle->setUUID( uuid );
                handle->setName( name );
            }

            m_prefabs.push_back( prefab );
            return prefab;
        }

        void CPrefabManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
        }

        SmartPtr<IResource> CPrefabManager::loadFromFile( const String &filePath )
        {
            auto uuid = StringUtil::getUUID();

            auto prefabResource = create( uuid );
            auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );

            return prefab;
        }

        SmartPtr<IResource> CPrefabManager::load( const String &name )
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
                if(ext == ".fbx" || ext == ".FBX")
                {
                    auto meshLoader = applicationManager->getMeshLoader();
                    if(meshLoader)
                    {
                        auto actor = meshLoader->loadActor( name );

                        prefab->setActor( actor );
                        return prefab;
                    }
                }
                else if(ext == ".hda" || ext == ".HDA")
                {
                }
                else if(ext == ".fbmeshbin" || ext == ".FBMESHBIN")
                {
                    auto sceneManager = applicationManager->getSceneManager();
                    auto scene = sceneManager->getCurrentScene();

                    auto actor = sceneManager->createActor();
                    auto meshComponent = actor->addComponent<Mesh>();
                    if(meshComponent)
                    {
                        meshComponent->setMeshPath( name );
                    }

                    auto meshRenderer = actor->addComponent<MeshRenderer>();
                    auto material = actor->addComponent<Material>();

                    prefab->setActor( actor );
                    return prefab;
                }
                else if(ext == ".prefab")
                {
                    //auto jsonStr = fileSystem->readAllText( name );

                    //auto pData = fb::make_ptr<Data<data::actor_data>>();
                    //auto data = pData->getDataAsType<data::actor_data>();
                    //DataUtil::parse( jsonStr, data );

                    //auto actor = loadActor( *data, nullptr );
                    //actor->updateTransform();

                    //prefab->setActor( actor );
                    //prefab->setData( pData );
                    //return prefab;
                }
            }
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CPrefabManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void CPrefabManager::unload( SmartPtr<ISharedObject> data )
        {
            for(auto prefab : m_prefabs)
            {
                prefab->unload( nullptr );
            }

            m_prefabs.clear();
        }

        SmartPtr<IResource> CPrefabManager::getByName( const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> CPrefabManager::getById( const String &uuid )
        {
            return nullptr;
        }

        void CPrefabManager::_getObject( void **ppObject ) const
        {
        }

        Pair<SmartPtr<IResource>, bool> CPrefabManager::createOrRetrieve( const String &uuid,
            const String &path,
            const String &type )
        {
            return Pair<SmartPtr<IResource>, bool>();
        }

        Pair<SmartPtr<IResource>, bool> CPrefabManager::createOrRetrieve( const String &path )
        {
            return Pair<SmartPtr<IResource>, bool>();
        }

        void CPrefabManager::savePrefab( const String &filePath, SmartPtr<IActor> prefab )
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
            FB_ASSERT( prefab );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            //auto data = prefab->toJson();
            //FB_ASSERT( !StringUtil::isNullOrEmpty( data ) );

            //fileSystem->writeAllText( filePath, data );
        }
    } // namespace scene
}     // end namespace fb
