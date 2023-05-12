#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ThreadDebug.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Thread/Threading.h>
#include <iostream>
#include <sstream>

namespace fb
{
    ThreadDebug::~ThreadDebug()
    {
    }

    ThreadDebug::ThreadDebug() :
        m_writeThreadId( 0 )
    {
    }

    void ThreadDebug::setWriteThreadId( u32 val )
    {
        m_writeThreadId = val;
    }

    u32 ThreadDebug::getWriteThreadId() const
    {
        return m_writeThreadId;
    }

    ThreadDebug::Call::~Call()
    {
        if(m_bWrite)
            m_debug->setWriteThreadId( 0 );
    }

    ThreadDebug::Call::Call( ThreadDebug *debug, bool bWrite, const char *file /*= __FILE__*/,
                             const char *function /*= 0*/, const unsigned int line /*= __LINE__*/ ) :
        m_debug( debug ),
        m_readThreadId( 0 ),
        m_bWrite( bWrite ),
        m_file( nullptr ),
        m_function( nullptr ),
        m_line( 0 )
    {
        if(m_bWrite)
        {
            auto writeThreadId = static_cast<u32>(Thread::getCurrentThreadId());

            if(debug->getWriteThreadId() != 0 && writeThreadId != debug->getWriteThreadId())
            {
                std::stringstream strStream;
                strStream << "Possible data race: " << file << " " << function << " " << line
                    << std::endl;
                std::cout << strStream.str().c_str() << std::endl;

                FB_LOG_MESSAGE( "Thread", strStream.str().c_str() );
            }

            debug->setWriteThreadId( writeThreadId );
        }
        else
        {
            m_readThreadId = static_cast<u32>(Thread::getCurrentThreadId());
        }

        if(debug->getWriteThreadId() != 0)
        {
            if(debug->getWriteThreadId() != m_readThreadId)
            {
                std::stringstream strStream;
                strStream << "Possible data race: " << file << " " << function << " " << line
                    << std::endl;
                std::cout << strStream.str().c_str() << std::endl;

                FB_LOG_MESSAGE( "Thread", strStream.str().c_str() );
            }
        }
    }

    void ThreadDebug::Call::setWrite( bool val )
    {
        m_bWrite = val;
    }

    bool ThreadDebug::Call::getWrite() const
    {
        return m_bWrite;
    }

    void ThreadDebug::Call::setReadThreadId( u32 val )
    {
        m_readThreadId = val;
    }

    u32 ThreadDebug::Call::getReadThreadId() const
    {
        return m_readThreadId;
    }
} // end namespace fb
