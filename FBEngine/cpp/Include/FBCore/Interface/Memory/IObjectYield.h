#ifndef IObjectYield_h__
#define IObjectYield_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Yield object interface. */
    class IObjectYield : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IObjectYield() override = default;

        /** Gets the line number. */
        virtual s32 getLineNumber() const = 0;

        /** Sets the line number. */
        virtual void setLineNumber( s32 lineNumber ) = 0;

        /** Yields the object. */
        virtual void yield() = 0;

        /** Stops the object. */
        virtual void stop() = 0;
    };
}  // end namespace fb

#endif  // IObjectYield_h__
