#ifndef FB_IStateManager_H_
#define FB_IStateManager_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{

    /**
     * @brief Manages the state context objects.
     *
     * This class is responsible for managing state context objects. It provides various
     * methods for adding, removing, and finding state objects, getting state data,
     * and sending messages to all state objects.
     */
    class IStateManager : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IStateManager() override = default;

        /**
         * @brief Sends a message to all state objects.
         *
         * @param taskId The ID of the task.
         * @param message The message to send.
         */
        virtual void sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) = 0;

        /**
         * @brief Adds a state object to the manager.
         *
         * @return The added state object.
         */
        virtual SmartPtr<IStateContext> addStateObject() = 0;

        /**
         * @brief Adds an existing state object to the manager.
         *
         * @param context The state object to add.
         */
        virtual void addStateObject( SmartPtr<IStateContext> context ) = 0;

        /**
         * @brief Removes a state object from the manager.
         *
         * @param stateContext The state object to remove.
         * @return True if the state object was successfully removed, false otherwise.
         */
        virtual bool removeStateObject( SmartPtr<IStateContext> stateContext ) = 0;

        /**
         * @brief Finds a state object by ID.
         *
         * @param id The ID of the state object to find.
         * @return The found state object, or null if not found.
         */
        virtual SmartPtr<IStateContext> findStateObject( u32 id ) const = 0;

        /**
         * @brief Gets the state objects in the state manager.
         *
         * @return An array of state objects.
         */
        virtual Array<SmartPtr<IStateContext>> getStateContexts() const = 0;

        /**
         * @brief Gets a state queue by task.
         *
         * @param taskId The ID of the task.
         * @return The state queue for the given task.
         */
        virtual SmartPtr<IStateQueue> getQueue( Thread::Task taskId ) = 0;

        /**
         * @brief Adds a state to the manager.
         *
         * @param state The state to add.
         */
        virtual void addState( SmartPtr<IState> state ) = 0;

        /**
         * @brief Removes a state from the manager.
         *
         * @param state The state to remove.
         * @return True if the state was successfully removed, false otherwise.
         */
        virtual bool removeState( SmartPtr<IState> state ) = 0;

        /**
         * @brief Gets an array of states by type.
         * @return An array of states of the given type.
         */
        virtual Array<SmartPtr<IState>> getStates( u32 typeInfo ) = 0;

        /**
         * @brief Adds a dirty state context that needs updating.
         *
         * @param context The dirty state context to add.
         */
        virtual void addDirty( SmartPtr<IStateContext> context ) = 0;

        /**
         * @brief Adds a dirty state context that needs updating.
         *
         * @param context The dirty state context to add.
         * @param task The task to add the dirty state context to.
         */
        virtual void addDirty( SmartPtr<IStateContext> context, Thread::Task task ) = 0;

        /** Create a state context. */
        template <class T>
        SmartPtr<T> createStateContext()
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateContext = addStateObject();
            FB_ASSERT( stateContext );

            if( auto state = factoryManager->make_object<T>() )
            {
                state->setStateContext( stateContext );
                stateContext->addState( state );
                return state;
            }

            return nullptr;
        }

        /** Gets the state data */
        template <class T>
        Array<SmartPtr<T>> getStatesByType()
        {
            auto states = getStates( T::typeInfo() );
            return Array<SmartPtr<T>>( states.begin(), states.end() );
        }

        /** Gets the state data. */
        template <class T>
        SmartPtr<T> getStateDataByType() const
        {
            auto typeInfo = T::typeInfo();

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto id = typeManager->getHash( typeInfo );
            auto stateData = getStateDataById( id );
            return fb::static_pointer_cast<T>( stateData );
        }

        /** Sets the state data. */
        template <class T>
        void setStateDataByType( SmartPtr<T> data )
        {
            auto typeInfo = T::typeInfo();

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto id = typeManager->getHash( typeInfo );
            setStateDataById( id, data );
        }

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
