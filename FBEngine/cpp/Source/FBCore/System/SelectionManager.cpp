#include <FBCore/FBCorePCH.h>
#include <FBCore/System/SelectionManager.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{

    SelectionManager::SelectionManager()
    {
    }

    SelectionManager::~SelectionManager()
    {
        unload( nullptr );
    }

    void SelectionManager::unload( SmartPtr<ISharedObject> data )
    {
        m_selection.clear();
    }

    void SelectionManager::addSelectedObject( SmartPtr<ISharedObject> object )
    {
        m_selection.push_back( object );

        auto objectListeners = getObjectListeners();
        for( auto listener : objectListeners )
        {
            if( listener )
            {
                listener->handleEvent( IEvent::Type::Scene, IEvent::addSelectedObject, {}, nullptr,
                                       object, nullptr );
            }
        }
    }

    void SelectionManager::removeSelectedObject( SmartPtr<ISharedObject> object )
    {
        auto it = std::find( m_selection.begin(), m_selection.end(), object );
        if( it != m_selection.end() )
        {
            m_selection.erase( it );
        }

        auto objectListeners = getObjectListeners();
        for( auto listener : objectListeners )
        {
            if( listener )
            {
                listener->handleEvent( IEvent::Type::Scene, IEvent::deselectObjects, {}, nullptr, object,
                                       nullptr );
            }
        }
    }

    void SelectionManager::clearSelection()
    {
        m_selection.clear();

        auto objectListeners = getObjectListeners();
        for( auto listener : objectListeners )
        {
            if( listener )
            {
                listener->handleEvent( IEvent::Type::Scene, IEvent::deselectAll, {}, nullptr, nullptr,
                                       nullptr );
            }
        }
    }

    Array<SmartPtr<ISharedObject>> SelectionManager::getSelection() const
    {
        return m_selection;
    }

}  // end namespace fb
