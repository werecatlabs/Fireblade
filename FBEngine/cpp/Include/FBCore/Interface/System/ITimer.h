#ifndef __ITimer_h__
#define __ITimer_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{
    /**
     * @brief Interface for a timer class.
     *
     * This class defines an interface for a timer, including functions for updating the timer,
     * retrieving and setting the current time and time intervals, getting the system time,
     * and resetting the timer.
     */
    class ITimer : public ISharedObject
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        ~ITimer() override = default;

        /**
         * @brief Updates the timer with a fixed time interval.
         *
         * This function should be called on a fixed interval in order to update the timer.
         */
        virtual void updateFixed() = 0;

        /**
         * @brief Gets the time since level load in seconds.
         * @return A time interval value.
         */
        virtual f64 getTimeSinceLevelLoad() = 0;

        /**
         * @brief Sets the time since level load in seconds.
         * @param t A time interval value.
         */
        virtual void setTimeSinceLevelLoad( time_interval t ) = 0;

        /**
         * @brief Adds to the time since level load in seconds.
         * @param deltaTime A time interval value.
         */
        virtual void addTimeSinceLevelLoad( time_interval deltaTime ) = 0;

        /**
         * @brief Gets the current smooth time in seconds.
         * @return A double value.
         */
        virtual double getSmoothTime() const = 0;

        /**
         * @brief Gets the current smooth delta time in seconds.
         * @return A double value.
         */
        virtual double getSmoothDeltaTime() const = 0;

        /**
         * @brief Sets the smoothing period for delta time.
         * @param smoothDT The smoothing period in milliseconds.
         */
        virtual void setSmoothDeltaTime( double smoothDT ) = 0;

        /**
         * @brief Returns the current virtual time in milliseconds.
         *
         * This value starts with 0 and can be manipulated using setTime(), stopTimer(),
         * startTimer(), etc. This value depends on the set speed of the timer if the timer
         * is stopped, etc. If you need the system time, use getRealTime().
         * @return A 32-bit unsigned integer value.
         */
        virtual u32 getTimeMilliseconds() const = 0;

        /**
         * @brief Gets the system time in milliseconds.
         * @return A 32-bit unsigned integer value.
         */
        virtual u32 getRealTime() const = 0;

        /**
         * @brief Returns the time interval in milliseconds between ticks.
         * @return A 32-bit unsigned integer value.
         */
        virtual u32 getTimeIntervalMilliseconds() const = 0;

        /**
         * @brief Gets the current time in seconds.
         * @return A time interval value.
         */
        virtual time_interval now() const = 0;

        /**
         * @brief Gets the current time in seconds.
         * @return A time interval value.
         */
        virtual time_interval getTime() const = 0;

        /**
         * @brief Gets the current delta time in seconds.
         * @return A double value.
         */
        virtual double getDeltaTime() const = 0;

        /**
         * @brief Gets the current time interval in seconds.
         * @return A time interval value.
         */
        virtual time_interval getTimeInterval() const = 0;

        /**
         * @brief Sets the frame smoothing period.
         * @param milliSeconds The smoothing period in milliseconds.
         */
        virtual void setFrameSmoothingPeriod( u32 milliSeconds ) = 0;

        /**
         * @brief Gets the frame smoothing period.
         * @return The frame smoothing period in milliseconds as an unsigned 32-bit integer.
         */
        virtual u32 getFrameSmoothingPeriod() const = 0;

        /**
         * @brief Resets the frame smoothing.
         *
         * This function resets the frame smoothing, clearing any accumulated values.
         */
        virtual void resetSmoothing() = 0;

        /**
         * @brief Gets the tick count for the specified task.
         * @return The tick count as an unsigned 32-bit integer.
         */
        virtual u32 getTickCount() = 0;

        /**
         * @brief Gets the tick count for the specified task.
         * @param task The task to get the tick count for.
         * @return The tick count as an unsigned 32-bit integer.
         */
        virtual u32 getTickCount( Thread::Task task ) = 0;

        /**
         * @brief Gets the fixed time interval for the specified task.
         * @param task The task to get the fixed time interval for.
         * @return The fixed time interval as a double-precision floating point value.
         */
        virtual f64 getFixedTimeInterval( Thread::Task task ) const = 0;

        /**
         * @brief Gets the fixed time interval.
         * @return The fixed time interval as a double-precision floating point value.
         */
        virtual f64 getFixedTimeInterval() const = 0;

        /**
         * @brief Sets the fixed time interval.
         * @param val The fixed time interval as a double-precision floating point value.
         */
        virtual void setFixedTimeInterval( f64 val ) = 0;

        /**
         * @brief Sets the fixed time interval for the specified task.
         * @param task The task to set the fixed time interval for.
         * @param val The fixed time interval as a double-precision floating point value.
         */
        virtual void setFixedTimeInterval( Thread::Task task, f64 val ) = 0;

        /**
         * @brief Gets the time for the specified task.
         * @param task The task to get the time for.
         * @return The time as a double-precision floating point value.
         */
        virtual f64 getTime( Thread::Task task ) const = 0;

        /**
         * @brief Gets the previous time for the specified task.
         * @param task The task to get the previous time for.
         * @return The previous time as a double-precision floating point value.
         */
        virtual f64 getPreviousTime( Thread::Task task ) const = 0;

        /**
         * @brief Gets the delta time for the specified task.
         * @param task The task to get the delta time for.
         * @return The delta time as a double-precision floating point value.
         */
        virtual f64 getDeltaTime( Thread::Task task ) const = 0;

        /**
         * @brief Gets the smooth delta time for the specified task.
         * @param task The task to get the smooth delta time for.
         * @return The smooth delta time as a double-precision floating point value.
         */
        virtual f64 getSmoothDeltaTime( Thread::Task task ) const = 0;

        /**
         * @brief Gets the start offset.
         * @return The start offset as a double-precision floating point value.
         */
        virtual f64 getStartOffset() const = 0;

        /**
         * @brief Sets the start offset.
         * @param val The start offset as a double-precision floating point value.
         */
        virtual void setStartOffset( f64 val ) = 0;

        /**
         * @brief Gets the start offset for the specified task.
         * @param task The task to get the start offset for.
         * @return The start offset as a double-precision floating point value.
         */
        virtual f64 getStartOffset( Thread::Task task ) const = 0;

        /**
         * @brief Sets the start offset for the specified task.
         * @param task The task to set the start offset for.
         * @param val The start offset as a double-precision
         */
        virtual void setStartOffset( Thread::Task task, f64 val ) = 0;

        /**
         * @brief Checks if the timer is steady.
         * @return True if the timer is steady, false otherwise.
         */
        virtual bool isSteady() const = 0;

        /**
         * @brief Resets the timer.
         *
         * This function resets the timer, clearing any accumulated values and setting the start time to the current time.
         */
        virtual void reset() = 0;

        /**
         * @brief Resets the timer with a specified start time.
         * @param t The start time as a double-precision floating point value.
         */
        virtual void reset( f64 t ) = 0;

        /**
         * @brief Gets the fixed time.
         * @return The fixed time as a double-precision floating point value.
         */
        virtual f64 getFixedTime() const = 0;

        /**
         * @brief Gets the fixed time for the specified task.
         * @param task The task to get the fixed time for.
         * @return The fixed time as a double-precision floating point value.
         */
        virtual f64 getFixedTime( u32 task ) const = 0;

        /**
         * @brief Gets the fixed offset for the specified task.
         * @param task The task to get the fixed offset for.
         * @return The fixed offset as a double-precision floating point value.
         */
        virtual f64 getFixedOffset( Thread::Task task ) const = 0;

        /**
         * @brief Sets the fixed offset for the specified task.
         * @param task The task to set the fixed offset for.
         * @param val The fixed offset as a double-precision floating point value.
         */
        virtual void setFixedOffset( Thread::Task task, f64 offset ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // FBTimer_h__
