#ifndef FBInputActionData_h__
#define FBInputActionData_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IInputActionData.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    /** Stores data for an action. */
    class InputActionData : public SharedObject<IInputActionData>
    {
    public:
        InputActionData();
        InputActionData( u32 first, u32 second, u32 actionId );

        u32 getPrimaryAction() const override;
        void setPrimaryAction( u32 primary ) override;

        u32 getSecondaryAction() const override;
        void setSecondaryAction( u32 secondary ) override;

        u32 getActionId() const override;
        void setActionId( u32 actionId ) override;

    protected:
        u32 m_primary = 0;
        u32 m_secondary = 0;
        u32 m_actionId = 0;
    };
}  // end namespace fb

#endif  // FBInputActionData_h__
