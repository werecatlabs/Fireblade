#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/MaterialResourceDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, MaterialResourceDirector, Director );

    MaterialResourceDirector::MaterialResourceDirector() = default;

    MaterialResourceDirector::~MaterialResourceDirector() = default;

    auto MaterialResourceDirector::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();

        auto textureTypes = Array<String>();
        textureTypes.reserve( 24 );

        textureTypes.emplace_back( "Texture" );
        textureTypes.emplace_back( "Normal Map" );
        textureTypes.emplace_back( "UI" );

        auto textureSizes = Array<String>();
        textureSizes.reserve( 12 );

        textureSizes.emplace_back( "32" );
        textureSizes.emplace_back( "64" );
        textureSizes.emplace_back( "128" );
        textureSizes.emplace_back( "256" );
        textureSizes.emplace_back( "512" );
        textureSizes.emplace_back( "1024" );
        textureSizes.emplace_back( "2048" );
        textureSizes.emplace_back( "4096" );
        textureSizes.emplace_back( "8192" );

        properties->setPropertyAsEnum( "type", m_textureType, textureTypes );
        properties->setPropertyAsEnum( "size", m_textureSize, textureSizes );

        properties->setProperty( "resourcePath", getResourcePath() );

        properties->setPropertyAsButton( "Save", "SaveButton" );
        properties->setPropertyAsButton( "Import", "ImportButton" );

        return properties;
    }

    void MaterialResourceDirector::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "type", m_textureType );
        properties->getPropertyValue( "size", m_textureSize );

        properties->getPropertyValue( "resourcePath", m_resourcePath );

        if( properties->isButtonPressed( "Save" ) )
        {
            save();
        }

        if( properties->isButtonPressed( "Import" ) )
        {
            import();
        }
    }
}  // namespace fb::scene
