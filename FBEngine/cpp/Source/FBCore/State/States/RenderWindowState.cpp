#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/RenderWindowState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RenderWindowState, BaseState );

    RenderWindowState::RenderWindowState() = default;

    RenderWindowState::~RenderWindowState() = default;

    auto RenderWindowState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<RenderWindowState>();

        state->m_size = m_size;
        state->m_colourDepth = m_colourDepth;
        state->m_fullscreen = m_fullscreen;
        state->m_visible = m_visible;
        state->m_label = m_label;
        state->m_windowHandle = m_windowHandle;

        return state;
    }

    auto RenderWindowState::getSize() const -> Vector2I
    {
        return m_size;
    }

    void RenderWindowState::setSize( const Vector2I &size )
    {
        m_size = size;
        setDirty( true );
    }

    auto RenderWindowState::getColourDepth() const -> u32
    {
        return m_colourDepth;
    }

    void RenderWindowState::setColourDepth( u32 colourDepth )
    {
        m_colourDepth = colourDepth;
        setDirty( true );
    }

    auto RenderWindowState::isFullscreen() const -> bool
    {
        return m_fullscreen;
    }

    void RenderWindowState::setFullscreen( bool fullscreen )
    {
        m_fullscreen = fullscreen;
        setDirty( true );
    }

    auto RenderWindowState::isVisible() const -> bool
    {
        return m_visible;
    }

    void RenderWindowState::setVisible( bool visible )
    {
        m_visible = visible;
        setDirty( true );
    }

    auto RenderWindowState::getLabel() const -> String
    {
        return m_label;
    }

    void RenderWindowState::setLabel( const String &label )
    {
        m_label = label;
        setDirty( true );
    }

    auto RenderWindowState::getWindowHandle() const -> String
    {
        return m_windowHandle;
    }

    void RenderWindowState::setWindowHandle( const String &windowHandle )
    {
        m_windowHandle = windowHandle;
        setDirty( true );
    }
}  // end namespace fb
