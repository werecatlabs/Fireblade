#ifndef IHelicopter_h__
#define IHelicopter_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{
    //-------------------------------------------------
    class IHelicopter : public IVehicleController
    {
    public:
        ~IHelicopter() override = default;
    };
}  // end namespace fb

#endif  // IHelicopter_h__
