#ifndef GraphNode_h__
#define GraphNode_h__

#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/List.h>
#include <FBCore/Core/Any.h>

namespace fb
{

    template <class T>
    class GraphNode : public T
    {
    public:
        GraphNode() = default;
        ~GraphNode() override = default;

        List<SmartPtr<GraphNode<T>>> getNeighbors() const
        {
            return m_neighbors;
        }

        void setNeighbors( const List<SmartPtr<GraphNode<T>>> &neighbors )
        {
            m_neighbors = neighbors;
        }

        void addNeighbor( SmartPtr<GraphNode<T>> neighbor )
        {
            m_neighbors = neighbor;
        }

        Any getUserData() const
        {
            return m_userData;
        }

        void setUserData( const Any &data )
        {
            m_userData = data;
        }

        virtual GraphNode<T> clone() const
        {
            auto clonedNode = fb::make_ptr<GraphNode<T>>();
            clonedNode->m_userData = this->m_userData;

            for( auto &neighbor : m_neighbors )
            {
                auto clonedNeighbor = neighbor->clone();
                clonedNode->addNeighbor( clonedNeighbor );
            }

            return clonedNode;
        }

        FB_CLASS_REGISTER_DECL;

    protected:
        Any m_userData;
        List<SmartPtr<GraphNode<T>>> m_neighbors;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, GraphNode, T, T );

}  // namespace fb

#endif  // GraphNode_h__
