#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Scene/Components/RigidbodyListener.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/Collision.h>
#include <FBCore/Scene/Transform.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/Physics/IRigidStatic3.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {

        FB_CLASS_REGISTER_DERIVED( fb::scene, Rigidbody, Component );

        Rigidbody::Rigidbody()
        {
        }

        Rigidbody::~Rigidbody()
        {
            unload( nullptr );
        }

        void Rigidbody::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Component::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                /*
                if( auto actor = getActor() )
                {
                    auto entity = (entt::registry::entity_type)actor->getEntity();

                    auto registry = sceneManager->getRegistry();
                    auto &component = registry->emplace<rigidbody_data>( entity );
                    component.owner = this;

                    setDataPtr( &component );
                }
                */

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Rigidbody::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                if( physicsManager )
                {
                    if( m_rigidDynamic )
                    {
                        physicsManager->removePhysicsBody( m_rigidDynamic );
                        m_rigidDynamic = nullptr;
                    }

                    if( m_rigidStatic )
                    {
                        physicsManager->removePhysicsBody( m_rigidStatic );
                        m_rigidStatic = nullptr;
                    }

                    if( m_material )
                    {
                        physicsManager->removeMaterial( m_material );
                        m_material = nullptr;
                    }
                }

                m_scene = nullptr;

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                /*
                if( auto actor = getActor() )
                {
                    auto entity = (entt::registry::entity_type)actor->getEntity();

                    auto registry = sceneManager->getRegistry();
                    registry->remove<rigidbody_data>( entity );
                }
                */

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Rigidbody::createRigidbodyObject()
        {
            auto actor = getActor();
            FB_ASSERT( actor );

            if( actor->isStatic() )
            {
                if( m_rigidStatic )
                {
                    return;
                }
            }
            else
            {
                if( m_rigidDynamic )
                {
                    return;
                }
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factory = applicationManager->getFactoryManager();
            FB_ASSERT( factory );

            auto physicsMgr = applicationManager->getPhysicsManager();
            if( physicsMgr )
            {
                SmartPtr<physics::IPhysicsBody3> body;
                Transform3<real_Num> transform = actor->getWorldTransform();

                if( actor->isStatic() )
                {
                    body = physicsMgr->addRigidStatic( transform );

                    auto listener = fb::make_ptr<RigidbodyListener>();
                    listener->setOwner( this );
                    body->addObjectListener( listener );

                    setRigidStatic( body );
                }
                else
                {
                    auto rigidDynamic = physicsMgr->addRigidDynamic( transform );

                    auto listener = fb::make_ptr<RigidbodyListener>();
                    listener->setOwner( this );
                    rigidDynamic->addObjectListener( listener );

                    if( rigidDynamic )
                    {
                        rigidDynamic->setMassSpaceInertiaTensor( m_massSpaceInertiaTensor );
                    }

                    body = rigidDynamic;
                    setRigidDynamic( body );
                }

                body->setMass( m_mass );

                attachShape();

                auto physicsScene = applicationManager->getPhysicsScene();
                //FB_ASSERT( physicsScene );

                if( physicsScene )
                {
                    physicsScene->addActor( body );
                }
            }
        }

        void Rigidbody::destroyRigidbodyObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();

            auto physicsScene = applicationManager->getPhysicsScene();
            if( physicsScene )
            {
                if( m_rigidDynamic )
                {
                    physicsScene->removeActor( m_rigidDynamic );
                }

                if( m_rigidStatic )
                {
                    physicsScene->removeActor( m_rigidStatic );
                }
            }

            if( physicsManager )
            {
                if( m_rigidDynamic )
                {
                    physicsManager->removePhysicsBody( m_rigidDynamic );
                    m_rigidDynamic = nullptr;
                }

                if( m_rigidStatic )
                {
                    physicsManager->removePhysicsBody( m_rigidStatic );
                    m_rigidStatic = nullptr;
                }

                if( m_material )
                {
                    physicsManager->removeMaterial( m_material );
                    m_material = nullptr;
                }
            }
        }

        void Rigidbody::attachShape()
        {
            try
            {
                auto actor = getActor();
                if( actor )
                {
                    auto collision = actor->getComponent<Collision>();
                    FB_ASSERT( collision );  // no collision

                    if( collision )
                    {
                        auto shape = collision->getShape();
                        FB_ASSERT( shape );

                        if( shape )
                        {
                            if( m_rigidDynamic )
                            {
                                m_rigidDynamic->addShape( shape );
                            }

                            if( m_rigidStatic )
                            {
                                m_rigidStatic->addShape( shape );
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Rigidbody::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActor() )
            {
                auto state = getState();
                switch( state )
                {
                case IComponent::State::Edit:
                case IComponent::State::Play:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto physicsManager = applicationManager->getPhysicsManager();

                    auto enabled = isEnabled() && actor->isEnabledInScene();

                    if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                        BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
                    {
                        if( enabled )
                        {
                            createRigidbodyObject();
                        }
                        else
                        {
                            destroyRigidbodyObject();
                        }
                    }
                    else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                             BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                    {
                        if( enabled )
                        {
                            createRigidbodyObject();
                        }
                        else
                        {
                            destroyRigidbodyObject();
                        }
                    }
                }
                break;
                default:
                {
                }
                };
            }
        }

        IFSM::ReturnType Rigidbody::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            switch( eventType )
            {
            case IFSM::Event::Change:
                break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Edit:
                case State::Play:
                {
                    auto actor = getActor();
                    auto enabled = isEnabled() && actor->isEnabledInScene();

                    if( enabled )
                    {
                        createRigidbodyObject();
                    }
                    else
                    {
                        destroyRigidbodyObject();
                    }
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Play:
                {
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Pending:
                break;
            case IFSM::Event::Complete:
                break;
            case IFSM::Event::NewState:
                break;
            case IFSM::Event::WaitForChange:
                break;
            default:
            {
            }
            break;
            }

            return IFSM::ReturnType::Ok;
        }

        void Rigidbody::componentLoaded( SmartPtr<IComponent> component )
        {
            if( component->isDerived<Collision>() )
            {
                attachShape();
            }
        }

        void Rigidbody::updateKinematicState( bool val )
        {
            // smart_ptr<physics::PxScene> scene = getPhysicScene();
            // if (!scene)
            //{
            //	DEBUG_LOG("Warning: not in a scene.");
            //	return;
            // }

            // FB_PXSCENE_WRITE_LOCK(scene->getScene());

            // if (m_dynamicActor)
            //{
            //	physx::PxRigidDynamic* dynamicActor = m_dynamicActor->isRigidDynamic();
            //	if (dynamicActor)
            //	{
            //		dynamicActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, val);
            //	}
            // }

            // if (m_clonedActor)
            //{
            //	physx::PxRigidDynamic* clonedActor = m_clonedActor->isRigidDynamic();
            //	if (clonedActor)
            //	{
            //		clonedActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, val);
            //	}
            // }

            // if (m_rigidActor)
            //{
            //	physx::PxRigidDynamic* rigidActor = m_rigidActor->isRigidDynamic();
            //	if (rigidActor)
            //	{
            //		rigidActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, val);
            //	}
            // }

            // if (m_currentState)
            //{
            //	m_currentState->setTransform(transform);
            // }
        }

        SmartPtr<physics::IRigidDynamic3> Rigidbody::getRigidDynamic() const
        {
            return m_rigidDynamic;
        }

        void Rigidbody::setRigidDynamic( SmartPtr<physics::IRigidDynamic3> val )
        {
            m_rigidDynamic = val;
        }

        SmartPtr<physics::IRigidStatic3> Rigidbody::getRigidStatic() const
        {
            return m_rigidStatic;
        }

        void Rigidbody::setRigidStatic( SmartPtr<physics::IRigidStatic3> val )
        {
            m_rigidStatic = val;
        }

        bool Rigidbody::isStatic() const
        {
            return m_isStatic;
        }

        void Rigidbody::setStatic( bool val )
        {
            m_isStatic = val;
        }

        bool Rigidbody::isKinematic() const
        {
            return m_isKinematic;
        }

        void Rigidbody::setKinematic( bool val )
        {
            m_isKinematic = val;
        }

        u32 Rigidbody::getGroupMask() const
        {
            return m_groupMask;
        }

        void Rigidbody::setGroupMask( u32 val )
        {
            m_groupMask = val;
        }

        u32 Rigidbody::getCollisionMask() const
        {
            return m_collisionMask;
        }

        void Rigidbody::setCollisionMask( u32 val )
        {
            m_collisionMask = val;
        }

        AABB3<real_Num> Rigidbody::getLocalBounds() const
        {
            return m_bounds;
        }

        void Rigidbody::setLocalBounds( const AABB3<real_Num> &val )
        {
            m_bounds = val;
        }

        SmartPtr<physics::IPhysicsScene3> Rigidbody::getScene() const
        {
            return m_scene;
        }

        void Rigidbody::setScene( SmartPtr<physics::IPhysicsScene3> scene )
        {
            m_scene = scene;
        }

        SmartPtr<physics::IPhysicsMaterial3> Rigidbody::getMaterial() const
        {
            return m_material;
        }

        void Rigidbody::setMaterial( SmartPtr<physics::IPhysicsMaterial3> material )
        {
            m_material = material;
        }

        void Rigidbody::updateComponents()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();

            auto typeInfo = Rigidbody::typeInfo();
            auto components = sceneManager->getComponentsByType<Rigidbody>( typeInfo );
            for( auto &component : components )
            {
                component->preUpdate();
            }
        }

        void Rigidbody::preUpdate()
        {
            //auto applicationManager = core::IApplicationManager::instance();
            //FB_ASSERT( applicationManager );

            //if( applicationManager->isPlaying() )
            //{
            //    if( m_rigidDynamic )
            //    {
            //        m_isUpdatingTransform = true;

            //        auto transform = m_rigidDynamic->getTransform();
            //        auto position = transform.getPosition();
            //        auto orientation = transform.getOrientation();

            //        auto actor = getActor();
            //        if( actor )
            //        {
            //            if( actor->getParent() )
            //            {
            //                auto t = actor->getTransform();
            //                auto localTranform = t->getLocalTransform();
            //                auto worldTranform = t->getWorldTransform();

            //                auto localPosition = worldTranform.convertWorldToLocalPosition( position );
            //                auto localOrientation =
            //                    worldTranform.convertWorldToLocalOrientation( orientation );

            //                t->setLocalPosition( localPosition );
            //                t->setLocalOrientation( localOrientation );
            //            }
            //            else
            //            {
            //                actor->setLocalPosition( position );
            //                actor->setLocalOrientation( orientation );
            //            }
            //        }

            //        m_isUpdatingTransform = false;
            //    }
            //}
        }

        void Rigidbody::update()
        {
        }

        void Rigidbody::postUpdate()
        {
        }

        Array<SmartPtr<ISharedObject>> Rigidbody::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;

            objects.push_back( m_rigidDynamic );
            objects.push_back( m_rigidStatic );
            objects.push_back( m_clonedActor );

            objects.push_back( m_scene );
            objects.push_back( m_material );

            return objects;
        }

        SmartPtr<Properties> Rigidbody::getProperties() const
        {
            try
            {
                auto properties = fb::make_ptr<Properties>();

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

        void Rigidbody::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
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

        void Rigidbody::updateTransform()
        {
            auto state = getState();
            switch( state )
            {
            case State::Edit:
            case State::Play:
            {
                if( auto actor = getActor() )
                {
                    auto p = actor->getPosition();
                    auto r = actor->getOrientation();

                    auto transform = Transform3<real_Num>( p, r );

                    if( m_rigidDynamic )
                    {
                        m_rigidDynamic->setTransform( transform );
                    }

                    if( m_rigidStatic )
                    {
                        m_rigidStatic->setTransform( transform );
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }

        Vector3<real_Num> Rigidbody::getLinearVelocity() const
        {
            if( m_rigidDynamic )
            {
                return m_rigidDynamic->getLinearVelocity();
            }

            return Vector3<real_Num>::zero();
        }

        void Rigidbody::setLinearVelocity( const Vector3<real_Num> &linearVelocity )
        {
            if( m_rigidDynamic )
            {
                m_rigidDynamic->setLinearVelocity( linearVelocity );
            }
        }

        Vector3<real_Num> Rigidbody::getAngularVelocity() const
        {
            if( m_rigidDynamic )
            {
                return m_rigidDynamic->getAngularVelocity();
            }

            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> Rigidbody::getPointVelocity( const Vector3<real_Num> &point )
        {
            if( m_rigidDynamic )
            {
                const auto globalPose = m_rigidDynamic->getTransform();
                const auto cmassLocalPose = m_rigidDynamic->getCMassLocalPose();
                const auto centerOfMass = globalPose.transformPoint( cmassLocalPose.getPosition() );
                const auto rpoint = point - centerOfMass;

                const auto linearVelocity = getLinearVelocity();
                const auto angularVelocity = getAngularVelocity();
                return linearVelocity + angularVelocity.crossProduct( rpoint );
            }

            return Vector3<real_Num>::zero();
        }

        void Rigidbody::setAngularVelocity( const Vector3<real_Num> &linearVelocity )
        {
            if( m_rigidDynamic )
            {
                m_rigidDynamic->setAngularVelocity( linearVelocity );
            }
        }

        Vector3<real_Num> Rigidbody::getLocalAngularVelocity() const
        {
            if( m_rigidDynamic )
            {
                auto av = getAngularVelocity();
                auto transform = m_rigidDynamic->getTransform();
                auto q = transform.getOrientation();
                return q.rotateInv( av );
            }

            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> Rigidbody::getLocalLinearVelocity() const
        {
            if( m_rigidDynamic )
            {
                auto linearVelocity = getLinearVelocity();
                auto transform = m_rigidDynamic->getTransform();
                auto q = transform.getOrientation();
                return q.rotateInv( linearVelocity );
            }

            return Vector3<real_Num>::zero();
        }

        void Rigidbody::addForce( const Vector3<real_Num> &force )
        {
            if( m_rigidDynamic )
            {
                m_rigidDynamic->addForce( force );
            }
        }

        void Rigidbody::addTorque( const Vector3<real_Num> &torque )
        {
            if( m_rigidDynamic )
            {
                m_rigidDynamic->addTorque( torque );
            }
        }

        void Rigidbody::setMass( real_Num mass )
        {
            m_mass = mass;

            if( m_rigidDynamic )
            {
                m_rigidDynamic->setMass( mass );
            }
        }

        real_Num Rigidbody::getMass() const
        {
            return m_mass;
        }

        Vector3<real_Num> Rigidbody::getLocalPointVelocity( const Vector3<real_Num> &point )
        {
            const auto globalPose = m_rigidDynamic->getTransform();
            auto wp = globalPose.transformPoint( point );

            const auto cmassLocalPose = m_rigidDynamic->getCMassLocalPose();
            const auto centerOfMass = globalPose.transformPoint( cmassLocalPose.getPosition() );
            const auto rpoint = point - centerOfMass;

            const auto linearVelocity = getLinearVelocity();
            const auto angularVelocity = getAngularVelocity();
            return globalPose.inverseTransformPoint( linearVelocity +
                                                     angularVelocity.crossProduct( rpoint ) );
        }

        Vector3<real_Num> Rigidbody::getMassSpaceInertiaTensor() const
        {
            if( m_rigidDynamic )
            {
                return m_rigidDynamic->getMassSpaceInertiaTensor();
            }

            return m_massSpaceInertiaTensor;
        }

        void Rigidbody::setMassSpaceInertiaTensor( const Vector3<real_Num> &massSpaceInertiaTensor )
        {
            m_massSpaceInertiaTensor = massSpaceInertiaTensor;

            if( m_rigidDynamic )
            {
                m_rigidDynamic->setMassSpaceInertiaTensor( m_massSpaceInertiaTensor );
            }
        }

        fb::Transform3<fb::real_Num> Rigidbody::getTransform() const
        {
            if( m_rigidDynamic )
            {
                return m_rigidDynamic->getTransform();
            }

            if( m_rigidStatic )
            {
                return m_rigidStatic->getTransform();
            }

            return Transform3<real_Num>();
        }

    }  // namespace scene
}  // end namespace fb
