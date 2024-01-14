#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/ViewportState.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ViewportState, BaseState );

    ViewportState::ViewportState() = default;

    ViewportState::~ViewportState()
    {
        unload( nullptr );
    }

    void ViewportState::unload( SmartPtr<ISharedObject> data )
    {
        m_camera = nullptr;
        BaseState::unload( nullptr );
    }

    void ViewportState::setBackgroundColour( const ColourF &colour )
    {
        if( m_backgroundColour != colour )
        {
            m_backgroundColour = colour;
            setDirty( true );
        }
    }

    auto ViewportState::getBackgroundColour() const -> ColourF
    {
        return m_backgroundColour;
    }

    auto ViewportState::getPosition() const -> Vector2F
    {
        return m_position;
    }

    void ViewportState::setPosition( const Vector2F &position )
    {
        m_position = position;
        setDirty( true );
    }

    auto ViewportState::getSize() const -> Vector2F
    {
        return m_size;
    }

    void ViewportState::setSize( const Vector2F &size )
    {
        m_size = size;
        setDirty( true );
    }

    auto ViewportState::getOverlaysEnabled() const -> bool
    {
        return m_overlaysEnabled;
    }

    void ViewportState::setOverlaysEnabled( bool overlaysEnabled )
    {
        if( m_overlaysEnabled != overlaysEnabled )
        {
            m_overlaysEnabled = overlaysEnabled;
            setDirty( true );
        }
    }

    auto ViewportState::getCamera() const -> SmartPtr<render::ICamera>
    {
        return m_camera;
    }

    void ViewportState::setCamera( SmartPtr<render::ICamera> camera )
    {
        if( getCamera() != camera )
        {
            m_camera = camera;
            setDirty( true );
        }
    }

    void ViewportState::setVisibilityMask( u32 mask )
    {
        m_visibilityMask = mask;
        setDirty( true );
    }

    auto ViewportState::getVisibilityMask() const -> u32
    {
        return m_visibilityMask;
    }

    void ViewportState::setSkiesEnabled( bool enabled )
    {
        m_skiesEnabled = enabled;
        setDirty( true );
    }

    auto ViewportState::getSkiesEnabled() const -> bool
    {
        return m_skiesEnabled;
    }

    auto ViewportState::getZOrder() const -> s32
    {
        return m_zOrder;
    }

    void ViewportState::setZOrder( s32 zorder )
    {
        m_zOrder = zorder;
        setDirty( true );
    }

    auto ViewportState::isActive() const -> bool
    {
        return m_active;
    }

    void ViewportState::setActive( bool active )
    {
        if( m_active != active )
        {
            m_active = active;
            setDirty( true );
        }
    }

    auto ViewportState::getPriority() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_priority;
    }

    void ViewportState::setPriority( u32 priority )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_priority != priority )
        {
            m_priority = priority;
            setDirty( true );
        }
    }

    void ViewportState::setAutoUpdated( bool autoUpdated )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_autoUpdated = autoUpdated;
        setDirty( true );
    }

    auto ViewportState::isAutoUpdated() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_autoUpdated;
    }

    auto ViewportState::getTexture() const -> SmartPtr<render::ITexture>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_texture;
    }

    void ViewportState::setTexture( SmartPtr<render::ITexture> texture )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_texture = texture;
        setDirty( true );
    }

    auto ViewportState::getBackgroundTexture() const -> SmartPtr<render::ITexture>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_backgroundTexture;
    }

    void ViewportState::setBackgroundTexture( SmartPtr<render::ITexture> backgroundTexture )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_backgroundTexture = backgroundTexture;
        setDirty( true );
    }

    auto ViewportState::getShadowsEnabled() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_shadowsEnabled;
    }

    void ViewportState::setShadowsEnabled( bool shadowsEnabled )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_shadowsEnabled = shadowsEnabled;
        setDirty( true );
    }

    auto ViewportState::getEnableUI() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_enableUI;
    }

    void ViewportState::setEnableUI( bool enableUI )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_enableUI = enableUI;
        setDirty( true );
    }

    void ViewportState::setMaterialScheme( const String &schemeName )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_materialScheme = schemeName;
        setDirty( true );
    }

    auto ViewportState::getMaterialScheme() const -> String
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_materialScheme;
    }

    auto ViewportState::getClear() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_clear;
    }

    void ViewportState::setClear( bool clear )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_clear = clear;
        setDirty( true );
    }

    auto ViewportState::getBuffers() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_buffers;
    }

    void ViewportState::setBuffers( u32 buffers )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_buffers = buffers;
        setDirty( true );
    }

    auto ViewportState::getEnableSceneRender() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_enableSceneRender;
    }

    void ViewportState::setEnableSceneRender( bool enableSceneRender )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_enableSceneRender = enableSceneRender;
        setDirty( true );
    }

}  // namespace fb
