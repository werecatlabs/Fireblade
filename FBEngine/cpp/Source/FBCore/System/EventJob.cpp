#include <FBCore/FBCorePCH.h>
#include <FBCore/System/EventJob.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    void EventJob::execute()
    {
        auto sender = getSender();
        auto object = getObject();

        auto listeners = getObjectListeners();
        for( auto &listener : listeners )
        {
            listener->handleEvent( eventType, eventValue, arguments, sender, object, event );
        }

        if( sender )
        {
            listeners = sender->getObjectListeners();
            for( auto &listener : listeners )
            {
                if( listener )
                {
                    listener->handleEvent( eventType, eventValue, arguments, sender, object, event );
                }
            }
        }
    }

    SmartPtr<ISharedObject> EventJob::getObject() const
    {
        return m_object;
    }

    void EventJob::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }

    SmartPtr<ISharedObject> EventJob::getSender() const
    {
        return m_sender;
    }

    void EventJob::setSender( SmartPtr<ISharedObject> sender )
    {
        m_sender = sender;
    }

}  // namespace fb
