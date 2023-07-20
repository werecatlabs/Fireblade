#ifndef IScriptData_h__
#define IScriptData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** An interface for an object to store data used by the script system.
     */
    class IScriptData : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IScriptData() override = default;

        /** Gets the script object that owns the data.
        @return The script object instance.
        */
        virtual SmartPtr<ISharedObject> getOwner() const = 0;

        /** Sets the script object that owns the data.
        @param object The script object instance.
        */
        virtual void setOwner( SmartPtr<ISharedObject> object ) = 0;

        /** Gets object data. */
        virtual void *getObjectData() const = 0;
    };
}  // end namespace fb

#endif  // IScriptData_h__
