#include <FBAudio/Sound2.h>
#include <FBCore/FBCore.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, Sound2, ISound2 );

    Sound2::Sound2()
    {
    }

    Sound2::~Sound2()
    {
        //if(m_sound)
        //	m_sound->release();
    }

    void Sound2::Initialize( const String &filePath, bool bLoop, bool bDeleteWhenFinished )
    {
        //FMOD_RESULT result;

        //FMOD_MODE mode = FMOD_2D | FMOD_SOFTWARE;
        //if(bLoop)
        //{
        //	mode |= FMOD_LOOP_NORMAL;
        //}

        //auto applicationManager = ApplicationManager::instance();
        //boost::shared_ptr<IFileSystem>& fileSystem = engine->getFileSystem();
        //boost::shared_ptr<IStream> data = fileSystem->open(filePath);
        //if(data)
        //{
        //	mode |= FMOD_OPENMEMORY;

        //	FMOD_CREATESOUNDEXINFO info;
        //	memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        //	info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        //	info.length = data->getSize();

        //	FMOD::Sound* sound = nullptr;

        //	FMOD::System* system = nullptr;
        //	m_system->getLowLevelSystem(&system);
        //	result = system->createSound((c8*)data->getData(), mode, &info, &sound);
        //	ERRCHECK(result);

        //	m_sound = sound;
        //}
        //else
        //{
        //	LOG_MESSAGE(String("Sound"), String("Warning: sound file not found: ") + filePath);
        //}

        //m_bDeleteWhenFinished = bDeleteWhenFinished;
    }

    void Sound2::play()
    {
        //if(!m_sound)
        //	return;

        //FMOD_RESULT       result;

        //FMOD::Channel* channel = nullptr;

        //FMOD::System* system = nullptr;
        //m_system->getLowLevelSystem(&system);
        //result = system->playSound(m_sound, 0, false, &channel);
        //ERRCHECK(result);

        //m_channel = channel;

        //if(m_pan != 0.0f)
        //{
        //	result = m_channel->setPan(m_pan);
        //	ERRCHECK(result);
        //}
    }

    bool Sound2::isPlaying() const
    {
        //if(!m_sound)
        //	return false;

        //bool bIsPlaying = false;
        //if(m_channel)
        //{
        //	FMOD_RESULT result;
        //	result = m_channel->isPlaying( &bIsPlaying );
        //	ERRCHECK(result);
        //}

        //return bIsPlaying;

        return false;
    }

    void Sound2::stop()
    {
        //if(!m_sound)
        //	return;

        //if(m_channel)
        //{
        //	FMOD_RESULT result = m_channel->stop();
        //	ERRCHECK(result);
        //}
    }

    void Sound2::setVolume( f32 fVolume )
    {
        //if(!m_sound)
        //	return;

        //FMOD_RESULT result;
        //FMOD::ChannelGroup*  channelgroup;

        ////m_system->getMasterChannelGroup(&channelgroup);
        ////result = channelgroup->setVolume(fVolume);
        //ERRCHECK(result);
    }

    f32 Sound2::getVolume() const
    {
        //if(!m_sound)
        //	return 0.0f;

        //f32 vol;
        //if(m_channel)
        //{
        //	FMOD_RESULT result;
        //	result = m_channel->getVolume(&vol);
        //	ERRCHECK(result);
        //}

        //return vol;

        return 0.f;
    }

    void Sound2::setLoop( bool lool )
    {
    }

    bool Sound2::getLoop() const
    {
        return false;
    }

    bool Sound2::GetDeleteWhenFinished() const
    {
        return m_bDeleteWhenFinished;
    }

    String Sound2::getName() const
    {
        //if(!m_sound)
        //	return StringUtil::EmptyString;

        //if( m_sound )
        //{
        //	char soundName[ 255 ];
        //	m_sound->getName( soundName, 255 );
        //	return String( soundName );
        //}
        //else
        //{
        //	return StringUtil::EmptyString;
        //}

        return StringUtil::EmptyString;
    }

    void Sound2::setPan( f32 pan )
    {
        m_pan = pan;
    }

    void Sound2::getSpectrum( Array<f32> &spectrum, u32 numValues ) const
    {
    }

    void Sound2::setPosition( const Vector3F &position )
    {
    }

    fb::Vector3F Sound2::getPosition() const
    {
        return Vector3F();
    }

    void Sound2::setMinMaxDistance( f32 minDistance, f32 maxDistance )
    {
    }

    void Sound2::getMinMaxDistance( f32 &minDistance, f32 &maxDistance )
    {
    }

    void Sound2::setResourceType( ResourceType resourceType )
    {
    }

}  // end namespace fb
