#ifndef IResourceReference_h__
#define IResourceReference_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IResourceReference : public ISharedObject
    {
    public:
        ~IResourceReference() override = 0;

        virtual String getOwnerUUID() const = 0;

        virtual void setOwnerUUID( const String &ownerUUID ) = 0;

        virtual String getResourceUUID() const = 0;

        virtual void setResourceUUID( const String &resourceUUID ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

} // namespace fb

#endif  // IResourceReference_h__
