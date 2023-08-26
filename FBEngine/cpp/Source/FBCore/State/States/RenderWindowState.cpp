#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/RenderWindowState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RenderWindowState, BaseState );

    RenderWindowState::RenderWindowState()
    {
    }

    RenderWindowState::~RenderWindowState()
    {
    }

    SmartPtr<IState> RenderWindowState::clone() const
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

    Vector2I RenderWindowState::getSize() const
    {
        return m_size;
    }

    void RenderWindowState::setSize( const Vector2I &size )
    {
        m_size = size;
        setDirty( true );
    }

    u32 RenderWindowState::getColourDepth() const
    {
        return m_colourDepth;
    }

    void RenderWindowState::setColourDepth( u32 colourDepth )
    {
        m_colourDepth = colourDepth;
        setDirty( true );
    }

    bool RenderWindowState::isFullscreen() const
    {
        return m_fullscreen;
    }

    void RenderWindowState::setFullscreen( bool fullscreen )
    {
        m_fullscreen = fullscreen;
        setDirty( true );
    }

    bool RenderWindowState::isVisible() const
    {
        return m_visible;
    }

    void RenderWindowState::setVisible( bool visible )
    {
        m_visible = visible;
        setDirty( true );
    }

    String RenderWindowState::getLabel() const
    {
        return m_label;
    }

    void RenderWindowState::setLabel( const String &label )
    {
        m_label = label;
        setDirty( true );
    }

    String RenderWindowState::getWindowHandle() const
    {
        return m_windowHandle;
    }

    void RenderWindowState::setWindowHandle( const String &windowHandle )
    {
        m_windowHandle = windowHandle;
        setDirty( true );
    }
}  // end namespace fb
