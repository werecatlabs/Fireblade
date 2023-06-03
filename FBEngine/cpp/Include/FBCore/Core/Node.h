#ifndef INode_h__
#define INode_h__

#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    template <class T>
    class Node : public SharedObject<T>
    {
    public:
        using NodeList = Array<SmartPtr<Node<T>>>;

        Node() : m_nodeId( 0 ), m_nodeType( 0 )
        {
        }

        ~Node() override
        {
        }

        u32 getNodeId() const
        {
            return m_nodeId;
        }

        void _setNodeId( u32 val )
        {
            m_nodeId = val;
        }

        u32 getNodeType() const
        {
            return m_nodeType;
        }

        void setNodeType( u32 val )
        {
            m_nodeType = val;
        }

        /** Returns a boolean to know if the node has a parent. */
        virtual bool hasParent() const
        {
            return m_parent != nullptr;
        }

        /** Returns the parent of this node. */
        virtual SmartPtr<Node> getParent() const
        {
            return m_parent;
        }

        /** Adds a child to this node. */
        virtual void addChild( SmartPtr<Node> child )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  

            child->remove();  // remove from current parent
            m_children.push_back( child );

            auto pThisNode = SharedObject<T>::template getSharedFromThis<Node<T>>();
            child->m_parent = pThisNode;
        }

        /** Removes a child of this node. */
        virtual bool removeChild( SmartPtr<Node> child )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            child->m_parent = nullptr;
            return false;  // m_children.erase_element(child);
        }

        /** Finds a child. */
        virtual SmartPtr<Node> findChild( u32 id ) const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  

            for( u32 i = 0; i < m_children.size(); ++i )
            {
                const SmartPtr<Node> &child = m_children[i];
                if( child->getNodeId() == id )
                {
                    return child;
                }
            }

            return nullptr;
        }

        /** Remove the child from its parent. */
        virtual void remove()
        {
            if( m_parent )
            {
                auto pThisNode = SharedObject<T>::template getSharedFromThis<Node<T>>();
                m_parent->removeChild( pThisNode );
            }
        }

        /** Removes all children. */
        virtual void removeAllChildren()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            for( u32 i = 0; i < m_children.size(); ++i )
            {
                removeChild( m_children[i] );
            }
        }

        /** Fills an Array with the children of this entity. */
        virtual Array<SmartPtr<Node>> getChildren() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            Array<SmartPtr<Node>> children;
            children.reserve( m_children.size() );

            for( u32 i = 0; i < m_children.size(); ++i )
                children.push_back( m_children[i] );

            return children;
        }

        FB_CLASS_REGISTER_TEMPLATE_DECL( Node, T );

    protected:
        atomic_u32 m_nodeId;
        atomic_u32 m_nodeType;

        SmartPtr<Node> m_parent;
        Array<SmartPtr<Node>> m_children;

        mutable RecursiveMutex m_mutex;  
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, Node, T, SharedObject<T> );

}  // end namespace fb

#endif  // INode_h__
