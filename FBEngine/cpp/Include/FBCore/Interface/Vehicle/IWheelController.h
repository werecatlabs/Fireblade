#ifndef IWheelController_h__
#define IWheelController_h__

#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{

    /**
     * @brief Interface for a vehicle wheel.
     */
    class IWheelController : public IVehicleComponent
    {
    public:
        /**
         * @brief Destructor for IWheelController.
         */
        ~IWheelController() override = default;

        /**
         * @brief Adds torque to the wheel.
         * @param torque The amount of torque to add.
         */
        virtual void addTorque( real_Num torque ) = 0;

        /**
         * @brief Sets the torque of the wheel.
         * @param torque The new torque value.
         */
        virtual void setTorque( real_Num torque ) = 0;

        /**
         * @brief Gets the torque of the wheel.
         * @return The current torque value.
         */
        virtual real_Num getTorque() const = 0;

        /**
         * @brief Gets the mass of the wheel.
         * @return The mass of the wheel.
         */
        virtual real_Num getMass() const = 0;

        /**
         * @brief Sets the mass of the wheel.
         * @param mass The new mass value.
         */
        virtual void setMass( real_Num mass ) = 0;

        /**
         * @brief Gets the spring rate of the wheel.
         * @return The spring rate of the wheel.
         */
        virtual real_Num getSpringRate() const = 0;

        /**
         * @brief Sets the spring rate of the wheel.
         * @param springRate The new spring rate value.
         */
        virtual void setSpringRate( real_Num springRate ) = 0;

        /**
         * @brief Gets the radius of the wheel.
         * @return The radius of the wheel.
         */
        virtual real_Num getRadius() const = 0;

        /**
         * @brief Sets the radius of the wheel.
         * @param radius The new radius value.
         */
        virtual void setRadius( real_Num radius ) = 0;

        /**
         * @brief Gets the suspension travel of the wheel.
         * @return The suspension travel of the wheel.
         */
        virtual real_Num getSuspensionTravel() const = 0;

        /**
         * @brief Sets the suspension travel of the wheel.
         * @param suspensionTravel The new suspension travel value.
         */
        virtual void setSuspensionTravel( real_Num suspensionTravel ) = 0;

        /**
         * @brief Gets the damping of the wheel.
         * @return The damping of the wheel.
         */
        virtual real_Num getDamping() const = 0;

        /**
         * @brief Sets the damping of the wheel.
         * @param damping The new damping value.
         */
        virtual void setDamping( real_Num damping ) = 0;

        /**
         * @brief Gets the suspension distance of the wheel.
         * @return The suspension distance of the wheel.
         */
        virtual real_Num getSuspensionDistance() const = 0;

        /**
         * @brief Sets the suspension distance of the wheel.
         * @param suspensionDistance The new suspension distance value.
         */
        virtual void setSuspensionDistance( real_Num suspensionDistance ) = 0;

        /**
         * @brief Gets the steering angle of the wheel.
         * @return The steering angle of the wheel.
         */
        virtual real_Num getSteeringAngle() const = 0;

        /**
         * @brief Sets the steering angle of the wheel.
         * @param steeringAngle The new steering angle value.
         */
        virtual void setSteeringAngle( real_Num steeringAngle ) = 0;

        /**
         * @brief Returns whether or not the wheel is a steering wheel.
         * @return True if the wheel is a steering wheel.
         */
        virtual bool isSteeringWheel() const = 0;

        /**
         * @brief Sets whether or not the wheel is a steering wheel.
         * @param steeringWheel True if the wheel is a steering wheel, false otherwise.
         */
        virtual void setSteeringWheel( bool steeringWheel ) = 0;
    };

}  // end namespace fb

#endif  // IWheel_h__
