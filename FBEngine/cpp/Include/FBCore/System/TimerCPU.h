#ifndef TimerCPU_h__
#define TimerCPU_h__

#include <FBCore/Interface/System/ITimer.h>

#include <deque>

#ifdef FB_USE_BOOST

#    include <boost/timer/timer.hpp>

namespace fb
{
    
    class TimerCPU : public ITimer
    {
    public:
        TimerCPU();
        ~TimerCPU() override;

        void update() override;

        u32 getTimeMilliseconds() const override;

        u32 getRealTime() const override;

        u32 getTimeIntervalMilliseconds() const override;

        time_interval getTime() const override;
        time_interval getTimeInterval() const override;

        void setFrameSmoothingPeriod( u32 milliSeconds ) override;
        u32 getFrameSmoothingPeriod() const override;

        void resetSmoothing() override;

        time_interval now() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        boost::timer::cpu_timer m_timer;
        boost::timer::cpu_times m_prevTime;
        boost::timer::cpu_times m_curTime;

        time_interval m_time;
        time_interval m_deltaTime;
        time_interval m_frameSmoothingTime;

        using EventTimesQueue = std::deque<f64>;
        EventTimesQueue mEventTimes;
    };
}  // end namespace fb

#endif

#endif  // TimerCPU_h__
