#ifndef _SplineCameraCtrl_H
#define _SplineCameraCtrl_H

#include <FBCore/Scene/Components/Camera/CameraController.h>

namespace fb
{
    namespace scene
    {
        //
        /** Class for a camera controller that uses a spline.
         */
        class SplineCameraCtrl : public CameraController
        {
        public:
            enum class States
            {
                SCS_INITIALIZING,
                SCS_IN_GAME,

                SCS_COUNT
            };

            enum class SplineCameraAnimatorId
            {
                SCAI_DEPTH_VALUE,
                SCAI_OFFSET_POSITION,

                SCAI_COUNT
            };

            SplineCameraCtrl();
            ~SplineCameraCtrl() override;

            void initialise( SmartPtr<render::ICamera> camera,
                             SmartPtr<render::ISceneNode> cameraSceneNode );

            //
            // ICameraController
            //
            void update() override;

            void setPosition( const Vector3F &position );
            Vector3F getPosition() const;

            void setRotation( const Vector3F &rotation );
            Vector3F getRotation() const;

            void setTargetPosition( const Vector3F &position );
            Vector3F getTargetPosition() const;

            void setOrientation( const QuaternionF &orientation );
            QuaternionF getOrientation() const;

            void setDirection( const Vector3F &direction );
            Vector3F getDirection() const;

            Vector3F getUp() const;
            Vector3F getRight() const;

            void setPropertyValue( const String &name, const String &value );
            void setProperties( const Properties &propertyGroup );
            void getProperties( Properties &propertyGroup ) const;

            //
            // SplineCameraCtrl
            //
            /** */
            // void setProceduralSpline(const ProceduralSpline3fPtr& spline);

            /** */
            // ProceduralSpline3fPtr& getProceduralSpline() const;

            /** */
            void setDepthOffset( f32 offset );

            /** */
            f32 getDepthOffset() const;

            /** */
            void setOffsetPosition( const Vector2F &offsetPosition );

            /** */
            Vector2F getOffsetPosition() const;

            /** sets the current spline time. */
            void setSplineTime( f32 splineTime );

            /** Gets the current spline time. */
            f32 getSplineTime() const;

            /** sets this current camera as active. */
            void _OnsetActive( bool active );

            /** sets the current spline speed. */
            void setSplineSpeed( f32 speed );

            /** Gets the current spline speed. */
            f32 getSplineSpeed() const;

            /** sets the targets the camera should look at. */
            void setTarget( Vector3F vTargetPos );

            /** Allows the camera to be frozen in position. */
            void setUpdate( bool shouldUpdate );

            /** Returns a boolean value to see if the camera should be frozen in position. */
            bool shouldUpdate();

            /** sets the velocity of the camera. */
            void setVelocity( const Vector3F &velocity );

            /** Gets the velocity of the camera. */
            const Vector3F &getVelocity() const;

            /** Gets the distance traveled. */
            f32 getDistanceTravelled() const;

            /** sets the linear target. */
            void setLinearTarget( const Vector3F &linearTarget );

            /** Gets the linear target. */
            const Vector3F &getLinearTarget() const;

            /** sets the angular target. */
            void setAngularTarget( const Vector3F &angularTarget );

            /** Gets the angular target. */
            const Vector3F &getAngularTarget() const;

            /** sets the linear speed of the camera. */
            void setLinearSpeed( f32 speed );

            /** Gets the linear speed of the camera. */
            f32 getLinearSpeed() const;

            /** sets the angular speed o f the camera. */
            void setAngularSpeed( f32 speed );

            /** Gets the angular speed of the camera. */
            f32 getAngularSpeed() const;

            /** Selects the next node. */
            void selectNextNode();

            /** Select the previous node. */
            void selectPrevNode();

            /** Move to next node. */
            void nextNode();

            /** Move to the previous node. */
            void prevNode();

            /** sets whether the camera position should be updated. */
            void setShouldUpdateCameraPosition( bool status );

            /** Returns a boolean indicating whether camera position should be updated. */
            bool getShouldUpdateCameraPosition() const;

            /** sets the total length of time it should take to follow the spline*/
            void setTotalSplineFollowTime( f32 totalTime );

            /** Gets the total amount of time it takes to follow the spline. */
            f32 getTotalSplineFollowTime() const;

            /** Gets the current time along the spline. */
            f32 getCurSplineTime() const;

            /** Gets the current distance along the spline. */
            f32 getCurSplineDistance() const;

            /** Gets the total length of the spline. */
            f32 getTotalSplineDistance() const;

            /** sets the linear offset of the spline. */
            void setSplineOffset( const Vector3F &offset );

            /** Gets the offset of the camera from the spline. */
            const Vector3F &getSplineOffset() const;

            /** */
            FB_DEPRECATED( void setSplineName( const String &splineName ); )

        private:
            /** */
            void OnEnterIdleState();

            /** */
            void OnEnterFollowSplineState();

            /** */
            void UpdateIdleState();

            /** */
            void UpdateFollowSplineState();

            /** */
            void OnLeaveIdleState();

            /** */
            void OnLeaveFollowSplineState();

            /** */
            void updateFocus();

            SmartPtr<IFSM> m_fsm;

            ///
            // ProceduralSpline3fPtr m_proceduralSpline;

            ///
            SmartPtr<render::ISceneNode> m_cameraSceneNode;

            ///
            Vector3F m_offset;

            ///
            Vector3F m_position;

            ///
            Vector3F m_velocity;

            ///
            Vector3F m_targetPosition;

            Vector3F m_up;
            Vector3F m_right;
            Vector3F m_direction;

            ///
            QuaternionF m_orientation;

            /// The offset position of the camera
            Vector2F m_offsetPosition;

            ///
            f32 m_distanceFromTarget;

            ///
            f32 m_minDistanceFromTarget;

            ///
            f32 m_time;

            ///
            bool m_getStartPos;

            ///
            Vector3F m_startPos;

            ///
            SmartPtr<render::ISceneNode> m_sceneNode;

            ///
            f32 m_splineTime;

            ///
            f32 m_timeIncrement;

            ///
            u8 m_viewportId;

            f32 m_depthOffset;

            /// Animators
            Array<SmartPtr<IAnimator>> m_animators;
        };

    }  // namespace scene
}  // end namespace fb

#endif
