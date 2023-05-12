#ifndef ICarController_h__
#define ICarController_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{

    /**
     * @brief Interface for a car controller.
     */
    class ICarController : public IVehicleController
    {
    public:
        /**
         * @brief Enumeration for car inputs.
         */
        enum class Input
        {
            THROTTLE, /**< The throttle input. */
            BRAKE,    /**< The brake input. */
            STEERING, /**< The steering input. */

            COUNT /**< The number of inputs. */
        };

        /** Destructor */
        ~ICarController() override = default;

        /**
         * @brief Gets the wheel controller for a given index.
         * @param index The index of the wheel controller to retrieve.
         * @return The wheel controller.
         */
        virtual SmartPtr<IWheelController> getWheelController( u32 index ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // ICarController_h__
