#ifndef _CRenderTarget_H
#define _CRenderTarget_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IRenderTarget.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    namespace render
    {

        /** Implements IRenderTarget interface for Ogre. */
        template <class T>
        class CRenderTarget : public CSharedObject<T>
        {
        public:
            /** Constructor. */
            CRenderTarget();

            /** Destructor. */
            ~CRenderTarget() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::preUpdate */
            void preUpdate() override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc ISharedObject::postUpdate */
            void postUpdate() override;

            /** @copydoc IRenderTarget::swapBuffers */
            void swapBuffers() override;

            /** @copydoc IRenderTarget::setPriority */
            void setPriority( u8 priority ) override;

            /** @copydoc IRenderTarget::getPriority */
            u8 getPriority() const override;

            /** @copydoc IRenderTarget::getPriority */
            bool isActive() const override;

            /** @copydoc IRenderTarget::getPriority */
            void setActive( bool state ) override;

            /** @copydoc IRenderTarget::getPriority */
            void setAutoUpdated( bool autoupdate ) override;

            /** @copydoc IRenderTarget::getPriority */
            bool isAutoUpdated() const override;

            /** @copydoc IRenderTarget::getPriority */
            void getMetrics( u32 &width, u32 &height, u32 &colourDepth );

            /** @copydoc IRenderTarget::getPriority */
            SmartPtr<IViewport> addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder = -1,
                                             f32 left = 0.0f, f32 top = 0.0f, f32 width = 1.0f,
                                             f32 height = 1.0f ) override;

            /** @copydoc IRenderTarget::getPriority */
            u32 getNumViewports() const override;

            /** @copydoc IRenderTarget::getPriority */
            SmartPtr<IViewport> getViewport( u32 index ) override;

            /** @copydoc IRenderTarget::getPriority */
            SmartPtr<IViewport> getViewportById( hash32 id ) override;

            /** @copydoc IRenderTarget::getViewportByZOrder */
            SmartPtr<IViewport> getViewportByZOrder( s32 zorder ) const override;

            /** @copydoc IRenderTarget::hasViewportWithZOrder */
            bool hasViewportWithZOrder( s32 zorder ) const override;

            /** @copydoc IRenderTarget::getPriority */
            Array<SmartPtr<IViewport>> getViewports() const override;

            /** @copydoc IRenderTarget::getPriority */
            void removeViewport( SmartPtr<IViewport> vp ) override;

            /** @copydoc IRenderTarget::getPriority */
            void removeAllViewports() override;

            /** @copydoc IRenderTarget::getPriority */
            void getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const;

            /** @copydoc IRenderTarget::getPriority */
            IRenderTarget::RenderTargetStats getRenderTargetStats() const override;

            /** @copydoc IRenderTarget::getPriority */
            void _getObject( void **ppObject ) const override;

            /** @copydoc IRenderTarget::getPriority */
            void copyContentsToMemory(
                void *buffer, u32 size,
                IRenderTarget::FrameBuffer bufferId = IRenderTarget::FrameBuffer::Auto ) override;

            /** @copydoc IRenderTarget::getPriority */
            Vector2I getSize() const override;

            /** @copydoc IRenderTarget::getPriority */
            void setSize( const Vector2I &size ) override;

            /** @copydoc IRenderTarget::getPriority */
            u32 getColourDepth() const override;

            /** @copydoc IRenderTarget::getPriority */
            void setColourDepth( u32 colourDepth ) override;

            /** @copydoc IRenderTarget::getPriority */
            bool getSwapBuffers() const;

            /** @copydoc IRenderTarget::getPriority */
            void setSwapBuffers( bool swapBuffers );

            /** Gets the render target object. */
            Ogre::RenderTarget *getRenderTarget() const;

            /** Sets the render target object. */
            void setRenderTarget( Ogre::RenderTarget *renderTarget );

            /** Gets the state object. */
            SmartPtr<IStateContext> getStateObject() const;

            /** Sets the state object. */
            void setStateObject( SmartPtr<IStateContext> stateObject );

            /** Gets the state listener. */
            SmartPtr<IStateListener> getStateListener() const;

            /** Sets the state listener. */
            void setStateListener( SmartPtr<IStateListener> stateListener );

            /** @copydoc IRenderTarget::addRenderTargetListener */
            void addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener );

            /** @copydoc IRenderTarget::removeRenderTargetListener */
            void removeRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener );

            /** @copydoc IRenderTarget::getRenderTargetListeners */
            Array<SmartPtr<IRenderTarget::Listener>> getRenderTargetListeners() const;

            /** @copydoc IRenderTarget::getTexture */
            SmartPtr<ITexture> getTexture() const;

            /** @copydoc IRenderTarget::setTexture */
            void setTexture( SmartPtr<ITexture> texture );

        protected:
            /** Sets up the state object. */
            virtual void setupStateObject();

            /** Destroys the state object. */
            virtual void destroyedStateObject();

            WeakPtr<ITexture> m_texture;

            AtomicSmartPtr<IStateContext> m_stateObject;
            AtomicSmartPtr<IStateListener> m_stateListener;

            Ogre::RenderTarget *m_renderTarget = nullptr;

            Array<SmartPtr<IViewport>> m_viewports;

            Array<SmartPtr<IRenderTarget::Listener>> m_rtListeners;

            Vector2I m_size;
            atomic_u32 m_colourDepth = 0;
            atomic_bool m_swapBuffers = true;
            atomic_bool m_isActive = true;

            static s32 m_ext;
        };

        template <class T>
        SmartPtr<ITexture> CRenderTarget<T>::getTexture() const
        {
            return m_texture.lock();
        }

        template <class T>
        void CRenderTarget<T>::setTexture( SmartPtr<ITexture> texture )
        {
            m_texture = texture;
        }

    }  // end namespace render
}  // end namespace fb

#endif
