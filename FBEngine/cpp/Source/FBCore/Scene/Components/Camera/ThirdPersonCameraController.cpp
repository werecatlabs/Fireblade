#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/ThirdPersonCameraController.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, ThirdPersonCameraController, CameraController );

    ThirdPersonCameraController::ThirdPersonCameraController() = default;

    ThirdPersonCameraController::~ThirdPersonCameraController()
    {
        unload( nullptr );
    }

    void ThirdPersonCameraController::load( SmartPtr<ISharedObject> data )
    {
    }

    void ThirdPersonCameraController::unload( SmartPtr<ISharedObject> data )
    {
    }

    void ThirdPersonCameraController::postUpdate()
    {
    }

    auto ThirdPersonCameraController::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = CameraController::getChildObjects();
        return objects;
    }

    auto ThirdPersonCameraController::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = CameraController::getProperties();
        return properties;
    }

    void ThirdPersonCameraController::setProperties( SmartPtr<Properties> properties )
    {
        CameraController::setProperties( properties );
    }
}  // namespace fb::scene
