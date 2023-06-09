#ifndef CSharedGraphicsObject_h__
#define CSharedGraphicsObject_h__

#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IState.h>

namespace fb
{
    namespace render
    {
        template <typename T>
        class CSharedGraphicsObject : public SharedObject<T>
        {
        public:
            CSharedGraphicsObject() = default;
            ~CSharedGraphicsObject() = default;

            void unload( SmartPtr<ISharedObject> data )
            {
                destroyStateContext();
            }

            bool isThreadSafe() const
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto renderTask = graphicsSystem->getRenderTask();

                auto task = Thread::getCurrentTask();

                const auto &loadingState = SharedObject<T>::getLoadingState();

                return loadingState == LoadingState::Loaded && task == renderTask;
            }

            void addMessage( SmartPtr<IStateMessage> message )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                if( auto stateObject = CSharedGraphicsObject<T>::getStateObject() )
                {
                    const auto stateTask = graphicsSystem->getStateTask();
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

            FB_CLASS_REGISTER_TEMPLATE_DECL( CSharedGraphicsObject, T );

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

                        if( auto stateListener = CSharedGraphicsObject<T>::getStateListener() )
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

                    if( auto stateListener = CSharedGraphicsObject<T>::getStateListener() )
                    {
                        stateListener->unload( nullptr );
                        CSharedGraphicsObject<T>::setStateListener( nullptr );
                    }
                }
            }

            /**< The state object associated with this scene node. */
            AtomicSmartPtr<IStateContext> m_stateObject;

            /**< The state listener associated with this scene node. */
            AtomicSmartPtr<IStateListener> m_stateListener;
        };

        template <typename T>
        SmartPtr<IStateContext> CSharedGraphicsObject<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <typename T>
        void CSharedGraphicsObject<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <typename T>
        SmartPtr<IStateListener> CSharedGraphicsObject<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <typename T>
        void CSharedGraphicsObject<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::render, CSharedGraphicsObject, T, SharedObject<T> );

    }  // namespace render
}  // namespace fb

#endif  // CSharedGraphicsObject_h__
