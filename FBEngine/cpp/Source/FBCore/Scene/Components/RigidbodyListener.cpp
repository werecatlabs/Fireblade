#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/RigidbodyListener.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>

namespace fb
{
    namespace scene
    {

        FB_CLASS_REGISTER_DERIVED( fb::scene, RigidbodyListener, SharedObject<IEventListener> );

        Parameter RigidbodyListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::transform )
            {
                auto t = Transform3<real_Num>( arguments[0].getVector3(), arguments[1].getQuaternion() );
                handleTransform( t );
            }

            return Parameter();
        }
        
        void RigidbodyListener::handleTransform( const Transform3<real_Num> &t )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            FB_ASSERT( m_owner );
            if( auto actor = m_owner->getActor() )
            {
                auto position = t.getPosition();
                auto orientation = t.getOrientation();

                if( auto transform = actor->getTransform() )
                {
                    transform->setLocalPosition( position );
                    transform->setLocalOrientation( orientation );

                    transform->setPosition( position );
                    transform->setOrientation( orientation );
                }

                auto components = actor->getComponents();
                for( auto &component : components )
                {
                    if( component != m_owner )
                    {
                        sceneManager->addDirtyComponentTransform( component );
                    }
                }

                if( auto p = actor->getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        FB_ASSERT( child );

                        auto childTransform = child->getTransform();
                        FB_ASSERT( childTransform );

                        childTransform->setDirty( true );
                    }
                }
            }
        }

        Rigidbody *RigidbodyListener::getOwner() const
        {
            return m_owner;
        }

        void RigidbodyListener::setOwner( Rigidbody *owner )
        {
            m_owner = owner;
        }

    }  // namespace scene
}  // namespace fb
