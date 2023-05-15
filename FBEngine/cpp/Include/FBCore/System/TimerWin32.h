#ifndef TimerWin_h__
#define TimerWin_h__

#include <FBCore/System/Timer.h>
#include <deque>

#ifdef FB_PLATFORM_WIN32
#    include <windows.h>
#endif

namespace fb
{
#ifdef FB_PLATFORM_WIN32

    class TimerWin32 : public Timer
    {
    public:
        TimerWin32();
        ~TimerWin32() override;

        //! returns the current time in milliseconds
        inline u32 getTimeMilliseconds() const override;

        //! initializes the real timer
        void initTimer();

        //! sets the current virtual (game) time
        void setTime( u32 time );

        //! stops the virtual (game) timer
        void stopTimer();

        //! starts the game timer
        void startTimer();

        //! sets the speed of the virtual timer
        void setSpeed( f32 speed );

        //! gets the speed of the virtual timer
        f32 getSpeed();

        //! returns if the timer currently is stopped
        inline bool isStopped() const;

        //! makes the virtual timer update the time value based on the real time
        void update() override;

        //! returns the current real time in milliseconds
        u32 getRealTime() const override;

        // returns the time in milli seconds between ticks
        u32 getTimeIntervalMilliseconds() const override;

        time_interval getTime() const override;
        time_interval getTimeInterval() const override;

        void setFrameSmoothingPeriod( u32 milliSeconds ) override;
        u32 getFrameSmoothingPeriod() const override;

        void resetSmoothing() override;

        FB_CLASS_REGISTER_DECL;

    private:
        void initVirtualTimer();

        f32 calculateEventTime( u32 now );

        time_interval now() const override;

        LARGE_INTEGER HighPerformanceFreq;
        bool32 HighPerformanceTimerSupport;
        bool32 MultiCore;

        f32 VirtualTimerSpeed;
        atomic_s32 VirtualTimerStopCounter;
        atomic_u32 StartRealTime;
        atomic_u32 LastVirtualTime;
        atomic_u32 StaticTime;
        atomic_u32 TimeDelta;
        atomic_u32 SmoothingPeriod;

        using EventTimesQueue = std::deque<unsigned long>;
        EventTimesQueue mEventTimes;

        RecursiveMutex m_mutex;
    };

    inline u32 TimerWin32::getTimeMilliseconds() const
    {
        if( isStopped() )
        {
            return LastVirtualTime;
        }

        return LastVirtualTime + static_cast<u32>( ( StaticTime - StartRealTime ) * VirtualTimerSpeed );
    }

    inline bool TimerWin32::isStopped() const
    {
        return VirtualTimerStopCounter != 0;
    }

#endif
}  // end namespace fb

#endif  // TimerWin_h__
