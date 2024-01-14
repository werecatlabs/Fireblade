#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/SoundResourceDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, SoundResourceDirector, Director );

    SoundResourceDirector::SoundResourceDirector() = default;

    SoundResourceDirector::~SoundResourceDirector() = default;

    auto SoundResourceDirector::getProperties() const -> SmartPtr<Properties>
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

        //properties->setPropertyAsEnum( "type", m_textureType, textureTypes );
        //properties->setPropertyAsEnum( "size", m_textureSize, textureSizes );

        properties->setProperty( "resourcePath", getResourcePath() );

        properties->setPropertyAsButton( "Save", "SaveButton" );
        properties->setPropertyAsButton( "Import", "ImportButton" );

        properties->setPropertyAsButton( "Play", "PlayButton" );
        properties->setPropertyAsButton( "Stop", "StopButton" );

        return properties;
    }

    void SoundResourceDirector::setProperties( SmartPtr<Properties> properties )
    {
        //properties->getPropertyValue( "type", m_textureType );
        //properties->getPropertyValue( "size", m_textureSize );

        properties->getPropertyValue( "resourcePath", m_resourcePath );

        if( properties->isButtonPressed( "Save" ) )
        {
            save();
        }

        if( properties->isButtonPressed( "Import" ) )
        {
            import();
        }

        if( properties->isButtonPressed( "Play" ) )
        {
            play();
        }

        if( properties->isButtonPressed( "Stop" ) )
        {
            stop();
        }
    }

    void SoundResourceDirector::play()
    {
    }

    void SoundResourceDirector::stop()
    {
    }

}  // namespace fb::scene
