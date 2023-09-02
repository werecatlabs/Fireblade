#ifndef FBPhysxSharedObject_h__
#define FBPhysxSharedObject_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
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
        class PhysxSharedObject : public T
        {
        public:
            PhysxSharedObject() = default;
            ~PhysxSharedObject() = default;

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

                const auto &loadingState = T::getLoadingState();

                return loadingState == LoadingState::Loaded && task == physicsTask;
            }

            void addMessage( SmartPtr<IStateMessage> message )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();

                if( auto stateObject = PhysxSharedObject<T>::getStateObject() )
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

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysxSharedObject, T );

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

                        if( auto stateListener = PhysxSharedObject<T>::getStateListener() )
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

                    if( auto stateListener = PhysxSharedObject<T>::getStateListener() )
                    {
                        stateListener->unload( nullptr );
                        PhysxSharedObject<T>::setStateListener( nullptr );
                    }
                }
            }

            /**< The state object associated with this scene node. */
            AtomicSmartPtr<IStateContext> m_stateObject;

            /**< The state listener associated with this scene node. */
            AtomicSmartPtr<IStateListener> m_stateListener;
        };

        template <typename T>
        SmartPtr<IStateContext> PhysxSharedObject<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <typename T>
        void PhysxSharedObject<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <typename T>
        SmartPtr<IStateListener> PhysxSharedObject<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <typename T>
        void PhysxSharedObject<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::physics, PhysxSharedObject, T, T );

    }  // namespace physics
}  // namespace fb


#endif // FBPhysxSharedObject_h__
