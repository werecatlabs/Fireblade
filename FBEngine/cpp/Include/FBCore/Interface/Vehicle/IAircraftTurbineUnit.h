#ifndef IAircraftTurbineUnit_h__
#define IAircraftTurbineUnit_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{

    class IAircraftTurbineUnit : public IVehicleComponent
    {
    public:
        ~IAircraftTurbineUnit() override = default;

        virtual SmartPtr<IBatteryPack> &getBatteryPack() = 0;
        virtual const SmartPtr<IBatteryPack> &getBatteryPack() const = 0;
        virtual void setBatteryPack( SmartPtr<IBatteryPack> val ) = 0;

        virtual SmartPtr<IESController> &getESC() = 0;
        virtual const SmartPtr<IESController> &getESC() const = 0;
        virtual void setESC( SmartPtr<IESController> val ) = 0;

        virtual SmartPtr<IAircraftPowerUnit> &getPowerUnit() = 0;
        virtual const SmartPtr<IAircraftPowerUnit> &getPowerUnit() const = 0;
        virtual void setPowerUnit( SmartPtr<IAircraftPowerUnit> val ) = 0;

        virtual SmartPtr<IAircraftPropeller> &getPropeller() = 0;
        virtual const SmartPtr<IAircraftPropeller> &getPropeller() const = 0;
        virtual void setPropeller( SmartPtr<IAircraftPropeller> val ) = 0;

        virtual Vector3<real_Num> getThrust() const = 0;
    };

}  // end namespace fb

#endif  //  IAircraftTurbineUnit_h__
