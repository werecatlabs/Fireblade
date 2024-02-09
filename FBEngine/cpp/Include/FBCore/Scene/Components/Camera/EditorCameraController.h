#ifndef __EditorCameraController_H
#define __EditorCameraController_H

#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Math/Ray3.h>

namespace fb
{
    namespace scene
    {

        /** Component that controls an editor camera in the scene.
         */
        class EditorCameraController : public CameraController
        {
        public:
            EditorCameraController();
            ~EditorCameraController() override;

            void update() override;

            bool handleInputEvent( const SmartPtr<IInputEvent> &event );

            void setPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getPosition() const;

            void setTargetPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getTargetPosition() const;

            void setOrientation( const Quaternion<real_Num> &orientation );
            Quaternion<real_Num> getOrientation() const;

            void setDirection( const Vector3<real_Num> &direction );
            Vector3<real_Num> getDirection() const;

            Vector3<real_Num> getUp() const;
            Vector3<real_Num> getRight() const;

            void addCamera( SmartPtr<render::ICamera> camera );
            bool removeCamera( SmartPtr<render::ICamera> camera );

            f32 getRotationSpeed() const;
            void setRotationSpeed( f32 val );

            bool getInvert() const;
            void setInvert( bool val );

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

            class FPSCameraInputListener : public IEventListener
            {
            public:
                explicit FPSCameraInputListener( EditorCameraController *fpsCameraCtrl );

                bool inputEvent( SmartPtr<IInputEvent> event );
                bool updateEvent( const SmartPtr<IInputEvent> &event );

                s32 getPriority() const;
                void setPriority( s32 priority );

            protected:
                EditorCameraController *m_fpsCameraCtrl;
            };

            bool isMouseKeyDown( s32 key );
            void allKeysUp();

            String m_name;

            Array<SmartPtr<render::ICamera>> m_cameras;

            Vector3<real_Num> m_targetVector;
            Vector2<real_Num> m_prevCursorPos;
            Vector2<real_Num> m_cursorPos;
            Vector2<real_Num> m_mousePos;
            Vector2<real_Num> m_relativeMouse;

            Vector3<real_Num> m_position;
            mutable Vector3<real_Num> m_targetPosition;
            Vector3<real_Num> m_rotation;

            f32 m_moveSpeed;
            f32 m_rotationSpeed;

            f32 m_translationSpeed;

            bool m_bInvert;

            Array<bool> m_mouseKeys;
            Array<bool> m_cursorKeys;

            Array<SCamKeyMap> m_keyMap;

            Properties m_propertyGroup;
        };

    }  // namespace scene
}  // namespace fb

#endif
