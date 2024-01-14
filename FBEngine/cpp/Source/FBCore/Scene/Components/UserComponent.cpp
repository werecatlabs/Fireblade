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

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, UserComponent, Component );

    const hash_type UserComponent::UPDATE_HASH = StringUtil::getHash( "update" );

    UserComponent::UserComponent()
    {
        auto applicationManager = core::ApplicationManager::instance();
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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

            if( auto scriptManager = applicationManager->getScriptManager() )
            {
                scriptManager->destroyObject( this );
            }

            setInvoker( nullptr );
            setReceiver( nullptr );

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
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pSceneManager = applicationManager->getSceneManager();
        auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
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

    auto UserComponent::getInvoker() const -> SmartPtr<IScriptInvoker>
    {
        return m_invoker;
    }

    void UserComponent::setInvoker( SmartPtr<IScriptInvoker> invoker )
    {
        m_invoker = invoker;
    }

    auto UserComponent::getReceiver() const -> SmartPtr<IScriptReceiver>
    {
        return m_receiver;
    }

    void UserComponent::setReceiver( SmartPtr<IScriptReceiver> receiver )
    {
        m_receiver = receiver;
    }

    auto UserComponent::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setProperty( "className", m_className );
        properties->setProperty( "updateInEditMode", m_updateInEditMode );
        return properties;
    }

    void UserComponent::setProperties( SmartPtr<Properties> properties )
    {
        auto className = String();
        properties->getPropertyValue( "className", className );
        properties->getPropertyValue( "updateInEditMode", m_updateInEditMode );

        if( className != m_className )
        {
            destroyScriptData();
            m_className = className;
            createScriptData();
        }
    }

    auto UserComponent::getUpdateInEditMode() const -> bool
    {
        return m_updateInEditMode;
    }

    void UserComponent::setUpdateInEditMode( bool updateInEditMode )
    {
        m_updateInEditMode = updateInEditMode;
    }

    auto UserComponent::getScriptClass() const -> SmartPtr<IScriptClass>
    {
        return m_scriptClass;
    }

    void UserComponent::setScriptClass( SmartPtr<IScriptClass> scriptClass )
    {
        m_scriptClass = scriptClass;
    }

    auto UserComponent::getClassName() const -> String
    {
        return m_className;
    }

    void UserComponent::setClassName( const String &className )
    {
        m_className = className;
    }

    auto UserComponent::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
                auto applicationManager = core::ApplicationManager::instance();
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
        auto scriptData = getScriptData();
        if( !scriptData )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto scriptManager = applicationManager->getScriptManager();
            FB_ASSERT( scriptManager );
            FB_ASSERT( scriptManager->isValid() );

            auto className = getClassName();
            if( !StringUtil::isNullOrEmpty( className ) )
            {
                scriptManager->createObject( className, this );
            }
        }
    }

    void UserComponent::destroyScriptData()
    {
        auto applicationManager = core::ApplicationManager::instance();

        if( auto scriptManager = applicationManager->getScriptManager() )
        {
            scriptManager->destroyObject( this );
        }
    }

    UserComponent::ScriptReceiver::ScriptReceiver() = default;

    UserComponent::ScriptReceiver::ScriptReceiver( UserComponent *owner ) : m_owner( owner )
    {
    }

    UserComponent::ScriptReceiver::~ScriptReceiver() = default;

    auto UserComponent::ScriptReceiver::setProperty( hash_type hash, void *param ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::setProperty( hash_type hash, const Parameters &params ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::setProperty( hash_type hash, const Parameter &param ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::setProperty( hash_type hash, const String &value ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::getProperty( hash_type hash, void *param ) const -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::getProperty( hash_type hash, Parameters &params ) const -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::getProperty( hash_type hash, Parameter &param ) const -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::getProperty( hash_type hash, String &value ) const -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::callFunction( hash_type hash, SmartPtr<ISharedObject> object,
                                                      Parameters &results ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::callFunction( hash_type hash, const Parameters &params,
                                                      Parameters &results ) -> s32
    {
        return 0;
    }

    auto UserComponent::ScriptReceiver::getOwner() const -> UserComponent *
    {
        return m_owner;
    }

    void UserComponent::ScriptReceiver::setOwner( UserComponent *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::scene
