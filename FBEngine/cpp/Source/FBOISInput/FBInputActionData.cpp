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

    hash_type InputActionData::getPrimaryAction() const
    {
        return m_primary;
    }

    void InputActionData::setPrimaryAction( hash_type primary )
    {
        m_primary = primary;
    }

    hash_type InputActionData::getSecondaryAction() const
    {
        return m_secondary;
    }

    void InputActionData::setSecondaryAction( hash_type secondary )
    {
        m_secondary = secondary;
    }

    hash_type InputActionData::getActionId() const
    {
        return m_actionId;
    }

    void InputActionData::setActionId( hash_type actionId )
    {
        m_actionId = actionId;
    }
} // end namespace fb
