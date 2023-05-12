#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateContext.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Interface/System/IAsyncOperation.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    StateContext::StateContext()
    {
    }

    StateContext::~StateContext()
    {
    }

    void StateContext::setOwner( SmartPtr<ISharedObject> owner )
    {
    }

    SmartPtr<ISharedObject> StateContext::getOwner() const
    {
        return nullptr;
    }

    bool StateContext::isDirty() const
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

    bool StateContext::removeStateListener( SmartPtr<IStateListener> stateListner )
    {
        return false;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> StateContext::getStateListeners() const
    {
        return nullptr;
    }

    void StateContext::addEventListener( SmartPtr<IEventListener> eventListner )
    {
    }

    bool StateContext::removeEventListener( SmartPtr<IEventListener> eventListner )
    {
        return false;
    }

    SharedPtr<Array<SmartPtr<IEventListener>>> StateContext::getEventListeners() const
    {
        return nullptr;
    }

    SmartPtr<IState> StateContext::getLatestOutputState() const
    {
        return nullptr;
    }

    SmartPtr<IState> StateContext::getOutputState( time_interval time ) const
    {
        return nullptr;
    }

    void StateContext::addOutputState( SmartPtr<IState> state )
    {
    }

    SmartPtr<IState> StateContext::getState() const
    {
        return m_state;
    }

    void StateContext::setState( SmartPtr<IState> state )
    {
        m_state = state;
    }

    void StateContext::sendMessage( SmartPtr<IStateMessage> message )
    {
    }

    SmartPtr<Properties> StateContext::getProperties() const
    {
        return nullptr;
    }

    void StateContext::setProperties( SmartPtr<Properties> properties )
    {
    }

    fb::Parameter StateContext::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                          const Array<Parameter> &arguments,
                                          SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
    {
        if( auto pEventListeners = getEventListeners() )
        {
            auto &eventListeners = *pEventListeners;
            for( auto eventListener : eventListeners )
            {
                eventListener->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        return Parameter();
    }

}  // end namespace fb
