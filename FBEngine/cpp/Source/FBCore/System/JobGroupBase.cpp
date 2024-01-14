#include <FBCore/FBCorePCH.h>
#include <FBCore/System/JobGroupBase.h>

namespace fb
{

    JobGroupBase::JobGroupBase()
    {
        m_isFinished = false;
        setState( State::Ready );
    }

    JobGroupBase::~JobGroupBase() = default;

    void JobGroupBase::update()
    {
    }

    auto JobGroupBase::getInterupt() const -> bool
    {
        return false;
    }

    void JobGroupBase::setInterupt( [[maybe_unused]] bool interupt )
    {
    }

    auto JobGroupBase::getState() const -> IJob::State
    {
        return m_state;
    }

    void JobGroupBase::setState( IJob::State state )
    {
        m_state = state;
    }

    auto JobGroupBase::getProgress() const -> u32
    {
        return m_progress;
    }

    void JobGroupBase::setProgress( u32 val )
    {
        m_progress = val;
    }

    auto JobGroupBase::getPriority() const -> s32
    {
        return m_progress;
    }

    void JobGroupBase::setPriority( s32 val )
    {
        m_progress = val;
    }

    auto JobGroupBase::isPrimary() const -> bool
    {
        return m_isPrimary ? true : false;
    }

    void JobGroupBase::setPrimary( bool val )
    {
        m_isPrimary = val;
    }

    auto JobGroupBase::isFinished() const -> bool
    {
        return m_isFinished;
    }

    auto JobGroupBase::wait() -> bool
    {
        while( !isFinished() )
        {
            Thread::yield();
        }

        return false;
    }

    auto JobGroupBase::wait( [[maybe_unused]] f64 maxWaitTime ) -> bool
    {
        return false;
    }

    auto JobGroupBase::getAffinity() const -> s32
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

    auto JobGroupBase::isCoroutine() const -> bool
    {
        return false;
    }

    void JobGroupBase::setCoroutine( [[maybe_unused]] bool val )
    {
    }
}  // end namespace fb
