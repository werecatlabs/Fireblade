#ifndef __AsyncOperation_h__
#define __AsyncOperation_h__

#include <FBCore/Interface/System/IAsyncOperation.h>

namespace fb
{
    class AsyncOperation : public IAsyncOperation
    {
    public:
        AsyncOperation();
        ~AsyncOperation() override;

        void removeCompleteEvent( std::function<void()> func ) override;
        void addCompleteEvent( std::function<void()> func ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        Array<std::function<void()>> m_completeEvents;
    };
} // end namespace fb

#endif  // __AsyncOperation_h__
