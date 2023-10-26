#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/Sound.h>

namespace fb
{
    Sound::Sound()
    {
    }

    Sound::~Sound()
    {
    }

    void Sound::play()
    {
    }

    void Sound::stop()
    {
    }

    bool Sound::isPlaying() const
    {
        return false;
    }

    void Sound::setVolume( f32 volume )
    {
    }

    f32 Sound::getVolume() const
    {
        return 0.0f;
    }

    void Sound::setLoop( bool loop )
    {
    }

    bool Sound::getLoop() const
    {
        return false;
    }

    String Sound::getName() const
    {
        return String();
    }

    void Sound::getSpectrum( Array<f32> &spectrum, u32 numValues ) const
    {
    }

    void Sound::setPan( f32 pan )
    {
    }

    void Sound::setPosition( const Vector3F &position )
    {
    }

    Vector3F Sound::getPosition() const
    {
        return Vector3F::zero();
    }

    void Sound::setMinMaxDistance( f32 minDistance, f32 maxDistance )
    {
    }

    void Sound::getMinMaxDistance( f32 &minDistance, f32 &maxDistance )
    {
    }
}  // namespace fb
