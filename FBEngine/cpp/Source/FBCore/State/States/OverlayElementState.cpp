#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/OverlayElementState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, OverlayElementState, BaseState );

    Vector2F OverlayElementState::getPosition() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_position;
    }

    void OverlayElementState::setPosition( const Vector2F &position )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_position = position;
        setDirty( true );
    }

    Vector2F OverlayElementState::getSize() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_size;
    }

    void OverlayElementState::setSize( const Vector2F &size )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_size = size;
        setDirty( true );
    }

    String OverlayElementState::getName() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_name;
    }

    void OverlayElementState::setName( const String &name )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_name = name;
        setDirty( true );
    }

    void OverlayElementState::setMetricsMode( [[maybe_unused]] u8 metricsMode )
    {
        if(m_metricsMode != metricsMode)
        {
            m_metricsMode = metricsMode;
            setDirty( true );
        }
    }

    u8 OverlayElementState::getMetricsMode() const
    {
        return m_metricsMode;
    }

    void OverlayElementState::setHorizontalAlignment( [[maybe_unused]] u8 gha )
    {
        m_gha = gha;
        setDirty( true );
    }

    u8 OverlayElementState::getHorizontalAlignment() const
    {
        return m_gha;
    }

    void OverlayElementState::setVerticalAlignment( [[maybe_unused]] u8 gva )
    {
        m_gva = gva;
        setDirty( true );
    }

    u8 OverlayElementState::getVerticalAlignment() const
    {
        return m_gva;
    }

    SmartPtr<render::IMaterial> OverlayElementState::getMaterial() const
    {
        return m_material;
    }

    void OverlayElementState::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
        setDirty( true );
    }

    void OverlayElementState::setCaption( [[maybe_unused]] const String &text )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_caption = text;
        setDirty( true );
    }

    String OverlayElementState::getCaption() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_caption;
    }

    void OverlayElementState::setVisible( [[maybe_unused]] bool visible )
    {
        m_visible = visible;
        setDirty( true );
    }

    bool OverlayElementState::isVisible() const
    {
        return m_visible;
    }

    u32 OverlayElementState::getZOrder() const
    {
        return m_zOrder;
    }

    void OverlayElementState::setZOrder( u32 zOrder )
    {
        m_zOrder = zOrder;
        setDirty( true );
    }

    void OverlayElementState::setColour( const ColourF &colour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_colour = colour;
        setDirty( true );
    }

    ColourF OverlayElementState::getColour() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_colour;
    }
} // end namespace fb
