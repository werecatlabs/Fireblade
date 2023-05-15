#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxVehicleInput.h>
#include <FBCore/Memory/Memory.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
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

        bool PhysxVehicleInput::getDigitalAccel() const
        {
            return m_inputs->getDigitalAccel();
        }

        bool PhysxVehicleInput::getDigitalBrake() const
        {
            return m_inputs->getDigitalBrake();
        }

        bool PhysxVehicleInput::getDigitalHandbrake() const
        {
            return m_inputs->getDigitalHandbrake();
        }

        bool PhysxVehicleInput::getDigitalSteerLeft() const
        {
            return m_inputs->getDigitalSteerLeft();
        }

        bool PhysxVehicleInput::getDigitalSteerRight() const
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

        real_Num PhysxVehicleInput::getAnalogAccel() const
        {
            return m_inputs->getAnalogAccel();
        }

        real_Num PhysxVehicleInput::getAnalogBrake() const
        {
            return static_cast<real_Num>( 0.0 );
        }

        real_Num PhysxVehicleInput::getAnalogHandbrake() const
        {
            return static_cast<real_Num>( 0.0 );
        }

        real_Num PhysxVehicleInput::getAnalogSteer() const
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

        bool PhysxVehicleInput::getGearUp() const
        {
            return false;
        }

        bool PhysxVehicleInput::getGearDown() const
        {
            return false;
        }

        physx::PxVehicleDrive4WRawInputData *PhysxVehicleInput::getInputs() const
        {
            return m_inputs;
        }

        void PhysxVehicleInput::setInputs( physx::PxVehicleDrive4WRawInputData *val )
        {
            m_inputs = val;
        }

    }  // namespace physics
}  // namespace fb
