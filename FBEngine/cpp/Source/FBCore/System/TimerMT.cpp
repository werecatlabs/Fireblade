#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TimerMT.h>
#include <FBCore/Math/MathUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TimerMT, Timer );

    TimerMT::TimerMT() : m_bStarted( false )
    {
        auto size = static_cast<s32>( Thread::Task::Count );
        m_accumulated.resize( size );
        m_startOffset.resize( size );
        m_fixedOffset.resize( size );

        m_frameSmoothingTime.resize( size );

        m_fixedTimePoints.resize( size );
        m_fixedTime.resize( size );
        m_fixedTimeInterval.resize( size );

        m_prevTime.resize( size );
        m_time.resize( size );
        m_deltaTime.resize( size );

        m_smoothTime.resize( size );
        m_prevSmoothTime.resize( size );
        m_smoothDeltaTime.resize( size );

        m_ticks.resize( size );

        m_minDeltaTime.resize( size );
        m_maxDeltaTime.resize( size );

        mEventTimes.resize( size );

        m_startPoint = std::chrono::high_resolution_clock::now();
    }

    TimerMT::~TimerMT()
    {
        unload( nullptr );
    }

    void TimerMT::update()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto seconds = std::chrono::duration<f64>( end - m_startPoint );
        auto nowTime = seconds.count();

        auto eTask = Thread::getCurrentTask();
        auto task = static_cast<s32>( eTask );

        auto delta = nowTime - m_time[task];

        m_deltaTime[task] = delta;

        auto time = m_time[task];
        m_prevTime[task] = time;
        m_time[task] = nowTime;

        if( m_enableSmoothing && m_frameSmoothingTime[task] != 0.0 )
        {
            auto smoothDeltaTime = calculateEventTime( nowTime, mEventTimes[task] );

            m_smoothDeltaTime[task] = smoothDeltaTime;
            m_prevSmoothTime[task] = m_smoothTime[task];
            m_smoothTime[task] = m_smoothTime[task] + smoothDeltaTime;
        }
        else
        {
            m_smoothDeltaTime[task] = delta;
            m_prevSmoothTime[task] = m_smoothTime[task];
            m_smoothTime[task] = m_time[task];
        }

        updateFixed();

        addAccumulated( eTask, delta );

        ++m_ticks[task];
    }

    void TimerMT::update( f64 dt )
    {
        auto eTask = Thread::getCurrentTask();
        auto task = static_cast<s32>( eTask );

        m_deltaTime[task] = dt;

        auto time = m_time[task];
        m_prevTime[task] = time;
        m_time[task] = time + dt;

        m_smoothDeltaTime[task] = dt;
        m_prevSmoothTime[task] = m_smoothTime[task];
        m_smoothTime[task] = m_time[task];

        updateFixed();
        addAccumulated( eTask, dt );

        ++m_ticks[task];
    }

    void TimerMT::updateFixed()
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTime[task] = m_fixedTime[task] + m_fixedTimeInterval[task];

        auto now = std::chrono::high_resolution_clock::now();
        m_fixedTimePoints[task] = now;
    }

    auto TimerMT::calculateEventTime( f64 fNow, std::deque<f64> &times ) -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        // Calculate the average time passed between events of the given type
        // during the last mFrameSmoothingTime seconds.
        times.push_back( fNow );

        if( times.size() == 1 )
        {
            return 0;
        }

        // Find the oldest time to keep
        auto it = times.begin(),
             end = times.end() - 2;  // We need at least two times
        while( it != end )
        {
            if( fNow - *it > m_frameSmoothingTime[task] )
            {
                ++it;
            }
            else
            {
                break;
            }
        }

        // Remove old times
        times.erase( times.begin(), it );

        return ( times.back() - times.front() ) / ( times.size() - 1 );
    }

    void TimerMT::reset()
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        m_currentPoint = m_startPoint = std::chrono::high_resolution_clock::now();

        // for (size_t i = 0; i < s32(Thread::Task::Count); ++i)
        //{
        //	mEventTimes[i].fill(0.0);
        // }

        // for (size_t i = 0; i < s32(Thread::Task::Count); ++i)
        //{
        //	m_fixedTime[i] = 0.0;
        // }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_prevSmoothTime[i] = m_prevTime[i];
        }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_smoothTime[i] = m_time[i];
        }

        // for (size_t i = 0; i < s32(Thread::Task::Count); ++i)
        //{
        //	m_time[i] = 0.0;
        // }

        for( auto &val : m_ticks )
        {
            val = 0;
        }
    }

    void TimerMT::reset( f64 t )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        setStartOffset( t );

        // for (size_t i = 0; i < s32(Thread::Task::Count); ++i)
        //{
        //	m_fixedTime[i] = 0.0;
        // }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_prevTime[i] = t;
        }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_time[i] = t;
        }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_prevSmoothTime[i] = m_prevTime[i];
        }

        for( size_t i = 0; i < static_cast<s32>( Thread::Task::Count ); ++i )
        {
            m_smoothTime[i] = m_time[i];
        }

        for( auto &val : m_ticks )
        {
            val = 0;
        }
    }

    auto TimerMT::getTickCount() -> u32
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_ticks[task];
    }

    auto TimerMT::getTickCount( Thread::Task task ) -> u32
    {
        auto iTask = static_cast<s32>( task );
        return m_ticks[iTask];
    }

    auto TimerMT::isSteady() const -> bool
    {
        return std::chrono::high_resolution_clock::is_steady;
    }

    auto TimerMT::getDerivedFixedTime() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        s32 ticks = m_ticks[task];
        return static_cast<f64>( ticks ) * getFixedTimeInterval();
    }

    auto TimerMT::getFixedTime() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_fixedTime[task];
    }

    auto TimerMT::getFixedTime( u32 task ) const -> f64
    {
        return m_fixedTime[task];
    }

    auto TimerMT::getFixedTimeNow() const -> f64
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        std::chrono::duration<f64> p = m_fixedTimePoints[task] - m_startPoint;
        std::chrono::duration<f64> seconds = now - m_fixedTimePoints[task];
        return getFixedTime() + seconds.count();
    }

    auto TimerMT::getFixedTimeNow( u32 task ) const -> f64
    {
        std::chrono::duration<f64> p = m_fixedTimePoints[task] - m_startPoint;
        return getFixedTime( task ) + ( now() - p.count() );
    }

    void TimerMT::setFixedTime( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTime[task] = val;
    }

    auto TimerMT::getFixedTimeInterval( Thread::Task task ) const -> f64
    {
        auto iTask = static_cast<s32>( task );
        return m_fixedTimeInterval[iTask];
    }

    auto TimerMT::getFixedTimeInterval() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_fixedTimeInterval[task];
    }

    void TimerMT::setFixedTimeInterval( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTimeInterval[task] = val;
    }

    void TimerMT::setFixedTimeInterval( Thread::Task task, f64 val )
    {
        m_fixedTimeInterval[static_cast<u32>( task )] = val;
    }

    void TimerMT::setFrameSmoothingTime( float val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_frameSmoothingTime[task] = val;

        for( auto &t : m_frameSmoothingTime )
        {
            t = val;
        }
    }

    auto TimerMT::getFrameSmoothingTime() const -> float
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return (f32)m_frameSmoothingTime[task];
    }

    void TimerMT::setStartTime( f64 time )
    {
        auto timeDuration = std::chrono::microseconds( static_cast<long long>( time * 1000000.0 ) );
        m_startPoint = std::chrono::high_resolution_clock::now() - timeDuration;
    }

    auto TimerMT::now128() const -> f64
    {
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<f64> seconds = end - m_startPoint;
        return seconds.count();
    }

    auto TimerMT::getSmoothTime() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_smoothTime[task];
    }

    auto TimerMT::getSmoothDeltaTime() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_smoothDeltaTime[task];
    }

    void TimerMT::setSmoothDeltaTime( f64 smoothDT )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_smoothDeltaTime[task] = smoothDT;
        m_prevSmoothTime[task] = m_smoothTime[task];
        m_smoothTime[task] = m_smoothTime[task] + smoothDT;
    }

    auto TimerMT::getMaxDeltaTime( Thread::Task task ) const -> f64
    {
        return m_maxDeltaTime[static_cast<s32>( task )];
    }

    void TimerMT::setMaxDeltaTime( Thread::Task task, const f64 t )
    {
        m_maxDeltaTime[static_cast<s32>( task )] = t;
    }

    auto TimerMT::getMinDeltaTime( Thread::Task task ) const -> f64
    {
        return m_minDeltaTime[static_cast<s32>( task )];
    }

    void TimerMT::setMinDeltaTime( Thread::Task task, const f64 t )
    {
        m_minDeltaTime[static_cast<s32>( task )] = t;
    }

    auto TimerMT::getStartOffset() const -> f64
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_startOffset[task];
    }

    void TimerMT::setStartOffset( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_startOffset[task] = val;
    }

    auto TimerMT::getStartOffset( Thread::Task task ) const -> f64
    {
        return m_startOffset[static_cast<s32>( task )];
    }

    void TimerMT::setStartOffset( Thread::Task task, f64 val )
    {
        m_startOffset[static_cast<s32>( task )] = val;
    }

    auto TimerMT::getFixedOffset( Thread::Task task ) const -> f64
    {
        return m_fixedOffset[static_cast<s32>( task )];
    }

    void TimerMT::setFixedOffset( Thread::Task task, f64 offset )
    {
        m_fixedOffset[static_cast<s32>( task )] = offset;
    }

    auto TimerMT::getAccumulated( Thread::Task task ) const -> f64
    {
        return m_accumulated[static_cast<s32>( task )];
    }

    void TimerMT::setAccumulated( Thread::Task task, f64 val )
    {
        m_accumulated[static_cast<s32>( task )] = val;
    }

    void TimerMT::addAccumulated( Thread::Task task, f64 val )
    {
        m_accumulated[static_cast<s32>( task )] += val;
    }

    auto TimerMT::now() const -> f64
    {
#if 0
        auto t = std::chrono::steady_clock::now();
        return t.time_since_epoch().count();
#else
        auto t = std::chrono::steady_clock::now();
        std::chrono::duration<f64> interval = t - m_startPoint;
        return interval.count();
#endif
    }
}  // end namespace fb
