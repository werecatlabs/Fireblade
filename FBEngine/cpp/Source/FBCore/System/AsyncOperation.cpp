#include <FBCore/FBCorePCH.h>
#include <FBCore/System/AsyncOperation.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, AsyncOperation, IAsyncOperation );

    AsyncOperation::AsyncOperation() = default;
    AsyncOperation::~AsyncOperation() = default;

    void AsyncOperation::removeCompleteEvent( std::function<void()> func )
    {
        //m_completeEvents.erase( std::remove( m_completeEvents.begin(), m_completeEvents.end(), func ),
        //                        m_completeEvents.end() );
    }

    void AsyncOperation::addCompleteEvent( std::function<void()> func )
    {
        m_completeEvents.push_back( func );
    }
}  // end namespace fb
