#ifndef IUITreeCtrl_h__
#define IUITreeCtrl_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * Interface for a tree control UI element.
         */
        class IUITreeCtrl : public IUIElement
        {
        public:
            /** The hash value used to clear the tree. */
            static const hash_type clearHash;

            /** Destructor. */
            ~IUITreeCtrl() override = default;

            /** Clears the tree. */
            virtual void clear() = 0;

            /** Adds a root node to the tree. */
            virtual SmartPtr<IUITreeNode> addRoot() = 0;

            /** Adds a node to the tree. */
            virtual SmartPtr<IUITreeNode> addNode() = 0;

            /** Expands a tree node. */
            virtual void expand( SmartPtr<IUITreeNode> node ) = 0;

            /** Gets the tree nodes. */
            virtual Array<SmartPtr<IUITreeNode>> getTreeNodes() const = 0;

            /** Sets the tree nodes. */
            virtual void setTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes ) = 0;

            /** Gets the selected tree nodes. */
            virtual Array<SmartPtr<IUITreeNode>> getSelectTreeNodes() const = 0;

            /** Sets the selected tree nodes. */
            virtual void setSelectTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes ) = 0;

            /** Adds a selected tree node. */
            virtual void addSelectTreeNode( SmartPtr<IUITreeNode> treeNode ) = 0;

            /** Clears the selected tree nodes. */
            virtual void clearSelectTreeNodes() = 0;

            /** Gets the selected tree node. */
            virtual SmartPtr<IUITreeNode> getSelectTreeNode() const = 0;

            /** Sets the selected tree node. */
            virtual void setSelectTreeNode( SmartPtr<IUITreeNode> treeNode ) = 0;

            /** Gets the root node. */
            virtual SmartPtr<IUITreeNode> getRoot() const = 0;

            /** Sets the root node. */
            virtual void setRoot( SmartPtr<IUITreeNode> root ) = 0;

            /** Gets the drag source element. */
            virtual SmartPtr<IUIElement> getDragSourceElement() const = 0;

            /** Sets the drag source element. */
            virtual void setDragSourceElement( SmartPtr<IUIElement> dragSource ) = 0;

            /** Gets the drop destination element. */
            virtual SmartPtr<IUIElement> getDropDestinationElement() const = 0;

            /** Sets the drop destination element. */
            virtual void setDropDestinationElement( SmartPtr<IUIElement> dropDestination ) = 0;

            /** Returns true if multiple nodes can be selected, false otherwise. */
            virtual bool isMultiSelect() const = 0;

            /** Sets whether multiple nodes can be selected. */
            virtual void setMultiSelect( bool multiSelect ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUITreeCtrl_h__
