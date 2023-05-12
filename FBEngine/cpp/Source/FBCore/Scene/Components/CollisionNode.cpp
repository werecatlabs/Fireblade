#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionNode.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, CollisionNode, BaseComponent );

        CollisionNode::CollisionNode()
        {
        }

        CollisionNode::~CollisionNode()
        {
        }

        u32 CollisionNode::getRootType() const
        {
            // FB_LOCK_MUTEX(Mutex);

            // SmartPtr<CollisionNode> parent = getParent();
            // if(!parent)
            //	return getNodeType();

            // SmartPtr<CollisionNode> root = parent->getRoot();
            // if(root)
            //	return root->getNodeType();

            // return parent->getNodeType();

            return 0;
        }

        SmartPtr<CollisionNode> CollisionNode::getRoot() const
        {
            // SmartPtr<CollisionNode> parent = getParent();
            // if(!parent)
            //{
            //	return SmartPtr<CollisionNode>::NULL_PTR;
            // }

            // SmartPtr<CollisionNode> root = parent->getRoot();
            // if(root)
            //	return root;

            return nullptr;
        }
    }  // namespace scene
}  // end namespace fb
