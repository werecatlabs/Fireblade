#ifndef IRoadNetwork_h__
#define IRoadNetwork_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IRoadNetwork : public ISharedObject
        {
        public:
            ~IRoadNetwork() override = default;

            virtual SmartPtr<IRoadNode> getNodeByGraphId( s32 id ) const = 0;

            virtual Array<SmartPtr<IRoadNode>> getNodes() const = 0;
            virtual void setNodes( const Array<SmartPtr<IRoadNode>> &nodes ) = 0;

            virtual Array<SmartPtr<IRoadNode>> getMergedNodes() const = 0;
            virtual void setMergedNodes( const Array<SmartPtr<IRoadNode>> &val ) = 0;

            virtual const Array<SmartPtr<IRoad>> &getRoads() const = 0;
            virtual Array<SmartPtr<IRoad>> &getRoads() = 0;
            virtual void removeRoad( SmartPtr<IRoad> road ) = 0;
            virtual void addRoad( SmartPtr<IRoad> road ) = 0;

            virtual void addRoads( const Array<SmartPtr<IRoad>> &roads ) = 0;
            virtual void removeRoads( const Array<SmartPtr<IRoad>> &roads ) = 0;

            virtual Array<SmartPtr<IRoadConnection>> getRoadConnections() const = 0;
            virtual void setRoadConnections(
                const Array<SmartPtr<IRoadConnection>> &roadConnections ) = 0;

            virtual void removeRoadConnection( SmartPtr<IRoadConnection> roadConnection ) = 0;
            virtual void addRoadConnection( SmartPtr<IRoadConnection> roadConnection ) = 0;

            virtual void setupGraph() = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadNetwork_h__
