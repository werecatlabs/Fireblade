#ifndef IRoadNode_h__
#define IRoadNode_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Procedural/IProceduralNode.h>

namespace fb
{
    namespace procedural
    {
        class IRoadNode : public IProceduralNode
        {
        public:
            ~IRoadNode() override = default;

            virtual SmartPtr<IRoadNode> getRoadNodeFromMerged( SmartPtr<IRoad> road ) const = 0;

            virtual void addRoad( const SmartPtr<IRoad> &road ) = 0;
            virtual void removeRoad( const SmartPtr<IRoad> &road ) = 0;
            virtual Array<SmartPtr<IRoad>> getRoads() const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadNode_h__
