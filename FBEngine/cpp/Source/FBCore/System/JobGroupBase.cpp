#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobGroupBase.h>

namespace fb
{

    JobGroupBase::JobGroupBase()
    {
        m_isFinished = false;
        setState( State::Ready );
    }

    JobGroupBase::~JobGroupBase()
    {
    }

    void JobGroupBase::update()
    {
    }

    bool JobGroupBase::getInterupt() const
    {
        return false;
    }

    void JobGroupBase::setInterupt( [[maybe_unused]] bool interupt )
    {
    }

    IJob::State JobGroupBase::getState() const
    {
        return m_state;
    }

    void JobGroupBase::setState( IJob::State state )
    {
        m_state = state;
    }

    u32 JobGroupBase::getProgress() const
    {
        return m_progress;
    }

    void JobGroupBase::setProgress( u32 val )
    {
        m_progress = val;
    }

    s32 JobGroupBase::getPriority() const
    {
        return m_progress;
    }

    void JobGroupBase::setPriority( s32 val )
    {
        m_progress = val;
    }

    bool JobGroupBase::isPrimary() const
    {
        return m_isPrimary ? true : false;
    }

    void JobGroupBase::setPrimary( bool val )
    {
        m_isPrimary = val;
    }

    bool JobGroupBase::isFinished() const
    {
        return m_isFinished;
    }

    bool JobGroupBase::wait()
    {
        while( !isFinished() )
        {
            Thread::yield();
        }

        return false;
    }

    bool JobGroupBase::wait( [[maybe_unused]] f64 maxWaitTime )
    {
        return false;
    }

    s32 JobGroupBase::getAffinity() const
    {
        return 0;
    }

    void JobGroupBase::setAffinity( [[maybe_unused]] s32 val )
    {
    }

    void JobGroupBase::execute()
    {
    }

    void JobGroupBase::coroutine_execute()
    {
    }

    void JobGroupBase::coroutine_execute_step( [[maybe_unused]] SmartPtr<IObjectYield> &rYield )
    {
    }

    bool JobGroupBase::isCoroutine() const
    {
        return false;
    }

    void JobGroupBase::setCoroutine( [[maybe_unused]] bool val )
    {
    }
}  // end namespace fb
