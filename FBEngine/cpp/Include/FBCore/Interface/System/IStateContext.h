#ifndef _IStateObject_H_
#define _IStateObject_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/Parameter.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{
    /**
     * @brief Interface for an object that contains state information. Inherits from ISharedObject.
     */
    class IStateContext : public ISharedObject
    {
    public:
        /**
         * @brief Destroy the IStateContext object.
         */
        ~IStateContext() override = default;

        /**
         * @brief Set the owner of the state context.
         *
         * @param owner A smart pointer to the ISharedObject representing the owner.
         */
        virtual void setOwner( SmartPtr<ISharedObject> owner ) = 0;

        /**
         * @brief Get the owner of the state context.
         *
         * @return SmartPtr<ISharedObject> A smart pointer to the ISharedObject representing the owner.
         */
        virtual SmartPtr<ISharedObject> getOwner() const = 0;

        /**
         * @brief Check if the state object needs updating.
         *
         * @return bool True if the state object is dirty and needs updating, false otherwise.
         */
        virtual bool isDirty() const = 0;

        /**
         * @brief Mark the state object as dirty or clean, indicating if it needs updating.
         *
         * @param dirty True if the state object needs updating, false otherwise.
         * @param cascade Optional boolean parameter indicating if the change should cascade to related objects. Defaults to true.
         */
        virtual void setDirty( bool dirty, bool cascade = true ) = 0;

        /**
         * @brief Add a message to the state context's message queue.
         *
         * @param taskId The identifier for the associated task.
         * @param message A smart pointer to the IStateMessage to be added.
         */
        virtual void addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) = 0;

        /**
         * @brief Add a state listener to the state context.
         *
         * @param stateListner A smart pointer to the IStateListener to be added.
         */
        virtual void addStateListener( SmartPtr<IStateListener> stateListner ) = 0;

        /**
         * @brief Remove a state listener from the state context.
         *
         * @param stateListner A smart pointer to the IStateListener to be removed.
         * @return bool True if the listener is successfully removed, false otherwise.
         */
        virtual bool removeStateListener( SmartPtr<IStateListener> stateListner ) = 0;

        /**
         * @brief Get a list of the state context's state listeners.
         *
         * @return SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> A shared pointer to a concurrent array of the state listeners.
         */
        virtual SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> getStateListeners() const = 0;

        /**
         * @brief Add an event listener to the state context.
         *
         * @param eventListener A smart pointer to the IEventListener to be added.
         */
        virtual void addEventListener( SmartPtr<IEventListener> eventListener ) = 0;

        /**
         * @brief Remove an event listener from the state context.
         *
         * @param eventListener A smart pointer to the IEventListener to be removed.
         * @return bool True if the listener is successfully removed, false otherwise.
         */
        virtual bool removeEventListener( SmartPtr<IEventListener> eventListener ) = 0;

        /**
         * @brief Get a list of the state context's event listeners.
         *
         * @return SharedPtr<Array<SmartPtr<IEventListener>>> A shared pointer to an array of the event listeners
         */
        virtual SharedPtr<Array<SmartPtr<IEventListener>>> getEventListeners() const = 0;

        /** Gets the latest object. */
        virtual SmartPtr<IState> getLatestOutputState() const = 0;

        /** Gets the objects state at a time interval. */
        virtual SmartPtr<IState> getOutputState( time_interval time ) const = 0;

        /** Adds an object state. */
        virtual void addOutputState( SmartPtr<IState> state ) = 0;

        /** Gets the objects state. */
        virtual SmartPtr<IState> getState() const = 0;

        /** Sets the objects state. */
        virtual void setState( SmartPtr<IState> state ) = 0;

        /** Used to send messages via the listeners. */
        virtual void sendMessage( SmartPtr<IStateMessage> message ) = 0;

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        virtual SmartPtr<Properties> getProperties() const = 0;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        virtual void setProperties( SmartPtr<Properties> properties ) = 0;

        /** Gets the latest object. */
        template <class T>
        SmartPtr<T> getLatestOutputStateByType() const
        {
            auto state = getLatestOutputState();
            FB_ASSERT( fb::dynamic_pointer_cast<T>( state ) );
            return fb::static_pointer_cast<T>( state );
        }

        /** Gets the latest object. */
        template <class T>
        SmartPtr<T> getLatestOutputStateByType( time_interval time ) const
        {
            auto state = getOutputState( time );
            FB_ASSERT( fb::dynamic_pointer_cast<T>( state ) );
            return fb::static_pointer_cast<T>( state );
        }

        /** Gets the state by type. */
        template <class T>
        SmartPtr<T> getStateByType() const
        {
            auto state = getState();
            return fb::static_pointer_cast<T>( state );
        }

        /** Handles a event.
        @param eventType The event type.
        @param eventValue The event value.
        @param sender The object triggering the event. This can be null.
        @param event The event data. This can be null.
        @return Contains a return parameter. */
        virtual Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                        const Array<Parameter> &arguments,
                                        SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
