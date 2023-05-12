#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Job.h>
#include <FBCore/System/JobYield.h>
#include <FBCore/Interface/System/IJobListener.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Job, CSharedObject<IJob> );

    IJob::JobState Job::getState() const
    {
        return m_state;
    }

    void Job::setState( JobState state )
    {
        m_state = state;
    }

    u32 Job::getProgress() const
    {
        return m_progress;
    }

    void Job::setProgress( u32 progress )
    {
        m_progress = progress;
    }

    s32 Job::getPriority() const
    {
        return m_priority;
    }

    void Job::setPriority( s32 priority )
    {
        m_priority = priority;
    }

    bool Job::isPrimary() const
    {
        return m_isPrimary;
    }

    void Job::setPrimary( bool primary )
    {
        m_isPrimary = primary;
    }

    bool Job::isFinished() const
    {
        auto state = getState();
        return state == JobState::Finish || state == JobState::Ready;
    }

    void Job::setFinished( bool finished )
    {
        m_isFinished = finished;
    }

    bool Job::wait()
    {
        while(!isFinished())
        {
            Thread::yield();
        }

        return true;
    }

    bool Job::wait( f64 maxWaitTime )
    {
        while(!isFinished())
        {
            Thread::yield();
        }

        return true;
    }

    s32 Job::getAffinity() const
    {
        return m_affinity;
    }

    void Job::setAffinity( s32 affinity )
    {
        m_affinity = affinity;
    }

    void Job::execute()
    {
    }

    void Job::coroutine_execute()
    {
        if(!m_yieldObject)
        {
            m_yieldObject = SmartPtr<core::JobYield>( new core::JobYield( this ) );
        }

        coroutine_execute_step( m_yieldObject );
    }

    void Job::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
    {
        setState( JobState::Finish );
    }

    bool Job::hasListeners() const
    {
        return m_listeners.empty() == false;
    }

    Array<SmartPtr<IJobListener>> Job::getListeners() const
    {
        return Array<SmartPtr<IJobListener>>( m_listeners.begin(), m_listeners.end() );
    }

    void Job::setListeners( const Array<SmartPtr<IJobListener>> &listeners )
    {
        m_listeners = ConcurrentArray<SmartPtr<IJobListener>>( listeners.begin(), listeners.end() );
    }

    void Job::addListener( SmartPtr<IJobListener> listener )
    {
        m_listeners.push_back( listener );
    }

    bool Job::isCoroutine() const
    {
        return m_isCoroutine;
    }

    void Job::setCoroutine( bool coroutine )
    {
        m_isCoroutine = coroutine;
    }
} // end namespace fb
