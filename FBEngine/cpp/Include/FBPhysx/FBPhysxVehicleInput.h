#ifndef FBPhysxVehicleInput_h__
#define FBPhysxVehicleInput_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsVehicleInput3.h>
#include <FBCore/Memory/SharedObject.h>

namespace physx
{
    class PxVehicleDrive4WRawInputData;
}

namespace fb::physics
{
    class PhysxVehicleInput : public SharedObject<IPhysicsVehicleInput3>
    {
    public:
        PhysxVehicleInput();
        ~PhysxVehicleInput() override;

        void setDigitalAccel( bool accelKeyPressed ) override;

        void setDigitalBrake( bool brakeKeyPressed ) override;

        void setDigitalHandbrake( bool handbrakeKeyPressed ) override;

        void setDigitalSteerLeft( bool steerLeftKeyPressed ) override;

        void setDigitalSteerRight( bool steerRightKeyPressed ) override;

        bool getDigitalAccel() const override;

        bool getDigitalBrake() const override;

        bool getDigitalHandbrake() const override;

        bool getDigitalSteerLeft() const override;

        bool getDigitalSteerRight() const override;

        void setAnalogAccel( real_Num accel ) override;

        void setAnalogBrake( real_Num brake ) override;

        void setAnalogHandbrake( real_Num handbrake ) override;

        void setAnalogSteer( real_Num steer ) override;

        real_Num getAnalogAccel() const override;

        real_Num getAnalogBrake() const override;

        real_Num getAnalogHandbrake() const override;

        real_Num getAnalogSteer() const override;

        void setGearUp( bool gearUpKeyPressed ) override;

        void setGearDown( bool gearDownKeyPressed ) override;

        bool getGearUp() const override;

        bool getGearDown() const override;

        physx::PxVehicleDrive4WRawInputData *getInputs() const;
        void setInputs( physx::PxVehicleDrive4WRawInputData *val );

    protected:
        physx::PxVehicleDrive4WRawInputData *m_inputs;
    };
}  // namespace fb::physics

#endif  // FBPhysxVehicleInput_h__
