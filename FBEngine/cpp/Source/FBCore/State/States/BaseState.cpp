#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/BaseState.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/Timer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, BaseState, IState );

    BaseState::BaseState() = default;

    BaseState::~BaseState()
    {
        unload( nullptr );
    }

    void BaseState::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            m_owner = nullptr;
            m_stateContext = nullptr;

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto BaseState::getTime() const -> time_interval
    {
        return m_updateTime;
    }

    void BaseState::setTime( time_interval time )
    {
        m_updateTime = time;
    }

    auto BaseState::isDirty() const -> bool
    {
        return m_dirtyTime > m_updateTime;
    }

    void BaseState::setDirty( bool dirty )
    {
        auto &applicationManager = core::ApplicationManager::instance();
        auto &timer = applicationManager->getTimer();

        if( dirty )
        {
            m_dirtyTime = timer->now();

            if( auto &stateContext = getStateContext() )
            {
                
                auto &stateManager = applicationManager->getStateManager();

                auto task = getTaskId();
                stateManager->addDirty( stateContext, task );
            }
        }
        else
        {
            m_updateTime = timer->now();
        }
    }

    auto BaseState::isRegistered() const -> bool
    {
        return m_isRegistered;
    }

    void BaseState::setRegistered( bool registered )
    {
        m_isRegistered = registered;
    }

    auto BaseState::getStateContext() -> SmartPtr<IStateContext> &
    {
        return m_stateContext;
    }

    auto BaseState::getStateContext() const -> const SmartPtr<IStateContext> &
    {
        return m_stateContext;
    }

    void BaseState::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    auto BaseState::getTaskId() const -> Thread::Task
    {
        return m_taskId;
    }

    void BaseState::setTaskId( Thread::Task task )
    {
        m_taskId = task;
    }

    auto BaseState::getProperties() const -> SmartPtr<Properties>
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto properties = factoryManager->make_ptr<Properties>();

        auto time = static_cast<f32>( m_updateTime );
        properties->setProperty( "time", time );

        return properties;
    }

    void BaseState::setProperties( SmartPtr<Properties> properties )
    {
        auto time = 0.f;
        properties->getPropertyValue( "time", time );
        m_updateTime = time;
    }

    auto BaseState::getOwner() const -> SmartPtr<ISharedObject>
    {
        return m_owner;
    }

    void BaseState::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    auto BaseState::clone() const -> SmartPtr<IState>
    {
        FB_ASSERT( false );  // derived clone not implemented
        auto state = fb::make_ptr<BaseState>();
        return state;
    }

    void BaseState::assign( SmartPtr<IState> state )
    {
    }

    void BaseState::makeClone( SmartPtr<BaseState> state ) const
    {
        if( state )
        {
            state->m_owner = m_owner;
            state->m_stateContext = m_stateContext;
            state->m_taskId = m_taskId.load();
            state->m_updateTime = m_updateTime;
            state->m_isRegistered = m_isRegistered;
        }
    }

}  // end namespace fb
