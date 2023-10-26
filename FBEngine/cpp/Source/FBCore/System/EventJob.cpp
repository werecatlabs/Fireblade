#include <FBCore/FBCorePCH.h>
#include <FBCore/System/EventJob.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/IApplicationManager.h>

namespace fb
{
    void EventJob::execute()
    {
        auto applicationManager = core::IApplicationManager::instance();

        auto sender = getSender();
        auto object = getObject();

        auto listeners = applicationManager->getObjectListeners();
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

        if( object )
        {
            listeners = object->getObjectListeners();
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
