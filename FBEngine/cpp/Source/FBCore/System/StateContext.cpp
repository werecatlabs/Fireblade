#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateContext.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/System/IAsyncOperation.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    StateContext::StateContext() = default;

    StateContext::~StateContext() = default;

    void StateContext::setOwner( SmartPtr<ISharedObject> owner )
    {
    }

    auto StateContext::getOwner() -> SmartPtr<ISharedObject> &
    {
        return m_owner;
    }

    auto StateContext::getOwner() const -> const SmartPtr<ISharedObject> &
    {
        return m_owner;
    }

    auto StateContext::isDirty() const -> bool
    {
        return false;
    }

    void StateContext::setDirty( bool dirty, bool cascade )
    {
    }

    void StateContext::addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message )
    {
    }

    void StateContext::addStateListener( SmartPtr<IStateListener> stateListner )
    {
    }

    auto StateContext::removeStateListener( SmartPtr<IStateListener> stateListner ) -> bool
    {
        return false;
    }

    auto StateContext::getStateListeners() const -> SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>>
    {
        return nullptr;
    }

    void StateContext::addEventListener( SmartPtr<IEventListener> eventListner )
    {
    }

    auto StateContext::removeEventListener( SmartPtr<IEventListener> eventListner ) -> bool
    {
        return false;
    }

    auto StateContext::getEventListeners() const -> SharedPtr<Array<SmartPtr<IEventListener>>>
    {
        return nullptr;
    }

    void StateContext::addState( SmartPtr<IState> state )
    {
        auto p = getStatesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IState>>>();
            setStatesPtr( p );
        }

        if( p )
        {
            auto &states = *p;
            states.push_back( state );
        }
    }

    void StateContext::removeState( SmartPtr<IState> state )
    {
        auto states = getStates();
        states.erase( std::remove( states.begin(), states.end(), state ), states.end() );

        auto newStates =
            fb::make_shared<ConcurrentArray<SmartPtr<IState>>>( states.begin(), states.end() );
        setStatesPtr( newStates );
    }

    auto StateContext::getStateByTypeId( u32 typeId ) -> SmartPtr<IState> &
    {
        if( auto p = getStatesPtr() )
        {
            auto &states = *p;
            for( auto &state : states )
            {
                if( state->getTypeInfo() == typeId )
                {
                    return state;
                }
            }
        }

        static SmartPtr<IState> nullState;
        return nullState;
    }

    auto StateContext::getStateByTypeId( u32 typeId ) const -> const SmartPtr<IState> &
    {
        if( auto p = getStatesPtr() )
        {
            auto &states = *p;
            for( auto &state : states )
            {
                if( state->getTypeInfo() == typeId )
                {
                    return state;
                }
            }
        }

        static SmartPtr<IState> nullState;
        return nullState;
    }

    auto StateContext::getStates() const -> Array<SmartPtr<IState>>
    {
        if( auto p = getStatesPtr() )
        {
            auto states = Array<SmartPtr<IState>>( p->begin(), p->end() );
            return states;
        }

        return {};
    }

    auto StateContext::getStatesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IState>>>
    {
        return m_states;
    }

    void StateContext::setStatesPtr( SharedPtr<ConcurrentArray<SmartPtr<IState>>> states )
    {
        m_states = states;
    }

    void StateContext::sendMessage( SmartPtr<IStateMessage> message )
    {
    }

    auto StateContext::getProperties() const -> SmartPtr<Properties>
    {
        return nullptr;
    }

    void StateContext::setProperties( SmartPtr<Properties> properties )
    {
    }

    auto StateContext::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                     const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                     SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( auto pEventListeners = getEventListeners() )
        {
            auto &eventListeners = *pEventListeners;
            for( auto eventListener : eventListeners )
            {
                eventListener->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        return {};
    }
}  // end namespace fb
