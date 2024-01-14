#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/CompositorState.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CompositorState, BaseState );

    CompositorState::CompositorState() = default;

    CompositorState::~CompositorState() = default;

    auto CompositorState::isEnabled() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_enabled;
    }

    void CompositorState::setEnabled( bool enabled )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_enabled != enabled )
        {
            m_enabled = enabled;
            setDirty( true );
        }
    }

}  // namespace fb
