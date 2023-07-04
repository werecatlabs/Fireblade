#include <FBCore/FBCorePCH.h>
#include <FBCore/System/SelectionManager.h>
#include <FBCore/FBCore.h>

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

        //for( auto listener : m_listeners )
        //{
        //    listener->addSelectedObject();
        //}
    }

    void SelectionManager::removeSelectedObject( SmartPtr<ISharedObject> object )
    {
        auto it = std::find( m_selection.begin(), m_selection.end(), object );
        if( it != m_selection.end() )
        {
            m_selection.erase( it );
        }

        //for( auto listener : m_listeners )
        //{
        //    listener->deselectObjects();
        //}
    }

    void SelectionManager::clearSelection()
    {
        m_selection.clear();

        //for( auto listener : m_listeners )
        //{
        //    listener->deselectAll();
        //}
    }

    Array<SmartPtr<ISharedObject>> SelectionManager::getSelection() const
    {
        return m_selection;
    }

}  // end namespace fb
