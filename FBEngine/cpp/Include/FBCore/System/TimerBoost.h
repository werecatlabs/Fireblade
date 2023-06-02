#ifndef TimerBoost_h__
#define TimerBoost_h__

#include <FBCore/System/Timer.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Core/Deque.h>
#include <FBCore/Core/Array.h>
#include <boost/chrono/chrono.hpp>

namespace fb
{
    class TimerBoost : public Timer
    {
    public:
        TimerBoost();
        ~TimerBoost() override;

        void update() override;
        void update( f64 dt );

        void updateFixed() override;

        f64 getTime() const override;
        f64 getDeltaTime() const override;
        f64 now() const override;

        float getFrameSmoothingTime() const override;
        void setFrameSmoothingTime( float val ) override;

        u32 getTickCount() override;
        u32 getTickCount( Thread::Task task ) override;

        void reset() override;
        void reset( f64 t ) override;

        bool isSteady() const override;

        f64 getDerivedFixedTime() const override;

        f64 getFixedTime() const override;
        f64 getFixedTime( u32 task ) const override;
        f64 getFixedTimeNow() const override;
        f64 getFixedTimeNow( u32 task ) const override;
        void setFixedTime( f64 val ) override;

        f64 getFixedTimeInterval( Thread::Task task ) const override;
        f64 getFixedTimeInterval() const override;
        void setFixedTimeInterval( f64 val ) override;
        void setFixedTimeInterval( Thread::Task task, f64 val ) override;

        void setStartTime( f64 time ) override;

        f64 now128() const;

        f64 getSmoothTime() const override;

        f64 getSmoothDeltaTime() const override;
        void setSmoothDeltaTime( f64 smoothDT ) override;

        f64 getTime( Thread::Task task ) const override;
        f64 getPreviousTime( Thread::Task task ) const override;
        f64 getDeltaTime( Thread::Task task ) const override;

        void setSmoothTime( Thread::Task task, f64 t );
        f64 getSmoothTime( Thread::Task task ) const;

        f64 getPrevSmoothTime( Thread::Task task ) const;
        f64 getSmoothDeltaTime( Thread::Task task ) const override;

        f64 getMaxDeltaTime( Thread::Task task ) const override;
        void setMaxDeltaTime( Thread::Task task, f64 t ) override;

        f64 getMinDeltaTime( Thread::Task task ) const override;
        void setMinDeltaTime( Thread::Task task, f64 t ) override;

        f64 getStartOffset() const override;
        void setStartOffset( f64 val ) override;

        f64 getStartOffset( Thread::Task task ) const override;
        void setStartOffset( Thread::Task task, f64 val ) override;

        f64 getFixedOffset( Thread::Task task ) const override;
        void setFixedOffset( Thread::Task task, f64 offset ) override;

        f64 getAccumulated( Thread::Task task ) const override;
        void setAccumulated( Thread::Task task, f64 val ) override;
        void addAccumulated( Thread::Task task, f64 val ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        f64 calculateEventTime( f64 fNow, std::deque<f64> &times );

        Array<atomic_f64> m_accumulated;
        Array<atomic_f64> m_startOffset;
        Array<atomic_f64> m_fixedOffset;

        Array<atomic_f64> m_frameSmoothingTime;

        Array<boost::chrono::steady_clock::time_point> m_fixedTimePoints;
        Array<atomic_f64> m_fixedTime;
        Array<atomic_f64> m_fixedTimeInterval;

        Array<atomic_f64> m_prevTime;
        Array<atomic_f64> m_time;
        Array<atomic_f64> m_deltaTime;

        Array<atomic_f64> m_smoothTime;
        Array<atomic_f64> m_prevSmoothTime;
        Array<atomic_f64> m_smoothDeltaTime;

        Array<atomic_u32> m_ticks;

        Array<atomic_f64> m_minDeltaTime;
        Array<atomic_f64> m_maxDeltaTime;

        boost::chrono::steady_clock::time_point m_startPoint;
        boost::chrono::steady_clock::time_point m_currentPoint;

        atomic_bool m_bStarted = false;

        using EventTimesQueue = std::deque<f64>;
        Array<EventTimesQueue> mEventTimes;
    };

    inline f64 TimerBoost::getDeltaTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_deltaTime[task];
    }

    inline f64 TimerBoost::getTime() const
    {
        auto task = static_cast<s32>( Thread::getCurrentTask() );
        return m_time[task];
    }

    inline f64 TimerBoost::getTime( Thread::Task task ) const
    {
        return m_time[static_cast<s32>( task )];
    }

    inline f64 TimerBoost::getPreviousTime( Thread::Task task ) const
    {
        return m_prevTime[static_cast<s32>( task )];
    }

    inline f64 TimerBoost::getDeltaTime( Thread::Task task ) const
    {
        return m_deltaTime[static_cast<s32>( task )];
    }

    inline void TimerBoost::setSmoothTime( Thread::Task task, const f64 t )
    {
        m_smoothTime[static_cast<s32>( task )] = t;
    }

    inline f64 TimerBoost::getSmoothTime( Thread::Task task ) const
    {
        return m_smoothTime[static_cast<s32>( task )];
    }

    inline f64 TimerBoost::getPrevSmoothTime( Thread::Task task ) const
    {
        return m_prevSmoothTime[static_cast<s32>( task )];
    }

    inline f64 TimerBoost::getSmoothDeltaTime( Thread::Task task ) const
    {
        return m_smoothDeltaTime[static_cast<s32>( task )];
    }

}  // namespace fb

#endif  // TimerBoost_h__
