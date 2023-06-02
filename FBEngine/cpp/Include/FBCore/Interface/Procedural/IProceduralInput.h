#ifndef IProceduralInput_h__
#define IProceduralInput_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralInput : public ISharedObject
        {
        public:
            ~IProceduralInput() override = default;

            virtual void setRoadConnections( const Array<SmartPtr<IRoadConnection>> &connections ) = 0;
            virtual Array<SmartPtr<IRoadConnection>> getRoadConnections() const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralInput_h__
