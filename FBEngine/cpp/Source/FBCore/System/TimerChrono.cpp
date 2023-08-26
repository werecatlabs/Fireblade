#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TimerChrono.h>
#include <chrono>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TimerChrono, Timer );

    TimerChrono::TimerChrono()
    {
    }

    TimerChrono::~TimerChrono()
    {
    }

    void TimerChrono::update()
    {
        time_interval time =
            static_cast<f64>( std::chrono::steady_clock::now().time_since_epoch().count() ) /
            static_cast<f64>( 1000000000LL );
        time_interval deltaTime = m_smoother.getValue( SmoothValue<time_interval>( time, time ) );
        time_interval elapsedTime = getTime() + deltaTime;

        setDeltaTime( deltaTime );
        setTime( elapsedTime );
    }

    u32 TimerChrono::getTimeMilliseconds() const
    {
        return static_cast<u32>( getTime() * 1000.0 );
    }

    u32 TimerChrono::getRealTime() const
    {
        time_interval time =
            static_cast<f64>( std::chrono::steady_clock::now().time_since_epoch().count() ) /
            static_cast<f64>( 1000000000LL );
        return static_cast<u32>( time * 1000.0 );
    }

    u32 TimerChrono::getTimeIntervalMilliseconds() const
    {
        return static_cast<u32>( getTimeInterval() * 1000.0 );
    }

    time_interval TimerChrono::now() const
    {
        return static_cast<f64>( std::chrono::steady_clock::now().time_since_epoch().count() ) /
               static_cast<f64>( 1000000000LL );
    }

    time_interval TimerChrono::getTime() const
    {
        u32 time = m_time;
        return *reinterpret_cast<time_interval *>( &time );
    }

    time_interval TimerChrono::getTimeInterval() const
    {
        u32 deltaTime = m_deltaTime;
        return *reinterpret_cast<time_interval *>( &deltaTime );
    }

    void TimerChrono::setFrameSmoothingPeriod( u32 milliSeconds )
    {
        m_smoother.setInterval( static_cast<float>( milliSeconds ) / 1000.0f );
    }

    u32 TimerChrono::getFrameSmoothingPeriod() const
    {
        return static_cast<u32>( m_smoother.getInterval() * 1000.0f );
    }

    void TimerChrono::resetSmoothing()
    {
    }

    void TimerChrono::setDeltaTime( time_interval val )
    {
        m_deltaTime = *reinterpret_cast<u32 *>( &val );
    }

    void TimerChrono::setTime( time_interval val )
    {
        m_time = *reinterpret_cast<u32 *>( &val );
    }
}  // end namespace fb
