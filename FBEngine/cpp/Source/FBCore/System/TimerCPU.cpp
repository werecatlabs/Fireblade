#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TimerCPU.h>

#ifdef FB_USE_BOOST

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TimerCPU, CSharedObject<ITimer> );

    TimerCPU::TimerCPU()
    {
        m_prevTime = m_timer.elapsed();
        m_curTime = m_prevTime;
    }

    TimerCPU::~TimerCPU()
    {
    }

    void TimerCPU::update()
    {
        m_prevTime = m_curTime;
        m_curTime = m_timer.elapsed();

#    if 1
        m_deltaTime =
            static_cast<f64>( m_curTime.wall - m_prevTime.wall ) / static_cast<f64>( 1000000000LL );
        m_time = m_time + m_deltaTime;
#    else
        m_deltaTime = calculateEventTime( (f64)( m_curTime.wall ) / (f64)( 1000000000LL ) );
#    endif
    }

    u32 TimerCPU::getTimeMilliseconds() const
    {
        return static_cast<u32>( m_time * 1000.0 );
    }

    u32 TimerCPU::getRealTime() const
    {
        return static_cast<u32>( m_time * 1000.0 );
    }

    u32 TimerCPU::getTimeIntervalMilliseconds() const
    {
        return static_cast<u32>( m_deltaTime * 1000.0 );
    }

    time_interval TimerCPU::getTimeInterval() const
    {
        return m_deltaTime;
    }

    void TimerCPU::setFrameSmoothingPeriod( [[maybe_unused]] u32 milliSeconds )
    {
    }

    u32 TimerCPU::getFrameSmoothingPeriod() const
    {
        return static_cast<u32>( m_frameSmoothingTime );
    }

    void TimerCPU::resetSmoothing()
    {
    }

    time_interval TimerCPU::getTime() const
    {
        return m_time;
    }

    time_interval TimerCPU::now() const
    {
        return static_cast<f64>( m_timer.elapsed().wall ) / static_cast<f64>( 1000000000LL );
    }
}  // end namespace fb

#endif
