#ifndef __CSELECTIONMANAGER_H_INCLUDED__
#define __CSELECTIONMANAGER_H_INCLUDED__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Interface/System/ISelectionManager.h>

namespace fb
{
    
    class CSelectionManager : public CSharedObject<ISelectionManager>
    {
    public:
        CSelectionManager();
        ~CSelectionManager() override;

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
