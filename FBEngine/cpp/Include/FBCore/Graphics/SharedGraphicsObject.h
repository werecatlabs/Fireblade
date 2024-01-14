#ifndef CSharedGraphicsObject_h__
#define CSharedGraphicsObject_h__

#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/System/ApplicationManager.h>
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
        class SharedGraphicsObject : public T
        {
        public:
            SharedGraphicsObject() = default;
            virtual ~SharedGraphicsObject() = default;

            void unload( SmartPtr<ISharedObject> data );

            bool isThreadSafe() const;

            void addMessage( SmartPtr<IStateMessage> message );

            /**
             * @brief Gets the state object associated with this scene node.
             * @return The state object.
             */
            SmartPtr<IStateContext> &getStateContext();
            const SmartPtr<IStateContext> &getStateContext() const;

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

            FB_CLASS_REGISTER_TEMPLATE_DECL( SharedGraphicsObject, T );

        protected:
            void destroyStateContext();

            /**< The state object associated with this scene node. */
            SmartPtr<IStateContext> m_stateContext;

            /**< The state listener associated with this scene node. */
            SmartPtr<IStateListener> m_stateListener;
        };

        template <typename T>
        void SharedGraphicsObject<T>::unload( SmartPtr<ISharedObject> data )
        {
            destroyStateContext();
        }

        template <typename T>
        bool SharedGraphicsObject<T>::isThreadSafe() const
        {
            auto taskFlags = Thread::getTaskFlags();
            return SharedGraphicsObject<T>::isLoaded() && ( ( taskFlags & Thread::Render_Flag ) != 0 );
        }

        template <typename T>
        void SharedGraphicsObject<T>::addMessage( SmartPtr<IStateMessage> message )
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( auto stateContext = SharedGraphicsObject<T>::getStateContext() )
            {
                const auto stateTask = graphicsSystem->getStateTask();
                stateContext->addMessage( stateTask, message );
            }
        }

        template <typename T>
        SmartPtr<IStateContext> &SharedGraphicsObject<T>::getStateContext()
        {
            return m_stateContext;
        }

        template <typename T>
        const SmartPtr<IStateContext> &SharedGraphicsObject<T>::getStateContext() const
        {
            return m_stateContext;
        }

        template <typename T>
        void SharedGraphicsObject<T>::setStateContext( SmartPtr<IStateContext> stateContext )
        {
            m_stateContext = stateContext;
        }

        template <typename T>
        SmartPtr<IStateListener> SharedGraphicsObject<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <typename T>
        void SharedGraphicsObject<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <typename T>
        void SharedGraphicsObject<T>::destroyStateContext()
        {
            auto applicationManager = core::ApplicationManager::instance();

            if( auto stateManager = applicationManager->getStateManager() )
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto stateListener = SharedGraphicsObject<T>::getStateListener() )
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

                if( auto stateListener = SharedGraphicsObject<T>::getStateListener() )
                {
                    stateListener->unload( nullptr );
                    SharedGraphicsObject<T>::setStateListener( nullptr );
                }
            }
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::render, SharedGraphicsObject, T, T );

    }  // namespace render
}  // namespace fb

#endif  // CSharedGraphicsObject_h__
