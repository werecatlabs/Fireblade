#include <FBCore/FBCorePCH.h>
#include <FBCore/System/AsyncOperation.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, AsyncOperation, IAsyncOperation );

    void AsyncOperation::removeCompleteEvent( std::function<void()> func )
    {
    }

    void AsyncOperation::addCompleteEvent( std::function<void()> func )
    {
    }
}  // end namespace fb
