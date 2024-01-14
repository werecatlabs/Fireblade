#ifndef __IState_h__
#define __IState_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{
    /**
     * @brief Interface for a state using the state context pattern.
     *
     * Implementations of this interface can specify behavior.
     */
    class IState : public ISharedObject
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        ~IState() override = default;

        /**
         * @brief Returns the time of this state.
         *
         * @return The time of this state as a time interval.
         */
        virtual time_interval getTime() const = 0;

        /**
         * @brief Sets the time of this state.
         *
         * @param time The time of this state as a time interval.
         */
        virtual void setTime( time_interval time ) = 0;

        /**
         * @brief Checks if the state object needs updating.
         *
         * @return True if the state object is dirty and needs updating, false otherwise.
         */
        virtual bool isDirty() const = 0;

        /**
         * @brief Sets the state object to be dirty and in need of updating.
         *
         * @param dirty True if the state object is dirty and needs updating, false otherwise.
         */
        virtual void setDirty( bool dirty ) = 0;

        /**
         * @brief Returns the owner state context.
         *
         * @return The owner state context as a shared pointer to IStateContext.
         */
        virtual SmartPtr<IStateContext> &getStateContext() = 0;

        /**
         * @brief Returns the owner state context.
         *
         * @return The owner state context as a shared pointer to IStateContext.
         */
        virtual const SmartPtr<IStateContext> &getStateContext() const = 0;

        /**
         * @brief Sets the owner state context.
         *
         * @param stateContext The owner state context as a shared pointer to IStateContext.
         */
        virtual void setStateContext( SmartPtr<IStateContext> stateContext ) = 0;

        /**
         * @brief Returns the task used to update the state.
         *
         * @return The task used to update the state as a Thread::Task.
         */
        virtual Thread::Task getTaskId() const = 0;

        /**
         * @brief Sets the task used to update the state.
         *
         * @param task The task used to update the state as a Thread::Task.
         */
        virtual void setTaskId( Thread::Task task ) = 0;

        /**
         * @brief Returns the data as a Properties object.
         *
         * @return The data as a shared pointer to Properties.
         */
        virtual SmartPtr<Properties> getProperties() const = 0;

        /**
         * @brief Sets the data as a Properties object.
         *
         * @param properties The data as a shared pointer to Properties.
         */
        virtual void setProperties( SmartPtr<Properties> properties ) = 0;

        /**
         * @brief Clones the state object.
         *
         * @return A new state object that is a clone of this one, as a shared pointer to IState.
         */
        virtual SmartPtr<IState> clone() const = 0;

        /**
         * @brief Assigns values from another state object.
         *
         * @param state The state object to assign values from, as a shared pointer to IState.
         */
        virtual void assign( SmartPtr<IState> state ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IState_h__
