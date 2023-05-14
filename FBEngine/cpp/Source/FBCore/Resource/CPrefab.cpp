#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/CPrefab.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, CPrefab, CResource<IPrefab> );

        CPrefab::~CPrefab()
        {
            unload( nullptr );
        }

        SmartPtr<IActor> CPrefab::createActor()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            if( actor )
            {
                //if( m_actor )
                //{
                //    auto data = getData();
                //    if( !data )
                //    {
                //        data = m_actor->toData();
                //    }

                //    FB_ASSERT( data->getDataAsType<data::actor_data>()->children.size() ==
                //               m_actor->getChildrenPtr()->size() );

                //    actor->fromData( data );

                //    FB_ASSERT( actor->getChildrenPtr()->size() == m_actor->getChildrenPtr()->size() );
                //}

                return actor;
            }

            return nullptr;
        }

        SmartPtr<IActor> CPrefab::getActor() const
        {
            return m_actor.lock();
        }

        void CPrefab::setActor( SmartPtr<IActor> actor )
        {
            m_actor = actor;
        }

        void CPrefab::save()
        {
        }

        void CPrefab::load( SmartPtr<ISharedObject> data )
        {
        }

        void CPrefab::unload( SmartPtr<ISharedObject> data )
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

        hash64 CPrefab::getFileSystemId() const
        {
            return m_fileId;
        }

        void CPrefab::setFileSystemId( hash64 id )
        {
            m_fileId = id;
        }

        SmartPtr<Properties> CPrefab::getProperties() const
        {
            auto properties = CResource<IPrefab>::getProperties();
            return properties;
        }

        void CPrefab::setProperties( SmartPtr<Properties> properties )
        {
        }

        SmartPtr<IStateContext> CPrefab::getStateObject() const
        {
            return m_stateObject;
        }

        void CPrefab::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void CPrefab::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        SmartPtr<IData> CPrefab::getData() const
        {
            return m_data;
        }

        void CPrefab::setData( SmartPtr<IData> data )
        {
            m_data = data;
        }
    }  // namespace scene
}  // end namespace fb
