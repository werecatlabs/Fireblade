#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/WheelController.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IRigidBody3.h>
#include <FBCore/Interface/Vehicle/IVehicleBody.h>
#include <FBCore/Physics/RaycastHit.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/ApplicationManager.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, WheelController, VehicleComponent<IWheelController> );

    WheelController::WheelController()
    {
        m_hit = fb::make_ptr<physics::RaycastHit>();
        m_hit->setCheckDynamic( false );
        m_hit->setCheckStatic( true );
    }

    WheelController::~WheelController() = default;

    void WheelController::update()
    {
        auto task = Thread::getCurrentTask();
        switch( task )
        {
        case Thread::Task::Physics:
        {
            auto state = getState();
            switch( state )
            {
            case State::AWAKE:
            {
                updateTransform();
                updateWheel();
            }
            break;
            case State::EDIT:
            {
                updateTransform();
                updateWheel();
            }
            break;
            case State::PLAY:
            {
                updateTransform();
                updateWheel();
            }
            break;
            }
        }
        break;
        }
    }

    void WheelController::updateWheel()
    {
        if( auto vehicle = getOwner() )
        {
            auto vehicleLocalTransform = vehicle->getLocalTransform();
            auto vehicleWorldTransform = vehicle->getWorldTransform();
            auto body = vehicle->getBody();

            auto localPos = m_localTransform.getPosition();
            auto pos = m_worldTransform.getPosition();
            auto up = vehicleWorldTransform.up();

            auto onGround = false;

            auto ray = Ray3( pos, -up );

            if( body->castWorldRay( ray, m_hit ) )
            {
                auto hitDistance = m_hit->getDistance();
                if( hitDistance > std::numeric_limits<real_Num>::epsilon() )
                {
                    if( hitDistance < m_suspensionDistance + m_radius )
                    {
                        onGround = true;
                    }
                }
            }

            if( onGround )
            {
                // Calculate the suspension compression and apply a force.
                const Vector3<real_Num> gravity( 0.0, 9.81, 0.0 );
                const auto mass = body->getMass();
                auto fullCompressionSpringForce =
                    mass * m_massFraction * static_cast<real_Num>( 2.0 ) * gravity.Y();

                auto compression = m_suspensionDistance - ( m_hit->getDistance() - m_radius );
                auto springForceMagnitude = compression * ( m_springForce + fullCompressionSpringForce );
                auto dampingForceMagnitude = -m_damping * m_wheelVelocity.y;
                auto totalForceMagnitude = springForceMagnitude + dampingForceMagnitude;

                // Apply the force to the car's Rigidbody.
                body->addLocalForceAtLocalPosition( totalForceMagnitude * Vector3<real_Num>::unitY(),
                                                    localPos );

                // Calculate and apply the tire friction.
                auto steeringAngle = getSteeringAngle();
                auto groundVelocity = body->getPointVelocity( m_worldTransform.getPosition() );
                groundVelocity = Quaternion<real_Num>::eulerDegrees( 0.0, steeringAngle, 0.0 ) *
                                 vehicleWorldTransform.inverseTransformVector( groundVelocity );

                auto frictionForce = -groundVelocity.normaliseCopy() * vehicle->getMass() * 10.0;
                frictionForce.X() *= 0.5;
                frictionForce.Y() = 0.0;
                frictionForce.Z() *= 0.001;

                body->addLocalForceAtLocalPosition( frictionForce, localPos );

                // Update the wheel's linear velocity.
                m_wheelVelocity = groundVelocity;
            }
        }
    }

    void WheelController::addTorque( real_Num torque )
    {
    }

    void WheelController::setTorque( real_Num torque )
    {
    }

    auto WheelController::getTorque() const -> real_Num
    {
        return 0.0f;
    }

    auto WheelController::getMass() const -> real_Num
    {
        return 0.0f;
    }

    void WheelController::setMass( real_Num mass )
    {
    }

    auto WheelController::getSpringRate() const -> real_Num
    {
        return 0.0f;
    }

    void WheelController::setSpringRate( real_Num springRate )
    {
    }

    auto WheelController::getRadius() const -> real_Num
    {
        return m_radius;
    }

    void WheelController::setRadius( real_Num radius )
    {
        m_radius = radius;
    }

    auto WheelController::getSuspensionTravel() const -> real_Num
    {
        return 0.0f;
    }

    void WheelController::setSuspensionTravel( real_Num suspensionTravel )
    {
    }

    auto WheelController::getDamping() const -> real_Num
    {
        return m_damping;
    }

    void WheelController::setDamping( real_Num damping )
    {
        m_damping = damping;
    }

    auto WheelController::getSuspensionDistance() const -> real_Num
    {
        return m_suspensionDistance;
    }

    void WheelController::setSuspensionDistance( real_Num suspensionDistance )
    {
        m_suspensionDistance = suspensionDistance;
    }

    auto WheelController::getSteeringAngle() const -> real_Num
    {
        return m_steeringAngle;
    }

    void WheelController::setSteeringAngle( real_Num steeringAngle )
    {
        m_steeringAngle = steeringAngle;
    }

    auto WheelController::isSteeringWheel() const -> bool
    {
        return m_isSteeringWheel;
    }

    void WheelController::setSteeringWheel( bool steeringWheel )
    {
        m_isSteeringWheel = steeringWheel;
    }
}  // namespace fb
