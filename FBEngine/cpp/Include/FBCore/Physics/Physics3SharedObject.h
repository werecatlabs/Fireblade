#ifndef FBPhysics3SharedObject_h__
#define FBPhysics3SharedObject_h__

#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IState.h>

namespace fb
{
    namespace physics
    {

        template <typename T>
        class Physics3SharedObject : public SharedObject<T>
        {
        public:
            Physics3SharedObject() = default;
            ~Physics3SharedObject() = default;

            void unload( SmartPtr<ISharedObject> data )
            {
                destroyStateContext();
            }

            bool isThreadSafe() const
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();
                auto physicsTask = physicsManager->getPhysicsTask();

                auto task = Thread::getCurrentTask();

                const auto &loadingState = SharedObject<T>::getLoadingState();

                return loadingState == LoadingState::Loaded && task == physicsTask;
            }

            void addMessage( SmartPtr<IStateMessage> message )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();

                if( auto stateObject = Physics3SharedObject<T>::getStateObject() )
                {
                    const auto stateTask = physicsManager->getStateTask();
                    stateObject->addMessage( stateTask, message );
                }
            }

            /**
             * @brief Gets the state object associated with this scene node.
             * @return The state object.
             */
            SmartPtr<IStateContext> getStateObject() const;

            /**
             * @brief Sets the state object associated with this scene node.
             * @param stateObject The state object.
             */
            void setStateObject( SmartPtr<IStateContext> stateObject );

            /**
             * @brief Gets the state listener associated with this scene node.
             * @return The state listener.
             */
            SmartPtr<IStateListener> getStateListener() const;

            /**
             * @brief Sets the state listener associated with this scene node.
             * @param stateListener The state listener.
             */
            void setStateListener( SmartPtr<IStateListener> stateListener );

            FB_CLASS_REGISTER_TEMPLATE_DECL( Physics3SharedObject, T );

        protected:
            void destroyStateContext()
            {
                auto applicationManager = core::IApplicationManager::instance();

                if( auto stateManager = applicationManager->getStateManager() )
                {
                    if( auto stateObject = getStateObject() )
                    {
                        if( auto state = stateObject->getState() )
                        {
                            state->unload( nullptr );
                            stateObject->setState( nullptr );
                        }

                        if( auto stateListener = Physics3SharedObject<T>::getStateListener() )
                        {
                            stateObject->removeStateListener( stateListener );
                        }

                        stateObject->unload( nullptr );

                        if( stateManager )
                        {
                            stateManager->removeStateObject( stateObject );
                        }

                        setStateObject( nullptr );
                    }

                    if( auto stateListener = Physics3SharedObject<T>::getStateListener() )
                    {
                        stateListener->unload( nullptr );
                        Physics3SharedObject<T>::setStateListener( nullptr );
                    }
                }
            }

            /**< The state object associated with this scene node. */
            AtomicSmartPtr<IStateContext> m_stateObject;

            /**< The state listener associated with this scene node. */
            AtomicSmartPtr<IStateListener> m_stateListener;
        };

        template <typename T>
        SmartPtr<IStateContext> Physics3SharedObject<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <typename T>
        void Physics3SharedObject<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <typename T>
        SmartPtr<IStateListener> Physics3SharedObject<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <typename T>
        void Physics3SharedObject<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::physics, Physics3SharedObject, T, SharedObject<T> );

    }  // namespace physics
}  // namespace fb


#endif // FBPhysxSharedObject_h__
