#ifndef _CRenderTarget_H
#define _CRenderTarget_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IRenderTarget.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        template <class T>
        class CRenderTargetOgreNext : public T
        {
        public:
            CRenderTargetOgreNext();
            ~CRenderTargetOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::RenderTarget *renderTarget );

            void preUpdate() override
            {
            }

            void update() override;

            void postUpdate() override
            {
            }

            void addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener );
            void removeRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener );
            Array<SmartPtr<IRenderTarget::Listener>> getRenderTargetListeners() const;

            void swapBuffers() override;

            void setPriority( u8 priority ) override;
            u8 getPriority() const override;

            bool isActive() const override;
            void setActive( bool state ) override;

            void setAutoUpdated( bool autoupdate ) override;
            bool isAutoUpdated( void ) const override;

            void getMetrics( u32 &width, u32 &height, u32 &colourDepth );
            u32 getWidth() const;
            u32 getHeight() const;
            u32 getColourDepth() const override;

            SmartPtr<IViewport> addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder = 0,
                                             f32 left = 0.0f, f32 top = 0.0f, f32 width = 1.0f,
                                             f32 height = 1.0f ) override;
            u32 getNumViewports() const override;
            SmartPtr<IViewport> getViewport( u32 index ) override;
            SmartPtr<IViewport> getViewportById( hash32 id ) override;

            SmartPtr<IViewport> getViewportByZOrder( s32 zorder ) const
            {
                for( auto [key, vp] : m_viewports )
                {
                    if( vp->getZOrder() == zorder )
                    {
                        return vp;
                    }
                }

                return nullptr;
            }

            bool hasViewportWithZOrder( s32 zorder ) const
            {
                for( auto [key, vp] : m_viewports )
                {
                    if( vp->getZOrder() == zorder )
                    {
                        return true;
                    }
                }

                return false;
            }

            Array<SmartPtr<IViewport>> getViewports() const override;

            void removeViewport( SmartPtr<IViewport> vp ) override;
            void removeAllViewports() override;

            void getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const;

            IRenderTarget::RenderTargetStats getRenderTargetStats() const override;

            void _getObject( void **ppObject ) const override;

            void copyContentsToMemory(
                void *buffer, u32 size,
                IRenderTarget::FrameBuffer bufferId = IRenderTarget::FrameBuffer::Auto ) override;

            Vector2I getSize() const override;

            void setSize( const Vector2I &size ) override;

            void setColourDepth( u32 val ) override;

            virtual SmartPtr<ITexture> getTexture() const
            {
                return nullptr;
            }

            virtual void setTexture( SmartPtr<ITexture> texture )
            {
            }

            SmartPtr<IStateContext> getStateObject() const
            {
                return m_stateObject;
            }

            void setStateObject( SmartPtr<IStateContext> stateObject )
            {
                m_stateObject = stateObject;
            }

            SmartPtr<IStateListener> getStateListener() const
            {
                return m_stateListener;
            }

            void setStateListener( SmartPtr<IStateListener> stateListener )
            {
                m_stateListener = stateListener;
            }

        protected:
            /** Sets up the state object. */
            virtual void setupStateObject();

            virtual void destroyedStateObject();

            // Ogre::v1::RenderTarget* m_renderTarget;

            using Viewports = std::map<u32, SmartPtr<IViewport>>;
            Viewports m_viewports;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
        };
    }  // end namespace render
}  // end namespace fb

#include <FBGraphicsOgreNext/Wrapper/CRenderTargetOgreNext.inl>

#endif
