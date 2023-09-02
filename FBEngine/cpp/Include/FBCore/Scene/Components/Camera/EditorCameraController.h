#ifndef __EditorCameraController_H
#define __EditorCameraController_H

#include <FBCore/Scene/Components/Camera/CameraController.h>

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

            bool OnEvent( const SmartPtr<IInputEvent> &event );

            void setPosition( const Vector3F &position );
            Vector3F getPosition() const;

            void setTargetPosition( const Vector3F &position );
            Vector3F getTargetPosition() const;

            void setOrientation( const QuaternionF &orientation );
            QuaternionF getOrientation() const;

            void setDirection( const Vector3F &direction );
            Vector3F getDirection() const;

            Vector3F getUp() const;
            Vector3F getRight() const;

            Ray3F getCameraToViewportRay( const Vector2F &screenPosition ) const;

            bool isInFrustum( const AABB3F &box ) const;

            void setPropertyValue( const String &name, const String &value );
            void setProperties( const Properties &propertyGroup );
            void getProperties( Properties &propertyGroup ) const;

            void addCamera( SmartPtr<render::ICamera> camera );
            bool removeCamera( SmartPtr<render::ICamera> camera );

            f32 getRotationSpeed() const;
            void setRotationSpeed( f32 val );

            bool getInvert() const;
            void setInvert( bool val );

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
                FPSCameraInputListener( EditorCameraController *fpsCameraCtrl );

                bool inputEvent( SmartPtr<IInputEvent> event ) ;
                bool updateEvent( const SmartPtr<IInputEvent> &event ) ;

                s32 getPriority() const ;
                void setPriority( s32 priority ) ;

            protected:
                EditorCameraController *m_fpsCameraCtrl;
            };

            bool isMouseKeyDown( s32 key );
            void allKeysUp();

            String m_name;

            Array<SmartPtr<render::ICamera>> m_cameras;

            Vector3F m_targetVector;
            Vector2F m_prevCursorPos;
            Vector2F m_cursorPos;
            Vector2F m_mousePos;
            Vector2F m_relativeMouse;

            Vector3F m_position;
            mutable Vector3F m_targetPosition;
            Vector3F m_rotation;

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
