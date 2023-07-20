#ifndef IElectricMotor_h__
#define IElectricMotor_h__

#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{

    class IElectricMotor : public IVehicleComponent
    {
    public:
        ~IElectricMotor() override = default;
    };

}  // namespace fb

#endif  // IElectricMotor_h__
