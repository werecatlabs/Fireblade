#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/BaseState.h>
#include <FBCore/Interface/IApplicationManager.h>
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

    BaseState::BaseState()
    {
    }

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
            m_stateObject = nullptr;

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    time_interval BaseState::getTime() const
    {
        return m_time;
    }

    void BaseState::setTime( time_interval time )
    {
        m_time = time;
    }

    bool BaseState::isDirty() const
    {
        return m_dirty > 0;
    }

    void BaseState::setDirty( bool dirty )
    {
        if( dirty )
        {
            ++m_dirty;

            if( auto stateContext = getStateContext() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto stateManager = applicationManager->getStateManager();
                stateManager->addDirty( stateContext );
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

    bool BaseState::isRegistered() const
    {
        return m_isRegistered;
    }

    void BaseState::setRegistered( bool registered )
    {
        m_isRegistered = registered;
    }

    SmartPtr<IStateContext> BaseState::getStateContext() const
    {
        auto p = m_stateObject.load();
        return p.lock();
    }

    void BaseState::setStateContext( SmartPtr<IStateContext> stateObject )
    {
        m_stateObject = stateObject;
    }

    Thread::Task BaseState::getTaskId() const
    {
        return m_taskId;
    }

    void BaseState::setTaskId( Thread::Task task )
    {
        m_taskId = task;
    }

    SmartPtr<Properties> BaseState::getProperties() const
    {
        auto applicationManager = core::IApplicationManager::instance();
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

    SmartPtr<ISharedObject> BaseState::getOwner() const
    {
        auto p = m_owner.load();
        return p.lock();
    }

    void BaseState::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    SmartPtr<IState> BaseState::clone() const
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
            state->m_stateObject = m_stateObject;
            state->m_taskId = m_taskId.load();
            state->m_time = m_time;
            state->m_dirty = m_dirty;
            state->m_isRegistered = m_isRegistered;
        }
    }

}  // end namespace fb
