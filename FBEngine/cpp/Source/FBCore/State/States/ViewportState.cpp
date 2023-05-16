#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/ViewportState.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ViewportState, BaseState );

    ViewportState::ViewportState()
    {
    }

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

    ColourF ViewportState::getBackgroundColour() const
    {
        return m_backgroundColour;
    }

    Vector2F ViewportState::getPosition() const
    {
        return m_position;
    }

    void ViewportState::setPosition( const Vector2F &position )
    {
        m_position = position;
        setDirty( true );
    }

    Vector2F ViewportState::getSize() const
    {
        return m_size;
    }

    void ViewportState::setSize( const Vector2F &size )
    {
        m_size = size;
        setDirty( true );
    }

    bool ViewportState::getOverlaysEnabled() const
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

    SmartPtr<render::ICamera> ViewportState::getCamera() const
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

    u32 ViewportState::getVisibilityMask() const
    {
        return m_visibilityMask;
    }

    void ViewportState::setSkiesEnabled( bool enabled )
    {
        m_skiesEnabled = enabled;
        setDirty( true );
    }

    bool ViewportState::getSkiesEnabled( void ) const
    {
        return m_skiesEnabled;
    }

    s32 ViewportState::getZOrder() const
    {
        return m_zOrder;
    }

    void ViewportState::setZOrder( s32 zorder )
    {
        m_zOrder = zorder;
        setDirty( true );
    }

    bool ViewportState::isActive() const
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

    u32 ViewportState::getPriority() const
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

    bool ViewportState::isAutoUpdated() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_autoUpdated;
    }

    SmartPtr<render::ITexture> ViewportState::getTexture() const
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

    SmartPtr<render::ITexture> ViewportState::getBackgroundTexture() const
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

    bool ViewportState::getShadowsEnabled() const
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

    bool ViewportState::getEnableUI() const
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
}  // namespace fb
