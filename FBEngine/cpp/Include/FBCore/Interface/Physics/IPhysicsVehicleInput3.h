#ifndef IPhysicsVehicleInput3_h__
#define IPhysicsVehicleInput3_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        class IPhysicsVehicleInput3 : public ISharedObject
        {
        public:
            ~IPhysicsVehicleInput3() override = default;

            /**
            \brief Record if the accel button has been pressed on keyboard.
            */
            virtual void setDigitalAccel( bool accelKeyPressed ) = 0;

            /**
            \brief Record if the brake button has been pressed on keyboard.
            */
            virtual void setDigitalBrake( bool brakeKeyPressed ) = 0;

            /**
            \brief Record if the handbrake button has been pressed on keyboard.
            */
            virtual void setDigitalHandbrake( bool handbrakeKeyPressed ) = 0;

            /**
            \brief Record if the left steer button has been pressed on keyboard.
            */
            virtual void setDigitalSteerLeft( bool steerLeftKeyPressed ) = 0;

            /**
            \brief Record if the right steer button has been pressed on keyboard.
            */
            virtual void setDigitalSteerRight( bool steerRightKeyPressed ) = 0;

            /**
            \brief Return if the accel button has been pressed on keyboard.
            */
            virtual bool getDigitalAccel() const = 0;

            /**
            \brief Return if the brake button has been pressed on keyboard.
            */
            virtual bool getDigitalBrake() const = 0;

            /**
            \brief Return if the handbrake button has been pressed on keyboard.
            */
            virtual bool getDigitalHandbrake() const = 0;

            /**
            \brief Return if the left steer button has been pressed on keyboard.
            */
            virtual bool getDigitalSteerLeft() const = 0;

            /**
            \brief Return if the right steer button has been pressed on keyboard.
            */
            virtual bool getDigitalSteerRight() const = 0;

            /**
            \brief Set the analog accel value from the gamepad
            */
            virtual void setAnalogAccel( real_Num accel ) = 0;

            /**
            \brief Set the analog brake value from the gamepad
            */
            virtual void setAnalogBrake( real_Num brake ) = 0;

            /**
            \brief Set the analog handbrake value from the gamepad
            */
            virtual void setAnalogHandbrake( real_Num handbrake ) = 0;

            /**
            \brief Set the analog steer value from the gamepad
            */
            virtual void setAnalogSteer( real_Num steer ) = 0;

            /**
            \brief Return the analog accel value from the gamepad
            */
            virtual real_Num getAnalogAccel() const = 0;

            /**
            \brief Return the analog brake value from the gamepad
            */
            virtual real_Num getAnalogBrake() const = 0;

            /**
            \brief Return the analog handbrake value from the gamepad
            */
            virtual real_Num getAnalogHandbrake() const = 0;

            /**
            \brief Return the analog steer value from the gamepad
            */
            virtual real_Num getAnalogSteer() const = 0;

            /**
            \brief Record if the gearup button has been pressed on keyboard or gamepad
            */
            virtual void setGearUp( bool gearUpKeyPressed ) = 0;

            /**
            \brief Record if the geardown button has been pressed on keyboard or gamepad
            */
            virtual void setGearDown( bool gearDownKeyPressed ) = 0;

            /**
            \brief Return if the gearup button has been pressed on keyboard or gamepad
            */
            virtual bool getGearUp() const = 0;

            /**
            \brief Record if the geardown button has been pressed on keyboard or gamepad
            */
            virtual bool getGearDown() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsVehicleInput3_h__
