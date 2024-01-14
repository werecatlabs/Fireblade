#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/Sound.h>

namespace fb
{
    Sound::Sound() = default;

    Sound::~Sound() = default;

    void Sound::play()
    {
    }

    void Sound::stop()
    {
    }

    auto Sound::isPlaying() const -> bool
    {
        return false;
    }

    void Sound::setVolume( f32 volume )
    {
    }

    auto Sound::getVolume() const -> f32
    {
        return 0.0f;
    }

    void Sound::setLoop( bool loop )
    {
    }

    auto Sound::getLoop() const -> bool
    {
        return false;
    }

    auto Sound::getName() const -> String
    {
        return {};
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

    auto Sound::getPosition() const -> Vector3F
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
