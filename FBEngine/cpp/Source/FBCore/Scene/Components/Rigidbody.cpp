#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Scene/Components/RigidbodyListener.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/Collision.h>
#include <FBCore/Scene/Components/Constraint.h>
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

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Rigidbody, Component );

    Rigidbody::Rigidbody() = default;

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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

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

            auto applicationManager = core::ApplicationManager::instance();
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

            Component::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Rigidbody::updateConstraints()
    {
        for( auto &constraint : m_constraints )
        {
            constraint->updateConstraint();
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

        auto applicationManager = core::ApplicationManager::instance();
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
        }
    }

    void Rigidbody::updateShapes()
    {
        removeFromScene();
        attachShape();
        addToScene();
    }

    void Rigidbody::addConstraint( SmartPtr<Constraint> constraint )
    {
        m_constraints.push_back( constraint );
    }

    void Rigidbody::removeConstraint( SmartPtr<Constraint> constraint )
    {
        m_constraints.erase( std::remove( m_constraints.begin(), m_constraints.end(), constraint ),
                             m_constraints.end() );
    }

    auto Rigidbody::hasConstraint( SmartPtr<Constraint> constraint ) const -> bool
    {
        return std::find( m_constraints.begin(), m_constraints.end(), constraint ) !=
               m_constraints.end();
    }

    auto Rigidbody::hasConstraints() const -> bool
    {
        return !m_constraints.empty();
    }

    auto Rigidbody::getConstraints() const -> Array<SmartPtr<Constraint>>
    {
        return m_constraints;
    }

    void Rigidbody::setConstraints( const Array<SmartPtr<Constraint>> &constraints )
    {
        m_constraints = constraints;
    }

    void Rigidbody::removeFromScene()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsScene = applicationManager->getPhysicsScene();
        //FB_ASSERT( physicsScene );

        if( auto rigidDynamic = getRigidDynamic() )
        {
            physicsScene->removeActor( rigidDynamic );
        }

        if( auto rigidStatic = getRigidStatic() )
        {
            physicsScene->removeActor( rigidStatic );
        }
    }

    void Rigidbody::addToScene()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsScene = applicationManager->getPhysicsScene();
        //FB_ASSERT( physicsScene );

        if( auto rigidDynamic = getRigidDynamic() )
        {
            physicsScene->addActor( rigidDynamic );
        }

        if( auto rigidStatic = getRigidStatic() )
        {
            physicsScene->addActor( rigidStatic );
        }
    }

    void Rigidbody::destroyRigidbodyObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsManager = applicationManager->getPhysicsManager();

        auto physicsScene = applicationManager->getPhysicsScene();
        if( physicsScene )
        {
            if( auto rigidDynamic = getRigidDynamic() )
            {
                physicsScene->removeActor( rigidDynamic );
            }

            if( auto rigidStatic = getRigidStatic() )
            {
                physicsScene->removeActor( rigidStatic );
            }
        }

        if( physicsManager )
        {
            if( auto rigidDynamic = getRigidDynamic() )
            {
                physicsManager->removePhysicsBody( rigidDynamic );
                setRigidDynamic( nullptr );
            }

            if( auto rigidStatic = getRigidStatic() )
            {
                physicsManager->removePhysicsBody( rigidStatic );
                setRigidStatic( nullptr );
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
            if( auto actor = getActor() )
            {
                auto root = actor->getSceneRoot();
                if( root )
                {
                    root->updateTransform();
                }

                auto transform = actor->getTransform();

                auto collision = actor->getComponent<Collision>();
                FB_ASSERT( collision );  // no collision

                if( collision )
                {
                    auto shape = collision->getShape();
                    //FB_ASSERT( shape );

                    if( shape )
                    {
                        auto localTransform = transform->getWorldTransform();
                        localTransform.setPosition( Vector3F::zero() );
                        localTransform.setOrientation( QuaternionF::identity() );

                        shape->setLocalPose( localTransform );

                        if( auto rigidDynamic = getRigidDynamic() )
                        {
                            rigidDynamic->addShape( shape );
                        }

                        if( auto rigidStatic = getRigidStatic() )
                        {
                            rigidStatic->addShape( shape );
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

    void Rigidbody::updateFlags( u32 flags, u32 oldFlags )
    {
        if( auto actor = getActor() )
        {
            auto state = getState();
            switch( state )
            {
            case State::Edit:
            case State::Play:
            {
                auto applicationManager = core::ApplicationManager::instance();
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

                attachShape();

                if( auto rigidDynamic = getRigidDynamic() )
                {
                    if( rigidDynamic->getNumShapes() > 0 )
                    {
                        addToScene();
                    }
                }

                if( auto rigidStatic = getRigidStatic() )
                {
                    if( rigidStatic->getNumShapes() > 0 )
                    {
                        addToScene();
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }
    }

    auto Rigidbody::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
                    attachShape();
                    updateConstraints();

                    if( auto rigidDynamic = getRigidDynamic() )
                    {
                        if( rigidDynamic->getNumShapes() > 0 )
                        {
                            addToScene();
                        }
                    }

                    if( auto rigidStatic = getRigidStatic() )
                    {
                        if( rigidStatic->getNumShapes() > 0 )
                        {
                            addToScene();
                        }
                    }
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

    auto Rigidbody::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                 const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                 SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IComponent::componentLoaded )
        {
            if( object->isDerived<Collision>() )
            {
                attachShape();
            }
        }

        return {};
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

    auto Rigidbody::getRigidDynamic() const -> SmartPtr<physics::IRigidDynamic3>
    {
        return m_rigidDynamic;
    }

    void Rigidbody::setRigidDynamic( SmartPtr<physics::IRigidDynamic3> val )
    {
        m_rigidDynamic = val;
    }

    auto Rigidbody::getRigidStatic() const -> SmartPtr<physics::IRigidStatic3>
    {
        return m_rigidStatic;
    }

    void Rigidbody::setRigidStatic( SmartPtr<physics::IRigidStatic3> val )
    {
        m_rigidStatic = val;
    }

    auto Rigidbody::isStatic() const -> bool
    {
        return m_isStatic;
    }

    void Rigidbody::setStatic( bool val )
    {
        m_isStatic = val;
    }

    auto Rigidbody::isKinematic() const -> bool
    {
        return m_isKinematic;
    }

    void Rigidbody::setKinematic( bool val )
    {
        m_isKinematic = val;
    }

    auto Rigidbody::getGroupMask() const -> u32
    {
        return m_groupMask;
    }

    void Rigidbody::setGroupMask( u32 val )
    {
        m_groupMask = val;
    }

    auto Rigidbody::getCollisionMask() const -> u32
    {
        return m_collisionMask;
    }

    void Rigidbody::setCollisionMask( u32 val )
    {
        m_collisionMask = val;
    }

    auto Rigidbody::getLocalBounds() const -> AABB3<real_Num>
    {
        return m_bounds;
    }

    void Rigidbody::setLocalBounds( const AABB3<real_Num> &val )
    {
        m_bounds = val;
    }

    auto Rigidbody::getScene() const -> SmartPtr<physics::IPhysicsScene3>
    {
        return m_scene;
    }

    void Rigidbody::setScene( SmartPtr<physics::IPhysicsScene3> scene )
    {
        m_scene = scene;
    }

    auto Rigidbody::getMaterial() const -> SmartPtr<physics::IPhysicsMaterial3>
    {
        return m_material;
    }

    void Rigidbody::setMaterial( SmartPtr<physics::IPhysicsMaterial3> material )
    {
        m_material = material;
    }

    void Rigidbody::updateComponents()
    {
        auto applicationManager = core::ApplicationManager::instance();
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
        //auto applicationManager = core::ApplicationManager::instance();
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

    auto Rigidbody::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;

        objects.emplace_back( m_rigidDynamic );
        objects.emplace_back( m_rigidStatic );
        objects.emplace_back( m_clonedActor );

        objects.emplace_back( m_scene );
        objects.emplace_back( m_material );

        return objects;
    }

    auto Rigidbody::getProperties() const -> SmartPtr<Properties>
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

    auto Rigidbody::getLinearVelocity() const -> Vector3<real_Num>
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

    auto Rigidbody::getAngularVelocity() const -> Vector3<real_Num>
    {
        if( m_rigidDynamic )
        {
            return m_rigidDynamic->getAngularVelocity();
        }

        return Vector3<real_Num>::zero();
    }

    auto Rigidbody::getPointVelocity( const Vector3<real_Num> &point ) -> Vector3<real_Num>
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

    auto Rigidbody::getLocalAngularVelocity() const -> Vector3<real_Num>
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

    auto Rigidbody::getLocalLinearVelocity() const -> Vector3<real_Num>
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

    auto Rigidbody::getMass() const -> real_Num
    {
        return m_mass;
    }

    auto Rigidbody::getLocalPointVelocity( const Vector3<real_Num> &point ) -> Vector3<real_Num>
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

    auto Rigidbody::getMassSpaceInertiaTensor() const -> Vector3<real_Num>
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

    auto Rigidbody::getTransform() const -> Transform3<real_Num>
    {
        if( m_rigidDynamic )
        {
            return m_rigidDynamic->getTransform();
        }

        if( m_rigidStatic )
        {
            return m_rigidStatic->getTransform();
        }

        return {};
    }
}  // namespace fb::scene
