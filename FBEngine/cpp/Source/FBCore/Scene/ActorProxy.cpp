#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/ActorProxy.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        ActorProxy::ActorProxy()
        {
        }

        ActorProxy::~ActorProxy()
        {
        }

        SmartPtr<IDirector> ActorProxy::getDirector() const
        {
            return m_director;
        }

        void ActorProxy::setDirector( SmartPtr<IDirector> director )
        {
            m_director = director;
        }

        SmartPtr<IActor> ActorProxy::getActorImpl() const
        {
            return m_actorImpl;
        }

        void ActorProxy::setActorImpl( SmartPtr<IActor> actorImpl )
        {
            m_actorImpl = actorImpl;
        }

        void ActorProxy::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateDirty( flags, oldFlags );
            }
        }

        Transform3<real_Num> ActorProxy::getLocalTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> ActorProxy::getWorldTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }
            return Transform3<real_Num>();
        }

        Vector3<real_Num> ActorProxy::getLocalPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void ActorProxy::setLocalPosition( const Vector3<real_Num> &localPosition )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalPosition( localPosition );
            }
        }

        Vector3<real_Num> ActorProxy::getLocalScale() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalScale();
            }

            return Vector3<real_Num>::unit();
        }

        void ActorProxy::setLocalScale( const Vector3<real_Num> &localScale )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalScale( localScale );
            }
        }

        Quaternion<real_Num> ActorProxy::getLocalOrientation() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalOrientation();
            }
            return Quaternion<real_Num>::identity();
        }

        void ActorProxy::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalOrientation( localOrientation );
            }
        }

        Vector3<real_Num> ActorProxy::getPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getPosition();
            }
            return Vector3<real_Num>::zero();
        }

        void ActorProxy::lookAt( const Vector3<real_Num> &position )
        {
            if( auto actor = getActorImpl() )
            {
                actor->lookAt( position );
            }
        }

        void ActorProxy::lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis )
        {
            if( auto actor = getActorImpl() )
            {
                actor->lookAt( position, yawAxis );
            }
        }

        void ActorProxy::setPosition( const Vector3<real_Num> &position )
        {
            if( auto transform = getTransform() )
            {
                transform->setPosition( position );
            }
        }

        Vector3<real_Num> ActorProxy::getScale() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getScale();
            }

            return Vector3<real_Num>::unit();
        }

        void ActorProxy::setScale( const Vector3<real_Num> &scale )
        {
            if( auto transform = getTransform() )
            {
                transform->setScale( scale );
            }
        }

        Quaternion<real_Num> ActorProxy::getOrientation() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void ActorProxy::setOrientation( const Quaternion<real_Num> &orientation )
        {
            if( auto transform = getTransform() )
            {
                transform->setOrientation( orientation );
            }
        }

        void ActorProxy::awake()
        {
            if( auto actor = getActorImpl() )
            {
                actor->awake();
            }
        }

        void ActorProxy::play()
        {
            if( auto actor = getActorImpl() )
            {
                actor->play();
            }
        }

        void ActorProxy::edit()
        {
            if( auto actor = getActorImpl() )
            {
                actor->edit();
            }
        }

        void ActorProxy::levelWasLoaded( SmartPtr<IScene> scene )
        {
            if( auto actor = getActorImpl() )
            {
                actor->levelWasLoaded( scene );
            }
        }

        void ActorProxy::reset()
        {
            if( auto actor = getActorImpl() )
            {
                actor->reset();
            }
        }

        void ActorProxy::hierarchyChanged()
        {
            if( auto actor = getActorImpl() )
            {
                actor->hierarchyChanged();
            }
        }

        void ActorProxy::childAdded( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childAdded( child );
            }
        }

        void ActorProxy::childRemoved( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childRemoved( child );
            }
        }

        void ActorProxy::childAddedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childAddedInHierarchy( child );
            }
        }

        void ActorProxy::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childRemovedInHierarchy( child );
            }
        }

        String ActorProxy::getName() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getName();
            }
            return String();
        }

        void ActorProxy::setName( const String &name )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setName( name );
            }
        }

        bool ActorProxy::getPerpetual() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getPerpetual();
            }
            return false;
        }

        void ActorProxy::setPerpetual( bool perpetual )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setPerpetual( perpetual );
            }
        }

        void ActorProxy::addComponentInstance( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addComponentInstance( component );
            }
        }

        void ActorProxy::removeComponentInstance( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeComponentInstance( component );
            }
        }

        u32 ActorProxy::getEntity() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getEntity();
            }

            return 0;
        }

        void ActorProxy::setEntity( u32 entity )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setEntity( entity );
            }
        }

        Array<SmartPtr<IComponent>> ActorProxy::getComponents() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getComponents();
            }

            return Array<SmartPtr<IComponent>>();
        }

        SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> ActorProxy::getComponentsPtr() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getComponentsPtr();
            }
            return nullptr;
        }

        SmartPtr<IActor> ActorProxy::getChildByIndex( u32 index ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getChildByIndex( index );
            }
            return nullptr;
        }

        u32 ActorProxy::getNumChildren() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNumChildren();
            }
            return 0;
        }

        s32 ActorProxy::getSiblingIndex() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSiblingIndex();
            }
            return -1;
        }

        void ActorProxy::addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                             SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addDirtyComponent( taskId, updateType, component );
            }
        }

        bool ActorProxy::isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                            SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isComponentDirty( taskId, updateType, component );
            }

            return false;
        }

        void ActorProxy::registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                   SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->registerComponentUpdate( taskId, updateType, object );
            }
        }

        void ActorProxy::unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                     SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->unregisterComponentUpdate( taskId, updateType, object );
            }
        }

        void ActorProxy::unregisterAllComponent( SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->unregisterAllComponent( object );
            }
        }

        void ActorProxy::triggerEnter( SmartPtr<IComponent> collision )
        {
            if( auto actor = getActorImpl() )
            {
                actor->triggerEnter( collision );
            }
        }

        void ActorProxy::triggerLeave( SmartPtr<IComponent> collision )
        {
            if( auto actor = getActorImpl() )
            {
                actor->triggerLeave( collision );
            }
        }

        void ActorProxy::componentLoaded( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->componentLoaded( component );
            }
        }

        void ActorProxy::setParent( SmartPtr<IActor> parent )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setParent( parent );
            }
        }

        SmartPtr<IActor> ActorProxy::getParent() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getParent();
            }

            return nullptr;
        }

        void ActorProxy::addChild( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addChild( child );
            }
        }

        void ActorProxy::removeChild( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeChild( child );
            }
        }

        void ActorProxy::removeChildren()
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeChildren();
            }
        }

        SmartPtr<IActor> ActorProxy::findChild( const String &name )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->findChild( name );
            }

            return nullptr;
        }

        SharedPtr<ConcurrentArray<SmartPtr<IActor>>> ActorProxy::getChildrenPtr() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getChildrenPtr();
            }

            return nullptr;
        }

        Array<SmartPtr<IActor>> ActorProxy::getAllChildren() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getAllChildren();
            }

            return Array<SmartPtr<IActor>>();
        }

        bool ActorProxy::isMine() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isMine();
            }

            return false;
        }

        void ActorProxy::setMine( bool mine )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setMine( mine );
            }
        }

        bool ActorProxy::isStatic() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isStatic();
            }

            return false;
        }

        void ActorProxy::setStatic( bool isstatic )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setStatic( isstatic );
            }
        }

        bool ActorProxy::isEnabledInScene() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isEnabledInScene();
            }

            return false;
        }

        bool ActorProxy::isEnabled() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isEnabled();
            }

            return false;
        }

        void ActorProxy::setEnabled( bool enabled )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->setEnabled( enabled );
            }
        }

        bool ActorProxy::isVisible() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isVisible();
            }

            return false;
        }

        void ActorProxy::setVisible( bool visible, bool cacade /*= true */ )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setVisible( visible, cacade );
            }
        }

        bool ActorProxy::isDirty() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isDirty();
            }

            return false;
        }

        void ActorProxy::setDirty( bool dirty )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setDirty( dirty );
            }
        }

        void ActorProxy::updateTransform()
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateTransform();
            }
        }

        SmartPtr<IActor> ActorProxy::getSceneRoot() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSceneRoot();
            }

            return nullptr;
        }

        u32 ActorProxy::getSceneLevel() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSceneLevel();
            }

            return 0;
        }

        SmartPtr<ITransform> ActorProxy::getTransform() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getTransform();
            }

            return nullptr;
        }

        void ActorProxy::setTransform( SmartPtr<ITransform> transform )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setTransform( transform );
            }
        }

        void ActorProxy::setState( State state )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setState( state );
            }
        }

        IActor::State ActorProxy::getState() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getState();
            }

            return IActor::State::None;
        }

        void ActorProxy::setGameState( GameState state )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setGameState( state );
            }
        }

        IActor::GameState ActorProxy::getGameState() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getGameState();
            }

            return IActor::GameState::None;
        }

        u32 ActorProxy::getFlags() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getFlags();
            }

            return false;
        }

        void ActorProxy::setFlags( u32 flags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setFlags( flags );
            }
        }

        bool ActorProxy::getFlag( u32 flag ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getFlag( flag );
            }

            return false;
        }

        void ActorProxy::setFlag( u32 flag, bool value )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setFlag( flag, value );
            }
        }

        u32 ActorProxy::getNewFlags() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNewFlags();
            }

            return 0;
        }

        void ActorProxy::setNewFlags( u32 flags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setNewFlags( flags );
            }
        }

        bool ActorProxy::getNewFlag( u32 flag ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNewFlag( flag );
            }

            return false;
        }

        void ActorProxy::setNewFlag( u32 flag, bool value )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setNewFlag( flag, value );
            }
        }

        SmartPtr<IScene> ActorProxy::getScene() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getScene();
            }

            return nullptr;
        }

        void ActorProxy::setScene( SmartPtr<IScene> scene )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setScene( scene );
            }
        }

        void ActorProxy::updateVisibility()
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateVisibility();
            }
        }
    }  // namespace scene
}  // namespace fb
