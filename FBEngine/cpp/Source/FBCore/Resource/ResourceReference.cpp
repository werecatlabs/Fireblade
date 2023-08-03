#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/ResourceReference.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, ResourceReference, IResourceReference );

    ResourceReference::ResourceReference()
    {
    }

    ResourceReference::~ResourceReference()
    {
    }

    void ResourceReference::setOwnerUUID( const String &ownerUUID )
    {
        m_ownerUUID = ownerUUID;
    }

    fb::String ResourceReference::getResourceUUID() const
    {
        return m_resourceUUID;
    }

    void ResourceReference::setResourceUUID( const String &resourceUUID )
    {
        m_resourceUUID = resourceUUID;
    }

    fb::String ResourceReference::getOwnerUUID() const
    {
        return m_ownerUUID;
    }

}  // namespace fb
