#ifndef IRoad_h__
#define IRoad_h__

#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/Interface/Procedural/IRoadHitPoint.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace procedural
    {
        class IRoad : public IProceduralObject
        {
        public:
            enum class RoadType
            {
                None,
                Residential,
                Trunk,
                Footway,
                Steps,

                Count
            };

            enum class LaneType
            {
                OneLane = 1,
                TwoLane,
                ThreeLane,
                FourLane,
                FiveLane,
                SixLane
            };

            ~IRoad() override = default;

            virtual Array<SmartPtr<IRoadHitPoint>> intersects( SmartPtr<IRoad> road ) = 0;

            virtual Array<SmartPtr<IRoadNode>> getRoadNodes() const = 0;

            virtual SmartPtr<IRoadNode> getNode( size_t index ) const = 0;
            virtual SmartPtr<IRoadNode> getFirstNode() const = 0;
            virtual SmartPtr<IRoadNode> getLastNode() const = 0;

            virtual String getRoadType() const = 0;
            virtual void setRoadType( const String &roadType ) = 0;

            virtual s32 getMarkerFromTransform( const Transform3<real_Num> &transform ) = 0;

            virtual void addRoadSection( SmartPtr<IRoadSection> section ) = 0;
            virtual void removeRoadSection( SmartPtr<IRoadSection> section ) = 0;

            virtual Array<SmartPtr<IRoadSection>> getRoadSections() const = 0;
            virtual void setRoadSections( Array<SmartPtr<IRoadSection>> roadSections ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoad_h__
