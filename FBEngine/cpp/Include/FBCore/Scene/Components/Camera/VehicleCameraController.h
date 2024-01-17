#ifndef VehicleCameraController_h__
#define VehicleCameraController_h__

#include <FBCore/Scene/Components/Camera/CameraController.h>

namespace fb
{
    namespace scene
    {
        /** Class for a vehicle camera controller.
         */
        class VehicleCameraController : public CameraController
        {
        public:
            /** Constructor. */
            VehicleCameraController();

            /** Destructor. */
            ~VehicleCameraController() override;

            /** @copydoc CameraController::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc CameraController::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc CameraController::update */
            void update() override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IActor> getTarget() const;

            void setTarget( SmartPtr<IActor> target );

            real_Num getDistance() const;

            void setDistance( real_Num distance );

            real_Num getMinDistance() const;
            void setMinDistance( real_Num minDistance );

            real_Num getMaxDistance() const;
            void setMaxDistance( real_Num maxDistance );

            FB_CLASS_REGISTER_DECL;

        protected:
            // The camera target.
            SmartPtr<IActor> m_target;

            real_Num m_distance = 5.0f;

            real_Num m_minDistance = 0.0f;
            real_Num m_maxDistance = 10.0f;

            real_Num m_targetOffset = 1.0f;
            real_Num m_height = 1.5f;

            real_Num m_minHeight = 0.0f;
            real_Num m_maxHeight = 5.0f;

            real_Num m_heightDamping = 2.0f;

            real_Num m_lookAtHeight = 0.0f;

            /// Time taken to snap back to original rotation
            real_Num m_rotationSnapTime = 0.35f;

            real_Num m_minRotationSnapTime = 0.0f;
            real_Num m_maxRotationSnapTime = 3.0f;

            /// Time taken to snap back to the original distance
            real_Num m_distanceSnapTime = 1.5f;

            real_Num m_minDistanceSnapTime = 0.0f;
            real_Num m_maxDistanceSnapTime = 3.0f;

            /// Rate at which speed zoom occurs.
            real_Num distanceMultiplier = 0.025f;

            real_Num m_targetRotationAngle = 0.0f;
            real_Num m_targetHeight = 1.0f;

            real_Num currentRotationAngle = 45.0f;
            real_Num m_currentHeight = 1.0f;

            real_Num m_zoomSpeed = 10.0f;

            Vector3<real_Num> m_targetPosition;

            Vector3<real_Num> m_lastPosition = Vector3<real_Num>::zero();
            Vector3<real_Num> m_lastTarget = Vector3<real_Num>::zero();
            Vector3<real_Num> m_velocity = Vector3<real_Num>::zero();
        };
    }  // namespace scene
}  // namespace fb

#endif  // VehicleCameraController_h__
