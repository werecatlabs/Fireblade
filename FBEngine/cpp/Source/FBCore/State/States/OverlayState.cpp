#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/OverlayState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, OverlayState, BaseState );

    OverlayState::OverlayState() = default;

    OverlayState::~OverlayState() = default;

    auto OverlayState::isVisible() const -> bool
    {
        return m_visible;
    }

    void OverlayState::setVisible( bool visible )
    {
        m_visible = visible;

        setDirty( true );
    }

}  // end namespace fb
