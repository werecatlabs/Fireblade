#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/CSound.h>

namespace fb
{
    CSound::CSound()
    {
    }

    CSound::~CSound()
    {
    }

    void CSound::play()
    {
    }

    void CSound::stop()
    {
    }

    bool CSound::isPlaying() const
    {
        return false;
    }

    void CSound::setVolume( f32 volume )
    {
    }

    f32 CSound::getVolume() const
    {
        return 0.0f;
    }

    void CSound::setLoop( bool loop )
    {
    }

    bool CSound::getLoop() const
    {
        return false;
    }

    String CSound::getName() const
    {
        return String();
    }

    void CSound::getSpectrum( Array<f32> &spectrum, u32 numValues ) const
    {
    }

    void CSound::setPan( f32 pan )
    {
    }

    void CSound::setPosition( const Vector3F &position )
    {
    }

    Vector3F CSound::getPosition() const
    {
        return Vector3F::zero();
    }

    void CSound::setMinMaxDistance( f32 minDistance, f32 maxDistance )
    {
    }

    void CSound::getMinMaxDistance( f32 &minDistance, f32 &maxDistance )
    {
    }
} // namespace fb
