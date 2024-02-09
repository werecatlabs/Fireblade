#ifndef __CViewport_h__
#define __CViewport_h__

#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {
        class Viewport : public SharedGraphicsObject<IViewport>
        {
        public:
            Viewport();
            ~Viewport() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setCamera( SmartPtr<ICamera> camera ) override;

            SmartPtr<ICamera> getCamera() const override;

            void setOverlaysEnabled( bool enabled ) override;

            bool getOverlaysEnabled() const override;

            void setBackgroundColour( const ColourF &colour ) override;

            ColourF getBackgroundColour() const override;

            void setZOrder( s32 zorder ) override;
            s32 getZOrder() const override;

            void setPriority( u8 priority );
            u8 getPriority() const;

            bool isActive() const override;
            void setActive( bool active ) override;

            void setAutoUpdated( bool autoUpdated ) override;

            bool isAutoUpdated() const override;

            void setSkiesEnabled( bool enabled ) override;

            bool getSkiesEnabled() const override;

            void setVisibilityMask( u32 mask ) override;

            u32 getVisibilityMask() const override;

            void setShadowsEnabled( bool enabled ) override;

            bool getShadowsEnabled() const override;

            Vector2<real_Num> getPosition() const override;

            Vector2<real_Num> getActualPosition() const override;

            void setPosition( const Vector2<real_Num> &position ) override;

            Vector2<real_Num> getSize() const override;

            Vector2<real_Num> getActualSize() const override;

            void setSize( const Vector2<real_Num> &size ) override;

            SmartPtr<ITexture> getBackgroundTexture() const override;

            void setBackgroundTexture( SmartPtr<ITexture> texture ) override;

            void setEnableUI( bool enabled ) override;

            bool getEnableUI() const override;

            void setEnableSceneRender( bool enabled ) override;

            bool getEnableSceneRender() const override;

            void setMaterialScheme( const String &schemeName ) override;

            String getMaterialScheme() const override;

            void setClearEveryFrame( bool clear, u32 buffers ) override;

            bool getClearEveryFrame() const override;

            u32 getClearBuffers() const override;

            SmartPtr<IWindow> getWindow() const;

            void setWindow( SmartPtr<IWindow> window );

            String getBackgroundTextureName() const override;

            void setBackgroundTextureName( const String &textureName ) override;

            SmartPtr<IRenderTarget> getRenderTarget() const override;

            void setRenderTarget( SmartPtr<IRenderTarget> renderTarget ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            virtual void removeViewportFromRT();

            WeakPtr<IRenderTarget> m_renderTarget;
            WeakPtr<IWindow> m_window;

            SmartPtr<ITexture> m_texture;
        };
    }  // namespace render
}  // namespace fb

#endif  // CViewport_h__
