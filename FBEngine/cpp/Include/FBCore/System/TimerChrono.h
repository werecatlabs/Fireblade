#ifndef TimerChrono_h__
#define TimerChrono_h__

#include <FBCore/System/Timer.h>
#include <FBCore/Math/General/Smooth.h>

namespace fb
{
    class TimerChrono : public Timer
    {
    public:
        TimerChrono();
        TimerChrono( const TimerChrono &other ) = delete;
        ~TimerChrono() override;

        void update() override;

        u32 getTimeMilliseconds() const override;

        u32 getRealTime() const override;

        u32 getTimeIntervalMilliseconds() const override;

        time_interval now() const override;

        time_interval getTime() const override;

        time_interval getTimeInterval() const override;

        void setFrameSmoothingPeriod( u32 milliSeconds ) override;

        u32 getFrameSmoothingPeriod() const override;

        void resetSmoothing() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        void setTime( time_interval val );
        void setDeltaTime( time_interval val );

        atomic_u32 m_time;
        atomic_u32 m_deltaTime;

        using TimerSmooth = Smooth<time_interval>;
        TimerSmooth m_smoother;
    };
}  // end namespace fb

#endif  // TimerChrono_h__
