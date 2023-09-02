#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/FiniteStateMachine.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, FiniteStateMachine, Component );

        FiniteStateMachine::FiniteStateMachine()
        {
        }

        FiniteStateMachine::~FiniteStateMachine()
        {
            unload( nullptr );
        }

        void FiniteStateMachine::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Component::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto fsmManager = sceneManager->getFsmManager();
                FB_ASSERT( fsmManager );

                m_fsm = fsmManager->createFSM();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void FiniteStateMachine::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto fsmManager = applicationManager->getFsmManager();

                    if( fsmManager )
                    {
                        if( m_fsm )
                        {
                            // if( m_componentFsmListener )
                            //{
                            //     m_componentFSM->removeListener( m_componentFsmListener );
                            //     m_componentFsmListener = nullptr;
                            // }

                            fsmManager->destroyFSM( m_fsm );
                            m_fsm = nullptr;
                        }
                    }

                    Component::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void FiniteStateMachine::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
                unload( data );
                load( data );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<ISharedObject>> FiniteStateMachine::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 1 );

            objects.push_back( m_fsm );
            return objects;
        }

        SmartPtr<Properties> FiniteStateMachine::getProperties() const
        {
            auto properties = Component::getProperties();

            if( m_fsm )
            {
                s32 newState = m_fsm->getNewState();
                properties->setProperty( "newState", newState );

                s32 previousState = m_fsm->getPreviousState();
                properties->setProperty( "previousState", previousState );

                s32 currentState = m_fsm->getCurrentState();
                properties->setProperty( "currentState", currentState );
            }

            return properties;
        }

        void FiniteStateMachine::setProperties( SmartPtr<Properties> properties )
        {
            if( m_fsm )
            {
                s32 newState = m_fsm->getNewState();
                properties->getPropertyValue( "newState", newState );

                s32 previousState = m_fsm->getPreviousState();
                properties->getPropertyValue( "previousState", previousState );

                s32 currentState = m_fsm->getCurrentState();
                properties->getPropertyValue( "currentState", currentState );

                if( m_fsm->getNewState() != newState )
                {
                    m_fsm->setNewState( newState );
                }
            }
        }
    }  // namespace scene
}  // end namespace fb
