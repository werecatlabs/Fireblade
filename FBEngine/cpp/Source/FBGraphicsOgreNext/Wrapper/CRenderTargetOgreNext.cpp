#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CRenderTargetOgreNext.h>
//#include <FBGraphicsOgreNext/Wrapper/CRenderTexture.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        template <class T>
        CRenderTargetOgreNext<T>::CRenderTargetOgreNext()
        {
            // Ogre::RenderTarget* rtt = m_texture->getBuffer()->getRenderTarget();
        }

        template <class T>
        CRenderTargetOgreNext<T>::~CRenderTargetOgreNext()
        {
            // m_renderTarget->removeAllViewports();
        }

        template <class T>
        void CRenderTargetOgreNext<T>::load( SmartPtr<ISharedObject> data )
        {
        }

        template <class T>
        void CRenderTargetOgreNext<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                for( auto &[k, vp] : m_viewports )
                {
                    vp->unload( nullptr );
                }

                m_viewports.clear();

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                destroyedStateObject();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CRenderTargetOgreNext<T>::initialise( Ogre::RenderTarget *renderTarget )
        {
            // m_renderTarget = renderTarget;
        }

        template <class T>
        void CRenderTargetOgreNext<T>::update()
        {
            // m_renderTarget->update(swapBuffers);
        }

        template <class T>
        void CRenderTargetOgreNext<T>::addRenderTargetListener(SmartPtr<IRenderTarget::Listener> listener)
        {

        }

        template <class T>
        void CRenderTargetOgreNext<T>::removeRenderTargetListener(SmartPtr<IRenderTarget::Listener> listener)
        {

        }

        template <class T>
        Array<SmartPtr<IRenderTarget::Listener>> CRenderTargetOgreNext<T>::getRenderTargetListeners() const
        {
            return Array<SmartPtr<IRenderTarget::Listener>>();
        }


        template <class T>
        void CRenderTargetOgreNext<T>::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
        {
        }

        template <class T>
        u32 CRenderTargetOgreNext<T>::getWidth() const
        {
            return 0;
        }

        template <class T>
        u32 CRenderTargetOgreNext<T>::getHeight() const
        {
            return 0;
        }

        template <class T>
        u32 CRenderTargetOgreNext<T>::getColourDepth() const
        {
            return 0;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgreNext<T>::addViewport( hash32 id, SmartPtr<ICamera> camera,
                                                        s32 ZOrder /*= 0*/, f32 left /*= 0.0f*/,
                                                        f32 top /*= 0.0f */, f32 width /*= 1.0f*/,
                                                        f32 height /*= 1.0f*/ )
        {
            try
            {
                Ogre::Camera *ogreCamera;
                camera->_getObject( (void **)&ogreCamera );

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
        u32 CRenderTargetOgreNext<T>::getNumViewports() const
        {
            return 0;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgreNext<T>::getViewport( u32 index )
        {
            return SmartPtr<IViewport>();
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgreNext<T>::getViewportById( hash32 id )
        {
            return SmartPtr<IViewport>();
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
        void CRenderTargetOgreNext<T>::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const
        {
        }

        template <class T>
        IRenderTarget::RenderTargetStats CRenderTargetOgreNext<T>::getRenderTargetStats() const
        {
            return IRenderTarget::RenderTargetStats();
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
        u8 CRenderTargetOgreNext<T>::getPriority() const
        {
            // return m_renderTarget->getPriority();
            return 0;
        }

        template <class T>
        bool CRenderTargetOgreNext<T>::isActive() const
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
        bool CRenderTargetOgreNext<T>::isAutoUpdated( void ) const
        {
            // return m_renderTarget->isAutoUpdated();
            return false;
        }

        template <class T>
        void CRenderTargetOgreNext<T>::copyContentsToMemory( void *buffer, u32 size,
                                                     IRenderTarget::FrameBuffer bufferId /*= FB_AUTO */ )
        {
        }

        template <class T>
        void CRenderTargetOgreNext<T>::swapBuffers()
        {
            // m_renderTarget->swapBuffers();
        }

        template <class T>
        Array<SmartPtr<IViewport>> CRenderTargetOgreNext<T>::getViewports() const
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
        Vector2I CRenderTargetOgreNext<T>::getSize() const
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
        void CRenderTargetOgreNext<T>::destroyedStateObject()
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



        template class CRenderTargetOgreNext<IRenderTarget>;
        //template class CRenderTarget<IRenderTexture>;
        template class CRenderTargetOgreNext<IWindow>;
    }  // end namespace render
}  // end namespace fb
