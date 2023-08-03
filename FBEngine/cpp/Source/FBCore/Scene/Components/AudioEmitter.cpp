#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/AudioEmitter.h>
#include <FBCore/Interface/Sound/ISound.h>

namespace fb
{
    namespace scene
    {

        FB_CLASS_REGISTER_DERIVED( fb::scene, AudioEmitter, Component );

        AudioEmitter::AudioEmitter()
        {
        }

        AudioEmitter::~AudioEmitter()
        {
        }

        void AudioEmitter::load( SmartPtr<ISharedObject> data )
        {
        }

        void AudioEmitter::unload( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<Properties> AudioEmitter::getProperties() const
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

        Array<SmartPtr<ISharedObject>> AudioEmitter::getChildObjects() const
        {
            auto children = Array<SmartPtr<ISharedObject>>();

            auto sound = getSound();
            children.push_back( sound );

            return children;
        }

        SmartPtr<ISound> AudioEmitter::getSound() const
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
    }  // namespace scene
}  // namespace fb
