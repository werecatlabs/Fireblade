#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UserComponent.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Script/ScriptInvoker.h>
#include <FBCore/Script/ScriptEvent.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/Script/IScriptManager.h>
#include <FBCore/Interface/Script/IScriptClass.h>
#include <FBCore/Interface/Script/IScriptData.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, UserComponent, Component );

        const hash_type UserComponent::UPDATE_HASH = StringUtil::getHash( "update" );

        UserComponent::UserComponent()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );
            FB_ASSERT( factoryManager->isValid() );

            auto invoker = fb::make_ptr<ScriptInvoker>( this );
            setInvoker( invoker );

            auto receiver = fb::make_ptr<ScriptReceiver>( this );
            setReceiver( receiver );

            auto updateEvent = fb::make_ptr<ScriptEvent>();
            updateEvent->setFunction( "update" );
            invoker->setEventFunction( UPDATE_HASH, updateEvent );

            //createObject( const String &className, SmartPtr<ISharedObject> object )
        }

        UserComponent::~UserComponent()
        {
            unload( nullptr );
        }

        void UserComponent::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                /*
                if( auto actor = getActor() )
                {
                    auto entity = static_cast<entt::registry::entity_type>( actor->getEntity() );

                    auto registry = sceneManager->getRegistry();
                    auto &component = registry->emplace<usercomponent_data>( entity );
                    component.owner = this;

                    setDataPtr( &component );
                }
                */

                createScriptData();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UserComponent::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                if( auto scriptManager = applicationManager->getScriptManager() )
                {
                    scriptManager->destroyObject( this );
                }

                setInvoker( nullptr );
                setReceiver( nullptr );

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                /*
                if( auto actor = getActor() )
                {
                    auto entity = static_cast<entt::registry::entity_type>( actor->getEntity() );

                    auto registry = sceneManager->getRegistry();
                    registry->remove<usercomponent_data>( entity );
                }
                */

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UserComponent::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UserComponent::updateComponents()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

            /*
            auto registry = sceneManager->getRegistry();

            auto view = registry->view<usercomponent_data>();

            view.each( []( const auto entity, auto &t ) {
                auto &data = static_cast<usercomponent_data &>( t );
                data.owner->update();
            } );
            */
        }

        void UserComponent::update()
        {
            auto state = getState();
            if( state == State::Play || getUpdateInEditMode() )
            {
                if( auto invoker = getInvoker() )
                {
                    invoker->event( UPDATE_HASH );
                }
            }
        }

        SmartPtr<IScriptInvoker> UserComponent::getInvoker() const
        {
            return m_invoker;
        }

        void UserComponent::setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_invoker = invoker;
        }

        SmartPtr<IScriptReceiver> UserComponent::getReceiver() const
        {
            return m_receiver;
        }

        void UserComponent::setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_receiver = receiver;
        }

        SmartPtr<Properties> UserComponent::getProperties() const
        {
            auto properties = Component::getProperties();
            properties->setProperty( "className", m_className );
            properties->setProperty( "updateInEditMode", m_updateInEditMode );
            return properties;
        }

        void UserComponent::setProperties( SmartPtr<Properties> properties )
        {
            auto className = String( "" );
            properties->getPropertyValue( "className", className );
            properties->getPropertyValue( "updateInEditMode", m_updateInEditMode );
            m_className = className;
        }

        bool UserComponent::getUpdateInEditMode() const
        {
            return m_updateInEditMode;
        }

        void UserComponent::setUpdateInEditMode( bool updateInEditMode )
        {
            m_updateInEditMode = updateInEditMode;
        }

        SmartPtr<IScriptClass> UserComponent::getScriptClass() const
        {
            return m_scriptClass;
        }

        void UserComponent::setScriptClass( SmartPtr<IScriptClass> scriptClass )
        {
            m_scriptClass = scriptClass;
        }

        String UserComponent::getClassName() const
        {
            return m_className;
        }

        void UserComponent::setClassName( const String &className )
        {
            m_className = className;
        }

        IFSM::ReturnType UserComponent::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            Component::handleComponentEvent( state, eventType );

            switch( eventType )
            {
            case IFSM::Event::Change:
            {
            }
            break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Destroyed:
                {
                }
                break;
                case State::Edit:
                case State::Play:
                {
                    createScriptData();
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Edit:
                {
                }
                break;
                case State::Play:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );
                    FB_ASSERT( applicationManager->isValid() );

                    auto scriptManager = applicationManager->getScriptManager();
                    FB_ASSERT( scriptManager );
                    FB_ASSERT( scriptManager->isValid() );

                    scriptManager->destroyObject( this );
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Pending:
            {
            }
            break;
            case IFSM::Event::Complete:
            {
            }
            break;
            case IFSM::Event::NewState:
            {
            }
            break;
            case IFSM::Event::WaitForChange:
            {
            }
            break;
            default:
            {
            }
            break;
            }

            return IFSM::ReturnType::Ok;
        }

        void UserComponent::createScriptData()
        {
            if( !getScriptData() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                auto scriptManager = applicationManager->getScriptManager();
                FB_ASSERT( scriptManager );
                FB_ASSERT( scriptManager->isValid() );

                if( !StringUtil::isNullOrEmpty( m_className ) )
                {
                    scriptManager->createObject( m_className, this );
                }
            }
        }

        UserComponent::ScriptReceiver::ScriptReceiver()
        {
        }

        UserComponent::ScriptReceiver::ScriptReceiver( UserComponent *owner ) : m_owner( owner )
        {
        }

        UserComponent::ScriptReceiver::~ScriptReceiver()
        {
        }

        s32 UserComponent::ScriptReceiver::setProperty( hash_type hash, void *param )
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::setProperty( hash_type hash, const Parameters &params )
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::setProperty( hash_type hash, const Parameter &param )
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::setProperty( hash_type hash, const String &value )
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::getProperty( hash_type hash, void *param ) const
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::getProperty( hash_type hash, Parameters &params ) const
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::getProperty( hash_type hash, Parameter &param ) const
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::getProperty( hash_type hash, String &value ) const
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::callFunction( hash_type hash, SmartPtr<ISharedObject> object,
                                                         Parameters &results )
        {
            return 0;
        }

        s32 UserComponent::ScriptReceiver::callFunction( hash_type hash, const Parameters &params,
                                                         Parameters &results )
        {
            return 0;
        }

        UserComponent *UserComponent::ScriptReceiver::getOwner() const
        {
            return m_owner;
        }

        void UserComponent::ScriptReceiver::setOwner( UserComponent *owner )
        {
            m_owner = owner;
        }
    }  // namespace scene
}  // end namespace fb
