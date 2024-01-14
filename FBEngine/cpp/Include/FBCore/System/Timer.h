#ifndef __Timer_h__
#define __Timer_h__

#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/FixedArray.h>

namespace fb
{

    /** Stub implementation of the timer interface. */
    class Timer : public ITimer
    {
    public:
        /** Constructor. */
        Timer();

        /** Destructor. */
        ~Timer() override;

        void update() override;
        void updateFixed() override;

        f64 getTimeSinceLevelLoad() override;
        void setTimeSinceLevelLoad( time_interval t ) override;
        void addTimeSinceLevelLoad( time_interval deltaTime ) override;

        f64 getSmoothTime() const override;
        f64 getSmoothDeltaTime() const override;
        void setSmoothDeltaTime( f64 smoothDT ) override;

        f64 getTime() const override;
        f64 getDeltaTime() const override;
        f64 now() const override;

        virtual float getFrameSmoothingTime() const;
        virtual void setFrameSmoothingTime( float val );

        u32 getTickCount() override;
        u32 getTickCount( Thread::Task task ) override;

        bool isSteady() const override;

        void reset() override;
        void reset( f64 t ) override;

        f64 getMinDeltaTime() const;
        void setMinDeltaTime( f64 val );

        f64 getMaxDeltaTime() const;
        void setMaxDeltaTime( f64 val );

        virtual f64 getDerivedFixedTime() const;
        f64 getFixedTime() const override;
        f64 getFixedTime( u32 task ) const override;
        virtual f64 getFixedTimeNow() const;
        virtual f64 getFixedTimeNow( u32 task ) const;
        virtual void setFixedTime( f64 val );

        f64 getFixedTimeInterval( Thread::Task task ) const override;
        f64 getFixedTimeInterval() const override;
        void setFixedTimeInterval( f64 val ) override;
        void setFixedTimeInterval( Thread::Task task, f64 val ) override;

        f64 getTime( Thread::Task task ) const override;
        f64 getPreviousTime( Thread::Task task ) const override;
        f64 getDeltaTime( Thread::Task task ) const override;

        f64 getSmoothDeltaTime( Thread::Task task ) const override;

        virtual void setStartTime( f64 time );

        bool getEnableSmoothing() const;
        void setEnableSmoothing( bool val );

        virtual f64 getMaxDeltaTime( Thread::Task task ) const;
        virtual void setMaxDeltaTime( Thread::Task task, f64 t );

        virtual f64 getMinDeltaTime( Thread::Task task ) const;
        virtual void setMinDeltaTime( Thread::Task task, f64 t );

        f64 getStartOffset() const override;
        void setStartOffset( f64 val ) override;

        f64 getStartOffset( Thread::Task task ) const override;
        void setStartOffset( Thread::Task task, f64 val ) override;

        f64 getFixedOffset( Thread::Task task ) const override;
        void setFixedOffset( Thread::Task task, f64 offset ) override;

        virtual f64 getAccumulated( Thread::Task task ) const;
        virtual void setAccumulated( Thread::Task task, f64 val );
        virtual void addAccumulated( Thread::Task task, f64 val );

        u32 getTimeMilliseconds() const override;
        u32 getRealTime() const override;

        u32 getTimeIntervalMilliseconds() const override;
        time_interval getTimeInterval() const override;

        void setFrameSmoothingPeriod( u32 milliSeconds ) override;
        u32 getFrameSmoothingPeriod() const override;

        void resetSmoothing() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        f64 m_minDeltaTime = 0.0;
        f64 m_maxDeltaTime = 0.0;
        atomic_bool m_enableSmoothing = false;
        atomic_f64 m_timeSinceLevelLoad = 0.0;
    };
}  // end namespace fb

#endif  // Timer_h__
