#ifndef __AsyncOperation_h__
#define __AsyncOperation_h__

#include <FBCore/Interface/System/IAsyncOperation.h>
#include <FBCore/Memory/ObjectYield.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{
    class AsyncOperation : public IAsyncOperation
    {
    public:
        AsyncOperation() = default;
        ~AsyncOperation() override = default;

        void removeCompleteEvent( std::function<void()> func );
        void addCompleteEvent( std::function<void()> func );

        FB_CLASS_REGISTER_DECL;

    protected:
        std::function<void()> m_completeEvent;
    };
}  // end namespace fb

#endif  // __AsyncOperation_h__
