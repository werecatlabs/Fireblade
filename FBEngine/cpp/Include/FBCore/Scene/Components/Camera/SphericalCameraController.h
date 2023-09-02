#ifndef SphericalCamera_h__
#define SphericalCamera_h__

#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace scene
    {
        class SphericalCameraController : public CameraController
        {
        public:
            static const f32 DEFAULT_FAR_DISTANCE;
            static const f32 DEFAULT_NEAR_DISTANCE;
            static const f32 DEFAULT_ZOOM_SPEED;
            static const f32 DEFAULT_MOVE_SPEED;

            static const Vector3F DEFAULT_SPHERICAL_COORDS;

            SphericalCameraController();
            ~SphericalCameraController() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::update */
            void update() override;

            bool handleEvent( const SmartPtr<IInputEvent> &event );

            Vector3<real_Num> getSphericalCoords() const;
            void setSphericalCoords( const Vector3<real_Num> &val );

            void setPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getPosition() const;

            void setTargetPosition( const Vector3<real_Num> &position );
            Vector3<real_Num> getTargetPosition() const;

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

            f32 getRotationSpeed() const;
            void setRotationSpeed( f32 val );

            f32 getMaxDistance() const;
            void setMaxDistance( f32 val );

            void focusSelection();

            SmartPtr<ui::IUIWindow> getUiWindow() const;
            void setUiWindow( SmartPtr<ui::IUIWindow> uiWindow );

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

            class InputListener : public IEventListener
            {
            public:
                InputListener( SphericalCameraController *owner );

                bool inputEvent( SmartPtr<IInputEvent> event ) ;
                bool updateEvent( const SmartPtr<IInputEvent> &event ) ;

                s32 getPriority() const ;
                void setPriority( s32 priority ) ;

            protected:
                SphericalCameraController *m_owner = nullptr;
            };

            bool isMouseBtnDown( s32 key );
            void allKeysUp();

            Vector3<real_Num> getCartCoords( Vector3<real_Num> &spherical );
            Vector3F getSphericalCoords( Vector3F &cartCoords );
            void wrapSphericalCoords( Vector3<real_Num> &spherical );

            SmartPtr<ui::IUIWindow> m_uiWindow;

            String m_name;

            Vector3<real_Num> TargetVector;
            Vector2<real_Num> PrevCursor;
            Vector2<real_Num> CursorPos;
            Vector2<real_Num> MousePos;
            Vector2<real_Num> m_relativeMouse;

            Vector3<real_Num> m_target;
            Vector3<real_Num> m_spherical;
            Vector3<real_Num> m_sphericalForce;
            Vector3<real_Num> m_position;
            mutable Vector3<real_Num> m_targetPosition;
            Vector3<real_Num> m_rotation;

            f32 m_rotationSpeed;

            /// The camera far distance.
            f32 m_maxDistance;

            /// The camera near distance.
            f32 m_nearDistance;

            /// The zoom speed.
            f32 m_zoomSpeed;

            /// The move speed.
            f32 m_moveSpeed;

            f32 m_translationSpeed;

            Array<bool> m_mouseKeys;
            Array<bool> m_cursorKeys;

            Array<SCamKeyMap> m_keyMap;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // SphericalCamera_h__
