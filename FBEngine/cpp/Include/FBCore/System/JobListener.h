#ifndef __CJobListener_h__
#define __CJobListener_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/System/IJobListener.h>
#include <FBCore/Interface/System/IJob.h>

namespace fb
{
    
    class JobListener : public CSharedObject<IJobListener>
    {
    public:
        JobListener();
        JobListener( SmartPtr<IJob> job );
        ~JobListener() override;

        Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                               const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object,
                               SmartPtr<IEvent> event );

        SmartPtr<IJob> getJob() const override;
        void setJob( SmartPtr<IJob> job ) override;

        void queued() override;
        void completed() override;

    protected:
        SmartPtr<IJob> m_job;
    };
}  // end namespace fb

#endif  // __CJobListener_h__
