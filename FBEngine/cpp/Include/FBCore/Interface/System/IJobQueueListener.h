#ifndef IJobQueueListener_h__
#define IJobQueueListener_h__

#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{

    /** A class to listen for job events. */
    class IJobQueueListener : public IEventListener
    {
    public:
        /** Virtual destructor. */
        ~IJobQueueListener() override = default;

        virtual void jobQueued( SmartPtr<IJob> job ) = 0;
        virtual void jobExecute( SmartPtr<IJob> job ) = 0;
        virtual void jobFinished( SmartPtr<IJob> job ) = 0;
    };

}  // end namespace fb

#endif  // IJobQueueListener_h__
