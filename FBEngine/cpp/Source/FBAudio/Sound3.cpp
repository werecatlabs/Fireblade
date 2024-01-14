#include <FBAudio/Sound3.h>
#include <FBCore/FBCore.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, Sound3, ISound3 );

    Sound3::Sound3()
    {
    }

    Sound3::~Sound3()
    {
    }

    void Sound3::Initialize( const Vector3F &position, const String &filePath, bool bLoop,
                             bool bDeleteWhenFinished )
    {
    }

    void Sound3::play()
    {
    }

    bool Sound3::isPlaying() const
    {
        bool bIsPlaying = false;

        return bIsPlaying;
    }

    //!stops the sound from playing
    void Sound3::stop()
    {
    }

    void Sound3::setPosition( const Vector3F &position )
    {
        m_position = position;
    }

    Vector3F Sound3::getPosition() const
    {
        return m_position;
    }

    void Sound3::setVolume( f32 fVolume )
    {
    }

    f32 Sound3::getVolume() const
    {
        f32 vol;

        return vol;
    }

    void Sound3::setLoop( bool lool )
    {
    }

    bool Sound3::getLoop() const
    {
        return false;
    }

    void Sound3::setMinMaxDistance( f32 minDistance, f32 maxDistance )
    {
    }

    void Sound3::getMinMaxDistance( f32 &minDistance, f32 &maxDistance )
    {
    }

    bool Sound3::GetDeleteWhenFinished() const
    {
        return m_bDeleteWhenFinished;
    }

    void Sound3::getSpectrum( Array<f32> &spectrum, u32 numValues ) const
    {
    }

    void Sound3::setPan( f32 pan )
    {
    }

    String Sound3::getName() const
    {
        return String();
    }

}  // end namespace fb
