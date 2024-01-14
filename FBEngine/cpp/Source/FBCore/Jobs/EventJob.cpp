#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/EventJob.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    void EventJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();

        auto sender = getSender();
        auto object = getObject();

        auto listeners = applicationManager->getObjectListeners();
        for( auto &listener : listeners )
        {
            listener->handleEvent( getEventType(), getEventValue(), getArguments(), sender, object,
                                   m_event );
        }

        if( sender )
        {
            listeners = sender->getObjectListeners();
            for( auto &listener : listeners )
            {
                if( listener )
                {
                    listener->handleEvent( getEventType(), getEventValue(), getArguments(), sender,
                                           object, m_event );
                }
            }
        }

        if( object )
        {
            listeners = object->getObjectListeners();
            for( auto &listener : listeners )
            {
                if( listener )
                {
                    listener->handleEvent( getEventType(), getEventValue(), getArguments(), sender,
                                           object, m_event );
                }
            }
        }
    }

    auto EventJob::getObject() const -> SmartPtr<ISharedObject>
    {
        return m_object;
    }

    void EventJob::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }

    auto EventJob::getSender() const -> SmartPtr<ISharedObject>
    {
        return m_sender;
    }

    void EventJob::setSender( SmartPtr<ISharedObject> sender )
    {
        m_sender = sender;
    }

    auto EventJob::getOwner() const -> IStateContext *
    {
        return m_owner;
    }

    void EventJob::setOwner( IStateContext *owner )
    {
        m_owner = owner;
    }

    auto EventJob::getEventType() const -> IEvent::Type
    {
        return m_eventType;
    }

    void EventJob::setEventType( IEvent::Type eventType )
    {
        m_eventType = eventType;
    }

    auto EventJob::getEventValue() const -> hash_type
    {
        return m_eventValue;
    }

    void EventJob::setEventValue( hash_type eventValue )
    {
        m_eventValue = eventValue;
    }

    auto EventJob::getArguments() const -> Array<Parameter>
    {
        return m_arguments;
    }

    void EventJob::setArguments( const Array<Parameter> &arguments )
    {
        m_arguments = arguments;
    }

    auto EventJob::getEvent() const -> SmartPtr<IEvent>
    {
        return m_event;
    }

    void EventJob::setEvent( SmartPtr<IEvent> event )
    {
        m_event = event;
    }
}  // namespace fb
