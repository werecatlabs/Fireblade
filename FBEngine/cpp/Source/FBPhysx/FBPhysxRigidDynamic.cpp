#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxBoxShape.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBPhysx/FBPhysxScene.h>
#include <FBPhysx/FBPhysxThread.h>
#include <extensions/PxRigidBodyExt.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, PhysxRigidDynamic, IRigidDynamic3 );

        PhysxRigidDynamic::PhysxRigidDynamic()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();

            auto stateListener = factoryManager->make_ptr<StateListener>();
            stateListener->setOwner( this );
            stateObject->addStateListener( stateListener );
            setStateListener( stateListener );

            auto state = factoryManager->make_ptr<RigidbodyState>();
            stateObject->setState( state );
            stateObject->setOwner( this );

            setStateObject( stateObject );

            auto physicsTask =
                applicationManager->hasTasks() ? Thread::Task::Physics : Thread::Task::Primary;
            state->setTaskId( physicsTask );
        }

        PhysxRigidDynamic::~PhysxRigidDynamic()
        {
            unload( nullptr );
        }

        void PhysxRigidDynamic::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto physxManager = fb::static_pointer_cast<PhysxManager>( physicsManager );
                FB_ASSERT( physxManager );

                auto physics = physxManager->getPhysics();
                FB_ASSERT( physics );

                ISharedObject::ScopedLock lock( physicsManager );

                auto transform = getTransform();
                auto t = PhysxUtil::toPx( transform );
                auto actor = physics->createRigidDynamic( t );
                actor->setActorFlag( physx::PxActorFlag::eVISUALIZATION, true );
                actor->userData = this;

                setActor( actor );
                setActorDynamic( actor );

                setSolverIterationCounts( 100, 100 );  // todo

                auto scene = fb::static_pointer_cast<PhysxScene>( getScene() );
                if( scene )
                {
                    auto pxScene = scene->getScene();
                    pxScene->addActor( *actor );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxRigidDynamic::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                ISharedObject::ScopedLock lock( physicsManager );

                if( auto stateObject = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                        stateListener->unload( nullptr );
                        setStateListener( nullptr );
                    }

                    stateManager->removeStateObject( stateObject );
                    stateObject->unload( nullptr );
                    setStateObject( nullptr );
                }

                for( auto shape : m_shapes )
                {
                    shape->unload( nullptr );
                }

                m_shapes.clear();

                if( auto actor = getActor() )
                {
                    auto numShapes = actor->getNbShapes();
                    if( numShapes > 0 )
                    {
                        Array<physx::PxShape *> shapes;
                        shapes.resize( numShapes );

                        actor->getShapes( &shapes[0], numShapes );

                        for( u32 i = 0; i < numShapes; ++i )
                        {
                            physx::PxShape *shape = shapes[i];
                            shape->userData = nullptr;

                            actor->detachShape( *shape, false );
                        }
                    }

                    auto pScene = actor->getScene();
                    if( pScene )
                    {
                        pScene->removeActor( *actor );
                    }

                    actor->release();
                    setActor( nullptr );
                }

                m_scene = nullptr;

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<IPhysicsShape3>> PhysxRigidDynamic::getShapes() const
        {
            return m_shapes;
        }

        void PhysxRigidDynamic::setTransform( const Transform3<real_Num> &transform )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<RigidbodyState>() )
                {
                    state->setTransform( transform );
                }
            }
        }

        Transform3<real_Num> PhysxRigidDynamic::getTransform() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<RigidbodyState>() )
                {
                    FB_ASSERT( state->getTransform().isSane() );
                    return state->getTransform();
                }
            }

            return Transform3<real_Num>();
        }

        void PhysxRigidDynamic::addVelocity( const Vector3<real_Num> &velocity,
                                             const Vector3<real_Num> &relPos /*= Vector3F( ) */ )
        {
        }

        void PhysxRigidDynamic::setVelocity( const Vector3<real_Num> &velocity )
        {
        }

        Vector3<real_Num> PhysxRigidDynamic::getVelocity() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto velocity = actor->getLinearVelocity();
                auto v = PhysxUtil::toFB( velocity );
                FB_ASSERT( v.isFinite() );

                FB_ASSERT( Math<real_Num>::Abs( v[0] ) < static_cast<real_Num>( 1e3 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[1] ) < static_cast<real_Num>( 1e3 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[2] ) < static_cast<real_Num>( 1e3 ) );

                return v;
            }

            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> PhysxRigidDynamic::getAngularVelocity() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto angularVelocity = actor->getAngularVelocity();
                auto v = PhysxUtil::toFB( angularVelocity );
                FB_ASSERT( v.isFinite() );

                FB_ASSERT( Math<real_Num>::Abs( v[0] ) < static_cast<real_Num>( 1e3 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[1] ) < static_cast<real_Num>( 1e3 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[2] ) < static_cast<real_Num>( 1e3 ) );

                return v;
            }

            return Vector3<real_Num>::zero();
        }

        void PhysxRigidDynamic::addForce( const Vector3<real_Num> &force )
        {
            if( auto actor = getActorDynamic() )
            {
                if( auto scene = actor->getScene() )
                {
                    FB_PXSCENE_WRITE_LOCK( scene );

                    auto f = PhysxUtil::toPx( force );
                    actor->addForce( f, physx::PxForceMode::eFORCE, true );
                }
            }
        }

        void PhysxRigidDynamic::setForce( const Vector3<real_Num> &force )
        {
        }

        Vector3<real_Num> PhysxRigidDynamic::getForce() const
        {
            return Vector3<real_Num>::ZERO;
        }

        void PhysxRigidDynamic::addTorque( const Vector3<real_Num> &torque )
        {
            if( auto actor = getActorDynamic() )
            {
                if( auto scene = actor->getScene() )
                {
                    FB_PXSCENE_WRITE_LOCK( scene );

                    auto t = physx::PxVec3( torque.X(), torque.Y(), torque.Z() );
                    actor->addTorque( t, physx::PxForceMode::eFORCE, true );
                }
            }
        }

        void PhysxRigidDynamic::setTorque( const Vector3<real_Num> &torque )
        {
        }

        Vector3<real_Num> PhysxRigidDynamic::getTorque() const
        {
            return Vector3<real_Num>::ZERO;
        }

        AABB3<real_Num> PhysxRigidDynamic::getLocalAABB() const
        {
            return m_localAABB;
        }

        AABB3<real_Num> PhysxRigidDynamic::getWorldAABB() const
        {
            return m_worldAABB;
        }

        real_Num PhysxRigidDynamic::getMass() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getMass();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setMass( real_Num mass )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto task = Thread::getCurrentTask();
            auto physicsTask = physicsManager->getPhysicsTask();

            const auto &loadingState = getLoadingState();

            if( loadingState == LoadingState::Loaded && task == physicsTask )
            {
                if( auto actor = getActorDynamic() )
                {
                    actor->setMass( mass );
                }
            }
            else
            {
                auto stateMessageFloat = factoryManager->make_ptr<StateMessageFloatValue>();
                stateMessageFloat->setType( STATE_MESSAGE_MASS );

                stateMessageFloat->setValue( mass );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( physicsTask, stateMessageFloat );
                }
            }
        }

        void PhysxRigidDynamic::setCollisionType( u32 type )
        {
            m_collisionType = type;
        }

        u32 PhysxRigidDynamic::getCollisionType() const
        {
            return m_collisionType;
        }

        void PhysxRigidDynamic::setCollisionMask( u32 mask )
        {
            m_collisionMask = mask;
        }

        u32 PhysxRigidDynamic::getCollisionMask() const
        {
            return m_collisionMask;
        }

        void PhysxRigidDynamic::setEnabled( bool enabled )
        {
            m_enabled = enabled;
        }

        bool PhysxRigidDynamic::isEnabled() const
        {
            return m_enabled;
        }

        void *PhysxRigidDynamic::getUserData() const
        {
            return m_userData;
        }

        void PhysxRigidDynamic::setUserData( void *userData )
        {
            m_userData = userData;
        }

        physx::PxRigidDynamic *PhysxRigidDynamic::getActorDynamic() const
        {
            auto p = m_actor.load();
            return (physx::PxRigidDynamic *)p.get();
        }

        void PhysxRigidDynamic::setActorDynamic( physx::PxRigidDynamic *actor )
        {
            m_actor = actor;

            if( actor )
            {
                actor->setActorFlag( physx::PxActorFlag::eVISUALIZATION, true );
                actor->userData = this;
            }
        }

        void PhysxRigidDynamic::setKinematicMode( bool val )
        {
            m_kinematicMode = val;
        }

        bool PhysxRigidDynamic::getKinematicMode() const
        {
            return m_kinematicMode;
        }

        void *PhysxRigidDynamic::getUserDataById( u32 id ) const
        {
            return nullptr;
        }

        void PhysxRigidDynamic::setUserDataById( u32 id, void *userData )
        {
        }

        void PhysxRigidDynamic::setRigidBodyFlag( PxRigidBodyFlag::Enum flag, bool value )
        {
            if( auto actor = getActorDynamic() )
            {
                auto iFlag = static_cast<s32>( flag );
                auto f = static_cast<physx::PxRigidBodyFlag::Enum>( iFlag );
                actor->setRigidBodyFlag( f, value );
            }
        }

        PxRigidBodyFlag::Enum PhysxRigidDynamic::getRigidBodyFlags() const
        {
            return static_cast<PxRigidBodyFlag::Enum>( 0 );
        }

        void PhysxRigidDynamic::setLinearVelocity( const Vector3<real_Num> &linVel,
                                                   bool autowake /*= true*/ )
        {
            if( auto actor = getActorDynamic() )
            {
                auto v = PhysxUtil::toPx( linVel );
                actor->setLinearVelocity( v );
            }
        }

        Vector3<real_Num> PhysxRigidDynamic::getLinearVelocity() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto linearVelocity = actor->getLinearVelocity();
                auto v = PhysxUtil::toFB( linearVelocity );
                FB_ASSERT( v.isFinite() );

                FB_ASSERT( Math<real_Num>::Abs( v[0] ) < static_cast<real_Num>( 1e8 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[1] ) < static_cast<real_Num>( 1e8 ) );
                FB_ASSERT( Math<real_Num>::Abs( v[2] ) < static_cast<real_Num>( 1e8 ) );

                return v;
            }

            return Vector3<real_Num>::zero();
        }

        void PhysxRigidDynamic::setAngularVelocity( const Vector3<real_Num> &angVel,
                                                    bool autowake /*= true*/ )
        {
            if( auto actor = getActorDynamic() )
            {
                auto v = PhysxUtil::toPx( angVel );
                actor->setAngularVelocity( v, autowake );
            }
        }

        void PhysxRigidDynamic::clearForce(
            PhysicsTypes::ForceMode mode /*= PhysicsTypes::ForceMode::Force*/ )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->clearForce();
            }
        }

        void PhysxRigidDynamic::clearTorque(
            PhysicsTypes::ForceMode mode /*= PhysicsTypes::ForceMode::Force*/ )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->clearTorque();
            }
        }

        void PhysxRigidDynamic::setCMassLocalPose( const Transform3<real_Num> &pose )
        {
            if( auto actor = getActorDynamic() )
            {
                auto t = PhysxUtil::toPx( pose );
                actor->setCMassLocalPose( t );
            }
        }

        Transform3<real_Num> PhysxRigidDynamic::getCMassLocalPose() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto p = actor->getCMassLocalPose();
                return PhysxUtil::toFB( p );
            }

            return Transform3<real_Num>();
        }

        void PhysxRigidDynamic::setMassSpaceInertiaTensor( const Vector3<real_Num> &m )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto task = Thread::getCurrentTask();
            auto physicsTask = physicsManager->getPhysicsTask();

            const auto &loadingState = getLoadingState();

            if( loadingState == LoadingState::Loaded && task == physicsTask )
            {
                if( auto actor = getActorDynamic() )
                {
                    auto pxTensor = PhysxUtil::toPx( m );
                    actor->setMassSpaceInertiaTensor( pxTensor );
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageVector3>();
                message->setType( STATE_MESSAGE_INERTIA_TENSOR );
                message->setValue( m );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( physicsTask, message );
                }
            }
        }

        Vector3<real_Num> PhysxRigidDynamic::getMassSpaceInertiaTensor() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto tensor = actor->getMassSpaceInertiaTensor();
                return Vector3<real_Num>( tensor.x, tensor.y, tensor.z );
            }

            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> PhysxRigidDynamic::getMassSpaceInvInertiaTensor() const
        {
            if( auto actor = getActorDynamic() )
            {
                auto tensor = actor->getMassSpaceInvInertiaTensor();
                return Vector3<real_Num>( tensor.x, tensor.y, tensor.z );
            }

            return Vector3<real_Num>::zero();
        }

        void PhysxRigidDynamic::setActorFlag( PxActorFlag::Enum flag, bool value )
        {
            if( auto actor = getActor() )
            {
                auto iFlag = static_cast<int>( flag );
                actor->setActorFlag( static_cast<physx::PxActorFlag::Enum>( iFlag ), value );
            }
        }

        PxActorFlag::Enum PhysxRigidDynamic::getActorFlags() const
        {
            if( auto actor = getActor() )
            {
                auto flags = actor->getActorFlags();
                auto iFlags = flags.operator physx::PxU32();
                return static_cast<PxActorFlag::Enum>( iFlags );
            }

            return static_cast<PxActorFlag::Enum>( 0 );
        }

        void PhysxRigidDynamic::setKinematicTarget( const Transform3<real_Num> &destination )
        {
            if( auto actor = getActorDynamic() )
            {
                auto t = PhysxUtil::toPx( destination );
                actor->setKinematicTarget( t );
            }
        }

        bool PhysxRigidDynamic::getKinematicTarget( Transform3<real_Num> &target )
        {
            if( auto actor = getActorDynamic() )
            {
                auto t = physx::PxTransform();
                if( actor->getKinematicTarget( t ) )
                {
                    target = PhysxUtil::toFB( t );
                    return true;
                }
            }

            return false;
        }

        bool PhysxRigidDynamic::isKinematic() const
        {
            if( auto actor = getActorDynamic() )
            {
                return ( actor->getRigidDynamicFlags() & physx::PxRigidDynamicFlag::eKINEMATIC );
            }

            return false;
        }

        void PhysxRigidDynamic::setKinematic( bool kinematic )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setRigidDynamicFlag( physx::PxRigidDynamicFlag::eKINEMATIC, kinematic );
            }
        }

        void PhysxRigidDynamic::setLinearDamping( real_Num damping )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setLinearDamping( damping );
            }
        }

        real_Num PhysxRigidDynamic::getLinearDamping() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getLinearDamping();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setAngularDamping( real_Num damping )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setAngularDamping( damping );
            }
        }

        real_Num PhysxRigidDynamic::getAngularDamping() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getAngularDamping();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setMaxAngularVelocity( real_Num maxAngVel )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setMaxAngularVelocity( maxAngVel );
            }
        }

        real_Num PhysxRigidDynamic::getMaxAngularVelocity() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getMaxAngularVelocity();
            }

            return static_cast<real_Num>( 0.0 );
        }

        bool PhysxRigidDynamic::isSleeping() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->isSleeping();
            }

            return false;
        }

        void PhysxRigidDynamic::setSleepThreshold( real_Num threshold )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setSleepThreshold( threshold );
            }
        }

        real_Num PhysxRigidDynamic::getSleepThreshold() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getSleepThreshold();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setStabilizationThreshold( real_Num threshold )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setStabilizationThreshold( threshold );
            }
        }

        real_Num PhysxRigidDynamic::getStabilizationThreshold() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getStabilizationThreshold();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setWakeCounter( real_Num wakeCounterValue )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setWakeCounter( wakeCounterValue );
            }
        }

        real_Num PhysxRigidDynamic::getWakeCounter() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getWakeCounter();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::wakeUp()
        {
            if( auto actor = getActorDynamic() )
            {
                actor->wakeUp();
            }
        }

        void PhysxRigidDynamic::putToSleep()
        {
            if( auto actor = getActorDynamic() )
            {
                actor->putToSleep();
            }
        }

        void PhysxRigidDynamic::setSolverIterationCounts( u32 minPositionIters,
                                                          u32 minVelocityIters /*= 1*/ )
        {
            FB_ASSERT( minPositionIters > 0 );
            FB_ASSERT( minVelocityIters > 0 );

            if( auto actor = getActorDynamic() )
            {
                actor->setSolverIterationCounts( minPositionIters, minVelocityIters );
            }
        }

        void PhysxRigidDynamic::getSolverIterationCounts( u32 &minPositionIters,
                                                          u32 &minVelocityIters ) const
        {
            if( auto actor = getActorDynamic() )
            {
                actor->getSolverIterationCounts( minPositionIters, minVelocityIters );
            }
        }

        real_Num PhysxRigidDynamic::getContactReportThreshold() const
        {
            if( auto actor = getActorDynamic() )
            {
                return actor->getContactReportThreshold();
            }

            return static_cast<real_Num>( 0.0 );
        }

        void PhysxRigidDynamic::setContactReportThreshold( real_Num threshold )
        {
            if( auto actor = getActorDynamic() )
            {
                actor->setContactReportThreshold( threshold );
            }
        }

        u32 PhysxRigidDynamic::getNumShapes() const
        {
            if( auto actor = getActor() )
            {
                return actor->getNbShapes();
            }

            return 0;
        }

        SmartPtr<IPhysicsBody3> PhysxRigidDynamic::clone()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto physxManager = fb::static_pointer_cast<PhysxManager>( physicsManager );
            FB_ASSERT( physxManager );

            if( auto actor = getActorDynamic() )
            {
                auto body = fb::make_ptr<PhysxRigidDynamic>();
                auto clonedActor =
                    PxCloneDynamic( *physxManager->getPhysics(), actor->getGlobalPose(), *actor );

                body->setActor( clonedActor );

                return body;
            }

            return nullptr;
        }

        void PhysxRigidDynamic::setActiveTransform( const Transform3<real_Num> &transform )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<RigidbodyState>() )
                {
                    state->updateTransform( transform );
                }
            }

            //if( auto p = getListenersPtr() )
            //{
            //    auto &listeners = *p;
            //    for( auto &listener : listeners )
            //    {
            //        if( listener )
            //        {
            //            listener->handleTransform( transform );
            //        }
            //    }
            //}
        }

        Array<SmartPtr<ISharedObject>> PhysxRigidDynamic::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;
            objects.reserve( 10 );

            objects.push_back( getStateObject() );
            objects.push_back( getStateListener() );

            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getState() )
                {
                    objects.push_back( state );
                }
            }

            return objects;
        }

        SmartPtr<Properties> PhysxRigidDynamic::getProperties() const
        {
            try
            {
                auto properties = fb::make_ptr<Properties>();

                auto transform = getTransform();

                auto position = transform.getPosition();
                properties->setProperty( "position", position );

                auto rotation = transform.getRotation();
                properties->setProperty( "rotation", rotation );

                auto velocity = getVelocity();
                properties->setProperty( "velocity", velocity );

                auto mass = getMass();
                properties->setProperty( "mass", mass );

                auto kinematic = isKinematic();
                properties->setProperty( "kinematic", kinematic );

                auto massSpaceInertiaTensor = getMassSpaceInertiaTensor();
                properties->setProperty( "massSpaceInertiaTensor", massSpaceInertiaTensor );

                return properties;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void PhysxRigidDynamic::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                auto transform = getTransform();

                auto position = transform.getPosition();
                properties->getPropertyValue( "position", position );

                auto rotation = transform.getRotation();
                properties->getPropertyValue( "rotation", rotation );

                auto velocity = getVelocity();
                properties->getPropertyValue( "velocity", velocity );

                auto mass = static_cast<real_Num>( 1000.0 );
                properties->getPropertyValue( "mass", mass );
                setMass( mass );

                auto kinematic = isKinematic();
                properties->getPropertyValue( "kinematic", kinematic );
                setKinematic( kinematic );

                auto massSpaceInertiaTensor = getMassSpaceInertiaTensor();
                properties->getPropertyValue( "massSpaceInertiaTensor", massSpaceInertiaTensor );
                setMassSpaceInertiaTensor( massSpaceInertiaTensor );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IStateContext> PhysxRigidDynamic::getStateObject() const
        {
            return m_stateObject;
        }

        void PhysxRigidDynamic::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void PhysxRigidDynamic::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            FB_ASSERT( m_owner );

            auto actor = m_owner->getActor();
            if( actor )
            {
                auto rigidbodyState = fb::static_pointer_cast<RigidbodyState>( state );
                if( rigidbodyState )
                {
                    auto transform = rigidbodyState->getTransform();

                    FB_ASSERT( transform.isValid() );
                    FB_ASSERT( transform.isSane() );

                    auto t = PhysxUtil::toPx( transform );
                    FB_ASSERT( t.isFinite() );
                    FB_ASSERT( t.isSane() );

                    if( t.isSane() )
                    {
                        FB_ASSERT( actor );

                        if( actor )
                        {
                            actor->setGlobalPose( t );
                            FB_ASSERT( actor->getGlobalPose().isSane() );
                        }
                    }

                    state->setDirty( false );

                    // auto mass = rigidbodyState->getMass();
                    // if (!Math<real_Num>::equals(mass, actor->getMass()))
                    //{
                    //	actor->setMass(mass);
                    // }
                }
            }
        }

        void PhysxRigidDynamic::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
#if _DEBUG
            auto applicationManager = core::IApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();
            auto task = Thread::getCurrentTask();
            auto physicsTask = physicsManager->getPhysicsTask();
            FB_ASSERT( task == physicsTask );
#endif

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
            else if( message->isExactly<StateMessageFloatValue>() )
            {
                auto stateMessageFloat = fb::static_pointer_cast<StateMessageFloatValue>( message );
                auto messageType = stateMessageFloat->getType();
                if( messageType == STATE_MESSAGE_MASS )
                {
                    m_owner->setMass( stateMessageFloat->getValue() );
                }
            }
            else if( message->isExactly<StateMessageVector3>() )
            {
                auto stateMessageVector = fb::static_pointer_cast<StateMessageVector3>( message );
                auto messageType = stateMessageVector->getType();

                if( messageType == STATE_MESSAGE_INERTIA_TENSOR )
                {
                    m_owner->setMassSpaceInertiaTensor( stateMessageVector->getValue() );
                }
            }
        }

        void PhysxRigidDynamic::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        PhysxRigidDynamic *PhysxRigidDynamic::StateListener::getOwner() const
        {
            return m_owner;
        }

        void PhysxRigidDynamic::StateListener::setOwner( PhysxRigidDynamic *owner )
        {
            m_owner = owner;
        }
    }  // end namespace physics
}  // end namespace fb
