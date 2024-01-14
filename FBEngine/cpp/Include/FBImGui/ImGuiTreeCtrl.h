#ifndef __ImGuiTreeCtrl_h__
#define __ImGuiTreeCtrl_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITreeCtrl.h>

namespace fb
{
    namespace ui
    {

        class ImGuiTreeCtrl : public CImGuiElement<IUITreeCtrl>
        {
        public:
            ImGuiTreeCtrl();
            ~ImGuiTreeCtrl() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IUITreeNode> addRoot() override;
            SmartPtr<IUITreeNode> addNode() override;

            void expand( SmartPtr<IUITreeNode> node ) override;

            void clear() override;

            Array<SmartPtr<IUITreeNode>> getTreeNodes() const override;
            void setTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes ) override;

            Array<SmartPtr<IUITreeNode>> getSelectTreeNodes() const override;
            void setSelectTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes ) override;

            void addSelectTreeNode( SmartPtr<IUITreeNode> treeNode ) override;
            void clearSelectTreeNodes() override;

            SmartPtr<IUITreeNode> getSelectTreeNode() const override;

            void setSelectTreeNode( SmartPtr<IUITreeNode> treeNode ) override;

            SmartPtr<IUITreeNode> getRoot() const override;
            void setRoot( SmartPtr<IUITreeNode> val ) override;

            bool isMultiSelect() const override;
            void setMultiSelect( bool multiSelect ) override;

            SmartPtr<IUIElement> getDragSourceElement() const override;
            void setDragSourceElement( SmartPtr<IUIElement> dragSource ) override;

            SmartPtr<IUIElement> getDropDestinationElement() const override;
            void setDropDestinationElement( SmartPtr<IUIElement> dropDestination ) override;

            s32 getSelectedSiblingIndex() const;

            void setSelectedSiblingIndex( s32 siblingIndex );

            static void createElement( SmartPtr<IUIElement> element );

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public BaseStateListener
            {
            public:
                void handleStateChanged( const SmartPtr<IStateMessage> &message );
            };

            void addTreeNode( SmartPtr<IUITreeNode> node );

            void removeTreeNode( SmartPtr<IUITreeNode> node );

            SharedPtr<ConcurrentArray<SmartPtr<IUITreeNode>>> getTreeNodesPtr() const;

            void setTreeNodesPtr( SharedPtr<ConcurrentArray<SmartPtr<IUITreeNode>>> ptr );

            mutable RecursiveMutex m_mutex;

            AtomicSmartPtr<IUIElement> m_dragSourceElement;
            AtomicSmartPtr<IUIElement> m_dropDestinationElement;

            SmartPtr<IUITreeNode> m_root;
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IUITreeNode>>> m_treeNodes;
            Array<SmartPtr<IUITreeNode>> m_treeSelectedNodes;
            SmartPtr<IUITreeNode> m_selectedTreeNode;

            bool m_multiSelect = true;

            s32 numNodesDisplayed = 0;

            s32 m_siblingIndex = -1;

            static u32 m_nodeIdExt;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // CEGUIText_h__
