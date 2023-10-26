#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Job.h>
#include <FBCore/System/JobYield.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Job, IJob );

    IJob::State Job::getState() const
    {
        return m_state;
    }

    void Job::setState( State state )
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
        return state == State::Finish || state == State::Ready;
    }

    bool Job::wait()
    {
        while( getState() == IJob::State::Queue )
        {
            Thread::yield();
        }

        while( !isFinished() )
        {
            Thread::yield();
        }

        return true;
    }

    bool Job::wait( f64 maxWaitTime )
    {
        while( getState() == IJob::State::Queue )
        {
            Thread::yield();
        }

        while( !isFinished() )
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
        if( !m_yieldObject )
        {
            m_yieldObject = SmartPtr<core::JobYield>( new core::JobYield( this ) );
        }

        coroutine_execute_step( m_yieldObject );
    }

    void Job::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
    {
        setState( State::Finish );
    }

    bool Job::isCoroutine() const
    {
        return m_isCoroutine;
    }

    void Job::setCoroutine( bool coroutine )
    {
        m_isCoroutine = coroutine;
    }
}  // end namespace fb
