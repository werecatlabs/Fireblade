#ifndef IESController_h__
#define IESController_h__

#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{
    
    class IESController : public IVehicleComponent
    {
    public:
        ~IESController() override = default;

        virtual SmartPtr<IBatteryPack> getBatteryPack() const = 0;
        virtual void setBatteryPack( SmartPtr<IBatteryPack> val ) = 0;

        virtual SmartPtr<IESController> getESC() const = 0;
        virtual void setESC( SmartPtr<IESController> val ) = 0;

        virtual SmartPtr<IVehiclePowerUnit> getMotor() const = 0;
        virtual void setMotor( SmartPtr<IVehiclePowerUnit> val ) = 0;
    };

}  // end namespace fb

#endif  // IESController_h__
