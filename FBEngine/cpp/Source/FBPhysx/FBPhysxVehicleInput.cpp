#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxVehicleInput.h>
#include <FBCore/Memory/Memory.h>
#include <PxPhysicsAPI.h>

namespace fb::physics
{
    PhysxVehicleInput::PhysxVehicleInput()
    {
        m_inputs = new physx::PxVehicleDrive4WRawInputData;
    }

    PhysxVehicleInput::~PhysxVehicleInput()
    {
        FB_SAFE_DELETE( m_inputs );
    }

    void PhysxVehicleInput::setDigitalAccel( const bool accelKeyPressed )
    {
        m_inputs->setDigitalAccel( accelKeyPressed );
    }

    void PhysxVehicleInput::setDigitalBrake( const bool brakeKeyPressed )
    {
        m_inputs->setDigitalBrake( brakeKeyPressed );
    }

    void PhysxVehicleInput::setDigitalHandbrake( const bool handbrakeKeyPressed )
    {
        m_inputs->setDigitalHandbrake( handbrakeKeyPressed );
    }

    void PhysxVehicleInput::setDigitalSteerLeft( const bool steerLeftKeyPressed )
    {
        //m_inputs->setDigitalSteerLeft(steerLeftKeyPressed);
        m_inputs->setDigitalSteerRight( steerLeftKeyPressed );
    }

    void PhysxVehicleInput::setDigitalSteerRight( const bool steerRightKeyPressed )
    {
        //m_inputs->setDigitalSteerRight(steerRightKeyPressed);
        m_inputs->setDigitalSteerLeft( steerRightKeyPressed );
    }

    auto PhysxVehicleInput::getDigitalAccel() const -> bool
    {
        return m_inputs->getDigitalAccel();
    }

    auto PhysxVehicleInput::getDigitalBrake() const -> bool
    {
        return m_inputs->getDigitalBrake();
    }

    auto PhysxVehicleInput::getDigitalHandbrake() const -> bool
    {
        return m_inputs->getDigitalHandbrake();
    }

    auto PhysxVehicleInput::getDigitalSteerLeft() const -> bool
    {
        return m_inputs->getDigitalSteerLeft();
    }

    auto PhysxVehicleInput::getDigitalSteerRight() const -> bool
    {
        return m_inputs->getDigitalSteerRight();
    }

    void PhysxVehicleInput::setAnalogAccel( const real_Num accel )
    {
        m_inputs->setAnalogAccel( accel );
    }

    void PhysxVehicleInput::setAnalogBrake( const real_Num brake )
    {
        m_inputs->setAnalogBrake( brake );
    }

    void PhysxVehicleInput::setAnalogHandbrake( const real_Num handbrake )
    {
        m_inputs->setAnalogHandbrake( handbrake );
    }

    void PhysxVehicleInput::setAnalogSteer( const real_Num steer )
    {
        m_inputs->setAnalogSteer( steer );
    }

    auto PhysxVehicleInput::getAnalogAccel() const -> real_Num
    {
        return m_inputs->getAnalogAccel();
    }

    auto PhysxVehicleInput::getAnalogBrake() const -> real_Num
    {
        return static_cast<real_Num>( 0.0 );
    }

    auto PhysxVehicleInput::getAnalogHandbrake() const -> real_Num
    {
        return static_cast<real_Num>( 0.0 );
    }

    auto PhysxVehicleInput::getAnalogSteer() const -> real_Num
    {
        return static_cast<real_Num>( 0.0 );
    }

    void PhysxVehicleInput::setGearUp( const bool gearUpKeyPressed )
    {
        m_inputs->setGearUp( gearUpKeyPressed );
    }

    void PhysxVehicleInput::setGearDown( const bool gearDownKeyPressed )
    {
        m_inputs->setGearDown( gearDownKeyPressed );
    }

    auto PhysxVehicleInput::getGearUp() const -> bool
    {
        return false;
    }

    auto PhysxVehicleInput::getGearDown() const -> bool
    {
        return false;
    }

    auto PhysxVehicleInput::getInputs() const -> physx::PxVehicleDrive4WRawInputData *
    {
        return m_inputs;
    }

    void PhysxVehicleInput::setInputs( physx::PxVehicleDrive4WRawInputData *val )
    {
        m_inputs = val;
    }

}  // namespace fb::physics
