#ifndef __CSELECTIONMANAGER_H_INCLUDED__
#define __CSELECTIONMANAGER_H_INCLUDED__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Interface/System/ISelectionManager.h>

namespace fb
{
    
    class SelectionManager : public SharedObject<ISelectionManager>
    {
    public:
        SelectionManager();
        ~SelectionManager() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void addSelectedObject( SmartPtr<ISharedObject> object ) override;
        void removeSelectedObject( SmartPtr<ISharedObject> object ) override;
        void clearSelection() override;

        Array<SmartPtr<ISharedObject>> getSelection() const override;

    private:
        Array<SmartPtr<ISharedObject>> m_selection;
    };
}  // end namespace fb

#endif
