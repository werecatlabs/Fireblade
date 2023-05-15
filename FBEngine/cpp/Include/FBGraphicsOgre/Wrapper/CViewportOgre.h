#ifndef _CViewport_H
#define _CViewport_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphics/Wrapper/CViewport.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/ColourF.h>

namespace fb
{
    namespace render
    {

        /**
         * Implementation of the IViewport interface using the Ogre3D rendering engine.
         */
        class CViewportOgre : public CViewport
        {
        public:
            /** Default constructor. */
            CViewportOgre();

            /** Default destructor. */
            ~CViewportOgre() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * Updates the viewport, rendering any changes made since the last update.
             */
            void update() override;

            void setPriority( u8 priority );
            u8 getPriority() const;

            bool isActive() const;
            void setActive( bool active );

            void setAutoUpdated( bool autoupdate ) override;
            bool isAutoUpdated() const override;

            void setId( u32 id );
            u32 getId() const;

            void setCamera( SmartPtr<ICamera> camera ) override;
            SmartPtr<ICamera> getCamera() const override;

            s32 getZOrder() const override;

            f32 getLeft() const override;
            f32 getTop() const override;
            f32 getWidth() const override;
            f32 getHeight() const override;
            s32 getActualLeft() const override;
            s32 getActualTop() const override;
            s32 getActualWidth() const override;

            s32 getActualHeight() const override;

            void setDimensions( f32 left, f32 top, f32 width, f32 height ) override;

            void setBackgroundColour( const ColourF &colour ) override;
            ColourF getBackgroundColour() const override;

            void setClearEveryFrame( bool clear, u32 buffers = IGraphicsScene::FBT_COLOUR |
                                                               IGraphicsScene::FBT_DEPTH ) override;
            bool getClearEveryFrame() const override;
            u32 getClearBuffers() const override;

            void setMaterialScheme( const String &schemeName ) override;
            String getMaterialScheme() const override;
            void getActualDimensions( s32 &left, s32 &top, s32 &width, s32 &height ) const override;

            bool _isUpdated() const override;
            void _clearUpdatedFlag() override;

            u32 _getNumRenderedFaces() const override;
            u32 _getNumRenderedBatches() const override;

            /** @copydoc IViewport::setRenderSceneEnabled */
            void setRenderSceneEnabled( bool enabled ) override;

            /** @copydoc IViewport::getRenderSceneEnabled */
            bool getRenderSceneEnabled() const override;

            void setOverlaysEnabled( bool enabled ) override;
            bool getOverlaysEnabled() const override;

            void setSkiesEnabled( bool enabled ) override;
            bool getSkiesEnabled() const override;

            void setShadowsEnabled( bool enabled ) override;
            bool getShadowsEnabled() const override;

            void setVisibilityMask( u32 mask ) override;
            u32 getVisibilityMask() const override;

            void _getObject( void **ppObject ) const override;
            void setZOrder( s32 zorder ) override;

            Vector2F getPosition() const override;
            void setPosition( const Vector2F &position ) override;

            Vector2F getSize() const override;
            void setSize( const Vector2F &size ) override;

            SmartPtr<ITexture> getBackgroundTexture() const override;
            void setBackgroundTexture( SmartPtr<ITexture> texture ) override;

            String getBackgroundTextureName() const override;
            void setBackgroundTextureName( const String &textureName ) override;

            SmartPtr<IRenderTarget> getRenderTarget() const override;

            void setRenderTarget( SmartPtr<IRenderTarget> renderTarget ) override;

            SmartPtr<IWindow> getWindow() const;
            void setWindow( SmartPtr<IWindow> window );

            void setUiEnabled( bool enabled ) override;
            bool getUiEnabled() const override;

            Ogre::Viewport *getViewport() const;

            void setViewport( Ogre::Viewport *viewport );

            FB_CLASS_REGISTER_DECL;

        protected:
            class ViewportStateListener : public CSharedObject<IStateListener>
            {
            public:
                ViewportStateListener() = default;
                ~ViewportStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CViewportOgre *getOwner() const;
                void setOwner( CViewportOgre *owner );

            protected:
                CViewportOgre *m_owner = nullptr;
            };

            void removeViewportFromRT();

            Ogre::Viewport *m_viewport = nullptr;

            WeakPtr<IRenderTarget> m_renderTarget;
            WeakPtr<IWindow> m_window;

            SmartPtr<ITexture> m_texture;

            String m_backgroundTextureName;

            bool m_uiEnabled = true;
            bool m_renderSceneEnabled = true;
            bool m_active = true;

            u32 m_id = 0;

            static u32 m_zOrderExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
