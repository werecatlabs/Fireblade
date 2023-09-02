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
            VehicleCameraController();
            ~VehicleCameraController() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

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
            SmartPtr<IActor> m_target;

            real_Num m_distance = 5.0f;

            real_Num m_minDistance = 0.0f;
            real_Num m_maxDistance = 10.0f;

            real_Num m_height = 1.5f;

            real_Num m_MinHeight = 0.0f;
            real_Num m_MaxHeight = 5.0f;

            real_Num m_heightDamping = 2.0f;

            real_Num m_lookAtHeight = 0.0f;

            /// Time taken to snap back to original rotation
            real_Num m_rotationSnapTime = 0.35f;

            real_Num m_MinRotationSnapTime = 0.0f;
            real_Num m_MaxRotationSnapTime = 3.0f;

            /// Time taken to snap back to the original distance
            real_Num m_distanceSnapTime = 1.5f;

            real_Num m_MinDistanceSnapTime = 0.0f;
            real_Num m_MaxDistanceSnapTime = 3.0f;

            /// Rate at which speed zoom occurs.
            real_Num distanceMultiplier = 0.025f;

            real_Num usedDistance;

            real_Num wantedRotationAngle;
            real_Num wantedHeight;

            real_Num currentRotationAngle = 45.0f;
            real_Num currentHeight;

            real_Num yVelocity = 0.0F;
            real_Num zVelocity = 0.0F;
            real_Num m_ZoomSpeed = 10.0f;

            Vector3<real_Num> lookAtVector;

            Quaternion<real_Num> currentRotation;
            Vector3<real_Num> wantedPosition;

            Vector3<real_Num> m_LastPosition = Vector3<real_Num>::zero();
            Vector3<real_Num> m_LastTarget = Vector3<real_Num>::zero();
            Vector3<real_Num> m_Velocity = Vector3<real_Num>::zero();
        };
    } // namespace scene
}     // namespace fb

#endif  // VehicleCameraController_h__
