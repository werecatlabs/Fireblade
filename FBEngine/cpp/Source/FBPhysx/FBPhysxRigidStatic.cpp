#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBPhysx/FBPhysxScene.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, PhysxRigidStatic, IRigidStatic3 );

        PhysxRigidStatic::PhysxRigidStatic() : m_kinematicMode( false )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            setStateObject(stateObject);

            auto stateListener = factoryManager->make_ptr<StateListener>();
            setStateListener(stateListener);

            stateListener->setOwner( this );
            stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<RigidbodyState>();
            stateObject->setState( state );
            stateObject->setOwner( this );
        }

        PhysxRigidStatic::~PhysxRigidStatic()
        {
            unload( nullptr );
        }

        void PhysxRigidStatic::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
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

        physx::PxRigidStatic *PhysxRigidStatic::getRigidStatic() const
        {
            return m_rigidStatic;
        }

        void PhysxRigidStatic::setRigidStatic( physx::PxRigidStatic *rigidStatic )
        {
            m_rigidStatic = rigidStatic;
        }

        Array<SmartPtr<ISharedObject>> PhysxRigidStatic::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;
            return objects;
        }

        SmartPtr<Properties> PhysxRigidStatic::getProperties() const
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
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<RigidbodyState>() )
                {
                    state->setTransform( transform );
                }
            }
        }

        Transform3<real_Num> PhysxRigidStatic::getTransform() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<RigidbodyState>() )
                {
                    return state->getTransform();
                }
            }

            return Transform3<real_Num>();
        }

        real_Num PhysxRigidStatic::getMass() const
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

        u32 PhysxRigidStatic::getCollisionType() const
        {
            return m_collisionType;
        }

        void PhysxRigidStatic::setCollisionMask( u32 mask )
        {
            m_collisionMask = mask;
        }

        u32 PhysxRigidStatic::getCollisionMask() const
        {
            return m_collisionMask;
        }

        void PhysxRigidStatic::setEnabled( bool enabled )
        {
        }

        bool PhysxRigidStatic::isEnabled() const
        {
            return false;
        }

        void *PhysxRigidStatic::getUserData() const
        {
            return m_userData;
        }

        void PhysxRigidStatic::setUserData( void *userData )
        {
            m_userData = userData;
        }

        bool PhysxRigidStatic::getKinematicMode() const
        {
            return m_kinematicMode;
        }

        void PhysxRigidStatic::setKinematicMode( bool val )
        {
            m_kinematicMode = val;
        }

        void *PhysxRigidStatic::getUserDataById( u32 id ) const
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

        SmartPtr<IPhysicsScene3> PhysxRigidStatic::getScene() const
        {
            return m_scene;
        }

        void PhysxRigidStatic::setActorFlag( PxActorFlag::Enum flag, bool value )
        {
            m_rigidStatic->setActorFlag( static_cast<physx::PxActorFlag::Enum>( flag ), value );
        }

        PxActorFlag::Enum PhysxRigidStatic::getActorFlags() const
        {
            auto actorFlags = m_rigidStatic->getActorFlags();
            auto iActorFlags = static_cast<u32>( actorFlags );
            return static_cast<PxActorFlag::Enum>( iActorFlags );
        }

        void PhysxRigidStatic::setRigidBodyFlag( [[maybe_unused]] PxRigidBodyFlag::Enum flag,
                                                 [[maybe_unused]] bool value )
        {
        }

        PxRigidBodyFlag::Enum PhysxRigidStatic::getRigidBodyFlags() const
        {
            return static_cast<PxRigidBodyFlag::Enum>( 0 );
        }

        Array<SmartPtr<IPhysicsShape3>> PhysxRigidStatic::getShapes() const
        {
            return m_shapes;
        }

        u32 PhysxRigidStatic::getNumShapes() const
        {
            return static_cast<u32>( m_shapes.size() );
        }

        void PhysxRigidStatic::setLinearVelocity( const Vector3<real_Num> &linVel,
                                                  bool autowake /*= true*/ )
        {
        }

        Vector3<real_Num> PhysxRigidStatic::getLinearVelocity() const
        {
            return Vector3<real_Num>::zero();
        }

        void PhysxRigidStatic::setAngularVelocity( const Vector3<real_Num> &angVel,
                                                   bool autowake /*= true*/ )
        {
        }

        Vector3<real_Num> PhysxRigidStatic::getAngularVelocity() const
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

        AABB3<real_Num> PhysxRigidStatic::getLocalAABB() const
        {
            return m_localAABB;
        }

        AABB3<real_Num> PhysxRigidStatic::getWorldAABB() const
        {
            return m_worldAABB;
        }

        void PhysxRigidStatic::setCMassLocalPose( const Transform3<real_Num> &pose )
        {
        }

        Transform3<real_Num> PhysxRigidStatic::getCMassLocalPose() const
        {
            return Transform3<real_Num>();
        }

        void PhysxRigidStatic::setMassSpaceInertiaTensor( const Vector3<real_Num> &m )
        {
        }

        Vector3<real_Num> PhysxRigidStatic::getMassSpaceInertiaTensor() const
        {
            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> PhysxRigidStatic::getMassSpaceInvInertiaTensor() const
        {
            return Vector3<real_Num>::zero();
        }

        SmartPtr<IPhysicsBody3> PhysxRigidStatic::clone()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto physxManager = fb::static_pointer_cast<PhysxManager>( physicsManager );
            FB_ASSERT( physxManager );

            auto body = fb::make_ptr<PhysxRigidStatic>();
            auto clonedActor = PxCloneStatic( *physxManager->getPhysics(),
                                              m_rigidStatic->getGlobalPose(), *m_rigidStatic );

            body->setRigidStatic( clonedActor );

            return body;
        }

        void PhysxRigidStatic::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                auto rigidState = fb::static_pointer_cast<RigidbodyState>( state );

                auto actor = owner->getRigidStatic();
                
                if( actor )
                {
                    auto transform = rigidState->getTransform();
                    auto t = PhysxUtil::toPx( transform );
                    auto pose = actor->getGlobalPose();

                    if( !( t.p == pose.p && t.q == pose.q ) )
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

        void PhysxRigidStatic::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessageTransform3>() )
            {
                auto stateMessageTransform = fb::static_pointer_cast<StateMessageTransform3>( message );
                if( stateMessageTransform )
                {
                    auto t = stateMessageTransform->getTransform();
                    m_owner->setTransform( t );
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
                    m_owner->addShape( object );
                }
                else if( messageType == STATE_MESSAGE_DETACH_SHAPE )
                {
                    m_owner->removeShape( object );
                }
            }
        }

        void PhysxRigidStatic::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        PhysxRigidStatic *PhysxRigidStatic::StateListener::getOwner() const
        {
            return m_owner;
        }

        void PhysxRigidStatic::StateListener::setOwner( PhysxRigidStatic *owner )
        {
            m_owner = owner;
        }
    }  // end namespace physics
}  // end namespace fb
