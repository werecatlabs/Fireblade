#ifndef _CRenderTargetOgre_H
#define _CRenderTargetOgre_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IRenderTarget.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBGraphicsOgre/Wrapper/CViewportOgre.h>
#include <OgreRenderTarget.h>
#include <OgreViewport.h>

namespace fb
{
    namespace render
    {

        /** Implements IRenderTarget interface for Ogre. */
        template <class T>
        class CRenderTargetOgre : public SharedObject<T>
        {
        public:
            /** Constructor. */
            CRenderTargetOgre();

            /** Destructor. */
            ~CRenderTargetOgre() override;

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
        s32 CRenderTargetOgre<T>::m_ext = 0;

        template <class T>
        CRenderTargetOgre<T>::CRenderTargetOgre()
        {
            setupStateObject();
        }

        template <class T>
        CRenderTargetOgre<T>::~CRenderTargetOgre()
        {
            unload( nullptr );
            destroyedStateObject();
        }

        template <class T>
        void CRenderTargetOgre<T>::load( SmartPtr<ISharedObject> data )
        {
        }

        template <class T>
        void CRenderTargetOgre<T>::unload( SmartPtr<ISharedObject> data )
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
        void CRenderTargetOgre<T>::preUpdate()
        {
            try
            {
                if( m_renderTarget )
                {
                    auto size = m_renderTarget->getNumViewports();
                    FB_ASSERT( size <= m_viewports.size() );

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
        void CRenderTargetOgre<T>::update()
        {
            try
            {
                if( m_renderTarget )
                {
                    m_renderTarget->_beginUpdate();

                    for( auto vp : m_viewports )
                    {
                        vp->update();
                    }

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
        void CRenderTargetOgre<T>::postUpdate()
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
        void CRenderTargetOgre<T>::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
        {
            if( m_renderTarget )
            {
                m_renderTarget->getMetrics( width, height );
            }
        }

        template <class T>
        u32 CRenderTargetOgre<T>::getColourDepth() const
        {
            return 0;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgre<T>::addViewport( hash32 id, SmartPtr<ICamera> camera,
                                                               s32 ZOrder /*= 0*/, f32 left /*= 0.0f*/,
                                                               f32 top /*= 0.0f */, f32 width /*= 1.0f*/,
                                                               f32 height /*= 1.0f*/ )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();

                if( ZOrder == -1 )
                {
                    ZOrder = m_ext++;
                }

                auto viewport = factoryManager->make_ptr<CViewportOgre>();

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



                graphicsSystem->loadObject( viewport );

                return viewport;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        template <class T>
        u32 CRenderTargetOgre<T>::getNumViewports() const
        {
            return (u32)m_viewports.size();
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgre<T>::getViewport( u32 index )
        {
            FB_ASSERT( index < (u32)m_viewports.size() );

            if( index < (u32)m_viewports.size() )
            {
                return m_viewports[index];
            }

            return nullptr;
        }

        template <class T>
        SmartPtr<IViewport> CRenderTargetOgre<T>::getViewportById( hash32 id )
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
        SmartPtr<IViewport> CRenderTargetOgre<T>::getViewportByZOrder( s32 zorder ) const
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
        bool CRenderTargetOgre<T>::hasViewportWithZOrder( s32 zorder ) const
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
        void CRenderTargetOgre<T>::removeViewport( SmartPtr<IViewport> vp )
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
        void CRenderTargetOgre<T>::removeAllViewports()
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
        void CRenderTargetOgre<T>::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS,
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
        IRenderTarget::RenderTargetStats CRenderTargetOgre<T>::getRenderTargetStats() const
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
        void CRenderTargetOgre<T>::_getObject( void **ppObject ) const
        {
            *ppObject = m_renderTarget;
        }

        template <class T>
        void CRenderTargetOgre<T>::setPriority( u8 priority )
        {
            m_renderTarget->setPriority( priority );
        }

        template <class T>
        u8 CRenderTargetOgre<T>::getPriority() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->getPriority();
            }

            return 0;
        }

        template <class T>
        bool CRenderTargetOgre<T>::isActive() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->isActive();
            }

            return false;
        }


        template <class T>
        void CRenderTargetOgre<T>::setActive( bool state )
        {
            if( m_renderTarget )
            {
                m_renderTarget->setActive( state );
            }
        }

        template <class T>
        void CRenderTargetOgre<T>::setAutoUpdated( bool autoupdate )
        {
            if( m_renderTarget )
            {
                m_renderTarget->setAutoUpdated( autoupdate );
            }
        }

        template <class T>
        bool CRenderTargetOgre<T>::isAutoUpdated() const
        {
            if( m_renderTarget )
            {
                return m_renderTarget->isAutoUpdated();
            }

            return false;
        }

        template <class T>
        void CRenderTargetOgre<T>::copyContentsToMemory(
            void *buffer, u32 size, IRenderTarget::FrameBuffer bufferId /*= FB_AUTO */ )
        {
            // m_renderTarget->copyContentsToMemory( buffer, size, bufferId );
        }

        template <class T>
        void CRenderTargetOgre<T>::swapBuffers()
        {
            if( m_renderTarget )
            {
                m_renderTarget->swapBuffers();
            }
        }

        template <class T>
        Array<SmartPtr<IViewport>> CRenderTargetOgre<T>::getViewports() const
        {
            return m_viewports;
        }

        template <class T>
        Vector2I CRenderTargetOgre<T>::getSize() const
        {
            if( auto texture = getTexture() )
            {
                return texture->getSize();
            }

            return m_size;
        }

        template <class T>
        void CRenderTargetOgre<T>::setSize( const Vector2I &size )
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
        void CRenderTargetOgre<T>::setColourDepth( u32 colourDepth )
        {
        }

        template <class T>
        bool CRenderTargetOgre<T>::getSwapBuffers() const
        {
            return m_swapBuffers;
        }

        template <class T>
        void CRenderTargetOgre<T>::setSwapBuffers( bool swapBuffers )
        {
            m_swapBuffers = swapBuffers;
        }

        template <class T>
        void CRenderTargetOgre<T>::setRenderTarget( Ogre::RenderTarget *renderTarget )
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
        Ogre::RenderTarget *CRenderTargetOgre<T>::getRenderTarget() const
        {
            return m_renderTarget;
        }

        template <class T>
        SmartPtr<IStateContext> CRenderTargetOgre<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <class T>
        void CRenderTargetOgre<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <class T>
        SmartPtr<IStateListener> CRenderTargetOgre<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CRenderTargetOgre<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <class T>
        void CRenderTargetOgre<T>::addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener )
        {
            m_rtListeners.push_back( listener );
        }

        template <class T>
        void CRenderTargetOgre<T>::removeRenderTargetListener(
            SmartPtr<IRenderTarget::Listener> listener )
        {
            auto it = std::find( m_rtListeners.begin(), m_rtListeners.end(), listener );
            if( it != m_rtListeners.end() )
            {
                m_rtListeners.erase( it );
            }
        }

        template <class T>
        Array<SmartPtr<IRenderTarget::Listener>> CRenderTargetOgre<T>::getRenderTargetListeners() const
        {
            return m_rtListeners;
        }

        template <class T>
        void CRenderTargetOgre<T>::setupStateObject()
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
        void CRenderTargetOgre<T>::destroyedStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateManager = applicationManager->getStateManager() )
            {
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
            }

            if( auto stateListener = getStateListener() )
            {
                stateListener->unload( nullptr );
                setStateListener( nullptr );
            }
        }

        template <class T>
        SmartPtr<ITexture> CRenderTargetOgre<T>::getTexture() const
        {
            return m_texture.lock();
        }

        template <class T>
        void CRenderTargetOgre<T>::setTexture( SmartPtr<ITexture> texture )
        {
            m_texture = texture;
        }

    }  // end namespace render
}  // end namespace fb

#endif
