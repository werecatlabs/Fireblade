#ifndef __CSELECTIONMANAGER_H_INCLUDED__
#define __CSELECTIONMANAGER_H_INCLUDED__

#include <FBCore/Interface/System/ISelectionManager.h>

namespace fb
{

    /** Selection manager class.
     *  This class is used to manage the selection of objects.
     *  It is used by the editor to manage the selection of objects.
     *  @see ISelectionManager
     */
    class SelectionManager : public ISelectionManager
    {
    public:
        /** Constructor. */
        SelectionManager();

        /** Destructor. */
        ~SelectionManager() override;

        /** @copydoc ISelectionManager::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISelectionManager::addSelectedObject */
        void addSelectedObject( SmartPtr<ISharedObject> object ) override;

        /** @copydoc ISelectionManager::removeSelectedObject */
        void removeSelectedObject( SmartPtr<ISharedObject> object ) override;

        /** @copydoc ISelectionManager::clearSelection */
        void clearSelection() override;

        /** @copydoc ISelectionManager::getSelection */
        Array<SmartPtr<ISharedObject>> getSelection() const override;

    private:
        // The selected objects.
        Array<SmartPtr<ISharedObject>> m_selection;
    };

}  // namespace fb

#endif
