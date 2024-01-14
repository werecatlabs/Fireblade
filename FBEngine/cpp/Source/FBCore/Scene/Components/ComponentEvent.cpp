#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ComponentEvent.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/Core/Properties.h>

namespace fb::scene
{
    ComponentEvent::ComponentEvent() = default;

    ComponentEvent::~ComponentEvent() = default;

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

    auto ComponentEvent::getListeners() const -> Array<SmartPtr<IComponentEventListener>>
    {
        return m_listeners;
    }

    void ComponentEvent::setListeners( const Array<SmartPtr<IComponentEventListener>> &listeners )
    {
        m_listeners = listeners;
    }

    auto ComponentEvent::getLabel() const -> String
    {
        return m_label;
    }

    void ComponentEvent::setLabel( const String &label )
    {
        m_label = label;
    }

    auto ComponentEvent::toData() const -> SmartPtr<ISharedObject>
    {
        return nullptr;
    }

    void ComponentEvent::fromData( SmartPtr<ISharedObject> data )
    {
    }

    auto ComponentEvent::getProperties() const -> SmartPtr<Properties>
    {
        return nullptr;
    }

    void ComponentEvent::setProperties( SmartPtr<Properties> properties )
    {
    }

    auto ComponentEvent::getEventHash() const -> hash_type
    {
        return m_eventHash;
    }

    void ComponentEvent::setEventHash( hash_type eventHash )
    {
        m_eventHash = eventHash;
    }
}  // namespace fb::scene
