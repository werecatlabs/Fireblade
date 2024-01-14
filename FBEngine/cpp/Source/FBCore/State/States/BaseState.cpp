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
        return m_time;
    }

    void BaseState::setTime( time_interval time )
    {
        m_time = time;
    }

    auto BaseState::isDirty() const -> bool
    {
        return m_dirty > 0;
    }

    void BaseState::setDirty( bool dirty )
    {
        if( dirty )
        {
            ++m_dirty;

            if( auto &stateContext = getStateContext() )
            {
                auto &applicationManager = core::ApplicationManager::instance();
                auto &stateManager = applicationManager->getStateManager();

                auto task = getTaskId();
                stateManager->addDirty( stateContext, task );
            }
        }
        else
        {
            if( m_dirty > 0 )
            {
                --m_dirty;
            }
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

        auto time = static_cast<f32>( m_time );
        properties->setProperty( "time", time );

        return properties;
    }

    void BaseState::setProperties( SmartPtr<Properties> properties )
    {
        auto time = 0.f;
        properties->getPropertyValue( "time", time );
        m_time = time;
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
            state->m_time = m_time;
            state->m_dirty = m_dirty;
            state->m_isRegistered = m_isRegistered;
        }
    }

}  // end namespace fb
