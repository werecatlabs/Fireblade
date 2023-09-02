#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/ThirdPersonCameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, ThirdPersonCameraController, CameraController );

        ThirdPersonCameraController::ThirdPersonCameraController()
        {
        }

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

        Array<SmartPtr<ISharedObject>> ThirdPersonCameraController::getChildObjects() const
        {
            auto objects = CameraController::getChildObjects();
            return objects;
        }

        SmartPtr<Properties> ThirdPersonCameraController::getProperties() const
        {
            auto properties = CameraController::getProperties();
            return properties;
        }

        void ThirdPersonCameraController::setProperties( SmartPtr<Properties> properties )
        {
            CameraController::setProperties( properties );
        }
    }  // namespace scene
}  // namespace fb
