#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/ResourceReference.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, ResourceReference, IResourceReference );

    ResourceReference::ResourceReference() = default;

    ResourceReference::~ResourceReference() = default;

    void ResourceReference::setOwnerUUID( const String &ownerUUID )
    {
        m_ownerUUID = ownerUUID;
    }

    auto ResourceReference::getResourceUUID() const -> fb::String
    {
        return m_resourceUUID;
    }

    void ResourceReference::setResourceUUID( const String &resourceUUID )
    {
        m_resourceUUID = resourceUUID;
    }

    auto ResourceReference::getOwnerUUID() const -> fb::String
    {
        return m_ownerUUID;
    }

}  // namespace fb
