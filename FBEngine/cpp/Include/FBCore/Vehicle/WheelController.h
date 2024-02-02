#ifndef __WheelController_h__
#define __WheelController_h__

#include <FBCore/Interface/Vehicle/IWheelController.h>
#include <FBCore/Vehicle/VehicleComponent.h>

namespace fb
{
    /** Simple wheel controller implementation. */
    class WheelController : public VehicleComponent<IWheelController>
    {
    public:
        /** Constructor. */
        WheelController();

        /** Destructor. */
        ~WheelController() override;

        void update() override;

        void addTorque( real_Num torque ) override;

        void setTorque( real_Num torque ) override;

        real_Num getTorque() const override;

        real_Num getMass() const override;

        void setMass( real_Num mass ) override;

        real_Num getSpringRate() const override;

        void setSpringRate( real_Num springRate ) override;

        real_Num getRadius() const override;

        void setRadius( real_Num radius ) override;

        real_Num getSuspensionTravel() const override;

        void setSuspensionTravel( real_Num suspensionTravel ) override;

        real_Num getDamping() const override;

        void setDamping( real_Num damping ) override;

        real_Num getSuspensionDistance() const override;

        void setSuspensionDistance( real_Num suspensionDistance ) override;

        real_Num getSteeringAngle() const override;

        void setSteeringAngle( real_Num steeringAngle ) override;

        /** @copydoc IWheelController::isSteeringWheel */
        bool isSteeringWheel() const override;

        /** @copydoc IWheelController::setSteeringWheel */
        void setSteeringWheel( bool steeringWheel ) override;

    protected:
        void updateWheel();

        // The ray cast hit of the wheel.
        SmartPtr<physics::IRaycastHit> m_hit;

        // The radius of the wheel.
        real_Num m_radius = static_cast<real_Num>( 0.35 );

        // The suspension distance.
        real_Num m_suspensionDistance = static_cast<real_Num>( 0.52 );

        // The suspension spring force.
        real_Num m_springForce = static_cast<real_Num>( 5000.0 );

        // The suspension damping.
        real_Num m_damping = static_cast<real_Num>( 1000.0 );

        // The mass fraction of the wheel.
        real_Num m_massFraction = static_cast<real_Num>( 0.25 );

        // The angular velocity of the wheel.
        real_Num m_angularVelocity = static_cast<real_Num>( 0.0 );

        // The steering angle of the wheel.
        real_Num m_steeringAngle = static_cast<real_Num>( 0.0 );

        // The linear velocity of the wheel.
        Vector3<real_Num> m_wheelVelocity;

        // To know if the wheel is a steering wheel.
        bool m_isSteeringWheel = false;
    };
}  // namespace fb

#endif  // WheelController_h__
