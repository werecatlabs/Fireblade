#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobListener.h>

namespace fb
{
    JobListener::JobListener()
    {
    }

    JobListener::JobListener( SmartPtr<IJob> job ) :
        m_job( job )
    {
    }

    JobListener::~JobListener()
    {
    }

    fb::Parameter JobListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                        const Array<Parameter> &arguments,
                                        SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
    {
        return Parameter();
    }

    void JobListener::setJob( SmartPtr<IJob> job )
    {
        m_job = job;
    }

    SmartPtr<IJob> JobListener::getJob() const
    {
        return m_job;
    }

    void JobListener::queued()
    {
    }

    void JobListener::completed()
    {
    }
} // end namespace fb
