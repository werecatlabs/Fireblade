#ifndef IObjectYield_h__
#define IObjectYield_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IObjectYield : public ISharedObject
    {
    public:
        /** Virtual destructor.
         */
        ~IObjectYield() override = default;

        virtual s32 getLineNumber() const = 0;
        virtual void setLineNumber( s32 lineNumber ) = 0;

        virtual void yield() = 0;
        virtual void stop() = 0;
    };
}  // end namespace fb

#endif  // IObjectYield_h__
