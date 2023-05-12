#ifndef __IJobListener_h__
#define __IJobListener_h__

#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{

    /** A class to listen for job events. */
    class IJobListener : public IEventListener
    {
    public:
        /** Virtual destructor. */
        ~IJobListener() override = default;

        virtual SmartPtr<IJob> getJob() const = 0;
        virtual void setJob( SmartPtr<IJob> val ) = 0;

        virtual void queued() = 0;
        virtual void completed() = 0;
    };

}  // end namespace fb

#endif  // __IJobListener_h__
