#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/WheelController.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IRigidbody3.h>
#include <FBCore/Interface/Vehicle/IVehicleBody.h>
#include <FBCore/Interface/Vehicle/IWheelController.h>
#include "FBCore/Physics/RaycastHit.h"
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/ApplicationManager.h>

namespace fb
{
    WheelController::WheelController()
    {
        hit = fb::make_ptr<physics::RaycastHit>();
        hit->setCheckDynamic( false );
        hit->setCheckStatic( true );
    }

    WheelController::~WheelController() = default;

    void WheelController::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

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
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        auto vehicle = getOwner();
        if( vehicle )
        {
            auto vehicleLocalTransform = vehicle->getLocalTransform();
            auto vehicleWorldTransform = vehicle->getWorldTransform();
            auto body = vehicle->getBody();

            auto vehiclePos = vehicleWorldTransform.getPosition();

            auto localPos = m_localTransform.getPosition();
            auto pos = m_worldTransform.getPosition();
            auto up = vehicleWorldTransform.up();

            auto onGround = false;

            auto ray = Ray3<real_Num>( pos, -up );

            if( body->castWorldRay( ray, hit ) )
            {
                auto hitDistance = hit->getDistance();
                if( hitDistance > std::numeric_limits<real_Num>::epsilon() )
                {
                    if( hitDistance < suspensionDistance + radius )
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
                    mass * massFraction * static_cast<real_Num>( 2.0 ) * gravity.Y();

                auto compression = suspensionDistance - ( hit->getDistance() - radius );
                auto springForceMagnitude = compression * ( springForce + fullCompressionSpringForce );
                auto dampingForceMagnitude = -damping * wheelVelocity.y;
                auto totalForceMagnitude = springForceMagnitude + dampingForceMagnitude;

                // Apply the force to the car's Rigidbody.
                body->addLocalForceAtLocalPosition( totalForceMagnitude * Vector3F::unitY(), localPos );

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
                wheelVelocity = groundVelocity;
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
        return 0.0f;
    }

    void WheelController::setRadius( real_Num radius )
    {
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
        return 0.0f;
    }

    void WheelController::setDamping( real_Num damping )
    {
    }

    auto WheelController::getSuspensionDistance() const -> real_Num
    {
        return 0.0f;
    }

    void WheelController::setSuspensionDistance( real_Num suspensionDistance )
    {
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
