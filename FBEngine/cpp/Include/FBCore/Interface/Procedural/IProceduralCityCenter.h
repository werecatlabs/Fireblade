#ifndef IProceduralCityCenter_h__
#define IProceduralCityCenter_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralCityCenter : public ISharedObject
        {
        public:
            ~IProceduralCityCenter() override = default;

            virtual Transform3<real_Num> getTransform() const = 0;
            virtual void setTransform( Transform3<real_Num> transform ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralCityCenter_h__
