#ifndef _FOLLOWSPLINECAMERA_H
#define _FOLLOWSPLINECAMERA_H

#include "FBCore/Scene/Components/Camera/CameraController.h"
#include "FBCore/Math/LinearSpline3.h"
#include "FBCore/Math/RotationalSpline3.h"

namespace fb
{
    namespace scene
    {
        
        /**
         */
        class FollowSplineCamera : public CameraController
        {
        public:
            FollowSplineCamera();
            ~FollowSplineCamera() override;

            /** */
            void initialize();

            /** */
            void update( u32 taskId, f64 t, f64 dt );

            /** */
            void setPosition( const Vector3F &position );

            /** */
            Vector3F getPosition() const;

            /** */
            void setTargetPosition( const Vector3F &position );

            /** */
            Vector3F getTargetPosition() const;

            /** */
            void setOrientation( const QuaternionF &orientation );

            /** */
            QuaternionF getOrientation() const;

            /** */
            Vector3F getDirection() const;

            /** */
            void setPropertyValue( const String &name, const String &value );

            /** */
            void setProperties( const Properties &propertyGroup );

            /** */
            void getProperties( Properties &propertyGroup ) const;

            /** */
            void startFollowingSpline( LinearSpline3F &linearSpline,
                                       RotationalSpline3F &rotationSpline );

            /** */
            void stopFollowingSpline();

            /** */
            void selectNextNode();

            /** */
            void nextNode();

            /** */
            void prevNode();

            /** */
            Ray3F getCameraToViewportRay( const Vector2F &screenPosition ) const;

            /** */
            const Properties &getPropertyGroup() const;

            /** */
            void setTranslationSpeed( f32 speed );

            /** */
            f32 getTranslationSpeed() const;

            /** */
            void setRotationSpeed( f32 speed );

            /** */
            f32 getRotationSpeed() const;

            /** */
            void setTotalSplineFollowTime( f32 totalTime );

            /** */
            f32 getTotalSplineFollowTime() const;

            /** */
            f32 getCurSplineTime() const;

            /** */
            f32 getCurSplineDistance() const;

            /** */
            f32 getTotalSplineDistance() const;

        private:
            ///
            String m_name;

            ///
            Vector3F m_position;

            /// The camera rotation in degrees.
            Vector3F m_rotation;

            ///
            Vector3F m_linearVelocity;

            ///
            Vector3F m_angularVelocity;

            ///
            Vector3F m_targetPosition;

            ///
            Vector3F m_targetRotation;

            ///
            mutable QuaternionF m_orientation;

            ///
            Array<Vector3F> m_ctrlPoints;

            ///
            LinearSpline3F m_linearSpline;

            ///
            RotationalSpline3F m_rotationSpline;

            ///
            s32 m_curNodeIdx;

            ///
            Properties m_propertyGroup;

            ///
            f32 m_translationSpeed;

            ///
            f32 m_rotationSpeed;

            ///
            f32 m_totalSplineFollowTime;

            ///
            f32 m_splineTime;

            ///
            f32 m_totalSplineDistance;

            ///
            bool m_followSpline;
        };

    }  // namespace scene
}  // end namespace fb

#endif
