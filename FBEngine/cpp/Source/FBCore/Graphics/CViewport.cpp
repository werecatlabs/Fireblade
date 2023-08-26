#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CViewport.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CViewport, IViewport );

        CViewport::CViewport()
        {
        }

        CViewport::~CViewport()
        {
        }

        void CViewport::load( SmartPtr<ISharedObject> data )
        {
            CSharedGraphicsObject<IViewport>::load( data );
        }

        void CViewport::unload( SmartPtr<ISharedObject> data )
        {
            CSharedGraphicsObject<IViewport>::unload( data );
        }

        void CViewport::setCamera( SmartPtr<ICamera> camera )
        {
            if( auto currentCamera = getCamera() )
            {
                currentCamera->setViewport( nullptr );
            }

            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setCamera( camera );
                }
            }

            if( camera )
            {
                camera->setViewport( this );
            }
        }

        SmartPtr<ICamera> CViewport::getCamera() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getCamera();
                }
            }

            return nullptr;
        }

        void CViewport::setOverlaysEnabled( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setOverlaysEnabled( enabled );
                }
            }
        }

        bool CViewport::getOverlaysEnabled() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getOverlaysEnabled();
                }
            }

            return false;
        }

        void CViewport::setBackgroundColour( const ColourF &colour )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setBackgroundColour( colour );
                }
            }
        }

        ColourF CViewport::getBackgroundColour() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getBackgroundColour();
                }
            }

            return ColourF::White;
        }

        void CViewport::setZOrder( s32 zorder )
        {
            //FB_ASSERT(getRenderTarget() && getRenderTarget()->hasViewportWithZOrder( zorder ) == false);

            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setZOrder( zorder );
                }
            }
        }

        s32 CViewport::getZOrder() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getZOrder();
                }
            }

            return 0;
        }

        void CViewport::setPriority( u8 priority )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setPriority( priority );
                }
            }
        }

        u8 CViewport::getPriority() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getPriority();
                }
            }

            return 0;
        }

        bool CViewport::isActive() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->isActive();
                }
            }

            return false;
        }

        void CViewport::setActive( bool active )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setActive( active );
                }
            }
        }

        void CViewport::setAutoUpdated( bool autoUpdated )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setAutoUpdated( autoUpdated );
                }
            }
        }

        bool CViewport::isAutoUpdated() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->isAutoUpdated();
                }
            }

            return false;
        }

        void CViewport::setSkiesEnabled( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setSkiesEnabled( enabled );
                }
            }
        }

        bool CViewport::getSkiesEnabled() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getSkiesEnabled();
                }
            }

            return false;
        }

        void CViewport::setVisibilityMask( u32 mask )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setVisibilityMask( mask );
                }
            }
        }

        u32 CViewport::getVisibilityMask() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getVisibilityMask();
                }
            }

            return 0;
        }

        void CViewport::setShadowsEnabled( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setShadowsEnabled( enabled );
                }
            }
        }

        bool CViewport::getShadowsEnabled() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getShadowsEnabled();
                }
            }

            return false;
        }

        Vector2F CViewport::getPosition() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getPosition();
                }
            }

            return Vector2F::zero();
        }

        Vector2F CViewport::getActualPosition() const
        {
            return Vector2F::zero();
        }

        void CViewport::setPosition( const Vector2F &position )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setPosition( position );
                }
            }
        }

        Vector2F CViewport::getSize() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getSize();
                }
            }

            return Vector2F::zero();
        }

        fb::Vector2F CViewport::getActualSize() const
        {
            return Vector2F::zero();
        }

        void CViewport::setSize( const Vector2F &size )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setSize( size );
                }
            }
        }

        SmartPtr<ITexture> CViewport::getBackgroundTexture() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getBackgroundTexture();
                }
            }

            return nullptr;
        }

        void CViewport::setBackgroundTexture( SmartPtr<ITexture> texture )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setBackgroundTexture( texture );
                }
            }
        }

        void CViewport::setEnableUI( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setEnableUI( enabled );
                }
            }
        }

        bool CViewport::getEnableUI() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getEnableUI();
                }
            }

            return false;
        }

        void CViewport::setEnableSceneRender( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setEnableSceneRender( enabled );
                }
            }
        }

        bool CViewport::getEnableSceneRender() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getEnableSceneRender();
                }
            }

            return false;
        }

        void CViewport::setMaterialScheme( const String &schemeName )
        {
            ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            //m_viewport->setMaterialScheme( schemeName.c_str() );
        }

        String CViewport::getMaterialScheme() const
        {
            return StringUtil::EmptyString;
        }

        void CViewport::setClearEveryFrame( bool clear, u32 buffers )
        {
            //if( m_viewport )
            //{
            //    m_viewport->setClearEveryFrame( clear, buffers );
            //}
        }

        bool CViewport::getClearEveryFrame() const
        {
            ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            //return m_viewport->getClearEveryFrame();

            return false;
        }

        u32 CViewport::getClearBuffers() const
        {
            ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            //return m_viewport->getClearBuffers();

            return 0;
        }

        SmartPtr<IWindow> CViewport::getWindow() const
        {
            return m_window.lock();
        }

        void CViewport::setWindow( SmartPtr<IWindow> window )
        {
            m_window = window;
        }

        String CViewport::getBackgroundTextureName() const
        {
            return "";
        }

        void CViewport::setBackgroundTextureName( const String &textureName )
        {
        }

        SmartPtr<IRenderTarget> CViewport::getRenderTarget() const
        {
            return m_renderTarget.lock();
        }

        void CViewport::setRenderTarget( SmartPtr<IRenderTarget> renderTarget )
        {
            if( m_renderTarget )
            {
                removeViewportFromRT();
            }

            m_renderTarget = renderTarget;
        }

        void CViewport::removeViewportFromRT()
        {
        }
    }  // namespace render
}  // namespace fb
