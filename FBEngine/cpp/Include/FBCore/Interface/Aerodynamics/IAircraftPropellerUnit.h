#ifndef IAircraftPropellerUnit_h__
#define IAircraftPropellerUnit_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{
    class IAircraftPropellerUnit : public IVehicleComponent
    {
    public:
        ~IAircraftPropellerUnit() override = default;

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
        virtual void setThrust( const Vector3<real_Num> &thrust ) = 0;

        virtual Vector3<real_Num> getPropwash() const = 0;
        virtual void setPropwash( const Vector3<real_Num> &propwash ) = 0;
    };
}  // end namespace fb

#endif  // IAircraftPropellerUnit_h__
