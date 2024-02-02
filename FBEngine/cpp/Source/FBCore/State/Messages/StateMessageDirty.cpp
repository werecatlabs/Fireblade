#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageDirty.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageDirty, StateMessage );

    StateMessageDirty::StateMessageDirty() = default;
    StateMessageDirty::~StateMessageDirty() = default;

    auto StateMessageDirty::isDirty() const -> bool
    {
        return m_isDirty;
    }

    void StateMessageDirty::setDirty( bool dirty )
    {
        m_isDirty = dirty;
    }

}  // end namespace fb
