#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/AudioEmitter.h>
#include <FBCore/Interface/Sound/ISound.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, AudioEmitter, Component );

    AudioEmitter::AudioEmitter() = default;

    AudioEmitter::~AudioEmitter() = default;

    void AudioEmitter::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void AudioEmitter::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto AudioEmitter::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();

        auto sound = getSound();
        properties->setProperty( "Sound", sound );

        return properties;
    }

    void AudioEmitter::setProperties( SmartPtr<Properties> properties )
    {
        auto sound = SmartPtr<ISound>();
        properties->getPropertyValue( "Sound", sound );
        setSound( sound );
    }

    auto AudioEmitter::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto children = Array<SmartPtr<ISharedObject>>();

        auto sound = getSound();
        children.emplace_back( sound );

        return children;
    }

    auto AudioEmitter::getSound() const -> SmartPtr<ISound>
    {
        return m_sound;
    }

    void AudioEmitter::setSound( SmartPtr<ISound> sound )
    {
        m_sound = sound;
    }

    void AudioEmitter::play()
    {
        if( auto sound = getSound() )
        {
            sound->play();
        }
    }

    void AudioEmitter::stop()
    {
    }

    void AudioEmitter::pause()
    {
    }

    void AudioEmitter::unpause()
    {
    }
}  // namespace fb::scene
