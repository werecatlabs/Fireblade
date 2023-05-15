#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CRenderTarget.h>
#include <FBGraphicsOgre/Wrapper/CRenderTexture.h>
#include <FBGraphicsOgre/Wrapper/CViewportOgre.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <atomic>
#include <type_traits>

namespace fb
{
    namespace render
    {
        template <class T>
        s32 CRenderTarget<T>::m_ext = 0;

        template <class T>
        CRenderTarget<T>::CRenderTarget()
        {
            setupStateObject();
        }

        template <class T>
        CRenderTarget<T>::~CRenderTarget()
        {
            unload( nullptr );
            destroyedStateObject();
        }

        template <class T>
        void CRenderTarget<T>::load( SmartPtr<ISharedObject> data )
        {
            try
            {
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                for( auto vp : m_viewports )
                {
                    vp->unload( nullptr );
                }

                m_viewports.clear();

                for( auto rt : m_rtListeners )
                {
                    rt->unload( nullptr );
                }

                m_rtListeners.clear();

                if( m_renderTarget )
                {
                    m_renderTarget->removeAllViewports();
                    m_renderTarget = nullptr;
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                m_texture = nullptr;
                m_renderTarget = nullptr;

                destroyedStateObject();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::preUpdate()
        {
            try
            {
                if( m_renderTarget )
                {
                    auto size = m_renderTarget->getNumViewports();
                    FB_ASSERT( size <= m_viewports.size() );
                    FB_ASSERT( size <= 1 );

                    for( size_t i = 0; i < size; ++i )
                    {
                        auto vp = m_renderTarget->getViewport( i );
                        FB_ASSERT( vp );

                        auto actualWidth = vp->getActualWidth();
                        auto actualHeight = vp->getActualHeight();

                        if( actualWidth <= 0 || actualHeight <= 0 )
                        {
                            vp->_updateDimensions();
                        }
                    }

                    for( size_t i = 0; i < size; ++i )
                    {
                        auto vp = m_renderTarget->getViewport( i );
                        FB_ASSERT( vp );

                        auto actualWidth = vp->getActualWidth();
                        auto actualHeight = vp->getActualHeight();

                        if( actualWidth > 0 && actualHeight > 0 )
                        {
                            vp->clear();
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::update()
        {
            try
            {
                if( m_renderTarget )
                {
                    m_renderTarget->_beginUpdate();

#if 1
                    for( auto vp : m_viewports )
                    {
                        vp->update();
                    }
#else
                    auto swapBuffers = getSwapBuffers();
                    auto size = m_renderTarget->getNumViewports();
                    for( size_t i = 0; i < size; ++i )
                    {
                        auto vp = m_renderTarget->getViewport( i );
                        FB_ASSERT( vp );

                        auto actualWidth = vp->getActualWidth();
                        auto actualHeight = vp->getActualHeight();

                        if( actualWidth > 0 && actualHeight > 0 )
                        {
                            if( vp->isAutoUpdated() )
                            {
                                m_renderTarget->_updateViewport( vp );
                            }
                        }
                    }
#endif

                    m_renderTarget->_endUpdate();
                }
                else
                {
                    FB_LOG_ERROR( "CRenderTarget<T>::update renderTarget null" );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::postUpdate()
        {
            try
            {
                if( m_renderTarget )
                {
                    auto swapBuffers = getSwapBuffers();
                    m_renderTarget->swapBuffers();
                }
                else
                {
                    FB_LOG_ERROR( "CRenderTarget<T>::postUpdate renderTarget null" );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
        {
            if( m_renderTarget )
            {
                m_renderTarget->getMetrics( width, height );
            }
        }

        template <class T>
        u32 CRenderTarget<T>::getColourDepth() const
        {
            return 0;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTarget<T>::addViewport( hash32 id, SmartPtr<ICamera> camera,
                                                           s32 ZOrder /*= 0*/, f32 left /*= 0.0f*/,
                                                           f32 top /*= 0.0f */, f32 width /*= 1.0f*/,
                                                           f32 height /*= 1.0f*/ )
        {
            try
            {
                FB_ASSERT( m_renderTarget );

                if( m_renderTarget )
                {
                    if( ZOrder == -1 )
                    {
                        ZOrder = m_ext++;
                    }

                    auto viewport = fb::make_ptr<CViewportOgre>();

                    auto handle = viewport->getHandle();
                    if( handle )
                    {
                        handle->setId( id );
                    }

                    viewport->setRenderTarget( this );
                    viewport->setZOrder( ZOrder );
                    viewport->setPosition( Vector2F( left, top ) );
                    viewport->setSize( Vector2F( width, height ) );

                    viewport->setCamera( camera );
                    m_viewports.push_back( viewport );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    graphicsSystem->loadObject( viewport );

                    return viewport;
                }
                FB_LOG_ERROR( "CRenderTarget<T>::update renderTarget null" );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        template <class T>
        u32 CRenderTarget<T>::getNumViewports() const
        {
            return (u32)m_viewports.size();
        }

        template <class T>
        SmartPtr<IViewport> CRenderTarget<T>::getViewport( u32 index )
        {
            FB_ASSERT( index < (u32)m_viewports.size() );

            if( index < (u32)m_viewports.size() )
            {
                return m_viewports[index];
            }

            return nullptr;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTarget<T>::getViewportById( hash32 id )
        {
            for( auto vp : m_viewports )
            {
                if( auto handle = vp->getHandle() )
                {
                    if( handle->getId() == id )
                    {
                        return vp;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTarget<T>::getViewportByZOrder( s32 zorder ) const
        {
            for( auto vp : m_viewports )
            {
                if( vp )
                {
                    if( vp->getZOrder() == zorder )
                    {
                        return vp;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        bool CRenderTarget<T>::hasViewportWithZOrder( s32 zorder ) const
        {
            for( auto vp : m_viewports )
            {
                if( vp )
                {
                    if( vp->getZOrder() == zorder )
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        template <class T>
        void CRenderTarget<T>::removeViewport( SmartPtr<IViewport> vp )
        {
            try
            {
                auto zorder = vp->getZOrder();

                if( m_renderTarget )
                {
                    m_renderTarget->removeViewport( zorder );
                }
                else
                {
                    FB_LOG_ERROR( "CRenderTarget<T>::removeViewport renderTarget null" );
                }

                vp->unload( nullptr );

                auto it = std::find( m_viewports.begin(), m_viewports.end(), vp );
                if( it != m_viewports.end() )
                {
                    m_viewports.erase( it );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::removeAllViewports()
        {
            try
            {
                if( m_renderTarget )
                {
                    m_renderTarget->removeAllViewports();
                }
                else
                {
                    FB_LOG_ERROR( "CRenderTarget<T>::removeAllViewports renderTarget null" );
                }

                for( auto vp : m_viewports )
                {
                    vp->unload( nullptr );
                }

                m_viewports.clear();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS,
                                              f32 &worstFPS ) const
        {
            if( m_renderTarget )
            {
                auto frameStats = m_renderTarget->getStatistics();

                lastFPS = frameStats.lastFPS;
                avgFPS = frameStats.avgFPS;
                bestFPS = frameStats.bestFPS;
                worstFPS = frameStats.worstFPS;
            }
            else
            {
                FB_LOG_ERROR( "CRenderTarget<T>::getStatistics renderTarget null" );
            }
        }

        template <class T>
        IRenderTarget::RenderTargetStats CRenderTarget<T>::getRenderTargetStats() const
        {
            auto stats = IRenderTarget::RenderTargetStats();

            if( m_renderTarget )
            {
                auto frameStats = m_renderTarget->getStatistics();

                stats.lastFPS = frameStats.lastFPS;
                stats.avgFPS = frameStats.avgFPS;
                stats.bestFPS = frameStats.bestFPS;
                stats.worstFPS = frameStats.worstFPS;
            }
            else
            {
                FB_LOG_ERROR( "CRenderTarget<T>::getRenderTargetStats renderTarget null" );
            }

            return stats;
        }

        template <class T>
        void CRenderTarget<T>::_getObject( void **ppObject ) const
        {
            *ppObject = m_renderTarget;
        }

        template <class T>
        void CRenderTarget<T>::setPriority( u8 priority )
        {
            m_renderTarget->setPriority( priority );
        }

        template <class T>
        u8 CRenderTarget<T>::getPriority() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->getPriority();
            }

            return 0;
        }

        template <class T>
        bool CRenderTarget<T>::isActive() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->isActive();
            }

            return false;
        }

        template <class T>
        void CRenderTarget<T>::setActive( bool state )
        {
            if( m_renderTarget )
            {
                m_renderTarget->setActive( state );
            }
        }

        template <class T>
        void CRenderTarget<T>::setAutoUpdated( bool autoupdate )
        {
            if( m_renderTarget )
            {
                m_renderTarget->setAutoUpdated( autoupdate );
            }
        }

        template <class T>
        bool CRenderTarget<T>::isAutoUpdated() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->isAutoUpdated();
            }

            return false;
        }

        template <class T>
        void CRenderTarget<T>::copyContentsToMemory( void *buffer, u32 size,
                                                     IRenderTarget::FrameBuffer bufferId /*= FB_AUTO */ )
        {
            // m_renderTarget->copyContentsToMemory( buffer, size, bufferId );
        }

        template <class T>
        void CRenderTarget<T>::swapBuffers()
        {
            if( m_renderTarget )
            {
                m_renderTarget->swapBuffers();
            }
        }

        template <class T>
        Array<SmartPtr<IViewport>> CRenderTarget<T>::getViewports() const
        {
            return m_viewports;
        }

        template <class T>
        Vector2I CRenderTarget<T>::getSize() const
        {
            if( auto texture = getTexture() )
            {
                return texture->getSize();
            }

            return m_size;
        }

        template <class T>
        void CRenderTarget<T>::setSize( const Vector2I &size )
        {
            if( auto texture = getTexture() )
            {
                texture->setSize( size );
            }
            else
            {
                auto targetSize = getSize();
                if( targetSize != size )
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    m_size = size;
                }
            }
        }

        template <class T>
        void CRenderTarget<T>::setColourDepth( u32 colourDepth )
        {
        }

        template <class T>
        bool CRenderTarget<T>::getSwapBuffers() const
        {
            return m_swapBuffers;
        }

        template <class T>
        void CRenderTarget<T>::setSwapBuffers( bool swapBuffers )
        {
            m_swapBuffers = swapBuffers;
        }

        template <class T>
        void CRenderTarget<T>::setRenderTarget( Ogre::RenderTarget *renderTarget )
        {
            if( m_renderTarget != renderTarget )
            {
                m_renderTarget = renderTarget;

                for( auto vp : m_viewports )
                {
                    vp->setRenderTarget( this );
                    vp->load( nullptr );
                }
            }
        }

        template <class T>
        Ogre::RenderTarget *CRenderTarget<T>::getRenderTarget() const
        {
            return m_renderTarget;
        }

        template <class T>
        SmartPtr<IStateContext> CRenderTarget<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <class T>
        void CRenderTarget<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <class T>
        SmartPtr<IStateListener> CRenderTarget<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CRenderTarget<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <class T>
        void CRenderTarget<T>::addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener )
        {
            m_rtListeners.push_back( listener );
        }

        template <class T>
        void CRenderTarget<T>::removeRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener )
        {
            auto it = std::find( m_rtListeners.begin(), m_rtListeners.end(), listener );
            if( it != m_rtListeners.end() )
            {
                m_rtListeners.erase( it );
            }
        }

        template <class T>
        Array<SmartPtr<IRenderTarget::Listener>> CRenderTarget<T>::getRenderTargetListeners() const
        {
            return m_rtListeners;
        }

        template <class T>
        void CRenderTarget<T>::setupStateObject()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateObject = stateManager->addStateObject();

                // auto sceneNodeStateListener = factoryManager->make_ptr<WindowStateListener>();
                // sceneNodeStateListener->setOwner( this );
                // m_stateListener = sceneNodeStateListener;
                // stateObject->addStateListener( m_stateListener );

                stateObject->setOwner( this );
                setStateObject( stateObject );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTarget<T>::destroyedStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            if( auto stateObject = getStateObject() )
            {
                stateObject->setOwner( nullptr );

                if( auto stateListener = getStateListener() )
                {
                    stateObject->removeStateListener( stateListener );
                }

                stateManager->removeStateObject( stateObject );

                stateObject->unload( nullptr );
                setStateObject( nullptr );
            }

            if( auto stateListener = getStateListener() )
            {
                stateListener->unload( nullptr );
                setStateListener( nullptr );
            }
        }

        template class CRenderTarget<IRenderTarget>;
        template class CRenderTarget<IRenderTexture>;
        template class CRenderTarget<IWindow>;
    }  // end namespace render
}  // end namespace fb
