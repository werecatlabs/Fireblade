#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/RigidbodyListener.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/FBCore.h>

#include "FBCore/Scene/SceneManager.h"

namespace fb
{
    namespace scene
    {

        Parameter RigidbodyListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                               const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                               SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        void RigidbodyListener::updateChild( SmartPtr<scene::IActor> actor )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

#if _DEBUG
            auto handle = actor->getHandle();
            if( handle )
            {
                auto name = handle->getName();

                if( name == "dynamics" )
                {
                    int stop = 0;
                    stop = 0;
                }

                if( name == "F40f40" )
                {
                    int stop = 0;
                    stop = 0;
                }
            }
#endif
            // sceneManager->addDirtyTransform( actor );

            auto transform = actor->getTransform();
            if( transform )
            {
                transform->setDirty( true );
                sceneManager->addDirtyTransform( transform );
            }

            auto components = actor->getComponents();
            for( auto component : components )
            {
                // if( !component->isDerived<Rigidbody>() )
                {
                    sceneManager->addDirtyComponentTransform( component );
                }
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    updateChild( child );
                }
            }
        }

        void RigidbodyListener::handleTransform( const Transform3<real_Num> &t )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            FB_ASSERT( m_owner );
            auto actor = m_owner->getActor();
            if( actor )
            {
                auto position = t.getPosition();
                auto orientation = t.getOrientation();

                auto transform = actor->getTransform();
                if( transform )
                {
                    transform->setLocalPosition( position );
                    transform->setLocalOrientation( orientation );

                    sceneManager->addDirtyTransform( transform );
                }

                auto components = actor->getComponents();
                for( auto component : components )
                {
                    if( !component->isDerived<Rigidbody>() )
                    {
                        sceneManager->addDirtyComponentTransform( component );
                    }
                }

                // auto children = actor->getChildren();
                // for( auto child : children )
                //{
                //     updateChild( child );
                // }
            }
        }

        Rigidbody *RigidbodyListener::getOwner() const
        {
            return m_owner;
        }

        void RigidbodyListener::setOwner( Rigidbody *val )
        {
            m_owner = val;
        }

    }  // namespace scene
}  // namespace fb
