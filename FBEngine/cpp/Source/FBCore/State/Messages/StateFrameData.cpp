#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateFrameData.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <memory>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateFrameData, StateMessage );

    StateFrameData::StateFrameData() = default;

    StateFrameData::StateFrameData( s32 videoBufferSize, s32 soundBufferSize )
    {
        m_videoBuffer = static_cast<u8 *>( malloc( videoBufferSize ) );
        m_videoBufferSize = videoBufferSize;

        m_soundBuffer = new u8[soundBufferSize];
        m_soundBufferSize = soundBufferSize;
    }

    StateFrameData::~StateFrameData()
    {
        if( m_videoBuffer )
        {
            free( m_videoBuffer );

            m_videoBuffer = nullptr;
        }

        if( m_soundBuffer )
        {
            delete[] m_soundBuffer;
            m_soundBuffer = nullptr;
        }
    }

    auto StateFrameData::getVideoBuffer() const -> unsigned char *
    {
        return m_videoBuffer;
    }

    void StateFrameData::setVideoBuffer( unsigned char *val )
    {
        m_videoBuffer = val;
    }

    auto StateFrameData::getVideoBufferSize() const -> int
    {
        return m_videoBufferSize;
    }

    void StateFrameData::setVideoBufferSize( int val )
    {
        m_videoBufferSize = val;

        if( m_videoBuffer )
        {
            free( m_videoBuffer );

            m_videoBuffer = nullptr;
        }

        m_videoBuffer = static_cast<u8 *>( malloc( m_videoBufferSize ) );

        if( !m_videoBuffer )
        {
            FB_EXCEPTION( "Error: could not allocate frame." );
        }
    }

    auto StateFrameData::getSoundBuffer() const -> unsigned char *
    {
        return m_soundBuffer;
    }

    void StateFrameData::setSoundBuffer( unsigned char *val )
    {
        m_soundBuffer = val;
    }

    auto StateFrameData::getSoundBufferSize() const -> int
    {
        return m_soundBufferSize;
    }

    void StateFrameData::setSoundBufferSize( int val )
    {
        m_soundBufferSize = val;

        if( m_soundBuffer )
        {
            delete[] m_soundBuffer;
            m_soundBuffer = nullptr;
        }

        m_soundBuffer = new unsigned char[m_soundBufferSize];
    }
}  // end namespace fb
