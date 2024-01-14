#ifndef _Sound2_H
#define _Sound2_H

#include <FBCore/Interface/Sound/ISound2.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{

    class Sound2 : public Resource<ISound2>
    {
    public:
        Sound2();
        ~Sound2();

        void Initialize( const String &filePath, bool bLoop, bool bDeleteWhenFinished );
        void play();
        void stop();
        bool isPlaying() const;

        void setVolume( f32 fVolume );
        f32 getVolume() const;

        void setLoop( bool lool );
        bool getLoop() const;

        String getName() const;

        bool GetDeleteWhenFinished() const;

        void setPan( f32 pan );

        void getSpectrum( Array<f32> &spectrum, u32 numValues ) const override;

        void setPosition( const Vector3F &position ) override;

        Vector3F getPosition() const override;

        void setMinMaxDistance( f32 minDistance, f32 maxDistance ) override;

        void getMinMaxDistance( f32 &minDistance, f32 &maxDistance ) override;

        void setResourceType( ResourceType resourceType ) override;

        FB_CLASS_REGISTER_DECL;

    private:
        f32 m_pan;

        bool m_bDeleteWhenFinished;
    };

}  // end namespace fb

#endif
