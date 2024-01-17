#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/ResourceDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, ResourceDirector, Director );

    ResourceDirector::ResourceDirector() = default;

    ResourceDirector::~ResourceDirector() = default;

    void ResourceDirector::import()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto resourceDatabase = applicationManager->getResourceDatabase();
        resourceDatabase->importFile( getResourcePath() );
    }

    String ResourceDirector::getResourcePath() const
    {
        return m_resourcePath;
    }

    void ResourceDirector::setResourcePath( const String &resourcePath )
    {
        m_resourcePath = resourcePath;
    }

    void ResourceDirector::setResourceUUID( const String &resourceUUID )
    {
        m_resourceUUID = resourceUUID;
    }

    String ResourceDirector::getResourceUUID() const
    {
        return m_resourceUUID;
    }

    void ResourceDirector::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "resourcePath", m_resourcePath );
        properties->getPropertyValue( "resourceUUID", m_resourceUUID );
    }

    SmartPtr<Properties> ResourceDirector::getProperties() const
    {
        auto properties = fb::make_ptr<Properties>();
        properties->setProperty( "resourcePath", getResourcePath() );
        properties->setProperty( "resourceUUID", getResourceUUID() );
        return properties;
    }
}  // namespace fb::scene
