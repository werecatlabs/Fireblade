#ifndef _FPSCAMERACONTROLLER_H
#define _FPSCAMERACONTROLLER_H

#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Math/Ray3.h>

namespace fb
{
    namespace scene
    {

        class FpsCameraController : public CameraController
        {
        public:
            FpsCameraController();
            ~FpsCameraController() override;

            void update() override;

            bool handleInputEvent( const SmartPtr<IInputEvent> &event );

            void setPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getPosition() const;

            void setTargetPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getTargetPosition() const;

            void setOrientation( const Quaternion<real_Num> &orientation );
            Quaternion<real_Num> getOrientation() const;

            Vector3<real_Num> getDirection() const;

            Ray3F getCameraToViewportRay( const Vector2<real_Num> &screenPosition ) const;

            bool isInFrustum( const AABB3F &box ) const;

            void setPropertyValue( const String &name, const String &value );
            void setProperties( const Properties &propertyGroup );
            void getProperties( Properties &propertyGroup ) const;

            void addCamera( SmartPtr<render::ICamera> camera );
            bool removeCamera( SmartPtr<render::ICamera> camera );

            FB_CLASS_REGISTER_DECL;

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

            Array<SmartPtr<render::ICamera>> m_cameras;
            SmartPtr<render::ICamera> m_selectedCamera;
            Vector3<real_Num> TargetVector;
            Vector2<real_Num> PrevCursor;
            Vector2<real_Num> CursorPos;
            Vector2<real_Num> MousePos;

            Vector3<real_Num> m_position;
            mutable Vector3<real_Num> m_targetPosition;
            Vector3<real_Num> m_targetDirection;

            mutable Quaternion<real_Num> m_orientation;

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
