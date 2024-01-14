#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/VehicleController.h>
#include <FBCore/FBCore.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, VehicleController, Component );

    VehicleController::VehicleController() = default;

    VehicleController::~VehicleController() = default;

    void VehicleController::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void VehicleController::unload( SmartPtr<ISharedObject> data )
    {
        Component::unload( data );
    }
}  // namespace fb::scene
