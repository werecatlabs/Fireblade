#ifndef FBInputActionData_h__
#define FBInputActionData_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IInputAction.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** Stores data for an action. */
    class InputActionData : public IInputAction
    {
    public:
        InputActionData();
        InputActionData( u32 first, u32 second, u32 actionId );

        hash_type getPrimaryAction() const override;
        void setPrimaryAction( hash_type primary ) override;

        hash_type getSecondaryAction() const override;
        void setSecondaryAction( hash_type secondary ) override;

        hash_type getActionId() const override;
        void setActionId( hash_type actionId ) override;

    protected:
        u32 m_primary = 0;
        u32 m_secondary = 0;
        u32 m_actionId = 0;
    };
}  // end namespace fb

#endif  // FBInputActionData_h__
