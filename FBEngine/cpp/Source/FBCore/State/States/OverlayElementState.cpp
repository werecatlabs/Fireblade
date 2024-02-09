#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/OverlayElementState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, OverlayElementState, BaseState );

    OverlayElementState::OverlayElementState() = default;
    OverlayElementState::~OverlayElementState() = default;

    auto OverlayElementState::getPosition() const -> Vector2<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_position;
    }

    void OverlayElementState::setPosition( const Vector2<real_Num> &position )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_position != position )
        {
            m_position = position;
            setDirty( true );
        }
    }

    auto OverlayElementState::getSize() const -> Vector2<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_size;
    }

    void OverlayElementState::setSize( const Vector2<real_Num> &size )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_size != size )
        {
            m_size = size;
            setDirty( true );
        }
    }

    auto OverlayElementState::getName() const -> String
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

    auto OverlayElementState::getMetricsMode() const -> u8
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

    auto OverlayElementState::getHorizontalAlignment() const -> u8
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

    auto OverlayElementState::getVerticalAlignment() const -> u8
    {
        return m_gva;
    }

    auto OverlayElementState::getMaterial() const -> SmartPtr<render::IMaterial>
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

    auto OverlayElementState::getCaption() const -> String
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

    auto OverlayElementState::isVisible() const -> bool
    {
        return m_visible;
    }

    auto OverlayElementState::getZOrder() const -> u32
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

    auto OverlayElementState::getColour() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_colour;
    }
}  // end namespace fb
