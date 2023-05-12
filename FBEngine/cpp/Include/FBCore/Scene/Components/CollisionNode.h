#ifndef CollisionNode_h__
#define CollisionNode_h__

#include <FBCore/Scene/Components/BaseComponent.h>
#include "FBCore/Base/Node.h"

namespace fb
{
    namespace scene
    {

        /** Used to create a collision graph for the game logic. */
        class CollisionNode : public BaseComponent
        {
        public:
            CollisionNode();
            ~CollisionNode() override;

            u32 getRootType() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<CollisionNode> getRoot() const;

            /// This node's type.
            u32 m_nodeType;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionNode_h__
