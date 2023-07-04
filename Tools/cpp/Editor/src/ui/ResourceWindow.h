//
// Created by Zane Desir on 11/11/2021.
//

#ifndef FB_ResourceWindow_H
#define FB_ResourceWindow_H

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {
        class ResourceWindow : public BaseWindow
        {
        public:
            ResourceWindow( SmartPtr<ui::IUIWindow> parent );
            ~ResourceWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection() override;

        protected:
            class TreeCtrlListener : public SharedObject<IEventListener>
            {
            public:
                TreeCtrlListener();
                ~TreeCtrlListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                void handleTreeSelectionChanged( SmartPtr<ui::IUITreeNode> node );

                void handleTreeSelectionActivated( SmartPtr<ui::IUITreeNode> node );

                void handleTreeDragStart( SmartPtr<ui::IUITreeNode> node );

                ResourceWindow *getOwner() const;

                void setOwner( ResourceWindow *owner );

            private:
                ResourceWindow *m_owner = nullptr;
            };

            void buildTree();

            void addMaterialToTree( SmartPtr<render::IMaterial> material,
                                    SmartPtr<ui::IUITreeNode> node );

            void addObjectToTree( SmartPtr<ISharedObject> object, SmartPtr<ui::IUITreeNode> node );

            // SmartPtr<ui::IUIDropdown> m_dropdown;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_treeListener;
            SmartPtr<render::IMaterial> m_material;

            SmartPtr<PropertiesWindow> m_propertiesWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // FB_MATERIALWINDOW_H
