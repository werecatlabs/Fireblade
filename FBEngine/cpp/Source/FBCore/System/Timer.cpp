#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Timer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Timer, ITimer );

    Timer::Timer() = default;

    Timer::~Timer() = default;

    auto Timer::getTimeSinceLevelLoad() -> f64
    {
        return m_timeSinceLevelLoad;
    }

    void Timer::setTimeSinceLevelLoad( time_interval t )
    {
        m_timeSinceLevelLoad = t;
    }

    void Timer::addTimeSinceLevelLoad( f64 val )
    {
        m_timeSinceLevelLoad += val;
    }

    void Timer::setMaxDeltaTime( f64 val )
    {
        m_maxDeltaTime = val;
    }

    auto Timer::getDerivedFixedTime() const -> f64
    {
        return 0.0;
    }

    auto Timer::getFixedTime() const -> f64
    {
        return 0.0;
    }

    auto Timer::getFixedTime( u32 task ) const -> f64
    {
        return 0.0;
    }

    auto Timer::getFixedTimeNow() const -> f64
    {
        return 0.0;
    }

    auto Timer::getFixedTimeNow( u32 task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setFixedTime( f64 val )
    {
    }

    auto Timer::getFixedTimeInterval() const -> f64
    {
        return 0.0;
    }

    auto Timer::getFixedTimeInterval( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setFixedTimeInterval( f64 val )
    {
    }

    void Timer::setFixedTimeInterval( Thread::Task task, f64 val )
    {
    }

    void Timer::setStartTime( f64 time )
    {
    }

    auto Timer::getEnableSmoothing() const -> bool
    {
        return m_enableSmoothing;
    }

    void Timer::setEnableSmoothing( bool val )
    {
        m_enableSmoothing = val;
    }

    auto Timer::getMaxDeltaTime() const -> f64
    {
        return m_maxDeltaTime;
    }

    void Timer::setMinDeltaTime( f64 val )
    {
        m_minDeltaTime = val;
    }

    auto Timer::getStartOffset() const -> f64
    {
        return 0.0;
    }

    auto Timer::getMinDeltaTime() const -> f64
    {
        return m_minDeltaTime;
    }

    void Timer::reset()
    {
    }

    void Timer::reset( f64 t )
    {
    }

    auto Timer::isSteady() const -> bool
    {
        return false;
    }

    auto Timer::getTickCount() -> u32
    {
        return 0;
    }

    auto Timer::getTickCount( Thread::Task task ) -> u32
    {
        return 0;
    }

    void Timer::setFrameSmoothingTime( float val )
    {
    }

    auto Timer::getFrameSmoothingTime() const -> float
    {
        return 0.0f;
    }

    auto Timer::now() const -> f64
    {
        return 0.0;
    }

    auto Timer::getDeltaTime() const -> f64
    {
        return 0.0;
    }

    auto Timer::getTime() const -> f64
    {
        return 0.0;
    }

    void Timer::update()
    {
    }

    void Timer::updateFixed()
    {
    }

    auto Timer::getSmoothTime() const -> f64
    {
        return 0.0;
    }

    auto Timer::getSmoothDeltaTime() const -> f64
    {
        return 0.0;
    }

    auto Timer::getSmoothDeltaTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setSmoothDeltaTime( f64 smoothDT )
    {
    }

    auto Timer::getTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    auto Timer::getPreviousTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    auto Timer::getDeltaTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    auto Timer::getMaxDeltaTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setMaxDeltaTime( Thread::Task task, const f64 t )
    {
    }

    auto Timer::getMinDeltaTime( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setMinDeltaTime( Thread::Task task, const f64 t )
    {
    }

    void Timer::setStartOffset( f64 val )
    {
    }

    auto Timer::getStartOffset( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setStartOffset( Thread::Task task, f64 val )
    {
    }

    auto Timer::getFixedOffset( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setFixedOffset( Thread::Task task, f64 offset )
    {
    }

    auto Timer::getAccumulated( Thread::Task task ) const -> f64
    {
        return 0.0;
    }

    void Timer::setAccumulated( Thread::Task task, f64 val )
    {
    }

    void Timer::addAccumulated( Thread::Task task, f64 val )
    {
    }

    auto Timer::getTimeMilliseconds() const -> u32
    {
        return static_cast<u32>( getTime() * 1000.0 );
    }

    auto Timer::getRealTime() const -> u32
    {
        return static_cast<u32>( getTime() * 1000.0 );
    }

    auto Timer::getTimeIntervalMilliseconds() const -> u32
    {
        return static_cast<u32>( getDeltaTime() * 1000.0 );
    }

    void Timer::setFrameSmoothingPeriod( u32 milliSeconds )
    {
    }

    auto Timer::getFrameSmoothingPeriod() const -> u32
    {
        return 0;
    }

    void Timer::resetSmoothing()
    {
    }
}  // end namespace fb
