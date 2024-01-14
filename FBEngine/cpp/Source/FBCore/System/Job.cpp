#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Job.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/System/JobYield.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Job, IJob );

    Job::Job() = default;

    Job::~Job()  = default;

    auto Job::getState() const -> IJob::State
    {
        return m_state;
    }

    void Job::setState( State state )
    {
        m_state = state;
    }

    auto Job::getProgress() const -> u32
    {
        return m_progress;
    }

    void Job::setProgress( u32 progress )
    {
        m_progress = progress;
    }

    auto Job::getPriority() const -> s32
    {
        return m_priority;
    }

    void Job::setPriority( s32 priority )
    {
        m_priority = priority;
    }

    auto Job::isPrimary() const -> bool
    {
        return m_isPrimary;
    }

    void Job::setPrimary( bool primary )
    {
        m_isPrimary = primary;
    }

    auto Job::isFinished() const -> bool
    {
        auto state = getState();
        return state == State::Finish || state == State::Ready;
    }

    auto Job::wait() -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto threadPool = applicationManager->getThreadPool();
        if( threadPool->getNumThreads() > 0 )
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

        return true;
    }

    auto Job::wait( f64 maxWaitTime ) -> bool
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

    auto Job::getAffinity() const -> s32
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

    auto Job::isCoroutine() const -> bool
    {
        return m_isCoroutine;
    }

    void Job::setCoroutine( bool coroutine )
    {
        m_isCoroutine = coroutine;
    }
}  // end namespace fb
