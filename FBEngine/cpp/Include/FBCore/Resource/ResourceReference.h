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
        ~ResourceReference() override;

        String getOwnerUUID() const override;

        void setOwnerUUID( const String &ownerUUID ) override;

        String getResourceUUID() const override;

        void setResourceUUID( const String &resourceUUID ) override;

        FB_CLASS_REGISTER_DECL;

    private:
        String m_ownerUUID;
        String m_resourceUUID;
    };
} // namespace fb

#endif  // ResourceReference_h__
