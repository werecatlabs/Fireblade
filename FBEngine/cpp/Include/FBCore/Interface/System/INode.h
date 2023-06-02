#ifndef INode_h__
#define INode_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Handle.h>

namespace fb
{
    //-------------------------------------------------
    class INode : public ISharedObject
    {
    public:
        ~INode() override = default;

        /** Returns a boolean to know if the node has a parent. */
        virtual bool hasParent() const = 0;

        /** Returns the parent of this node. */
        virtual SmartPtr<INode> getParent() const = 0;

        /** Adds a child to this node. */
        virtual void addChild( SmartPtr<INode> child ) = 0;

        /** Removes a child of this node. */
        virtual bool removeChild( SmartPtr<INode> child ) = 0;

        /** Finds a child. */
        virtual SmartPtr<INode> findChild( hash32 id ) const = 0;

        /** Remove the child from its parent. */
        virtual void remove() = 0;

        /** Removes all children. */
        virtual void removeAllChildren() = 0;

        /** Fills an Array with the children of this entity. */
        virtual Array<SmartPtr<INode>> getChildren() const = 0;

        /** Dirty flag. */
        virtual bool isDirty() const = 0;

        /** Dirty flag. */
        virtual void setDirty( bool dirty, bool cascade = true ) = 0;

        /** Associated object. */
        virtual SmartPtr<ISharedObject> &getObject() = 0;

        /** Associated object. */
        virtual const SmartPtr<ISharedObject> &getObject() const = 0;

        /** Associated object. */
        virtual void setObject( SmartPtr<ISharedObject> val ) = 0;
    };
}  // end namespace fb

#endif  // INode_h__
