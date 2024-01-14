#ifndef _Sound3_H
#define _Sound3_H

#include <FBCore/Interface/Sound/ISound3.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{

    class Sound3 : public Resource<ISound3>
    {
    public:
        Sound3();
        ~Sound3();

        void Initialize( const Vector3F &position, const String &filePath, bool bLoop,
                         bool bDeleteWhenFinished );
        void play();
        void stop();
        bool isPlaying() const;
        void setPosition( const Vector3F &position );
        Vector3F getPosition() const;

        void setVolume( f32 fVolume );
        f32 getVolume() const;

        void setLoop( bool lool );
        bool getLoop() const;

        void setMinMaxDistance( f32 minDistance, f32 maxDistance );
        void getMinMaxDistance( f32 &minDistance, f32 &maxDistance );

        String getName() const;

        //void getSpectrum(Array<f32>& spectrumData, u32 numvalues) const;

        bool GetDeleteWhenFinished() const;

        void getSpectrum( Array<f32> &spectrum, u32 numValues ) const override;

        void setPan( f32 pan ) override;

        FB_CLASS_REGISTER_DECL;

    private:
        Vector3F m_position;

        bool m_vLoop;
        bool m_bDeleteWhenFinished;
    };

}  // end namespace fb

#endif
