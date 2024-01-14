#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CRenderTargetOgreNext.h>
//#include <FBGraphicsOgreNext/Wrapper/CRenderTexture.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb::render
{

    template <class T>
    void CRenderTargetOgreNext<T>::addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener )
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::removeRenderTargetListener(
        SmartPtr<IRenderTarget::Listener> listener )
    {
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getRenderTargetListeners() const
        -> Array<SmartPtr<IRenderTarget::Listener>>
    {
        return {};
    }

    template <class T>
    void CRenderTargetOgreNext<T>::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
    {
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getWidth() const -> u32
    {
        return 0;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getHeight() const -> u32
    {
        return 0;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getColourDepth() const -> u32
    {
        return 0;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder /*= 0*/,
                                                f32 left /*= 0.0f*/, f32 top /*= 0.0f */,
                                                f32 width /*= 1.0f*/, f32 height /*= 1.0f*/ )
        -> SmartPtr<IViewport>
    {
        try
        {
            Ogre::Camera *ogreCamera;
            camera->_getObject( reinterpret_cast<void **>( &ogreCamera ) );

            // FB_ASSERT( ogreCamera );
            if( !ogreCamera )
            {
                String message( "CWindow::addViewport - error could not find camera object. " );
                // FB_EXCEPTION( message.c_str() );
            }

            Ogre::Viewport *vp = nullptr;  //
            // = m_renderTarget->addViewport( ogreCamera, ZOrder, left, top, width, height );

            SmartPtr<CViewportOgreNext> viewport( new CViewportOgreNext );
            viewport->initialise( vp );
            m_viewports[id] = viewport;

            viewport->setCamera( camera );

            return m_viewports[id];
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getNumViewports() const -> u32
    {
        return 0;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getViewport( u32 index ) -> SmartPtr<IViewport>
    {
        return {};
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getViewportById( hash32 id ) -> SmartPtr<IViewport>
    {
        return {};
    }

    template <class T>
    void CRenderTargetOgreNext<T>::removeViewport( SmartPtr<IViewport> vp )
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::removeAllViewports()
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS,
                                                  f32 &worstFPS ) const
    {
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getRenderTargetStats() const -> IRenderTarget::RenderTargetStats
    {
        return {};
    }

    template <class T>
    void CRenderTargetOgreNext<T>::_getObject( void **ppObject ) const
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setPriority( u8 priority )
    {
        // m_renderTarget->setPriority(priority);
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getPriority() const -> u8
    {
        // return m_renderTarget->getPriority();
        return 0;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::isActive() const -> bool
    {
        // return m_renderTarget->isActive();
        return false;
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setActive( bool state )
    {
        // m_renderTarget->setActive( state );
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setAutoUpdated( bool autoupdate )
    {
        // m_renderTarget->setAutoUpdated(autoupdate);
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::isAutoUpdated() const -> bool
    {
        // return m_renderTarget->isAutoUpdated();
        return false;
    }

    template <class T>
    void CRenderTargetOgreNext<T>::copyContentsToMemory(
        void *buffer, u32 size, IRenderTarget::FrameBuffer bufferId /*= FB_AUTO */ )
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::swapBuffers()
    {
        // m_renderTarget->swapBuffers();
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getViewports() const -> Array<SmartPtr<IViewport>>
    {
        Array<SmartPtr<IViewport>> viewports;
        viewports.reserve( m_viewports.size() );

        auto it = m_viewports.begin();
        for( ; it != m_viewports.end(); ++it )
        {
            viewports.push_back( it->second );
        }

        return viewports;
    }

    template <class T>
    auto CRenderTargetOgreNext<T>::getSize() const -> Vector2I
    {
        return Vector2I::zero();  // m_renderTarget->getWidth(), m_renderTarget->getHeight());
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setSize( const Vector2I &size )
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setColourDepth( u32 val )
    {
    }

    template <class T>
    void CRenderTargetOgreNext<T>::setupStateObject()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateContext = stateManager->addStateObject();

            // auto sceneNodeStateListener = factoryManager->make_ptr<WindowStateListener>();
            // sceneNodeStateListener->setOwner( this );
            // m_stateListener = sceneNodeStateListener;
            // stateContext->addStateListener( m_stateListener );

            stateContext->setOwner( this );
            setStateContext( stateContext );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    template <class T>
    void CRenderTargetOgreNext<T>::destroyedStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        if( auto stateContext = getStateContext() )
        {
            stateContext->setOwner( nullptr );

            if( auto stateListener = getStateListener() )
            {
                stateContext->removeStateListener( stateListener );
            }

            stateManager->removeStateObject( stateContext );

            stateContext->unload( nullptr );
            setStateContext( nullptr );
        }

        if( auto stateListener = getStateListener() )
        {
            stateListener->unload( nullptr );
            setStateListener( nullptr );
        }
    }

    template class CRenderTargetOgreNext<IRenderTarget>;
    //template class CRenderTarget<IRenderTexture>;
    template class CRenderTargetOgreNext<IWindow>;
}  // namespace fb::render
