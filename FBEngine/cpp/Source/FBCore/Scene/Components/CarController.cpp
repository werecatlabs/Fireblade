#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CarController.h>
#include <FBCore/Scene/Components/WheelController.h>
#include <FBCore/FBCore.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, CarController, VehicleController );

    CarController::CarController() = default;

    CarController::~CarController()
    {
        unload( nullptr );
    }

    void CarController::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            VehicleController::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            m_vehicleCallback = fb::make_ptr<VehicleCallback>();
            m_vehicleCallback->setOwner( this );

            auto vehicleController = factoryManager->make_object<IVehicleController>();
            setVehicleController( vehicleController );

            if( vehicleController )
            {
                vehicleController->load( nullptr );
                vehicleController->setVehicleCallback( m_vehicleCallback );
                vehicleController->setDisplayDebugData( true );

                auto vehicleManager = applicationManager->getVehicleManager();
                if( vehicleManager )
                {
                    vehicleManager->addVehicle( vehicleController );
                }
            }

            auto actor = getActor();
            if( actor )
            {
                m_collision = actor->getComponent<Collision>();
                m_chassis = actor->getComponent<Rigidbody>();
            }

            if( m_chassis )
            {
                m_chassis->setMass( m_mass );
            }

            if( vehicleController )
            {
                vehicleController->setMass( m_mass );
            }

            auto collisionBox = fb::dynamic_pointer_cast<CollisionBox>( m_collision );
            if( collisionBox )
            {
                auto extents = Vector3<real_Num>( m_width * 0.5f, m_height * 0.5f, m_length * 0.5f );
                collisionBox->setExtents( extents );
            }

            auto inputManager = applicationManager->getInputDeviceManager();
            if( inputManager )
            {
                auto inputListener = fb::make_ptr<InputListener>();
                inputListener->setOwner( this );
                inputManager->addListener( inputListener );
            }

            m_wheels.reserve( 4 );
            m_poweredWheels.reserve( 4 );

            auto moi = Vector3F::unit() * 1000.0f;
            setMOI( moi );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CarController::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( m_chassis )
                {
                    m_chassis->unload( data );
                    m_chassis = nullptr;
                }

                if( m_collision )
                {
                    m_collision->unload( data );
                    m_collision = nullptr;
                }

                if( m_inputListener )
                {
                    m_inputListener->unload( data );
                    m_inputListener = nullptr;
                }

                if( m_vehicleController )
                {
                    auto vehicleManager = applicationManager->getVehicleManager();
                    if( vehicleManager )
                    {
                        vehicleManager->removeVehicle( m_vehicleController );
                    }

                    m_vehicleController->unload( data );
                    m_vehicleController = nullptr;
                }

                m_vehicleCallback = nullptr;

                m_collision = nullptr;
                m_chassis = nullptr;

                for( auto wheel : m_wheels )
                {
                    wheel->unload( data );
                }

                m_wheels.clear();

                VehicleController::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CarController::play()
    {
        if( m_vehicleController )
        {
            m_vehicleController->setState( IVehicleController::State::PLAY );
        }
    }

    void CarController::edit()
    {
        if( m_vehicleController )
        {
            m_vehicleController->setState( IVehicleController::State::EDIT );
        }
    }

    void CarController::setupWheels()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto inputManager = applicationManager->getInputDeviceManager();
        if( inputManager )
        {
            if( inputManager->isKeyPressed( KeyCodes::KEY_UP ) )
            {
                setThrottle( 1.0f );
            }
            else
            {
                setThrottle( 0.0f );
            }

            if( inputManager->isKeyPressed( KeyCodes::KEY_DOWN ) )
            {
                setBrake( 1.0f );
            }
            else
            {
                setBrake( 0.0f );
            }

            if( inputManager->isKeyPressed( KeyCodes::KEY_LEFT ) )
            {
                setSteering( -1.0f );
            }
            else if( inputManager->isKeyPressed( KeyCodes::KEY_RIGHT ) )
            {
                setSteering( 1.0f );
            }
            else
            {
                setSteering( 0.0f );
            }
        }

        auto vehicleController = getVehicleController();
        if( !vehicleController )
        {
            return;
        }

        if( vehicleController )
        {
            vehicleController->setChannel( static_cast<s32>( IVehicleController::Input::THROTTLE ),
                                           m_throttle );
            vehicleController->setChannel( static_cast<s32>( IVehicleController::Input::BRAKE ),
                                           m_brake );
            vehicleController->setChannel( static_cast<s32>( IVehicleController::Input::STEERING ),
                                           m_steering );
        }

        // position wheels
        auto actor = getActor();
        auto wheelControllers = actor->getComponentsInChildren<WheelController>();
        if( wheelControllers.size() == 4 )
        {
            auto halfWheelBase = m_wheelBase / static_cast<real_Num>( 2.0 );
            auto halfWidth = m_width / static_cast<real_Num>( 2.0 );

            auto frontLeft = wheelControllers[static_cast<u32>( Wheels::FRONT_LEFT )];
            if( frontLeft )
            {
                auto frontLeftActor = frontLeft->getActor();

                auto wheelPosition =
                    Vector3<real_Num>( -halfWidth, -m_wheelChassisOffset, -halfWheelBase );

                auto localWheelTransform = Transform3<real_Num>();
                localWheelTransform.setPosition( wheelPosition );

                frontLeftActor->setLocalPosition( wheelPosition );

                auto wheelController =
                    vehicleController->getWheelController( static_cast<u32>( Wheels::FRONT_LEFT ) );
                frontLeft->setWheelController( wheelController );
                wheelController->setLocalTransform( localWheelTransform );
                wheelController->setSteeringWheel( true );
            }

            auto frontRight = wheelControllers[static_cast<u32>( Wheels::FRONT_RIGHT )];
            if( frontRight )
            {
                auto frontRightActor = frontRight->getActor();

                auto wheelPosition =
                    Vector3<real_Num>( halfWidth, -m_wheelChassisOffset, -halfWheelBase );

                auto localWheelTransform = Transform3<real_Num>();
                localWheelTransform.setPosition( wheelPosition );

                frontRightActor->setLocalPosition( wheelPosition );

                auto wheelController =
                    vehicleController->getWheelController( static_cast<u32>( Wheels::FRONT_RIGHT ) );
                frontRight->setWheelController( wheelController );
                wheelController->setLocalTransform( localWheelTransform );
                wheelController->setSteeringWheel( true );
            }

            auto rearLeft = wheelControllers[static_cast<u32>( Wheels::REAR_LEFT )];
            if( rearLeft )
            {
                auto frontLeftActor = rearLeft->getActor();

                auto wheelPosition =
                    Vector3<real_Num>( -halfWidth, -m_wheelChassisOffset, halfWheelBase );

                auto localWheelTransform = Transform3<real_Num>();
                localWheelTransform.setPosition( wheelPosition );

                frontLeftActor->setLocalPosition( wheelPosition );

                auto wheelController =
                    vehicleController->getWheelController( static_cast<u32>( Wheels::REAR_LEFT ) );
                rearLeft->setWheelController( wheelController );
                wheelController->setLocalTransform( localWheelTransform );
            }

            auto rearRight = wheelControllers[static_cast<u32>( Wheels::REAR_RIGHT )];
            if( rearRight )
            {
                auto frontRightActor = rearRight->getActor();

                auto wheelPosition =
                    Vector3<real_Num>( halfWidth, -m_wheelChassisOffset, halfWheelBase );

                auto localWheelTransform = Transform3<real_Num>();
                localWheelTransform.setPosition( wheelPosition );

                frontRightActor->setLocalPosition( wheelPosition );

                auto wheelController =
                    vehicleController->getWheelController( static_cast<u32>( Wheels::REAR_RIGHT ) );
                rearRight->setWheelController( wheelController );
                wheelController->setLocalTransform( localWheelTransform );
            }

            // suspension
            for( u32 i = 0; i < wheelControllers.size(); ++i )
            {
                auto wc = wheelControllers[i];

                auto wheelController = getVehicleController()->getWheelController( i );

                // wheelController->setRadius(m_radius);
                // wheelController->setSuspensionTravel(m_suspensionTravel);
                // wheelController->setDamping(m_damping);
            }
        }
    }

    CarController::VehicleCallback::VehicleCallback( CarController *controller ) : m_owner( controller )
    {
    }

    CarController::VehicleCallback::~VehicleCallback()
    {
        m_owner = nullptr;
    }

    auto CarController::VehicleCallback::getData( const String &filePath ) -> String
    {
        return "";
    }

    void CarController::VehicleCallback::addForce( s32 bodyId, const Vector3<real_Num> &force,
                                                   const Vector3<real_Num> &pos )
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                chassis->addForce( force );
            }
        }
    }

    void CarController::VehicleCallback::addTorque( s32 bodyId, const Vector3<real_Num> &torque )
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                chassis->addTorque( torque );
            }
        }
    }

    void CarController::VehicleCallback::addLocalForce( s32 bodyId, const Vector3<real_Num> &force,
                                                        const Vector3<real_Num> &pos )
    {
    }

    void CarController::VehicleCallback::addLocalTorque( s32 bodyId, const Vector3<real_Num> &torque )
    {
    }

    auto CarController::VehicleCallback::getAngularVelocity() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                return chassis->getAngularVelocity();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getLinearVelocity() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                return chassis->getLinearVelocity();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getLocalAngularVelocity() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                return chassis->getLocalAngularVelocity();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getLocalLinearVelocity() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto chassis = owner->getChassis() )
            {
                return chassis->getLocalLinearVelocity();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getScale() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto actor = owner->getActor() )
            {
                return actor->getScale();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getPosition() const -> Vector3<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto actor = owner->getActor() )
            {
                if( auto rb = owner->getChassis() )
                {
                    if( auto r = rb->getRigidDynamic() )
                    {
                        auto transform = r->getTransform();
                        return transform.getPosition();
                    }
                }

                return actor->getPosition();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto CarController::VehicleCallback::getOrientation() const -> Quaternion<real_Num>
    {
        if( auto owner = getOwner() )
        {
            if( auto actor = owner->getActor() )
            {
                if( auto rb = owner->getChassis() )
                {
                    if( auto r = rb->getRigidDynamic() )
                    {
                        auto transform = r->getTransform();
                        return transform.getOrientation();
                    }
                }

                return actor->getOrientation();
            }
        }

        return Quaternion<real_Num>::identity();
    }

    void CarController::VehicleCallback::displayLocalVector( s32 Bdy, const Vector3<real_Num> &V,
                                                             const Vector3<real_Num> &Org,
                                                             s32 colour ) const
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto debug = graphicsSystem->getDebug();
        FB_ASSERT( debug );

        debug->drawLine( 0, V, Org, colour );
    }

    void CarController::VehicleCallback::displayLocalVector( s32 Bdy, s32 id, const Vector3<real_Num> &V,
                                                             const Vector3<real_Num> &Org,
                                                             s32 colour ) const
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto debug = graphicsSystem->getDebug();
        FB_ASSERT( debug );

        debug->drawLine( id, V, Org, colour );
    }

    auto CarController::VehicleCallback::getCallbackFunction() const -> void *
    {
        return nullptr;
    }

    void CarController::VehicleCallback::setCallbackFunction( void *val )
    {
    }

    auto CarController::VehicleCallback::getCallbackDataFunction() const -> void *
    {
        return nullptr;
    }

    void CarController::VehicleCallback::setCallbackDataFunction( void *val )
    {
    }

    auto CarController::VehicleCallback::getInputData() const -> FixedArray<f32, 8>
    {
        return {};
    }

    auto CarController::VehicleCallback::getControlAngles() const -> FixedArray<f32, 11>
    {
        return {};
    }

    auto CarController::VehicleCallback::getPointVelocity( const Vector3<real_Num> &p )
        -> Vector3<real_Num>
    {
        auto chassis = m_owner->getChassis();
        return chassis->getPointVelocity( p );
    }

    auto CarController::VehicleCallback::castLocalRay( const Ray3<real_Num> &ray,
                                                       SmartPtr<physics::IRaycastHit> &data ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        // auto raycastScene = applicationManager->getRaycastScene();
        auto raycastScene = applicationManager->getPhysicsScene();
        if( raycastScene )
        {
            auto origin = ray.getOrigin();
            auto dir = ray.getDirection();

            if( auto owner = getOwner() )
            {
                auto actor = owner->getActor();
                auto rb = owner->getChassis();

                if( auto rigidDynamic = rb->getRigidDynamic() )
                {
                    auto worldTransform = rigidDynamic->getTransform();
                    auto worigin = worldTransform.transformPoint( origin );
                    auto wdir = worldTransform.transformVector( dir );

                    return raycastScene->castRay( Ray3<real_Num>( worigin, wdir ), data );
                }
            }
        }

        return false;
    }

    auto CarController::VehicleCallback::castWorldRay( const Ray3<real_Num> &ray,
                                                       SmartPtr<physics::IRaycastHit> &data ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        // auto raycastScene = applicationManager->getRaycastScene();
        auto raycastScene = applicationManager->getPhysicsScene();
        if( raycastScene )
        {
            return raycastScene->castRay( ray, data );
        }

        return false;
    }

    auto CarController::VehicleCallback::getOwner() const -> SmartPtr<CarController>
    {
        return m_owner;
    }

    void CarController::VehicleCallback::setOwner( SmartPtr<CarController> owner )
    {
        m_owner = owner;
    }

    void CarController::VehicleCallback::displayVector( s32 Bdy, s32 id, const Vector3<real_Num> &V,
                                                        const Vector3<real_Num> &Org, s32 colour ) const
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto debug = graphicsSystem->getDebug();
        FB_ASSERT( debug );

        debug->drawLine( id, V, Org, colour );
    }

    auto CarController::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Component::getProperties() )
        {
            properties->setProperty( "cg", m_cg );
            properties->setProperty( "mass", m_mass );
            properties->setProperty( "moi", m_moi );

            properties->setProperty( "radius", m_radius );
            properties->setProperty( "suspensionTravel", m_suspensionTravel );
            properties->setProperty( "damping", m_damping );

            if( m_collision )
            {
                auto handle = m_collision->getHandle();
                auto uuid = handle->getUUID();
                properties->setProperty( "Collision", uuid, "resource", false );
            }
            else
            {
                properties->setProperty( "Collision", "", "resource", false );
            }

            if( m_chassis )
            {
                auto handle = m_chassis->getHandle();
                auto uuid = handle->getUUID();
                properties->setProperty( "Chassis", uuid, "resource", false );
            }
            else
            {
                properties->setProperty( "Chassis", "", "resource", false );
            }

            properties->setProperty( "Reset", "ResetButton", "button", false );
            properties->setProperty( "Reset Transform", "ResetButton", "button", false );

            return properties;
        }

        return nullptr;
    }

    void CarController::setProperties( SmartPtr<Properties> properties )
    {
        Component::setProperties( properties );

        properties->getPropertyValue( "cg", m_cg );
        properties->getPropertyValue( "mass", m_mass );
        properties->getPropertyValue( "moi", m_moi );

        properties->getPropertyValue( "radius", m_radius );
        properties->getPropertyValue( "suspensionTravel", m_suspensionTravel );
        properties->getPropertyValue( "damping", m_damping );

        if( properties->hasProperty( "Reset" ) )
        {
            auto &resetButton = properties->getPropertyObject( "Reset" );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                m_mass = static_cast<real_Num>( 1490.0 );

                // m_radius = 0.29;
                // m_suspensionDistance = 0.1;
                // m_wheelDamping = 12000;

                auto actor = getActor();
                if( actor )
                {
                    auto wheelControllers = actor->getComponentsInChildren<WheelController>();
                    for( auto wheelController : wheelControllers )
                    {
                        wheelController->reset();
                    }
                }
            }
        }

        if( properties->hasProperty( "Reset Transform" ) )
        {
            auto &resetButton = properties->getPropertyObject( "Reset Transform" );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                auto actor = getActor();
                if( actor )
                {
                    actor->setLocalPosition( Vector3<real_Num>( 0, 5, 0 ) );
                    actor->setLocalOrientation( Quaternion<real_Num>::identity() );
                }
            }
        }

        if( m_chassis )
        {
            m_chassis->setMassSpaceInertiaTensor( m_moi );
        }
    }

    auto CarController::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( 5 );

        objects.emplace_back( m_inputListener );
        objects.emplace_back( m_vehicleController );
        objects.emplace_back( m_vehicleCallback );

        objects.emplace_back( m_collision );
        objects.emplace_back( m_chassis );

        return objects;
    }

    auto CarController::getChassis() const -> SmartPtr<Rigidbody>
    {
        return m_chassis;
    }

    void CarController::setChassis( SmartPtr<Rigidbody> chassis )
    {
        m_chassis = chassis;
    }

    auto CarController::getVehicleController() const -> SmartPtr<IVehicleController>
    {
        return m_vehicleController;
    }

    void CarController::setVehicleController( SmartPtr<IVehicleController> vehicleController )
    {
        m_vehicleController = vehicleController;
    }

    auto CarController::getThrottle() const -> f32
    {
        return m_throttle;
    }

    void CarController::setThrottle( f32 throttle )
    {
        m_throttle = throttle;
    }

    auto CarController::getBrake() const -> f32
    {
        return m_brake;
    }

    void CarController::setBrake( f32 brake )
    {
        m_brake = brake;
    }

    auto CarController::getSteering() const -> f32
    {
        return m_steering;
    }

    void CarController::setSteering( f32 steering )
    {
        m_steering = steering;
    }

    auto CarController::getMOI() const -> Vector3F
    {
        return m_moi;
    }

    void CarController::setMOI( const Vector3F &moi )
    {
        m_moi = moi;

        if( m_chassis )
        {
            m_chassis->setMassSpaceInertiaTensor( m_moi );
        }
    }

    auto CarController::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        //FB_ASSERT( Thread::getCurrentTask() == Thread::Task::Application );

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
            {
                setupWheels();

                if( m_vehicleController )
                {
                    m_vehicleController->setState( IVehicleController::State::EDIT );
                }
            }
            break;
            case State::Play:
            {
                setupWheels();

                if( m_vehicleController )
                {
                    m_vehicleController->setState( IVehicleController::State::PLAY );
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
            case State::Edit:
            {
                if( m_vehicleController )
                {
                    m_vehicleController->setState( IVehicleController::State::RESET );
                }
            }
            break;
            case State::Play:
            {
                if( m_vehicleController )
                {
                    m_vehicleController->setState( IVehicleController::State::RESET );
                }
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

    void CarController::InputListener::unload( SmartPtr<ISharedObject> data )
    {
        m_owner = nullptr;
    }

    auto CarController::InputListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                    const Array<Parameter> &arguments,
                                                    SmartPtr<ISharedObject> sender,
                                                    SmartPtr<ISharedObject> object,
                                                    SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::inputEvent )
        {
            auto result = inputEvent( event );
            return Parameter( result );
        }

        return {};
    }

    auto CarController::InputListener::inputEvent( SmartPtr<IInputEvent> event ) -> bool
    {
        if( auto owner = getOwner() )
        {
            switch( auto eventType = event->getEventType() )
            {
            case IInputEvent::EventType::Key:
            {
                if( auto keyboardState = event->getKeyboardState() )
                {
                    if( keyboardState->isPressedDown() )
                    {
                        if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_W ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 0, 1.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_A ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 2, -1.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_S ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 1, 1.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_D ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 2, 1.0 );
                            }
                        }
                    }
                    else if( !keyboardState->isPressedDown() )
                    {
                        if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_W ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 0, 0.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_A ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 2, 0.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_S ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 1, 0.0 );
                            }
                        }
                        else if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_D ) )
                        {
                            auto vehicleController = m_owner->getVehicleController();
                            if( vehicleController )
                            {
                                vehicleController->setChannel( 2, 0.0 );
                            }
                        }
                    }
                }
            }
            break;
            case IInputEvent::EventType::Joystick:
            {
                if( auto joystick = event->getJoystickState() )
                {
                    if( joystick->getEventType() == static_cast<u32>( IJoystickState::Type::AxisMoved ) )
                    {
                        auto throttle = joystick->getAxis( 2 );
                        auto steering = joystick->getAxis( 1 );

                        if( auto vehicleController = owner->getVehicleController() )
                        {
                            vehicleController->setChannel( 0, MathF::Abs( throttle ) );
                            vehicleController->setChannel( 2, steering );
                        }
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }

        return false;
    }

    auto CarController::InputListener::updateEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        return false;
    }

    void CarController::InputListener::setPriority( s32 priority )
    {
        m_priority = priority;
    }

    auto CarController::InputListener::getPriority() const -> s32
    {
        return m_priority;
    }

    auto CarController::InputListener::getOwner() const -> SmartPtr<CarController>
    {
        return m_owner;
    }

    void CarController::InputListener::setOwner( SmartPtr<CarController> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::scene
