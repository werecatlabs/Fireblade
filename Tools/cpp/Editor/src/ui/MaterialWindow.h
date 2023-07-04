//
// Created by Zane Desir on 11/11/2021.
//

#ifndef FB_MATERIALWINDOW_H
#define FB_MATERIALWINDOW_H

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {
        class MaterialWindow : public BaseWindow
        {
        public:
            MaterialWindow( SmartPtr<ui::IUIWindow> parent );
            ~MaterialWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection() override;

            SmartPtr<render::IMaterial> getMaterial() const;
            void setMaterial( SmartPtr<render::IMaterial> material );

            SmartPtr<ui::IUITreeCtrl> getTree() const;
            void setTree( SmartPtr<ui::IUITreeCtrl> tree );

        protected:
            class DropdownListener : public SharedObject<IEventListener>
            {
            public:
                DropdownListener() = default;
                ~DropdownListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                MaterialWindow *getOwner() const;

                void setOwner( MaterialWindow *owner );

            private:
                MaterialWindow *m_owner = nullptr;
            };

            class TreeCtrlListener : public SharedObject<IEventListener>
            {
            public:
                TreeCtrlListener();
                ~TreeCtrlListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                MaterialWindow *getOwner() const;

                void setOwner( MaterialWindow *owner );

            private:
                MaterialWindow *m_owner = nullptr;
            };

            void buildTree();

            void addMaterialToTree( SmartPtr<render::IMaterial> material,
                                    SmartPtr<ui::IUITreeNode> node );

            void addObjectToTree( SmartPtr<ISharedObject> object, SmartPtr<ui::IUITreeNode> node );

            void handleDropdownSelection();

            SmartPtr<ui::IUIDropdown> m_dropdown;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_treeListener;
            AtomicSmartPtr<render::IMaterial> m_material;

            SmartPtr<PropertiesWindow> m_propertiesWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // FB_MATERIALWINDOW_H
