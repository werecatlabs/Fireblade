#ifndef IProceduralNode_h__
#define IProceduralNode_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralNode : public IProceduralObject
        {
        public:
            ~IProceduralNode() override = default;

            virtual void connect( SmartPtr<IProceduralNode> node ) = 0;
            virtual void disconnect( SmartPtr<IProceduralNode> node ) = 0;
            virtual void disconnectAll() = 0;

            virtual Array<SmartPtr<IProceduralNode>> getConnectedNodes() const = 0;
            virtual size_t getNumConnections() const = 0;
            virtual void clearConnections() = 0;

            virtual bool hasMergedNode( SmartPtr<IProceduralNode> n ) = 0;

            virtual void addMergedNode( SmartPtr<IProceduralNode> n ) = 0;
            virtual void removeMergedNode( SmartPtr<IProceduralNode> n ) = 0;

            virtual Array<SmartPtr<IProceduralNode>> getMergedNodes() const = 0;
            virtual void setMergedNodes( Array<SmartPtr<IProceduralNode>> val ) = 0;

            virtual s32 getGraphId() const = 0;
            virtual void setGraphId( s32 id ) = 0;

            virtual s32 getRoadId() const = 0;
            virtual void setRoadId( s32 id ) = 0;

            virtual SmartPtr<IProceduralNode> getMergedNode() const = 0;
            virtual void setMergedNode( SmartPtr<IProceduralNode> node ) = 0;

            virtual void setIsConnection( bool bIsConnection ) = 0;
            virtual bool isConnection() const = 0;

            virtual String getConnectionType() const = 0;

            virtual SmartPtr<IProceduralNode> getNetworkNode() const = 0;
            virtual void setNetworkNode( SmartPtr<IProceduralNode> val ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralNode_h__
