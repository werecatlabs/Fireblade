#ifndef _ModelViewCameraCtrl_H
#define _ModelViewCameraCtrl_H

#include "FBCore/Scene/Components/Camera/CameraController.h"

namespace fb
{
    namespace scene
    {
        //
        class TargetViewCameraCtrl : public CameraController
        {
        public:
            TargetViewCameraCtrl();
            ~TargetViewCameraCtrl() override;

            void initialise( SmartPtr<render::ISceneNode> cameraSceneNode );
            void update() override;

            void setPosition( const Vector3F &position );
            Vector3F getPosition() const;

            void setTargetPosition( const Vector3F &position );
            Vector3F getTargetPosition() const;

            //
            // FSM functions
            //
            void OnEnterState( u8 state );
            void OnUpdateState();
            void OnLeaveState( u8 state );

        protected:
            Vector3F CalcSphericalCoords( const Vector3F &vector );
            Vector3F sphericalToCartesian( const Vector3F &coords );

            void WrapCoords( Vector3F &coords );

            Vector3F m_targetPosition;
            Vector3F m_curTargetPosition;

            Vector3F m_coords;
            Vector3F m_velocity;

            SmartPtr<render::ISceneNode> m_cameraSceneNode;
        };

    }  // namespace scene
}  // end namespace fb

#endif
