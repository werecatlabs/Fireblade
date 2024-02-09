#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/UIElementState.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, UIElementState, BaseState );

    UIElementState::UIElementState() = default;

    UIElementState::~UIElementState() = default;

    auto UIElementState::getPosition() const -> Vector2<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_position;
    }

    void UIElementState::setPosition( const Vector2<real_Num> &position )
    {
        if( getPosition() != position )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_position = position;
            setDirty( true );
        }
    }

    auto UIElementState::getSize() const -> Vector2<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_size;
    }

    void UIElementState::setSize( const Vector2<real_Num> &size )
    {
        if( getSize() != size )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_size = size;
            setDirty( true );
        }
    }

    void UIElementState::setVisible( bool visible )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_visible != visible )
        {
            m_visible = visible;
            setDirty( true );
        }
    }

    auto UIElementState::isVisible() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_visible;
    }

    auto UIElementState::getElementVisible() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_elementVisible;
    }

    void UIElementState::setElementVisible( bool elementVisible )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_elementVisible = elementVisible;
    }

    auto UIElementState::getMaterial() const -> SmartPtr<render::IMaterial>
    {
        return m_material;
    }

    void UIElementState::setMaterial( SmartPtr<render::IMaterial> material )
    {
        if( getMaterial() != material )
        {
            m_material = material;
            setDirty( true );
        }
    }

    auto UIElementState::getTexture() const -> SmartPtr<render::ITexture>
    {
        return m_texture;
    }

    void UIElementState::setTexture( SmartPtr<render::ITexture> texture )
    {
        if( getTexture() != texture )
        {
            m_texture = texture;
            setDirty( true );
        }
    }

    auto UIElementState::getCaption() const -> String
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_caption;
    }

    void UIElementState::setCaption( const String &caption )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_caption != caption )
        {
            m_caption = caption;
            setDirty( true );
        }
    }

    auto UIElementState::getColour() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_colour;
    }

    void UIElementState::setColour( const ColourF &colour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        //if( m_colour != colour )
        {
            m_colour = colour;
            setDirty( true );
        }
    }

    auto UIElementState::getTextSize() const -> u32
    {
        return m_textSize;
    }

    void UIElementState::setTextSize( u32 textSize )
    {
        m_textSize = textSize;
        setDirty( true );
    }

    auto UIElementState::getZOrder() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_zOrder;
    }

    void UIElementState::setZOrder( u32 zOrder )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_zOrder != zOrder )
        {
            m_zOrder = zOrder;
            setDirty( true );
        }
    }

    auto UIElementState::getHandleInputEvents() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_handleInputEvents;
    }

    void UIElementState::setHandleInputEvents( bool handleInputEvents )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if( m_handleInputEvents != handleInputEvents )
        {
            m_handleInputEvents = handleInputEvents;
            setDirty( true );
        }
    }

    auto UIElementState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<UIElementState>();
        makeClone( state );

        SpinRWMutex::ScopedLock lock( m_mutex, false );
        //state->m_texture = m_texture;
        state->m_colour = m_colour;

        state->m_zOrder = m_zOrder;

        state->m_metricsMode = m_metricsMode;
        state->m_gha = m_gha;
        state->m_gva = m_gva;

        state->m_position = m_position;
        state->m_size = m_size;
        state->m_name = m_name;
        state->m_caption = m_caption;

        state->m_textSize = m_textSize;

        state->m_visible = m_visible;
        state->m_handleInputEvents = m_handleInputEvents;

        return state;
    }
}  // namespace fb
