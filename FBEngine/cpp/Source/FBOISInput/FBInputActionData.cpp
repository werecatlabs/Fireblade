#include <FBOISInput/FBInputActionData.h>
#include <FBCore/FBCore.h>

namespace fb
{
    InputActionData::InputActionData()
    {
    }

    InputActionData::InputActionData( u32 first, u32 second, u32 actionId ) :
        m_primary( first ),
        m_secondary( second ),
        m_actionId( actionId )
    {
    }

    u32 InputActionData::getPrimaryAction() const
    {
        return m_primary;
    }

    void InputActionData::setPrimaryAction( u32 primary )
    {
        m_primary = primary;
    }

    u32 InputActionData::getSecondaryAction() const
    {
        return m_secondary;
    }

    void InputActionData::setSecondaryAction( u32 secondary )
    {
        m_secondary = secondary;
    }

    u32 InputActionData::getActionId() const
    {
        return m_actionId;
    }

    void InputActionData::setActionId( u32 actionId )
    {
        m_actionId = actionId;
    }
}  // end namespace fb
