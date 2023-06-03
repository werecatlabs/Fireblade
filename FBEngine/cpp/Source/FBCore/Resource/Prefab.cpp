#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/Prefab.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, Prefab, Resource<IPrefab> );

        Prefab::~Prefab()
        {
            unload( nullptr );
        }

        SmartPtr<IActor> Prefab::createActor()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            if( actor )
            {
                if( m_actor )
                {
                    auto data = getData();
                    if( !data )
                    {
                        data = m_actor->toData();
                    }

                    actor->fromData( data );

                    FB_ASSERT( actor->getChildrenPtr()->size() == m_actor->getChildrenPtr()->size() );
                }

                return actor;
            }

            return nullptr;
        }

        SmartPtr<IActor> Prefab::getActor() const
        {
            return m_actor.lock();
        }

        void Prefab::setActor( SmartPtr<IActor> actor )
        {
            m_actor = actor;
        }

        void Prefab::save()
        {
        }

        void Prefab::load( SmartPtr<ISharedObject> data )
        {
        }

        void Prefab::unload( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            if( auto actor = getActor() )
            {
                sceneManager->destroyActor( actor );
                setActor( nullptr );
            }
        }

        hash64 Prefab::getFileSystemId() const
        {
            return m_fileId;
        }

        void Prefab::setFileSystemId( hash64 id )
        {
            m_fileId = id;
        }

        SmartPtr<Properties> Prefab::getProperties() const
        {
            auto properties = Resource<IPrefab>::getProperties();
            return properties;
        }

        void Prefab::setProperties( SmartPtr<Properties> properties )
        {
        }

        SmartPtr<IStateContext> Prefab::getStateObject() const
        {
            return m_stateObject;
        }

        void Prefab::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void Prefab::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        SmartPtr<ISharedObject> Prefab::getData() const
        {
            return m_data;
        }

        void Prefab::setData( SmartPtr<ISharedObject> data )
        {
            m_data = data;
        }
    }  // namespace scene
}  // end namespace fb
