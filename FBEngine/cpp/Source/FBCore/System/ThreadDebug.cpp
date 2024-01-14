#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ThreadDebug.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Thread/Threading.h>
#include <iostream>
#include <sstream>

namespace fb
{
    ThreadDebug::~ThreadDebug() = default;

    ThreadDebug::ThreadDebug() = default;

    void ThreadDebug::setWriteThreadId( u32 val )
    {
        m_writeThreadId = val;
    }

    auto ThreadDebug::getWriteThreadId() const -> u32
    {
        return m_writeThreadId;
    }

    ThreadDebug::Call::~Call()
    {
        if( m_bWrite )
        {
            m_debug->setWriteThreadId( 0 );
        }
    }

    ThreadDebug::Call::Call( ThreadDebug *debug, bool bWrite, const char *file /*= __FILE__*/,
                             const char *function /*= 0*/, const unsigned int line /*= __LINE__*/ ) :
        m_debug( debug ),

        m_bWrite( bWrite ),
        m_file( nullptr ),
        m_function( nullptr )

    {
        if( m_bWrite )
        {
            auto writeThreadId = static_cast<u32>( Thread::getCurrentThreadId() );

            if( debug->getWriteThreadId() != 0 && writeThreadId != debug->getWriteThreadId() )
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
            m_readThreadId = static_cast<u32>( Thread::getCurrentThreadId() );
        }

        if( debug->getWriteThreadId() != 0 )
        {
            if( debug->getWriteThreadId() != m_readThreadId )
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

    auto ThreadDebug::Call::getWrite() const -> bool
    {
        return m_bWrite;
    }

    void ThreadDebug::Call::setReadThreadId( u32 val )
    {
        m_readThreadId = val;
    }

    auto ThreadDebug::Call::getReadThreadId() const -> u32
    {
        return m_readThreadId;
    }
}  // end namespace fb
