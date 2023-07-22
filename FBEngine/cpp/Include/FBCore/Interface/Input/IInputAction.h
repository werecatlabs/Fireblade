#ifndef IInputActionData_h__
#define IInputActionData_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * Interface for data associated with an input action, which is composed of a primary and a secondary action.
     */
    class IInputAction : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IInputAction() override = default;

        /** Returns the primary action associated with the input action. */
        virtual hash_type getPrimaryAction() const = 0;

        /** Sets the primary action associated with the input action. */
        virtual void setPrimaryAction( hash_type primary ) = 0;

        /** Returns the secondary action associated with the input action. */
        virtual hash_type getSecondaryAction() const = 0;

        /** Sets the secondary action associated with the input action. */
        virtual void setSecondaryAction( hash_type secondary ) = 0;

        /** Returns the ID of the input action. */
        virtual hash_type getActionId() const = 0;

        /** Sets the ID of the input action. */
        virtual void setActionId( hash_type actionId ) = 0;
    };

}  // end namespace fb

#endif  // IInputActionData_h__
