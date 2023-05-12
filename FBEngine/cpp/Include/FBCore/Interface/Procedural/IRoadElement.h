#ifndef IRoadElement_h__
#define IRoadElement_h__

#include <FBCore/Interface/Procedural/IProceduralObject.h>

namespace fb
{
    namespace procedural
    {
        class IRoadElement : public IProceduralObject
        {
        public:
            ~IRoadElement() override = default;

            virtual SmartPtr<IRoadSection> getParentSection() const = 0;
            virtual void setParentSection( SmartPtr<IRoadSection> val ) = 0;

            virtual Array<SmartPtr<IRoadNode>> getRoadNodes() const = 0;

            virtual Array<SmartPtr<IRoadNode>> getSidewalks() const = 0;
            virtual void setSidewalks( Array<SmartPtr<IRoadNode>> val ) = 0;

            virtual IRoad::RoadType getRoadType() const = 0;
            virtual void setRoadType( IRoad::RoadType val ) = 0;

            virtual IRoad::LaneType getLaneType() const = 0;
            virtual void setLaneType( IRoad::LaneType val ) = 0;

            virtual bool isLit() const = 0;
            virtual void setIsLit( bool val ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadElement_h__
