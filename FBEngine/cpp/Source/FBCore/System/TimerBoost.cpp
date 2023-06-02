#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TimerBoost.h>
#include <FBCore/Math/MathUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TimerBoost, CSharedObject<Timer> );

    TimerBoost::TimerBoost() : m_bStarted( false )
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

        m_startPoint = boost::chrono::steady_clock::now();
    }

    TimerBoost::~TimerBoost()
    {
        unload( nullptr );
    }

    void TimerBoost::update()
    {
        auto end = boost::chrono::steady_clock::now();
        auto seconds = boost::chrono::duration<f64>( end - m_startPoint );
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

    void TimerBoost::update( f64 dt )
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

    void TimerBoost::updateFixed()
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTime[task] = m_fixedTime[task] + m_fixedTimeInterval[task];

        auto now = boost::chrono::high_resolution_clock::now();
        m_fixedTimePoints[task] = now;
    }

    f64 TimerBoost::calculateEventTime( f64 fNow, std::deque<f64> &times )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        // Calculate the average time passed between events of the given type
        // during the last mFrameSmoothingTime seconds.
        times.push_back( fNow );

        if( times.size() == 1 )
            return 0;

        // Find the oldest time to keep
        auto it = times.begin(),
             end = times.end() - 2;  // We need at least two times
        while( it != end )
        {
            if( fNow - *it > m_frameSmoothingTime[task] )
                ++it;
            else
                break;
        }

        // Remove old times
        times.erase( times.begin(), it );

        return ( times.back() - times.front() ) / ( times.size() - 1 );
    }

    void TimerBoost::reset()
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        m_currentPoint = m_startPoint = boost::chrono::high_resolution_clock::now();

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

    void TimerBoost::reset( f64 t )
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

    u32 TimerBoost::getTickCount()
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_ticks[task];
    }

    u32 TimerBoost::getTickCount( Thread::Task task )
    {
        auto iTask = static_cast<s32>( task );
        return m_ticks[iTask];
    }

    bool TimerBoost::isSteady() const
    {
        return boost::chrono::high_resolution_clock::is_steady;
    }

    f64 TimerBoost::getDerivedFixedTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        s32 ticks = m_ticks[task];
        return static_cast<f64>( ticks ) * getFixedTimeInterval();
    }

    f64 TimerBoost::getFixedTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_fixedTime[task];
    }

    f64 TimerBoost::getFixedTime( u32 task ) const
    {
        return m_fixedTime[task];
    }

    f64 TimerBoost::getFixedTimeNow() const
    {
        auto now = boost::chrono::high_resolution_clock::now();
        auto task = static_cast<s32>( Thread::getCurrentTask() );

        boost::chrono::duration<f64> p = m_fixedTimePoints[task] - m_startPoint;
        boost::chrono::duration<f64> seconds = now - m_fixedTimePoints[task];
        return getFixedTime() + seconds.count();
    }

    f64 TimerBoost::getFixedTimeNow( u32 task ) const
    {
        boost::chrono::duration<f64> p = m_fixedTimePoints[task] - m_startPoint;
        return getFixedTime( task ) + ( now() - p.count() );
    }

    void TimerBoost::setFixedTime( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTime[task] = val;
    }

    f64 TimerBoost::getFixedTimeInterval( Thread::Task task ) const
    {
        auto iTask = static_cast<s32>( task );
        return m_fixedTimeInterval[iTask];
    }

    f64 TimerBoost::getFixedTimeInterval() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_fixedTimeInterval[task];
    }

    void TimerBoost::setFixedTimeInterval( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_fixedTimeInterval[task] = val;
    }

    void TimerBoost::setFixedTimeInterval( Thread::Task task, f64 val )
    {
        m_fixedTimeInterval[static_cast<u32>( task )] = val;
    }

    void TimerBoost::setFrameSmoothingTime( float val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_frameSmoothingTime[task] = val;

        for( auto &t : m_frameSmoothingTime )
        {
            t = val;
        }
    }

    float TimerBoost::getFrameSmoothingTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return (f32)m_frameSmoothingTime[task];
    }

    void TimerBoost::setStartTime( f64 time )
    {
        auto timeDuration = boost::chrono::microseconds( static_cast<long long>( time * 1000000.0 ) );
        m_startPoint = boost::chrono::high_resolution_clock::now() - timeDuration;
    }

    f64 TimerBoost::now128() const
    {
        boost::chrono::steady_clock::time_point end = boost::chrono::high_resolution_clock::now();
        boost::chrono::duration<f64> seconds = end - m_startPoint;
        return seconds.count();
    }

    f64 TimerBoost::getSmoothTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_smoothTime[task];
    }

    f64 TimerBoost::getSmoothDeltaTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_smoothDeltaTime[task];
    }

    void TimerBoost::setSmoothDeltaTime( f64 smoothDT )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_smoothDeltaTime[task] = smoothDT;
        m_prevSmoothTime[task] = m_smoothTime[task];
        m_smoothTime[task] = m_smoothTime[task] + smoothDT;
    }

    f64 TimerBoost::getMaxDeltaTime( Thread::Task task ) const
    {
        return m_maxDeltaTime[static_cast<s32>( task )];
    }

    void TimerBoost::setMaxDeltaTime( Thread::Task task, const f64 t )
    {
        m_maxDeltaTime[static_cast<s32>( task )] = t;
    }

    f64 TimerBoost::getMinDeltaTime( Thread::Task task ) const
    {
        return m_minDeltaTime[static_cast<s32>( task )];
    }

    void TimerBoost::setMinDeltaTime( Thread::Task task, const f64 t )
    {
        m_minDeltaTime[static_cast<s32>( task )] = t;
    }

    f64 TimerBoost::getStartOffset() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_startOffset[task];
    }

    void TimerBoost::setStartOffset( f64 val )
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        m_startOffset[task] = val;
    }

    f64 TimerBoost::getStartOffset( Thread::Task task ) const
    {
        return m_startOffset[static_cast<s32>( task )];
    }

    void TimerBoost::setStartOffset( Thread::Task task, f64 val )
    {
        m_startOffset[static_cast<s32>( task )] = val;
    }

    f64 TimerBoost::getFixedOffset( Thread::Task task ) const
    {
        return m_fixedOffset[static_cast<s32>( task )];
    }

    void TimerBoost::setFixedOffset( Thread::Task task, f64 offset )
    {
        m_fixedOffset[static_cast<s32>( task )] = offset;
    }

    f64 TimerBoost::getAccumulated( Thread::Task task ) const
    {
        return m_accumulated[static_cast<s32>( task )];
    }

    void TimerBoost::setAccumulated( Thread::Task task, f64 val )
    {
        m_accumulated[static_cast<s32>( task )] = val;
    }

    void TimerBoost::addAccumulated( Thread::Task task, f64 val )
    {
        m_accumulated[static_cast<s32>( task )] += val;
    }

    f64 TimerBoost::now() const
    {
        auto t = boost::chrono::steady_clock::now();
        boost::chrono::duration<f64> interval = t - m_startPoint;
        return interval.count();
    }
}  // end namespace fb
