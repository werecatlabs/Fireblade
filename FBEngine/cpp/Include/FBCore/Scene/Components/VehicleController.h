#ifndef VehicleController_h__
#define VehicleController_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        /** Used to control a vehicle.
         */
        class VehicleController : public Component
        {
        public:
            VehicleController();
            ~VehicleController() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // VehicleController_h__
