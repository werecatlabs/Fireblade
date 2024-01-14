#ifndef ThirdPersonCameraControllerl_h__
#define ThirdPersonCameraControllerl_h__

#include <FBCore/Scene/Components/Camera/CameraController.h>

namespace fb
{
    namespace scene
    {

        /** Class for a third person camera controller.
         */
        class ThirdPersonCameraController : public CameraController
        {
        public:
            ThirdPersonCameraController();
            ~ThirdPersonCameraController() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void postUpdate() override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<scene::IActor> target;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ThirdPersonCameraController_h__
