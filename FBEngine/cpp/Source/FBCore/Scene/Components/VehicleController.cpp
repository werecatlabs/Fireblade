#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/VehicleController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, VehicleController, Component );

        VehicleController::VehicleController()
        {
        }

        VehicleController::~VehicleController()
        {
        }

        void VehicleController::load( SmartPtr<ISharedObject> data )
        {
            Component::load( data );
        }

        void VehicleController::unload( SmartPtr<ISharedObject> data )
        {
            Component::unload( data );
        }
    }  // namespace scene




}  // end namespace fb
