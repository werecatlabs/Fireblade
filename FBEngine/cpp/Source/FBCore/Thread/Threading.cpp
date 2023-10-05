#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Core/StringTypes.h>
#include <thread>

#if FB_USE_BOOST
#    include <boost/thread.hpp>
#endif

#if FB_USE_ONETBB
//#    include <tbb/tbb_thread.h>
#elif FB_USE_TBB
#    include <tbb/tbb_thread.h>
#else
#    include <thread>
#endif

#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
#    include <boost/detail/interlocked.hpp>
#endif

namespace fb
{
    FB_THREAD_LOCAL_STORAGE s32 CURRENT_TASK_ID = 0;
    FB_THREAD_LOCAL_STORAGE u32 CURRENT_THREAD_ID = 0;
    FB_THREAD_LOCAL_STORAGE u32 CURRENT_THREAD_FLAGS = 0;

    const u32 Thread::Primary_Flag = 1 << 1;
    const u32 Thread::Ai_Flag = 1 << 2;
    const u32 Thread::Animation_Flag = 1 << 3;
    const u32 Thread::Application_Flag = 1 << 4;
    const u32 Thread::Collision_Flag = 1 << 5;
    const u32 Thread::Controls_Flag = 1 << 6;
    const u32 Thread::Dynamics_Flag = 1 << 7;
    const u32 Thread::GarbageCollect_Flag = 1 << 8;
    const u32 Thread::Physics_Flag = 1 << 9;
    const u32 Thread::None_Flag = 1 << 10;
    const u32 Thread::Render_Flag = 1 << 11;
    const u32 Thread::Sound_Flag = 1 << 12;

    FixedArray<u32, (u32)Thread::Task::Count> Thread::m_taskFlags;
    SpinRWMutex Thread::m_taskFlagsMutex;

    Thread::Task Thread::getCurrentTask()
    {
        FB_ASSERT( CURRENT_TASK_ID >= 0 );
        FB_ASSERT( CURRENT_TASK_ID < static_cast<int>( Thread::Task::Count ) );
        return static_cast<Task>( CURRENT_TASK_ID );
    }

    void Thread::setCurrentTask( Task task )
    {
        FB_ASSERT( static_cast<s32>( task ) >= 0 );
        FB_ASSERT( static_cast<s32>( task ) < static_cast<int>( Thread::Task::Count ) );
        CURRENT_TASK_ID = static_cast<u32>( task );
    }

    u32 Thread::getTaskFlags()
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, false );
        auto task = (u32)getCurrentTask();
        return m_taskFlags[task];
    }

    void Thread::setTaskFlags( u32 taskFlags )
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, true );
        auto task = (u32)getCurrentTask();
        m_taskFlags[task] = taskFlags;
    }

    u32 Thread::getTaskFlags( Task task )
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, false );
        return m_taskFlags[(u32)task];
    }

    void Thread::setTaskFlags( Task task, u32 taskFlags )
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, true );
        m_taskFlags[(u32)task] = taskFlags;
    }

    bool Thread::getTaskFlag( u32 flag )
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, false );
        auto task = (u32)getCurrentTask();
        return (m_taskFlags[task] & flag) != 0;
    }

    void Thread::setTaskFlag( u32 flag, bool value )
    {
        SpinRWMutex::ScopedLock lock( m_taskFlagsMutex, true );

        auto task = (u32)getCurrentTask();

        if( value )
        {
            m_taskFlags[task] |= flag;
        }
        else
        {
            m_taskFlags[task] &= ~flag;
        }
    }

    void Thread::sleep( time_interval seconds )
    {
#if 0
		if (seconds == 0)
			return;

		static LARGE_INTEGER s_freq = { 0,0 };

		if (s_freq.QuadPart == 0)
			QueryPerformanceFrequency(&s_freq);

		LARGE_INTEGER from, now;
		QueryPerformanceCounter(&from);
		int ticks_to_wait = (int)((double)s_freq.QuadPart / (1.0 / seconds));
		bool done = false;

		int ticks_passed;
		int ticks_left;

		do
		{
			QueryPerformanceCounter(&now);
			ticks_passed = (int)((__int64)now.QuadPart - (__int64)from.QuadPart);
			ticks_left = ticks_to_wait - ticks_passed;

			if (now.QuadPart < from.QuadPart)    // time wrap
				done = true;

			if (ticks_passed >= ticks_to_wait)
				done = true;

			if (!done)
			{
				if (ticks_left > (int)s_freq.QuadPart * 2 / 1000)
					tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(seconds * 0.25));
				else
					tbb::this_tbb_thread::yield();
			}
		} while (!done);
#else

#    if FB_USE_TBB
        if( seconds > 0.0 )
        {
            tbb::this_tbb_thread::sleep( tbb::tick_count::interval_t( seconds ) );
        }
#    elif FB_USE_ONETBB
        if( seconds > time_interval( 0.0 ) )
        {
            std::chrono::duration<double, std::milli> t( seconds * 1000 );
            std::this_thread::sleep_for( t );
        }
#    else
        if( seconds > time_interval( 0.0 ) )
        {
            std::chrono::duration<double, std::milli> t( seconds * 1000 );
            std::this_thread::sleep_for( t );
        }
#    endif
#endif
    }

    void Thread::interlockedExchange( volatile long *target, long value )
    {
#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
        BOOST_INTERLOCKED_EXCHANGE( target, value );
#else
        *target = value;
#endif
    }

    void Thread::interlockedExchangePointer( void **target, void *value )
    {
#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
        BOOST_INTERLOCKED_EXCHANGE_POINTER( target, value );
#else
        *target = value;
#endif
    }

    long Thread::interlockedCompareExchange( volatile long *a, long b, long c )
    {
#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
        return BOOST_INTERLOCKED_COMPARE_EXCHANGE( a, b, c );
#else
        return 0;
#endif
    }

    long Thread::interlockedDecrement( volatile long *val )
    {
#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
        return BOOST_INTERLOCKED_DECREMENT( val );
#else
        return --( *val );
#endif
    }

    long Thread::interlockedIncrement( volatile long *val )
    {
#if defined( FB_USE_INTERLOCKED_FUNCTIONS ) && defined( FB_USE_BOOST )
        return BOOST_INTERLOCKED_INCREMENT( val );
#else
        return ++( *val );
#endif
    }

    void Thread::yield()
    {
#if FB_USE_TBB
        tbb::this_tbb_thread::yield();
#elif FB_USE_ONETBB
        std::this_thread::yield();
#else
        std::this_thread::yield();
#endif
    }

    Thread::ThreadId Thread::getCurrentThreadId()
    {
        return static_cast<ThreadId>( CURRENT_THREAD_ID );
    }

    void Thread::setCurrentThreadId( ThreadId threadId )
    {
        CURRENT_THREAD_ID = static_cast<u32>( threadId );
    }

    u32 Thread::hardware_concurrency()
    {
#if FB_USE_BOOST
        return boost::thread::hardware_concurrency();
#elif FB_USE_TBB
        return tbb::tbb_thread::hardware_concurrency();
#else
        return 0;
#endif
    }

    u32 Thread::physical_concurrency()
    {
#if FB_USE_BOOST
        return boost::thread::physical_concurrency();
#elif FB_USE_TBB
        return tbb::tbb_thread::hardware_concurrency();
#else
        return 0;
#endif
    }

    String Thread::getTaskName( Task id )
    {
        switch( id )
        {
        case Task::Primary:
            return String( "PRIMARY" );
        case Task::Render:
            return String( "RENDER" );
        case Task::Physics:
            return String( "PHYSICS" );
        case Task::Controls:
            return String( "CONTROLS" );
        case Task::Collision:
            return String( "COLLISION" );
        case Task::Application:
            return String( "APPLICATION" );
        // case Thread::Task::Scene:
        //	return String("SCENE");
        // case Thread::Task::Water:
        //	return String("WATER");
        case Task::Ai:
            return String( "AI" );
        case Task::Animation:
            return String( "ANIMATION" );
        // case Thread::Task::Video:
        //	return String("VIDEO");
        case Task::Sound:
            return String( "SOUND" );
        default:
        {
        }
        }

        return String( "UNKNOWN" );
    }
}  // end namespace fb
