#ifndef FBPhysxSharedObject_h__
#define FBPhysxSharedObject_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/ApplicationManager.h>
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
                auto applicationManager = core::ApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();
                auto physicsTask = physicsManager->getPhysicsTask();

                auto task = Thread::getCurrentTask();

                const auto &loadingState = T::getLoadingState();

                return loadingState == LoadingState::Loaded && task == physicsTask;
            }

            void addMessage( SmartPtr<IStateMessage> message )
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();

                if( auto stateContext = PhysxSharedObject<T>::getStateContext() )
                {
                    const auto stateTask = physicsManager->getStateTask();
                    stateContext->addMessage( stateTask, message );
                }
            }

            /**
             * @brief Gets the state object associated with this scene node.
             * @return The state object.
             */
            SmartPtr<IStateContext> getStateContext() const;

            /**
             * @brief Sets the state object associated with this scene node.
             * @param stateContext The state object.
             */
            void setStateContext( SmartPtr<IStateContext> stateContext );

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
                auto applicationManager = core::ApplicationManager::instance();

                if( auto stateManager = applicationManager->getStateManager() )
                {
                    if( auto stateContext = getStateContext() )
                    {
                        if( auto stateListener = PhysxSharedObject<T>::getStateListener() )
                        {
                            stateContext->removeStateListener( stateListener );
                        }

                        stateContext->unload( nullptr );

                        if( stateManager )
                        {
                            stateManager->removeStateObject( stateContext );
                        }

                        setStateContext( nullptr );
                    }

                    if( auto stateListener = PhysxSharedObject<T>::getStateListener() )
                    {
                        stateListener->unload( nullptr );
                        PhysxSharedObject<T>::setStateListener( nullptr );
                    }
                }
            }

            /**< The state object associated with this scene node. */
            AtomicSmartPtr<IStateContext> m_stateContext;

            /**< The state listener associated with this scene node. */
            AtomicSmartPtr<IStateListener> m_stateListener;
        };

        template <typename T>
        SmartPtr<IStateContext> PhysxSharedObject<T>::getStateContext() const
        {
            return m_stateContext;
        }

        template <typename T>
        void PhysxSharedObject<T>::setStateContext( SmartPtr<IStateContext> stateContext )
        {
            m_stateContext = stateContext;
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
