#ifndef _FPSCAMERACONTROLLER_H
#define _FPSCAMERACONTROLLER_H

#include "FBCore/Scene/Components/Camera/CameraController.h"

namespace fb
{
    namespace scene
    {
        
        class FpsCameraController : public CameraController
        {
        public:
            FpsCameraController();
            ~FpsCameraController() override;

            void initialise();

            void update() override;

            bool OnEvent( const SmartPtr<IInputEvent> &event );

            void setPosition( const Vector3F &position );
            Vector3F getPosition() const;

            void setTargetPosition( const Vector3F &position );
            Vector3F getTargetPosition() const;

            void setOrientation( const QuaternionF &orientation );
            QuaternionF getOrientation() const;

            Vector3F getDirection() const;

            Ray3F getCameraToViewportRay( const Vector2F &screenPosition ) const;

            bool isInFrustum( const AABB3F &box ) const;

            void setPropertyValue( const String &name, const String &value );
            void setProperties( const Properties &propertyGroup );
            void getProperties( Properties &propertyGroup ) const;

            void addCamera( SmartPtr<render::ICamera> camera );
            bool removeCamera( SmartPtr<render::ICamera> camera );

        private:
            struct SCamKeyMap
            {
                SCamKeyMap();
                ;
                SCamKeyMap( s32 a, s32 k, s32 k1 );

                s32 action;
                s32 keycode;
                s32 keycode1;
            };

            bool isMouseKeyDown( s32 key );
            void allKeysUp();

            String m_name;

            Array<SmartPtr<render::ICamera>> m_cameras;
            SmartPtr<render::ICamera> m_selectedCamera;
            Vector3F TargetVector;
            Vector2F PrevCursor;
            Vector2F CursorPos;
            Vector2F MousePos;

            Vector3F m_position;
            mutable Vector3F m_targetPosition;
            Vector3F m_targetDirection;

            mutable QuaternionF m_orientation;

            f32 MoveSpeed;
            f32 m_rotationSpeed;
            f32 m_translationSpeed;

            Array<bool> MouseKeys;
            Array<bool> CursorKeys;

            Array<SCamKeyMap> KeyMap;

            Properties m_propertyGroup;
        };

    }  // namespace scene
}  // end namespace fb

#endif
