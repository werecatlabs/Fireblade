#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ComponentEvent.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/Base/Properties.h>

namespace fb
{
    namespace scene
    {
        ComponentEvent::ComponentEvent()
        {
        }

        ComponentEvent::~ComponentEvent()
        {
        }

        void ComponentEvent::addListener( SmartPtr<IComponentEventListener> listener )
        {
            m_listeners.push_back( listener );
        }

        void ComponentEvent::removeListener( SmartPtr<IComponentEventListener> listener )
        {
            auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
            if( it != m_listeners.end() )
            {
                m_listeners.erase( it );
            }
        }

        void ComponentEvent::removeListeners()
        {
            m_listeners.clear();
        }

        Array<SmartPtr<IComponentEventListener>> ComponentEvent::getListeners() const
        {
            return m_listeners;
        }

        void ComponentEvent::setListeners( const Array<SmartPtr<IComponentEventListener>> &listeners )
        {
            m_listeners = listeners;
        }

        String ComponentEvent::getLabel() const
        {
            return m_label;
        }

        void ComponentEvent::setLabel( const String &label )
        {
            m_label = label;
        }

        SmartPtr<ISharedObject> ComponentEvent::toData() const
        {
            return nullptr;
        }

        void ComponentEvent::fromData( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<Properties> ComponentEvent::getProperties() const
        {
            return nullptr;
        }

        void ComponentEvent::setProperties( SmartPtr<Properties> properties )
        {
        }

        hash_type ComponentEvent::getEventHash() const
        {
            return m_eventHash;
        }

        void ComponentEvent::setEventHash( hash_type eventHash )
        {
            m_eventHash = eventHash;
        }
    }  // namespace scene
}  // namespace fb
