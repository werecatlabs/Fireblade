#ifndef _ISystemAddress_H
#define _ISystemAddress_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    class ISystemAddress : public ISharedObject
    {
    public:
        ~ISystemAddress() override = default;

        virtual void setBinaryAddress( u32 binaryAddress ) = 0;
        virtual u32 getBinaryAddress() const = 0;

        virtual void setPort( u16 port ) = 0;
        virtual u16 getPort() const = 0;
    };

}  // end namespace fb

#endif
