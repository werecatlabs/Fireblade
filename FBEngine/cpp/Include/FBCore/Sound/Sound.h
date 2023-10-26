#ifndef CSound_h__
#define CSound_h__

#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{

	class Sound : public Resource<ISound>
	{
	public:
		Sound();
		~Sound();

        void play() override;

        void stop() override;

        bool isPlaying() const override;

        void setVolume( f32 volume ) override;

        f32 getVolume() const override;

        void setLoop( bool loop ) override;

        bool getLoop() const override;

        String getName() const override;

        void getSpectrum( Array<f32> &spectrum, u32 numValues ) const override;

        void setPan( f32 pan ) override;

        void setPosition( const Vector3F &position ) override;

        Vector3F getPosition() const override;

        void setMinMaxDistance( f32 minDistance, f32 maxDistance ) override;

        void getMinMaxDistance( f32 &minDistance, f32 &maxDistance ) override;
    };

}  // namespace fb

#endif  // CSound_h__
