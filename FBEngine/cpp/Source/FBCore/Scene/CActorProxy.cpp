#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/CActorProxy.h>
#include <FBCore/Scene/CSceneManager.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        CActorProxy::CActorProxy()
        {
        }

        CActorProxy::~CActorProxy()
        {
        }

        SmartPtr<IDirector> CActorProxy::getDirector() const
        {
            return m_director;
        }

        void CActorProxy::setDirector( SmartPtr<IDirector> director )
        {
            m_director = director;
        }

        SmartPtr<IActor> CActorProxy::getActorImpl() const
        {
            return m_actorImpl;
        }

        void CActorProxy::setActorImpl( SmartPtr<IActor> actorImpl )
        {
            m_actorImpl = actorImpl;
        }

        void CActorProxy::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateDirty( flags, oldFlags );
            }
        }

        Transform3<real_Num> CActorProxy::getLocalTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> CActorProxy::getWorldTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }
            return Transform3<real_Num>();
        }

        Vector3<real_Num> CActorProxy::getLocalPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void CActorProxy::setLocalPosition( const Vector3<real_Num> &localPosition )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalPosition( localPosition );
            }
        }

        Vector3<real_Num> CActorProxy::getLocalScale() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalScale();
            }

            return Vector3<real_Num>::unit();
        }

        void CActorProxy::setLocalScale( const Vector3<real_Num> &localScale )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalScale( localScale );
            }
        }

        Quaternion<real_Num> CActorProxy::getLocalOrientation() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalOrientation();
            }
            return Quaternion<real_Num>::identity();
        }

        void CActorProxy::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalOrientation( localOrientation );
            }
        }

        Vector3<real_Num> CActorProxy::getPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getPosition();
            }
            return Vector3<real_Num>::zero();
        }

        void CActorProxy::lookAt( const Vector3<real_Num> &position )
        {
            if( auto actor = getActorImpl() )
            {
                actor->lookAt( position );
            }
        }

        void CActorProxy::lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis )
        {
            if( auto actor = getActorImpl() )
            {
                actor->lookAt( position, yawAxis );
            }
        }

        void CActorProxy::setPosition( const Vector3<real_Num> &position )
        {
            if( auto transform = getTransform() )
            {
                transform->setPosition( position );
            }
        }

        Vector3<real_Num> CActorProxy::getScale() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getScale();
            }

            return Vector3<real_Num>::unit();
        }

        void CActorProxy::setScale( const Vector3<real_Num> &scale )
        {
            if( auto transform = getTransform() )
            {
                transform->setScale( scale );
            }
        }

        Quaternion<real_Num> CActorProxy::getOrientation() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void CActorProxy::setOrientation( const Quaternion<real_Num> &orientation )
        {
            if( auto transform = getTransform() )
            {
                transform->setOrientation( orientation );
            }
        }

        void CActorProxy::awake()
        {
            if( auto actor = getActorImpl() )
            {
                actor->awake();
            }
        }

        void CActorProxy::play()
        {
            if( auto actor = getActorImpl() )
            {
                actor->play();
            }
        }

        void CActorProxy::edit()
        {
            if( auto actor = getActorImpl() )
            {
                actor->edit();
            }
        }

        void CActorProxy::levelWasLoaded( SmartPtr<IScene> scene )
        {
            if( auto actor = getActorImpl() )
            {
                actor->levelWasLoaded( scene );
            }
        }

        void CActorProxy::reset()
        {
            if( auto actor = getActorImpl() )
            {
                actor->reset();
            }
        }

        void CActorProxy::hierarchyChanged()
        {
            if( auto actor = getActorImpl() )
            {
                actor->hierarchyChanged();
            }
        }

        void CActorProxy::childAdded( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childAdded( child );
            }
        }

        void CActorProxy::childRemoved( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childRemoved( child );
            }
        }

        void CActorProxy::childAddedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childAddedInHierarchy( child );
            }
        }

        void CActorProxy::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->childRemovedInHierarchy( child );
            }
        }

        String CActorProxy::getName() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getName();
            }
            return String();
        }

        void CActorProxy::setName( const String &name )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setName( name );
            }
        }

        bool CActorProxy::getPerpetual() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getPerpetual();
            }
            return false;
        }

        void CActorProxy::setPerpetual( bool perpetual )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setPerpetual( perpetual );
            }
        }

        void CActorProxy::addComponentInstance( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addComponentInstance( component );
            }
        }

        void CActorProxy::removeComponentInstance( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeComponentInstance( component );
            }
        }

        u32 CActorProxy::getEntity() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getEntity();
            }

            return 0;
        }

        void CActorProxy::setEntity( u32 entity )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setEntity( entity );
            }
        }

        Array<SmartPtr<IComponent>> CActorProxy::getComponents() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getComponents();
            }

            return Array<SmartPtr<IComponent>>();
        }

        SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> CActorProxy::getComponentsPtr() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getComponentsPtr();
            }
            return nullptr;
        }

        SmartPtr<IActor> CActorProxy::getChildByIndex( u32 index ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getChildByIndex( index );
            }
            return nullptr;
        }

        u32 CActorProxy::getNumChildren() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNumChildren();
            }
            return 0;
        }

        s32 CActorProxy::getSiblingIndex() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSiblingIndex();
            }
            return -1;
        }

        void CActorProxy::addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                             SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addDirtyComponent( taskId, updateType, component );
            }
        }

        bool CActorProxy::isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                            SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isComponentDirty( taskId, updateType, component );
            }

            return false;
        }

        void CActorProxy::registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                   SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->registerComponentUpdate( taskId, updateType, object );
            }
        }

        void CActorProxy::unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                     SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->unregisterComponentUpdate( taskId, updateType, object );
            }
        }

        void CActorProxy::unregisterAllComponent( SmartPtr<IComponent> object )
        {
            if( auto actor = getActorImpl() )
            {
                actor->unregisterAllComponent( object );
            }
        }

        void CActorProxy::triggerEnter( SmartPtr<IComponent> collision )
        {
            if( auto actor = getActorImpl() )
            {
                actor->triggerEnter( collision );
            }
        }

        void CActorProxy::triggerLeave( SmartPtr<IComponent> collision )
        {
            if( auto actor = getActorImpl() )
            {
                actor->triggerLeave( collision );
            }
        }

        void CActorProxy::componentLoaded( SmartPtr<IComponent> component )
        {
            if( auto actor = getActorImpl() )
            {
                actor->componentLoaded( component );
            }
        }

        void CActorProxy::setParent( SmartPtr<IActor> parent )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setParent( parent );
            }
        }

        SmartPtr<IActor> CActorProxy::getParent() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getParent();
            }

            return nullptr;
        }

        void CActorProxy::addChild( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->addChild( child );
            }
        }

        void CActorProxy::removeChild( SmartPtr<IActor> child )
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeChild( child );
            }
        }

        void CActorProxy::removeChildren()
        {
            if( auto actor = getActorImpl() )
            {
                actor->removeChildren();
            }
        }

        SmartPtr<IActor> CActorProxy::findChild( const String &name )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->findChild( name );
            }

            return nullptr;
        }

        SharedPtr<ConcurrentArray<SmartPtr<IActor>>> CActorProxy::getChildrenPtr() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getChildrenPtr();
            }

            return nullptr;
        }

        Array<SmartPtr<IActor>> CActorProxy::getAllChildren() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getAllChildren();
            }

            return Array<SmartPtr<IActor>>();
        }

        bool CActorProxy::isMine() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isMine();
            }

            return false;
        }

        void CActorProxy::setMine( bool mine )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setMine( mine );
            }
        }

        bool CActorProxy::isStatic() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isStatic();
            }

            return false;
        }

        void CActorProxy::setStatic( bool isstatic )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setStatic( isstatic );
            }
        }

        bool CActorProxy::isEnabledInScene() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isEnabledInScene();
            }

            return false;
        }

        bool CActorProxy::isEnabled() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isEnabled();
            }

            return false;
        }

        void CActorProxy::setEnabled( bool enabled )
        {
            if( auto actor = getActorImpl() )
            {
                return actor->setEnabled( enabled );
            }
        }

        bool CActorProxy::isVisible() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isVisible();
            }

            return false;
        }

        void CActorProxy::setVisible( bool visible, bool cacade /*= true */ )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setVisible( visible, cacade );
            }
        }

        bool CActorProxy::isDirty() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->isDirty();
            }

            return false;
        }

        void CActorProxy::setDirty( bool dirty )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setDirty( dirty );
            }
        }

        void CActorProxy::updateTransform()
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateTransform();
            }
        }

        SmartPtr<IActor> CActorProxy::getSceneRoot() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSceneRoot();
            }

            return nullptr;
        }

        u32 CActorProxy::getSceneLevel() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getSceneLevel();
            }

            return 0;
        }

        SmartPtr<ITransform> CActorProxy::getTransform() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getTransform();
            }

            return nullptr;
        }

        void CActorProxy::setTransform( SmartPtr<ITransform> transform )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setTransform( transform );
            }
        }

        void CActorProxy::setState( State state )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setState( state );
            }
        }

        IActor::State CActorProxy::getState() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getState();
            }

            return IActor::State::None;
        }

        void CActorProxy::setGameState( GameState state )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setGameState( state );
            }
        }

        IActor::GameState CActorProxy::getGameState() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getGameState();
            }

            return IActor::GameState::None;
        }

        u32 CActorProxy::getFlags() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getFlags();
            }

            return false;
        }

        void CActorProxy::setFlags( u32 flags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setFlags( flags );
            }
        }

        bool CActorProxy::getFlag( u32 flag ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getFlag( flag );
            }

            return false;
        }

        void CActorProxy::setFlag( u32 flag, bool value )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setFlag( flag, value );
            }
        }

        u32 CActorProxy::getNewFlags() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNewFlags();
            }

            return 0;
        }

        void CActorProxy::setNewFlags( u32 flags )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setNewFlags( flags );
            }
        }

        bool CActorProxy::getNewFlag( u32 flag ) const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getNewFlag( flag );
            }

            return false;
        }

        void CActorProxy::setNewFlag( u32 flag, bool value )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setNewFlag( flag, value );
            }
        }

        SmartPtr<IScene> CActorProxy::getScene() const
        {
            if( auto actor = getActorImpl() )
            {
                return actor->getScene();
            }

            return nullptr;
        }

        void CActorProxy::setScene( SmartPtr<IScene> scene )
        {
            if( auto actor = getActorImpl() )
            {
                actor->setScene( scene );
            }
        }

        void CActorProxy::updateVisibility()
        {
            if( auto actor = getActorImpl() )
            {
                actor->updateVisibility();
            }
        }
    }  // namespace scene
}  // namespace fb
