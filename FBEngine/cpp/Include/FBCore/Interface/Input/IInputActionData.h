#ifndef IInputActionData_h__
#define IInputActionData_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * Interface for data associated with an input action, which is composed of a primary and a secondary action.
     */
    class IInputActionData : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IInputActionData() override = default;

        /** Returns the primary action associated with the input action. */
        virtual u32 getPrimaryAction() const = 0;

        /** Sets the primary action associated with the input action. */
        virtual void setPrimaryAction( u32 primary ) = 0;

        /** Returns the secondary action associated with the input action. */
        virtual u32 getSecondaryAction() const = 0;

        /** Sets the secondary action associated with the input action. */
        virtual void setSecondaryAction( u32 secondary ) = 0;

        /** Returns the ID of the input action. */
        virtual u32 getActionId() const = 0;

        /** Sets the ID of the input action. */
        virtual void setActionId( u32 actionId ) = 0;
    };

}  // end namespace fb

#endif  // IInputActionData_h__
