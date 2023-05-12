#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/UIElementState.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ITexture.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, UIElementState, BaseState );

    UIElementState::UIElementState()
    {
    }

    UIElementState::~UIElementState()
    {
    }

    Vector2F UIElementState::getPosition() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_position;
    }

    void UIElementState::setPosition( const Vector2F &position )
    {
        if(getPosition() != position)
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_position = position;
            setDirty( true );
        }
    }

    Vector2F UIElementState::getSize() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_size;
    }

    void UIElementState::setSize( const Vector2F &size )
    {
        if(getSize() != size)
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_size = size;
            setDirty( true );
        }
    }

    void UIElementState::setVisible( bool visible )
    {
        m_visible = visible;
        setDirty( true );
    }

    bool UIElementState::isVisible() const
    {
        return m_visible;
    }

    SmartPtr<render::IMaterial> UIElementState::getMaterial() const
    {
        return m_material;
    }

    void UIElementState::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
        setDirty( true );
    }

    SmartPtr<render::ITexture> UIElementState::getTexture() const
    {
        return m_texture;
    }

    void UIElementState::setTexture( SmartPtr<render::ITexture> texture )
    {
        m_texture = texture;
        setDirty( true );
    }

    String UIElementState::getCaption() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_caption;
    }

    void UIElementState::setCaption( const String &caption )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_caption = caption;
        setDirty( true );
    }

    ColourF UIElementState::getColour() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_colour;
    }

    void UIElementState::setColour( const ColourF &colour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_colour = colour;
        setDirty( true );
    }

    u32 UIElementState::getTextSize() const
    {
        return m_textSize;
    }

    void UIElementState::setTextSize( u32 textSize )
    {
        m_textSize = textSize;
        setDirty( true );
    }

    u32 UIElementState::getZOrder() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_zOrder;
    }

    void UIElementState::setZOrder( u32 zOrder )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if(m_zOrder != zOrder)
        {
            m_zOrder = zOrder;
            setDirty( true );
        }
    }

    bool UIElementState::getHandleInputEvents() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_handleInputEvents;
    }

    void UIElementState::setHandleInputEvents( bool handleInputEvents )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        if(m_handleInputEvents != handleInputEvents)
        {
            m_handleInputEvents = handleInputEvents;
            setDirty( true );
        }
    }

    SmartPtr<IState> UIElementState::clone() const
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
} // namespace fb
