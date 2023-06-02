#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/CompositorState.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CompositorState, BaseState );

    CompositorState::CompositorState()
    {
    }

    CompositorState::~CompositorState()
    {
    }

    bool CompositorState::isEnabled() const
    {
        return m_enabled;
    }

    void CompositorState::setEnabled( bool enabled )
    {
        m_enabled = enabled;
        setDirty( true );
    }

}  // namespace fb
