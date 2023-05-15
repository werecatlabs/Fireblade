#ifndef _CCamera_H
#define _CCamera_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    namespace render
    {

        /** Implements ICamera interface for Ogre. */
        class CCameraOgre : public CGraphicsObjectOgre<ICamera>
        {
        public:
            CCameraOgre() = default;
            ~CCameraOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Camera *camera );

            void _update();

            void registerForUpdates( bool registerObject );
            bool isRegisteredForUpdates();

            void setViewport( SmartPtr<IViewport> viewport ) override;
            SmartPtr<IViewport> getViewport() const override;

            // IGraphicsObject functions

            void setMaterialName( const String &materialName, s32 index = -1 ) override;
            String getMaterialName( s32 index = -1 ) const override;

            void setCastShadows( bool castShadows ) override;
            bool getCastShadows() const override;

            void setReceiveShadows( bool receiveShadows ) override;
            bool getReceiveShadows() const override;

            void setRenderQueueGroup( u8 renderQueue );

            void setVisibilityFlags( u32 flags ) override;
            u32 getVisibilityFlags() const override;

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
            f32 getAspectRatio() const override;

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

            void setFOVy( f32 fov ) override;

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

            FB_CLASS_REGISTER_DECL;

        protected:
            class CCameraStateListener : public StateListener
            {
            public:
                CCameraStateListener() = default;
                ~CCameraStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                void setOwner( CCameraOgre *owner );
                CCameraOgre *getOwner() const override;

            protected:
                CCameraOgre *m_owner = nullptr;
            };

            void setupStateObject() override;

            WeakPtr<ITexture> m_targetTexture;

            WeakPtr<ITexture> m_editorTexture;

            /// The viewport this camera belongs to.
            WeakPtr<IViewport> m_viewport;

            /// The ogre camera.
            Ogre::Camera *m_camera = nullptr;

            /// Camera position - default (0,0,0)
            mutable Vector3F m_position;

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

            String m_materialName;

            bool m_castShadows = false;
            bool m_receiveShadows = false;
            bool m_renderUI = false;

            static u32 m_nameExt;
        };
    } // end namespace render
}     // end namespace fb

#endif
