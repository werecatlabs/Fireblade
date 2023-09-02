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

            float distance = 5.0f;

            float m_MinDistance = 0.0f;
            float m_MaxDistance = 10.0f;

            float height = 1.5f;

            float m_MinHeight = 0.0f;
            float m_MaxHeight = 5.0f;

            float heightDamping = 2.0f;

            float lookAtHeight = 0.0f;

            float rotationSnapTime = 0.35f;  // Time taken to snap back to original rotation

            float m_MinRotationSnapTime = 0.0f;
            float m_MaxRotationSnapTime = 3.0f;

            float distanceSnapTime = 1.5f;  // Time taken to snap back to the original distance

            float m_MinDistanceSnapTime = 0.0f;
            float m_MaxDistanceSnapTime = 3.0f;

            float distanceMultiplier = 0.025f;  // Rate at which speed zoom occurs.

            float usedDistance;

            float wantedRotationAngle;
            float wantedHeight;

            float currentRotationAngle = 45.0f;
            float currentHeight;

            float yVelocity = 0.0F;
            float zVelocity = 0.0F;
            float m_ZoomSpeed = 10.0f;

            Vector3<real_Num> lookAtVector;

            Quaternion<real_Num> currentRotation;
            Vector3<real_Num> wantedPosition;

            Vector3<real_Num> m_LastPosition = Vector3<real_Num>::zero();
            Vector3<real_Num> m_Velocity = Vector3<real_Num>::zero();
        };
    }  // namespace scene
}  // namespace fb

#endif  // ThirdPersonCameraController_h__
