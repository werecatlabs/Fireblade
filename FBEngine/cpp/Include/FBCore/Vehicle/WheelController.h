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
        WheelController();

        ~WheelController();

        void update();

        void updateWheel();

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

        bool isSteeringWheel() const override;

        void setSteeringWheel( bool steeringWheel ) override;

    protected:
        SmartPtr<physics::IRaycastHit> hit;

        real_Num radius = 0.35f;  // The radius of the wheel.
        real_Num suspensionDistance = 0.52f;  // The suspension distance.
        real_Num springForce = 5000.0f;  // The suspension spring force.
        real_Num damping = 1000.0f;  // The suspension damping.
        real_Num massFraction = 0.25f;

        real_Num angularVelocity;

        real_Num m_steeringAngle = static_cast<real_Num>( 0.0 );

        Vector3<real_Num> wheelVelocity;  // The linear velocity of the wheel.

        bool m_isSteeringWheel = false;

    };

}  // namespace fb

#endif  // WheelController_h__
