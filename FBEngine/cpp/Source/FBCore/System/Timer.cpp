#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Timer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Timer, SharedObject<ITimer> );

    Timer::Timer() : m_minDeltaTime( 0.0 ), m_maxDeltaTime( 1e10 ), m_enableSmoothing( false )
    {
    }

    Timer::~Timer()
    {
    }

    f64 Timer::getTimeSinceLevelLoad()
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

    f64 Timer::getDerivedFixedTime() const
    {
        return 0.0;
    }

    f64 Timer::getFixedTime() const
    {
        return 0.0;
    }

    f64 Timer::getFixedTime( u32 task ) const
    {
        return 0.0;
    }

    f64 Timer::getFixedTimeNow() const
    {
        return 0.0;
    }

    f64 Timer::getFixedTimeNow( u32 task ) const
    {
        return 0.0;
    }

    void Timer::setFixedTime( f64 val )
    {
    }

    f64 Timer::getFixedTimeInterval() const
    {
        return 0.0;
    }

    f64 Timer::getFixedTimeInterval( Thread::Task task ) const
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

    bool Timer::getEnableSmoothing() const
    {
        return m_enableSmoothing;
    }

    void Timer::setEnableSmoothing( bool val )
    {
        m_enableSmoothing = val;
    }

    f64 Timer::getMaxDeltaTime() const
    {
        return m_maxDeltaTime;
    }

    void Timer::setMinDeltaTime( f64 val )
    {
        m_minDeltaTime = val;
    }

    f64 Timer::getStartOffset() const
    {
        return 0.0;
    }

    f64 Timer::getMinDeltaTime() const
    {
        return m_minDeltaTime;
    }

    void Timer::reset()
    {
    }

    void Timer::reset( f64 t )
    {
    }

    bool Timer::isSteady() const
    {
        return false;
    }

    u32 Timer::getTickCount()
    {
        return 0;
    }

    u32 Timer::getTickCount( Thread::Task task )
    {
        return 0;
    }

    void Timer::setFrameSmoothingTime( float val )
    {
    }

    float Timer::getFrameSmoothingTime() const
    {
        return 0.0f;
    }

    f64 Timer::now() const
    {
        return 0.0;
    }

    f64 Timer::getDeltaTime() const
    {
        return 0.0;
    }

    f64 Timer::getTime() const
    {
        return 0.0;
    }

    void Timer::update()
    {
    }

    void Timer::updateFixed()
    {
    }

    f64 Timer::getSmoothTime() const
    {
        return 0.0;
    }

    f64 Timer::getSmoothDeltaTime() const
    {
        return 0.0;
    }

    f64 Timer::getSmoothDeltaTime( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setSmoothDeltaTime( f64 smoothDT )
    {
    }

    f64 Timer::getTime( Thread::Task task ) const
    {
        return 0.0;
    }

    f64 Timer::getPreviousTime( Thread::Task task ) const
    {
        return 0.0;
    }

    f64 Timer::getDeltaTime( Thread::Task task ) const
    {
        return 0.0;
    }

    f64 Timer::getMaxDeltaTime( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setMaxDeltaTime( Thread::Task task, const f64 t )
    {
    }

    f64 Timer::getMinDeltaTime( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setMinDeltaTime( Thread::Task task, const f64 t )
    {
    }

    void Timer::setStartOffset( f64 val )
    {
    }

    f64 Timer::getStartOffset( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setStartOffset( Thread::Task task, f64 val )
    {
    }

    f64 Timer::getFixedOffset( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setFixedOffset( Thread::Task task, f64 offset )
    {
    }

    f64 Timer::getAccumulated( Thread::Task task ) const
    {
        return 0.0;
    }

    void Timer::setAccumulated( Thread::Task task, f64 val )
    {
    }

    void Timer::addAccumulated( Thread::Task task, f64 val )
    {
    }

    u32 Timer::getTimeMilliseconds() const
    {
        return static_cast<u32>( getTime() * 1000.0 );
    }

    u32 Timer::getRealTime() const
    {
        return static_cast<u32>( getTime() * 1000.0 );
    }

    u32 Timer::getTimeIntervalMilliseconds() const
    {
        return static_cast<u32>( getDeltaTime() * 1000.0 );
    }

    time_interval Timer::getTimeInterval() const
    {
        return getDeltaTime();  // todo remove
    }

    void Timer::setFrameSmoothingPeriod( u32 milliSeconds )
    {
    }

    u32 Timer::getFrameSmoothingPeriod() const
    {
        return 0;
    }

    void Timer::resetSmoothing()
    {
    }
}  // end namespace fb
