#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBPhysx/FBPhysxScene.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb::physics
{
    FB_CLASS_REGISTER_DERIVED( fb, PhysxRigidStatic, IRigidStatic3 );

    PhysxRigidStatic::PhysxRigidStatic() 
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        setStateContext( stateContext );

        auto stateListener = factoryManager->make_ptr<StateListener>();
        setStateListener( stateListener );

        stateListener->setOwner( this );
        stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<RigidbodyState>();
        stateContext->addState( state );
        stateContext->setOwner( this );
    }

    PhysxRigidStatic::~PhysxRigidStatic()
    {
        unload( nullptr );
    }

    void PhysxRigidStatic::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );

            if( auto physics = physicsManager->getPhysics() )
            {
                auto t = PhysxUtil::toPx( getTransform() );
                auto actor = physics->createRigidStatic( t );

                setActor( actor );
                setRigidStatic( actor );
            }

            auto scene = fb::static_pointer_cast<PhysxScene>( getScene() );
            if( scene )
            {
                auto pxScene = scene->getScene();
                pxScene->addActor( *getRigidStatic() );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxRigidStatic::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            for( auto shape : m_shapes )
            {
                shape->unload( nullptr );
            }

            m_shapes.clear();

            if( m_rigidStatic )
            {
                auto numShapes = m_rigidStatic->getNbShapes();
                if( numShapes > 0 )
                {
                    Array<physx::PxShape *> shapes;
                    shapes.resize( numShapes );

                    m_rigidStatic->getShapes( &shapes[0], numShapes );

                    for( u32 i = 0; i < numShapes; ++i )
                    {
                        physx::PxShape *shape = shapes[i];
                        shape->userData = nullptr;

                        m_rigidStatic->detachShape( *shape, false );
                    }
                }

                auto pScene = m_rigidStatic->getScene();
                if( pScene )
                {
                    pScene->removeActor( *m_rigidStatic );
                }

                m_rigidStatic->release();
                m_rigidStatic = nullptr;
            }

            m_scene = nullptr;

            setLoadingState( LoadingState::Unloaded );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PhysxRigidStatic::getRigidStatic() const -> physx::PxRigidStatic *
    {
        return m_rigidStatic;
    }

    void PhysxRigidStatic::setRigidStatic( physx::PxRigidStatic *rigidStatic )
    {
        m_rigidStatic = rigidStatic;
    }

    auto PhysxRigidStatic::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;
        return objects;
    }

    auto PhysxRigidStatic::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();

        auto mass = getMass();
        properties->setProperty( "mass", mass );

        return properties;
    }

    void PhysxRigidStatic::setProperties( SmartPtr<Properties> properties )
    {
        auto mass = static_cast<real_Num>( 1000.0 );
        properties->getPropertyValue( "mass", mass );
        setMass( mass );
    }

    void PhysxRigidStatic::setTransform( const Transform3<real_Num> &transform )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<RigidbodyState>() )
            {
                state->setTransform( transform );
            }
        }
    }

    auto PhysxRigidStatic::getTransform() const -> Transform3<real_Num>
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<RigidbodyState>() )
            {
                return state->getTransform();
            }
        }

        return {};
    }

    auto PhysxRigidStatic::getMass() const -> real_Num
    {
        return static_cast<real_Num>( 0.0 );
    }

    void PhysxRigidStatic::setMass( real_Num mass )
    {
    }

    void PhysxRigidStatic::setCollisionType( u32 type )
    {
        m_collisionType = type;
    }

    auto PhysxRigidStatic::getCollisionType() const -> u32
    {
        return m_collisionType;
    }

    void PhysxRigidStatic::setCollisionMask( u32 mask )
    {
        m_collisionMask = mask;
    }

    auto PhysxRigidStatic::getCollisionMask() const -> u32
    {
        return m_collisionMask;
    }

    void PhysxRigidStatic::setEnabled( bool enabled )
    {
    }

    auto PhysxRigidStatic::isEnabled() const -> bool
    {
        return false;
    }

    auto PhysxRigidStatic::getUserData() const -> void *
    {
        return m_userData;
    }

    void PhysxRigidStatic::setUserData( void *userData )
    {
        m_userData = userData;
    }

    auto PhysxRigidStatic::getKinematicMode() const -> bool
    {
        return m_kinematicMode;
    }

    void PhysxRigidStatic::setKinematicMode( bool val )
    {
        m_kinematicMode = val;
    }

    auto PhysxRigidStatic::getUserDataById( u32 id ) const -> void *
    {
        return nullptr;
    }

    void PhysxRigidStatic::setUserDataById( u32 id, void *userData )
    {
    }

    void PhysxRigidStatic::setScene( SmartPtr<IPhysicsScene3> scene )
    {
        m_scene = scene;
    }

    auto PhysxRigidStatic::getScene() const -> SmartPtr<IPhysicsScene3>
    {
        return m_scene;
    }

    void PhysxRigidStatic::setActorFlag( ActorFlag::Enum flag, bool value )
    {
        m_rigidStatic->setActorFlag( static_cast<physx::PxActorFlag::Enum>( flag ), value );
    }

    auto PhysxRigidStatic::getActorFlags() const -> ActorFlag::Enum
    {
        auto actorFlags = m_rigidStatic->getActorFlags();
        auto iActorFlags = static_cast<u32>( actorFlags );
        return static_cast<ActorFlag::Enum>( iActorFlags );
    }

    void PhysxRigidStatic::setRigidBodyFlag( [[maybe_unused]] RigidBodyFlag::Enum flag,
                                             [[maybe_unused]] bool value )
    {
    }

    auto PhysxRigidStatic::getRigidBodyFlags() const -> RigidBodyFlag::Enum
    {
        return static_cast<RigidBodyFlag::Enum>( 0 );
    }

    auto PhysxRigidStatic::getShapes() const -> Array<SmartPtr<IPhysicsShape3>>
    {
        return m_shapes;
    }

    auto PhysxRigidStatic::getNumShapes() const -> u32
    {
        return static_cast<u32>( m_shapes.size() );
    }

    void PhysxRigidStatic::setLinearVelocity( const Vector3<real_Num> &linVel, bool autowake /*= true*/ )
    {
    }

    auto PhysxRigidStatic::getLinearVelocity() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    void PhysxRigidStatic::setAngularVelocity( const Vector3<real_Num> &angVel,
                                               bool autowake /*= true*/ )
    {
    }

    auto PhysxRigidStatic::getAngularVelocity() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    void PhysxRigidStatic::addForce( const Vector3<real_Num> &force )
    {
    }

    void PhysxRigidStatic::clearForce(
        PhysicsTypes::ForceMode mode /*= PhysicsTypes::ForceMode::Force*/ )
    {
    }

    void PhysxRigidStatic::addTorque( const Vector3<real_Num> &torque )
    {
    }

    void PhysxRigidStatic::clearTorque(
        PhysicsTypes::ForceMode mode /*= PhysicsTypes::ForceMode::Force*/ )
    {
    }

    auto PhysxRigidStatic::getLocalAABB() const -> AABB3<real_Num>
    {
        return m_localAABB;
    }

    auto PhysxRigidStatic::getWorldAABB() const -> AABB3<real_Num>
    {
        return m_worldAABB;
    }

    void PhysxRigidStatic::setCMassLocalPose( const Transform3<real_Num> &pose )
    {
    }

    auto PhysxRigidStatic::getCMassLocalPose() const -> Transform3<real_Num>
    {
        return {};
    }

    void PhysxRigidStatic::setMassSpaceInertiaTensor( const Vector3<real_Num> &m )
    {
    }

    auto PhysxRigidStatic::getMassSpaceInertiaTensor() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto PhysxRigidStatic::getMassSpaceInvInertiaTensor() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto PhysxRigidStatic::clone() -> SmartPtr<IPhysicsBody3>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsManager = applicationManager->getPhysicsManager();
        FB_ASSERT( physicsManager );

        auto physxManager = fb::static_pointer_cast<PhysxManager>( physicsManager );
        FB_ASSERT( physxManager );

        auto body = fb::make_ptr<PhysxRigidStatic>();
        auto clonedActor =
            PxCloneStatic( *physxManager->getPhysics(), m_rigidStatic->getGlobalPose(), *m_rigidStatic );

        body->setRigidStatic( clonedActor );

        return body;
    }

    void PhysxRigidStatic::StateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsManager = applicationManager->getPhysicsManager();
        FB_ASSERT( physicsManager );

        ISharedObject::ScopedLock lock( physicsManager );

        if( auto owner = getOwner() )
        {
            auto rigidState = fb::static_pointer_cast<RigidbodyState>( state );

            auto actor = owner->getRigidStatic();

            if( actor )
            {
                auto transform = rigidState->getTransform();
                auto t = PhysxUtil::toPx( transform );
                auto pose = actor->getGlobalPose();

                if( !PhysxUtil::equals(t, pose) )
                {
                    auto scene = actor->getScene();
                    if( scene )
                    {
                        scene->removeActor( *actor );
                    }

                    FB_ASSERT( t.isSane() );

                    if( t.isSane() )
                    {
                        if( actor )
                        {
                            actor->setGlobalPose( t );
                            FB_ASSERT( actor->getGlobalPose().isSane() );
                        }
                    }

                    if( scene )
                    {
                        if( actor->getNbShapes() > 0 )
                        {
                            scene->addActor( *actor );
                        }
                    }
                }

                if( actor->getNbShapes() == 0 )
                {
                    auto scene = actor->getScene();
                    if( scene )
                    {
                        scene->removeActor( *actor );
                    }
                }

                state->setDirty( false );
            }
        }
    }

    void PhysxRigidStatic::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsManager = applicationManager->getPhysicsManager();
        FB_ASSERT( physicsManager );

        ISharedObject::ScopedLock lock( physicsManager );

        if( auto owner = getOwner() )
        {
            if( message->isExactly<StateMessageTransform3>() )
            {
                auto stateMessageTransform = fb::static_pointer_cast<StateMessageTransform3>( message );
                if( stateMessageTransform )
                {
                    auto t = stateMessageTransform->getTransform();
                    owner->setTransform( t );
                }
            }
            else if( message->isExactly<StateMessageObject>() )
            {
                FB_ASSERT( fb::dynamic_pointer_cast<StateMessageObject>( message ) );
                auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                FB_ASSERT( objectMessage );

                auto messageType = objectMessage->getType();
                auto object = objectMessage->getObject();

                if( messageType == STATE_MESSAGE_ATTACH_SHAPE )
                {
                    owner->addShape( object );
                }
                else if( messageType == STATE_MESSAGE_DETACH_SHAPE )
                {
                    owner->removeShape( object );
                }
            }
        }
    }

    void PhysxRigidStatic::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto PhysxRigidStatic::StateListener::getOwner() const -> SmartPtr<PhysxRigidStatic>
    {
        return m_owner;
    }

    void PhysxRigidStatic::StateListener::setOwner( SmartPtr<PhysxRigidStatic> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::physics
