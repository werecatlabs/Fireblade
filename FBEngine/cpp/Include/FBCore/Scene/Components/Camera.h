#ifndef CameraComponent_h__
#define CameraComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/Ray3.h>

namespace fb
{
    namespace scene
    {
        /** @brief Camera component.
         *
         * This component is used to render the scene from a camera's point of view.
         * It is also used to render the scene to a texture.
         */
        class Camera : public Component
        {
        public:
            /** Constructor. */
            Camera();

            /** Destructor. */
            ~Camera() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc Component::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** Gets the target texture. */
            SmartPtr<render::ITexture> getTargetTexture() const;

            /** Sets the target texture. */
            void setTargetTexture( SmartPtr<render::ITexture> targetTexture );

            /** Gets the editor texture. */
            SmartPtr<render::ITexture> getEditorTexture() const;

            /** Sets the editor texture. */
            void setEditorTexture( SmartPtr<render::ITexture> editorTexture );

            /** Gets the render camera. */
            SmartPtr<render::ICamera> getCamera() const;

            /** Sets the render camera. */
            void setCamera( SmartPtr<render::ICamera> camera );

            /** Gets the camera scene node. */
            SmartPtr<render::ISceneNode> getNode() const;

            /** Sets the camera scene node. */
            void setNode( SmartPtr<render::ISceneNode> node );

            /** Gets whether the camera is active or not. */
            bool isActive() const;

            /** Sets whether the camera is active or not. */
            void setActive( bool active );

            /** @copydoc Component::updateTransform */
            void updateTransform() override;

            /**
             * @brief Updates the z order.
             */
            void updateOrder();

            /** @brief Gets the z order of the given actor.
             */
            s32 getZOrder( SmartPtr<IActor> other );

            /** Gets the z order. */
            u32 getZOrder() const;

            /** Sets the z order. */
            void setZOrder( u32 zOrder );

            /** @copydoc Component::updateTransform */
            void updateTransform( const Transform3<real_Num> &transform ) override;

            /** Gets a ray from the camera to the viewport. */
            Ray3<real_Num> getCameraToViewportRay( const Vector2<real_Num> &screenPosition );

            /** Gets whether the given box is in the camera's frustum. */
            bool isInFrustum( const AABB3<real_Num> &box ) const;

            /** Gets the render target. */
            SmartPtr<render::IRenderTarget> getRenderTarget() const;

            SmartPtr<render::IViewport> getViewport() const;

            void setViewport( SmartPtr<render::IViewport> viewport );

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Updates the active state. */
            void updateActiveState( bool active );

            /** Create a viewport. */
            void createViewport();

            /** @copydoc Component::updateFlags */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc Component::handleComponentEvent */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            // The editor texture is used to render the scene to a texture.
            SmartPtr<render::ITexture> m_editorTexture;

            // The viewport is used to render the scene.
            SmartPtr<render::IViewport> m_viewport;

            // The render camera.
            SmartPtr<render::ICamera> m_camera;

            // The camera scene node.
            SmartPtr<render::ISceneNode> m_node;

            /** The component's z order. */
            s32 m_zOrder = -1;

            /** To know if the camera is active or not. */
            bool m_isActive = false;

            // Used to generate unique names.
            static u32 m_nameExt;

            // Used to generate unique z orders.
            static u32 m_zorderExt;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CameraComponent_h__
