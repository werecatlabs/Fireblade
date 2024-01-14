#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageSetFlag.h"
#include "FBCore/Core/Properties.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{

    StateMessageSetFlag::StateMessageSetFlag() = default;

    StateMessageSetFlag::~StateMessageSetFlag() = default;

    u32 StateMessageSetFlag::getFlags() const
    {
        return m_flags;
    }

    void StateMessageSetFlag::setFlags( u32 flags )
    {
        m_flags = flags;
    }

}  // namespace fb
