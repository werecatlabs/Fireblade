#ifndef StateFrameData_h__
#define StateFrameData_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{

    class StateFrameData : public StateMessage
    {
    public:
        StateFrameData();
        StateFrameData( s32 videoBufferSize, s32 soundBufferSize );
        ~StateFrameData() override;

        u8 *getVideoBuffer() const;
        void setVideoBuffer( u8 *val );

        s32 getVideoBufferSize() const;
        void setVideoBufferSize( s32 val );

        u8 *getSoundBuffer() const;
        void setSoundBuffer( u8 *val );

        s32 getSoundBufferSize() const;
        void setSoundBufferSize( s32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        u8 *m_videoBuffer = nullptr;
        u8 *m_soundBuffer = nullptr;
        s32 m_videoBufferSize = 0;
        s32 m_soundBufferSize = 0;
    };

}  // end namespace fb

#endif  // StateFrameData_h__
