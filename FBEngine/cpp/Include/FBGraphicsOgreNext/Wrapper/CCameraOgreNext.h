#ifndef _CCamera_H
#define _CCamera_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    namespace render
    {
        class CCameraOgreNext : public CGraphicsObjectOgreNext<ICamera>
        {
        public:
            CCameraOgreNext();
            CCameraOgreNext( CSceneManagerOgreNext *creator );
            ~CCameraOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Camera *camera );

            void setViewport( SmartPtr<IViewport> viewport ) override;
            SmartPtr<IViewport> getViewport() const override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            //
            // IFrustum functions
            //
            void setNearClipDistance( f32 nearDist ) override;
            f32 getNearClipDistance() const override;

            void setFarClipDistance( f32 farDist ) override;
            f32 getFarClipDistance() const override;

            void setAspectRatio( f32 ratio ) override;
            inline f32 getAspectRatio() const override;

            bool isVisible( const AABB3F &bound ) const override;
            bool isVisible( const Sphere3F &bound ) const override;
            bool isVisible( const Vector3F &vert ) const override;

            //
            // ICamera functions
            //
            void setPosition( const Vector3F &position ) override;
            Vector3F getPosition() const override;

            void setDirection( const Vector3F &vec );
            Vector3F getDirection() const override;
            Vector3F getDirection( const Vector2F &screenPosition ) const override;
            Vector3F getDirection( const Vector2F &screenPosition,
                                   Vector3F &worldPosition ) const override;

            void setOrientation( const QuaternionF &q ) override;
            QuaternionF getOrientation() const override;

            Vector3F getUp() const override;
            Vector3F getRight() const override;

            void lookAt( const Vector3F &targetPoint );

            void roll( const f32 &angle );
            void yaw( const f32 &angle );
            void pitch( const f32 &angle );

            void rotate( const Vector3F &axis, const f32 &angle );
            void rotate( const QuaternionF &q );

            void setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis = Vector3F::UNIT_Y );

            QuaternionF getDerivedOrientation() const;
            Vector3F getDerivedPosition() const;
            Vector3F getDerivedDirection() const;
            Vector3F getDerivedUp() const;
            Vector3F getDerivedRight() const;
            QuaternionF getRealOrientation() const;
            Vector3F getRealPosition() const;
            Vector3F getRealDirection() const;
            Vector3F getRealUp() const;
            Vector3F getRealRight() const;

            void setLodBias( f32 factor = 1.0 ) override;
            f32 getLodBias() const override;

            Ray3F getRay( f32 screenx, f32 screeny ) const override;
            void getRay( f32 screenx, f32 screeny, Ray3F &outRay ) const;
            Vector2F getScreenPosition( const Vector3F &position ) override;

            void setWindow( f32 Left, f32 Top, f32 Right, f32 Bottom ) override;
            void resetWindow();
            bool isWindowSet() const;

            void setAutoAspectRatio( bool autoratio ) override;
            bool getAutoAspectRatio() const override;

            f32 getFOVy() const override;

            void setFOVy( f32 val ) override;

            Matrix4F getViewMatrix() const override;

            Matrix4F getProjectionMatrix() const override;

            void *getRenderViewMatrix() const override;

            void *getRenderProjectionMatrix() const override;

            SmartPtr<ITexture> getTargetTexture() const override;
            void setTargetTexture( SmartPtr<ITexture> targetTexture ) override;

            SmartPtr<ITexture> getEditorTexture() const override;

            void setEditorTexture( SmartPtr<ITexture> editorTexture ) override;

            /** @copydoc ICamera::setRenderUI */
            void setRenderUI( bool enabled ) override;

            /** @copydoc ICamera::getRenderUI */
            bool getRenderUI() const override;

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getProperties */
            SmartPtr<Properties> getProperties() const;

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::setProperties */
            void setProperties( SmartPtr<Properties> properties );

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const;

            Ogre::Camera *getCamera() const;
            void setCamera( Ogre::Camera *camera );

            SmartPtr<Compositor> getCompositor() const;
            void setCompositor( SmartPtr<Compositor> compositor );

            FB_CLASS_REGISTER_DECL;

        protected:
            class CCameraStateListener : public IStateListener
            {
            public:
                CCameraStateListener() = default;
                ~CCameraStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                void setOwner( CCameraOgreNext *owner );

                CCameraOgreNext *getOwner() const;

            protected:
                CCameraOgreNext *m_owner = nullptr;
            };

            void setupStateObject() override;

            void createCompositor();

            void destroyCompositor();

            SmartPtr<Compositor> m_compositor;

            SmartPtr<ITexture> m_targetTexture;

            SmartPtr<ITexture> m_editorTexture;

            /// The ogre camera.
            //AtomicRawPtr<Ogre::Camera> m_camera;
            RawPtr<Ogre::Camera> m_camera;

            /// The viewport this camera belongs to.
            SmartPtr<CViewportOgreNext> m_viewport;

            /// Camera position - default (0,0,0)
            mutable Vector3F mPosition;

            /// Camera orientation, Quaternion style
            mutable QuaternionF m_orientation;

            Vector3F m_direction;

            Vector3F m_realDirection;
            Vector3F m_realRight;

            Vector3F m_realUp;
            Matrix4F inverseVP;

            /// Derived position of the camera, including reflection
            mutable QuaternionF m_derivedOrientation;

            /// Derived position of the camera
            mutable Vector3F m_derivedPosition;

            /// Real world orientation of the camera
            mutable QuaternionF m_realOrientation;

            /// Real world position of the camera
            mutable Vector3F m_realPosition;

            /// The aspect ratio of the camera.
            atomic_f32 m_aspectRatio;

            /// The name of the camera.
            String m_name;

            atomic_bool m_renderUI = false;

            mutable Matrix4F m_viewMatrix;
            mutable Matrix4F m_projectionMatrix;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
