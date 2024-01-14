#ifndef __FBThreadTypes_H__
#define __FBThreadTypes_H__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Thread/SpinRWMutex.h>

namespace fb
{
    /**
     * @brief The id of the current task that is being processed.
     */
    extern FB_THREAD_LOCAL_STORAGE s32 CURRENT_TASK_ID;

    /**
     * @brief An engine id of the thread that is currently executing.
     */
    extern FB_THREAD_LOCAL_STORAGE u32 CURRENT_THREAD_ID;

    /**
     * @brief Flags associated with the current executing thread.
     */
    extern FB_THREAD_LOCAL_STORAGE u32 CURRENT_THREAD_FLAGS;

    /**
     * @brief The id of the current update event.
     */
    extern FB_THREAD_LOCAL_STORAGE u32 CURRENT_UPDATE_EVENT;

    /**
     * @brief A class to provide system thread functions.
     */
    class FBCore_API Thread
    {
    public:
        /**
         * @brief Thread IDs used within the FBCore API.
         */
        enum class ThreadId
        {
            Primary = -1,
            WorkerThread = 0,
            WorkerThreadMax = WorkerThread + 50,

            Count
        };

        /**
         * @brief Different tasks that can be executed within a thread.
         */
        enum class Task
        {
            Primary = 0,
            Ai,
            Animation,
            Application,
            Collision,
            Controls,
            Dynamics,
            GarbageCollect,
            Physics,
            None,
            Render,
            Sound,

            Count
        };

        /**
         * @brief Different states during the update process.
         */
        enum class UpdateState
        {
            PreUpdate,
            Update,
            PostUpdate,
            Transform,

            Count
        };

        static const u32 Primary_Flag;
        static const u32 Ai_Flag;
        static const u32 Animation_Flag;
        static const u32 Application_Flag;
        static const u32 Collision_Flag;
        static const u32 Controls_Flag;
        static const u32 Dynamics_Flag;
        static const u32 GarbageCollect_Flag;
        static const u32 Physics_Flag;
        static const u32 None_Flag;
        static const u32 Render_Flag;
        static const u32 Sound_Flag;

        /**
         * @brief Get the current task being executed by the thread.
         * @return The current task.
         */
        static Task getCurrentTask();

        /**
         * @brief Set the current task for the thread.
         * @param task The task to set.
         */
        static void setCurrentTask( Task task );

        static u32 getTaskFlags();
        static void setTaskFlags( u32 taskFlags );

        static u32 getTaskFlags( Task task );
        static void setTaskFlags( Task task, u32 taskFlags );

        static bool getTaskFlag( u32 flag );
        static void setTaskFlag( u32 flag, bool value );

        /**
         * @brief Get the ID of the current executing thread.
         * @return The thread ID.
         */
        static ThreadId getCurrentThreadId();

        /**
         * @brief Set the ID of the current executing thread.
         * @param threadId The thread ID to set.
         */
        static void setCurrentThreadId( ThreadId threadId );

        /**
         * @brief Get the number of hardware threads available on the system.
         * @return The number of hardware threads.
         */
        static u32 hardware_concurrency();

        /**
         * @brief Get the number of physical threads available on the system.
         * @return The number of physical threads.
         */
        static u32 physical_concurrency();

        /**
         * @brief Make the current thread sleep for the specified duration.
         * @param seconds The duration to sleep in seconds.
         */
        static void sleep( time_interval seconds );

        /**
         * @brief Yield the current thread's execution to allow other threads to run.
         */
        static void yield();

        /**
         * @brief Atomically exchange the value of a pointer.
         * @param target The target pointer to exchange.
         * @param value The value to set.
         */
        static void interlockedExchangePointer( void **target, void *value );

        /**
         * @brief Atomically exchange the value of a variable.
         * @param target The target variable to exchange.
         * @param value The value to set.
         */
        static void interlockedExchange( volatile long *target, long value );

        /**
         * @brief Increment the value of a variable atomically.
         * @param val The variable to increment.
         * @return The new value of the variable after incrementing.
         */
        static long interlockedIncrement( volatile long *val );

        /**
         * @brief Decrement the value of a variable atomically.
         * @param val The variable to decrement.
         * @return The new value of the variable after decrementing.
         */
        static long interlockedDecrement( volatile long *val );

        /**
         * @brief Atomically compare the value of a variable with an expected value, and if they are equal, replace the value.
         * @param a The target variable.
         * @param b The expected value.
         * @param c The new value to set if the comparison is successful.
         * @return The original value of the variable before the comparison.
         */
        static long interlockedCompareExchange( volatile long *a, long b, long c );

        /**
         * @brief Get the name associated with a specific task ID.
         * @param id The task ID.
         * @return The name of the task.
         */
        static String getTaskName( Task id );

    private:
        static FixedArray<u32, static_cast<u32>( Task::Count )> m_taskFlags;
        static SpinRWMutex m_taskFlagsMutex;
    };
}  // end namespace fb

#endif
