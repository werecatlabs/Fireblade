#ifndef IGroundEffect_h__
#define IGroundEffect_h__

#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{
    //-------------------------------------------------
    class IGroundEffect : public IVehicleComponent
    {
    public:
        ~IGroundEffect() override = default;
    };
}  // end namespace fb

#endif  // IGroundEffect_h__
