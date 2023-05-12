#ifndef IMaterialNode_h__
#define IMaterialNode_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace render
    {

        /** An interface for a material node. */
        class IMaterialNode : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IMaterialNode() override = default;

            /** Adds a child to this material node.
             * @param child The child to be added to the node.
             */
            virtual void addChild( SmartPtr<IMaterialNode> child ) = 0;

            /** Adds a child to this material node at the specified index.
             * @param child The child to be added to the node.
             * @param index The index at which to add the child.
             */
            virtual void addChild( SmartPtr<IMaterialNode> child, int index ) = 0;

            /** Removes a child from this material node.
             * @param child The child to be removed from the node.
             */
            virtual void removeChild( SmartPtr<IMaterialNode> child ) = 0;

            /** Removes this material node from its parent. */
            virtual void remove() = 0;

            /** Gets the number of children of this material node.
             * @return The number of children.
             */
            virtual u32 getNumChildren() const = 0;

            /** Gets a child of this material node by its index.
             * @param index The index of the child to get.
             * @return A smart pointer to the child at the specified index.
             */
            virtual SmartPtr<IMaterialNode> getChildByIndex( u32 index ) const = 0;

            /** Gets a child of this material node by its ID.
             * @param id The ID of the child to get.
             * @return A smart pointer to the child with the specified ID.
             */
            virtual SmartPtr<IMaterialNode> getChildById( hash32 id ) const = 0;

            /** Gets an array of all children of this material node.
             * @return An array of smart pointers to the children of this node.
             */
            virtual Array<SmartPtr<IMaterialNode>> getChildren() const = 0;

            /** Gets the parent of this material node.
             * @return A smart pointer to the parent of this node. This can be null.
             */
            virtual SmartPtr<IMaterialNode> getParent() const = 0;

            /** Sets the parent of this material node.
             * @param parent A smart pointer to the parent of this node. This can be null.
             */
            virtual void setParent( SmartPtr<IMaterialNode> parent ) = 0;

            /** Gets the owner material of this node.
             * @return A smart pointer to the parent material of this node. This can be null.
             */
            virtual SmartPtr<IMaterial> getMaterial() const = 0;

            /** Sets the owner material of this node.
             * @param material A smart pointer to the parent material of this node. This can be null.
             */
            virtual void setMaterial( SmartPtr<IMaterial> material ) = 0;

            /** Gets the data of this material node as a properties object.
             * @return A smart pointer to the data of this material node as a properties object.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data of this material node as a properties object.
             * @param properties A smart pointer to the data of this material node as a properties object.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            /** Gets the state object associated with this material node.
             * @return A smart pointer to the state object associated with this material node.
             */
            virtual SmartPtr<IStateContext> getStateObject() const = 0;

            /**
             * Sets the state object associated with this overlay element.
             * @param stateContext A pointer to the state object.
             */
            virtual void setStateObject( SmartPtr<IStateContext> stateContext ) = 0;

            /**
             * Gets the state listener for this overlay element.
             * @return A pointer to the state listener.
             */
            virtual SmartPtr<IStateListener> getStateListener() const = 0;

            /**
             * Sets the state listener for this overlay element.
             * @param stateListener A pointer to the state listener.
             */
            virtual void setStateListener( SmartPtr<IStateListener> stateListener ) = 0;

            /**
             * Checks if this overlay element is enabled.
             * @return `true` if the overlay element is enabled, `false` otherwise.
             */
            virtual bool isEnabled() const = 0;

            /**
             * Sets the enabled status for this overlay element.
             * @param enabled The enabled status to set. `true` to enable the overlay element, `false` to disable it.
             */
            virtual void setEnabled( bool enabled ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialNode_h__
