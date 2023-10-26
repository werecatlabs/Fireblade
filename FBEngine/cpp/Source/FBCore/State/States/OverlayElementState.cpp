#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/OverlayElementState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/System/RttiClassDefinition.h>

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
        if( m_position != position )
        {
            m_position = position;
            setDirty( true );
        }
    }

    Vector2F OverlayElementState::getSize() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_size;
    }

    void OverlayElementState::setSize( const Vector2F &size )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_size != size )
        {
            m_size = size;
            setDirty( true );
        }
    }

    String OverlayElementState::getName() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_name;
    }

    void OverlayElementState::setName( const String &name )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_name != name )
        {
            m_name = name;
            setDirty( true );
        }
    }

    void OverlayElementState::setMetricsMode( [[maybe_unused]] u8 metricsMode )
    {
        if( m_metricsMode != metricsMode )
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
        if( m_gha != gha )
        {
            m_gha = gha;
            setDirty( true );
        }
    }

    u8 OverlayElementState::getHorizontalAlignment() const
    {
        return m_gha;
    }

    void OverlayElementState::setVerticalAlignment( [[maybe_unused]] u8 gva )
    {
        if( m_gva != gva )
        {
            m_gva = gva;
            setDirty( true );
        }
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
        if( getMaterial() != material )
        {
            m_material = material;
            setDirty( true );
        }
    }

    void OverlayElementState::setCaption( [[maybe_unused]] const String &text )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_caption != text )
        {
            m_caption = text;
            setDirty( true );
        }
    }

    String OverlayElementState::getCaption() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_caption;
    }

    void OverlayElementState::setVisible( [[maybe_unused]] bool visible )
    {
        if( m_visible != visible )
        {
            m_visible = visible;
            setDirty( true );
        }
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
        if( m_zOrder != zOrder )
        {
            m_zOrder = zOrder;
            setDirty( true );
        }
    }

    void OverlayElementState::setColour( const ColourF &colour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_colour != colour )
        {
            m_colour = colour;
            setDirty( true );
        }
    }

    ColourF OverlayElementState::getColour() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_colour;
    }
}  // end namespace fb
