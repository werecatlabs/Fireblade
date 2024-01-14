#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/VehicleController.h>
#include <FBCore/Vehicle/VehicleBody.h>
#include <FBCore/Vehicle/WheelController.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Vehicle/IVehicleBody.h>
#include <FBCore/Interface/Vehicle/IWheelController.h>
#include <FBCore/Interface/Vehicle/IVehicleCallback.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    VehicleController::VehicleController()
    {
        m_channels.resize( 12 );
    }

    VehicleController::~VehicleController() = default;

    void VehicleController::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto pThis = getSharedFromThis<VehicleController>();

            auto body = fb::make_ptr<VehicleBody>();
            body->setParentVehicle( pThis );
            setBody( body );

            auto worldTransform = Transform3<real_Num>();
            setWorldTransform( worldTransform );

            auto localTransform = Transform3<real_Num>();
            setLocalTransform( localTransform );

            auto bodyTransform = Transform3<real_Num>();
            m_bodyTransform = bodyTransform;

            m_wheels.resize( 4 );

            for( auto &w : m_wheels )
            {
                w = factoryManager->make_ptr<WheelController>();
                w->setOwner( pThis );
            }

            m_wheels[0]->setSteeringWheel( true );
            m_wheels[1]->setSteeringWheel( true );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void VehicleController::reset()
    {
    }

    void VehicleController::update()
    {
        updateTransform();

        auto state = getState();
        switch( state )
        {
        case State::EDIT:
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            for( auto &w : m_wheels )
            {
                if( w )
                {
                    w->update();
                }
            }

            auto throttleValue = getChannel( 0 );
            auto brakeValue = getChannel( 1 );
            auto steeringValue = getChannel( 2 ) * 70.0f;

            for( auto &w : m_wheels )
            {
                if( w )
                {
                    if( w->isSteeringWheel() )
                    {
                        w->setSteeringAngle( steeringValue );
                    }
                }
            }
        }
        break;
        case State::PLAY:
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            for( auto &w : m_wheels )
            {
                if( w )
                {
                    w->update();
                }
            }

            auto throttleValue = getChannel( 0 );
            auto brakeValue = getChannel( 1 );
            auto steeringValue = getChannel( 2 ) * 50.0f;

            for( auto &w : m_wheels )
            {
                if( w )
                {
                    if( w->isSteeringWheel() )
                    {
                        w->setSteeringAngle( steeringValue );
                    }
                }
            }

            //throttleValue = 1.0f; // hack
            auto thrust = -Vector3<real_Num>::unitZ() * throttleValue * 1500.0;
            addLocalForce( 0, thrust, Vector3<real_Num>::zero() );

            auto brake = Vector3<real_Num>::unitZ() * brakeValue * 1000.0;
            addLocalForce( 0, brake, Vector3<real_Num>::zero() );
        }
        break;
        default:
        {
        }
        }

        if( m_callback )
        {
            m_callback->addForce( 0, m_force, Vector3<real_Num>::zero() );
            m_callback->addTorque( 0, m_torque );

            m_force = Vector3<real_Num>::zero();
            m_torque = Vector3<real_Num>::zero();
        }
    }

    auto VehicleController::getChannel( s32 idx ) const -> f32
    {
        return m_channels[idx];
    }

    void VehicleController::setChannel( s32 idx, f32 channel )
    {
        m_channels[idx] = channel;
    }

    void VehicleController::updateTransform()
    {
        auto p = getPosition();
        auto q = getOrientation();
        auto s = getScale();

        FB_ASSERT( MathUtil<real_Num>::isFinite( p ) );
        FB_ASSERT( MathUtil<real_Num>::isFinite( q ) );

        // p = Vector3<real_Num>::zero();
        // q = Quaternion<real_Num>::identity();

        // if (m_bodyTransform)
        {
            m_bodyTransform.setPosition( p );
            m_bodyTransform.setOrientation( q );
            m_bodyTransform.setScale( s );

            // if (m_worldTransform)
            {
                // if (m_localTransform)
                {
                    m_worldTransform.transformFromParent( m_bodyTransform, m_localTransform );
                }
            }
        }

        if( auto body = getBody() )
        {
            auto cg = m_bodyTransform.transformPoint( m_cg );
            body->setWorldCenterOfMass( cg );
        }
    }

    auto VehicleController::getPosition() const -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getPosition();
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getScale() const -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getScale();
        }

        return Vector3<real_Num>::zero();
    }

    void VehicleController::setPosition( const Vector3<real_Num> &position )
    {
    }

    auto VehicleController::getOrientation() const -> Quaternion<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getOrientation();
        }

        return Quaternion<real_Num>::identity();
    }

    void VehicleController::setOrientation( const Quaternion<real_Num> &orientation )
    {
    }

    auto VehicleController::isUserControlled() const -> bool
    {
        return false;
    }

    void VehicleController::setUserControlled( bool val )
    {
    }

    auto VehicleController::getMass() const -> real_Num
    {
        if( auto body = getBody() )
        {
            return body->getMass();
        }

        return static_cast<real_Num>( 1.0 );
    }

    void VehicleController::setMass( real_Num mass )
    {
        if( auto body = getBody() )
        {
            body->setMass( mass );
        }
    }

    auto VehicleController::getBody() const -> SmartPtr<IVehicleBody>
    {
        return m_rigidbody;
    }

    void VehicleController::setBody( SmartPtr<IVehicleBody> val )
    {
        m_rigidbody = val;
    }

    auto VehicleController::getWorldTransform() const -> Transform3<real_Num>
    {
        return m_worldTransform;
    }

    void VehicleController::setWorldTransform( Transform3<real_Num> val )
    {
        m_worldTransform = val;
    }

    auto VehicleController::getLocalTransform() const -> Transform3<real_Num>
    {
        return m_localTransform;
    }

    void VehicleController::setLocalTransform( Transform3<real_Num> val )
    {
        m_localTransform = val;
    }

    void VehicleController::drawPoint( s32 body, int id, Vector3<real_Num> positon, u32 color )
    {
        auto size = static_cast<real_Num>( 0.1 );
        auto offset0 = Vector3<real_Num>::forward() * size;
        auto offset1 = Vector3<real_Num>::up() * size;
        auto offset2 = Vector3<real_Num>::right() * size;

        auto p = m_worldTransform.getOrientation() * positon;
        p += m_worldTransform.getPosition();

        displayVector( body, id, p + offset0, p - offset0, color );
        displayVector( body, id + 1, p + offset1, p - offset1, color );
        displayVector( body, id + 2, p + offset2, p - offset2, color );
    }

    void VehicleController::drawLocalPoint( s32 body, int id, Vector3<real_Num> positon, u32 color )
    {
        auto size = 0.1f;
        auto offset0 = Vector3<real_Num>::forward() * size;
        auto offset1 = Vector3<real_Num>::up() * size;
        auto offset2 = Vector3<real_Num>::right() * size;

        // if (m_worldTransform)
        {
            auto p = m_worldTransform.getOrientation() * positon;
            p += m_worldTransform.getPosition();

            displayVector( body, id, p + offset0, p - offset0, color );
            displayVector( body, id + 1000, p + offset1, p - offset1, color );
            displayVector( body, id + 2000, p + offset2, p - offset2, color );
        }
    }

    void VehicleController::displayLocalVector( s32 bodyId, const Vector3<real_Num> &start,
                                                const Vector3<real_Num> &end, u32 colour )
    {
        try
        {
            if( m_callback )
            {
                m_callback->displayLocalVector( bodyId,
                                                Vector3<real_Num>( start.X(), start.Y(), start.Z() ),
                                                Vector3<real_Num>( end.X(), end.Y(), end.Z() ), colour );
            }
        }
        catch( std::exception &Err )
        {
            FB_LOG_EXCEPTION( Err );
        }
    }

    void VehicleController::displayVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                                           const Vector3<real_Num> &end, u32 colour )
    {
        try
        {
            if( m_callback )
            {
                m_callback->displayVector( bodyId, id, start, end, colour );
            }
        }
        catch( std::exception &Err )
        {
            FB_LOG_EXCEPTION( Err );
        }
    }

    void VehicleController::displayLocalVector( s32 bodyId, s32 id, const Vector3<real_Num> &start,
                                                const Vector3<real_Num> &end, u32 colour )
    {
        try
        {
            if( m_callback )
            {
                auto worldTransform = getWorldTransform();

                FB_ASSERT( worldTransform.isValid() );

                FB_ASSERT( start.isValid() );
                FB_ASSERT( end.isValid() );

                auto worldStart = worldTransform.transformPoint( start );
                auto worldEnd = worldTransform.transformPoint( end );

                FB_ASSERT( worldStart.isValid() );
                FB_ASSERT( worldEnd.isValid() );

                m_callback->displayVector( bodyId, id, start, end, colour );
            }
        }
        catch( std::exception &Err )
        {
            FB_LOG_EXCEPTION( Err );
        }
    }

    auto VehicleController::getDisplayDebugData() const -> bool
    {
        return m_displayDebugData;
    }

    void VehicleController::setDisplayDebugData( bool displayDebugData )
    {
        m_displayDebugData = displayDebugData;
    }

    void VehicleController::addForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc )
    {
        FB_ASSERT( Force.length() < 1e5 );
        FB_ASSERT( Loc.length() < 1e5 );

        // if (m_callback)
        //{
        //	m_callback->addForce(Bdy, Force, Loc);
        // }

        const Vector3<real_Num> centerOfMass = m_worldTransform.transformPoint( m_cg );
        const Vector3<real_Num> torque = ( Loc - centerOfMass ).crossProduct( Force );

        addForce( Force );
        addTorque( torque );
    }

    void VehicleController::addTorque( s32 bodyIdx, Vector3<real_Num> Torque )
    {
        FB_ASSERT( Torque.length() < 1e5 );

        // if (m_callback)
        //{
        //	m_callback->addTorque(Bdy, Torque);
        // }

        addTorque( Torque );
    }

    void VehicleController::addLocalForce( s32 bodyIdx, Vector3<real_Num> Force, Vector3<real_Num> Loc )
    {
        FB_ASSERT( Force.length() < 1e10 );
        FB_ASSERT( Loc.length() < 1e10 );

        // if (m_callback)
        //{
        //	m_callback->addLocalForce(Bdy, Force, Loc);
        // }

        const auto worldPoint = m_worldTransform.transformPoint( Loc );
        const auto force = m_worldTransform.transformVector( Force );

        if( auto body = getBody() )
        {
            const auto centerOfMass = body->getWorldCenterOfMass();
            const auto torque = ( worldPoint - centerOfMass ).crossProduct( force );

            addForce( force );
            addTorque( torque );
        }
    }

    void VehicleController::addLocalTorque( s32 bodyIdx, Vector3<real_Num> Torque )
    {
        FB_ASSERT( Torque.length() < 1e5 );

        // if (m_callback)
        //{
        //	m_callback->addLocalTorque(Bdy, Torque);
        // }

        auto torque = m_worldTransform.getOrientation() * Torque;
        addTorque( torque );
    }

    auto VehicleController::getPointVelocity( const Vector3<real_Num> &p ) -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getPointVelocity( p );
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getAngularVelocity() -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getAngularVelocity();
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getLinearVelocity() -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getLinearVelocity();
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getLocalAngularVelocity() -> Vector3<real_Num>
    {
        if( m_callback )
        {
            auto worldAngularVelocity = m_callback->getAngularVelocity();
            return m_worldTransform.inverseTransformVector( worldAngularVelocity );
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getLocalLinearVelocity() -> Vector3<real_Num>
    {
        if( m_callback )
        {
            return m_callback->getLocalLinearVelocity();
        }

        return Vector3<real_Num>::zero();
    }

    auto VehicleController::getVehicleCallback() -> SmartPtr<IVehicleCallback> &
    {
        return m_callback;
    }

    auto VehicleController::getVehicleCallback() const -> const SmartPtr<IVehicleCallback> &
    {
        return m_callback;
    }

    void VehicleController::setVehicleCallback( SmartPtr<IVehicleCallback> callback )
    {
        m_callback = callback;
    }

    auto VehicleController::getCG() const -> Vector3<real_Num>
    {
        return m_cg;
    }

    void VehicleController::setState( State state )
    {
        m_vehicleState = state;

        switch( state )
        {
        case State::EDIT:
        {
            for( auto &w : m_wheels )
            {
                w->setState( IVehicleComponent::State::EDIT );
            }
        }
        break;
        case State::PLAY:
        {
            for( auto &w : m_wheels )
            {
                w->setState( IVehicleComponent::State::PLAY );
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    auto VehicleController::getState() const -> IVehicleController::State
    {
        return m_vehicleState;
    }

    void VehicleController::addForce( const Vector3<real_Num> &force )
    {
        m_force += force;
    }

    void VehicleController::addTorque( const Vector3<real_Num> &torque )
    {
        m_torque += torque;
    }

    void VehicleController::clearForces()
    {
        m_force = Vector3<real_Num>::zero();
        m_torque = Vector3<real_Num>::zero();
    }

    auto VehicleController::getWheelController( u32 index ) const -> SmartPtr<IWheelController>
    {
        return m_wheels[index];
    }
}  // namespace fb
