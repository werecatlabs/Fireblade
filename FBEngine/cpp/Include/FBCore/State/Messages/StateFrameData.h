#ifndef StateFrameData_h__
#define StateFrameData_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    
    class StateFrameData : public StateMessage
    {
    public:
        StateFrameData();
        StateFrameData( int videoBufferSize, int soundBufferSize );
        ~StateFrameData() override;

        unsigned char *getVideoBuffer() const;
        void setVideoBuffer( unsigned char *val );

        int getVideoBufferSize() const;
        void setVideoBufferSize( int val );

        unsigned char *getSoundBuffer() const;
        void setSoundBuffer( unsigned char *val );

        int getSoundBufferSize() const;
        void setSoundBufferSize( int val );

        FB_CLASS_REGISTER_DECL;

    protected:
        unsigned char *m_videoBuffer;
        unsigned char *m_soundBuffer;
        int m_videoBufferSize;
        int m_soundBufferSize;
    };

    using StateFrameDataPtr = SmartPtr<StateFrameData>;
}  // end namespace fb

#endif  // StateFrameData_h__
