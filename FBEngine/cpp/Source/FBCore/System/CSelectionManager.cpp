#include <FBCore/FBCorePCH.h>
#include <FBCore/System/CSelectionManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    
    CSelectionManager::CSelectionManager()
    {
    }

    
    CSelectionManager::~CSelectionManager()
    {
        unload( nullptr );
    }

    
    void CSelectionManager::unload( SmartPtr<ISharedObject> data )
    {
        m_selection.clear();
    }

    
    void CSelectionManager::addSelectedObject( SmartPtr<ISharedObject> object )
    {
        m_selection.push_back( object );

        //for( auto listener : m_listeners )
        //{
        //    listener->addSelectedObject();
        //}
    }

    
    void CSelectionManager::removeSelectedObject( SmartPtr<ISharedObject> object )
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

    
    void CSelectionManager::clearSelection()
    {
        m_selection.clear();

        //for( auto listener : m_listeners )
        //{
        //    listener->deselectAll();
        //}
    }

    
    Array<SmartPtr<ISharedObject>> CSelectionManager::getSelection() const
    {
        return m_selection;
    }

}  // end namespace fb
