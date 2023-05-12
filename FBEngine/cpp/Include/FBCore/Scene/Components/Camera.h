#ifndef CameraComponent_h__
#define CameraComponent_h__

#include <FBCore/Scene/Components/BaseComponent.h>

namespace fb
{
    namespace scene
    {
        class Camera : public BaseComponent
        {
        public:
            Camera();
            ~Camera() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            SmartPtr<render::ITexture> getTargetTexture() const;

            void setTargetTexture( SmartPtr<render::ITexture> targetTexture );

            SmartPtr<render::ITexture> getEditorTexture() const;

            void setEditorTexture( SmartPtr<render::ITexture> editorTexture );

            SmartPtr<render::ICamera> getCamera() const;
            void setCamera( SmartPtr<render::ICamera> camera );

            SmartPtr<render::ISceneNode> getNode() const;
            void setNode( SmartPtr<render::ISceneNode> node );

            bool isActive() const;

            void setActive( bool active );

            void setupCamera();

            void updateTransform() override;

            /**
             * @brief Updates the z order.
             */
            void updateOrder();

            s32 getZOrder( SmartPtr<IActor> obj );

            u32 getZOrder() const;

            void setZOrder( u32 zOrder );

            FB_CLASS_REGISTER_DECL;

        protected:
            void createViewport();

            SmartPtr<render::IRenderTarget> getRenderTarget() const;

            void updateDirty( u32 flags, u32 oldFlags );
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType );

            WeakPtr<render::ITexture> m_editorTexture;
            SmartPtr<render::IViewport> m_viewport;
            SmartPtr<render::ICamera> m_camera;
            SmartPtr<render::ISceneNode> m_node;

            /**
             * @brief The component's z order.
             */
            s32 m_zOrder = -1;

            bool m_isActive = false;

            static u32 m_nameExt;
            static u32 m_zorderExt;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CameraComponent_h__
