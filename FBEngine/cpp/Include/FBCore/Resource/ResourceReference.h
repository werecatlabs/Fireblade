#ifndef ResourceReference_h__
#define ResourceReference_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceReference.h>

namespace fb
{

    class ResourceReference : public IResourceReference
    {
    public:
        ResourceReference();
        ~ResourceReference();

        String getOwnerUUID() const;

        void setOwnerUUID( const String &ownerUUID );

        String getResourceUUID() const;

        void setResourceUUID( const String &resourceUUID );

        FB_CLASS_REGISTER_DECL;

    private:
        String m_ownerUUID;
        String m_resourceUUID;
    };

}  // namespace fb

#endif  // ResourceReference_h__
