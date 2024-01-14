#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Viewport.h>
#include <FBCore/FBCore.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, Viewport, IViewport );

    Viewport::Viewport() = default;

    Viewport::~Viewport() = default;

    void Viewport::load( SmartPtr<ISharedObject> data )
    {
        SharedGraphicsObject<IViewport>::load( data );
    }

    void Viewport::unload( SmartPtr<ISharedObject> data )
    {
        SharedGraphicsObject<IViewport>::unload( data );
    }

    void Viewport::setCamera( SmartPtr<ICamera> camera )
    {
        if( auto currentCamera = getCamera() )
        {
            currentCamera->setViewport( nullptr );
        }

        if( auto stateContext = getStateContext() )
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

    auto Viewport::getCamera() const -> SmartPtr<ICamera>
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getCamera();
            }
        }

        return nullptr;
    }

    void Viewport::setOverlaysEnabled( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setOverlaysEnabled( enabled );
            }
        }
    }

    auto Viewport::getOverlaysEnabled() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getOverlaysEnabled();
            }
        }

        return false;
    }

    void Viewport::setBackgroundColour( const ColourF &colour )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setBackgroundColour( colour );
            }
        }
    }

    auto Viewport::getBackgroundColour() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getBackgroundColour();
            }
        }

        return ColourF::White;
    }

    void Viewport::setZOrder( s32 zorder )
    {
        //FB_ASSERT(getRenderTarget() && getRenderTarget()->hasViewportWithZOrder( zorder ) == false);

        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setZOrder( zorder );
            }
        }
    }

    auto Viewport::getZOrder() const -> s32
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getZOrder();
            }
        }

        return 0;
    }

    void Viewport::setPriority( u8 priority )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setPriority( priority );
            }
        }
    }

    auto Viewport::getPriority() const -> u8
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getPriority();
            }
        }

        return 0;
    }

    auto Viewport::isActive() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->isActive();
            }
        }

        return false;
    }

    void Viewport::setActive( bool active )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setActive( active );
            }
        }
    }

    void Viewport::setAutoUpdated( bool autoUpdated )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setAutoUpdated( autoUpdated );
            }
        }
    }

    auto Viewport::isAutoUpdated() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->isAutoUpdated();
            }
        }

        return false;
    }

    void Viewport::setSkiesEnabled( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setSkiesEnabled( enabled );
            }
        }
    }

    auto Viewport::getSkiesEnabled() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getSkiesEnabled();
            }
        }

        return false;
    }

    void Viewport::setVisibilityMask( u32 mask )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setVisibilityMask( mask );
            }
        }
    }

    auto Viewport::getVisibilityMask() const -> u32
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getVisibilityMask();
            }
        }

        return 0;
    }

    void Viewport::setShadowsEnabled( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setShadowsEnabled( enabled );
            }
        }
    }

    auto Viewport::getShadowsEnabled() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getShadowsEnabled();
            }
        }

        return false;
    }

    auto Viewport::getPosition() const -> Vector2F
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getPosition();
            }
        }

        return Vector2F::zero();
    }

    auto Viewport::getActualPosition() const -> Vector2F
    {
        return Vector2F::zero();
    }

    void Viewport::setPosition( const Vector2F &position )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setPosition( position );
            }
        }
    }

    auto Viewport::getSize() const -> Vector2F
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getSize();
            }
        }

        return Vector2F::zero();
    }

    auto Viewport::getActualSize() const -> fb::Vector2F
    {
        return Vector2F::zero();
    }

    void Viewport::setSize( const Vector2F &size )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setSize( size );
            }
        }
    }

    auto Viewport::getBackgroundTexture() const -> SmartPtr<ITexture>
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getBackgroundTexture();
            }
        }

        return nullptr;
    }

    void Viewport::setBackgroundTexture( SmartPtr<ITexture> texture )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setBackgroundTexture( texture );
            }
        }
    }

    void Viewport::setEnableUI( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setEnableUI( enabled );
            }
        }
    }

    auto Viewport::getEnableUI() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getEnableUI();
            }
        }

        return false;
    }

    void Viewport::setEnableSceneRender( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                state->setEnableSceneRender( enabled );
            }
        }
    }

    auto Viewport::getEnableSceneRender() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<ViewportState>() )
            {
                return state->getEnableSceneRender();
            }
        }

        return false;
    }

    void Viewport::setMaterialScheme( const String &schemeName )
    {
        ScopedLock lock( core::ApplicationManager::instance()->getGraphicsSystem() );
        //m_viewport->setMaterialScheme( schemeName.c_str() );
    }

    auto Viewport::getMaterialScheme() const -> String
    {
        return StringUtil::EmptyString;
    }

    void Viewport::setClearEveryFrame( bool clear, u32 buffers )
    {
        //if( m_viewport )
        //{
        //    m_viewport->setClearEveryFrame( clear, buffers );
        //}
    }

    auto Viewport::getClearEveryFrame() const -> bool
    {
        ScopedLock lock( core::ApplicationManager::instance()->getGraphicsSystem() );
        //return m_viewport->getClearEveryFrame();

        return false;
    }

    auto Viewport::getClearBuffers() const -> u32
    {
        ScopedLock lock( core::ApplicationManager::instance()->getGraphicsSystem() );
        //return m_viewport->getClearBuffers();

        return 0;
    }

    auto Viewport::getWindow() const -> SmartPtr<IWindow>
    {
        return m_window.lock();
    }

    void Viewport::setWindow( SmartPtr<IWindow> window )
    {
        m_window = window;
    }

    auto Viewport::getBackgroundTextureName() const -> String
    {
        return "";
    }

    void Viewport::setBackgroundTextureName( const String &textureName )
    {
    }

    auto Viewport::getRenderTarget() const -> SmartPtr<IRenderTarget>
    {
        return m_renderTarget.lock();
    }

    void Viewport::setRenderTarget( SmartPtr<IRenderTarget> renderTarget )
    {
        if( m_renderTarget )
        {
            removeViewportFromRT();
        }

        m_renderTarget = renderTarget;
    }

    void Viewport::removeViewportFromRT()
    {
    }
}  // namespace fb::render
