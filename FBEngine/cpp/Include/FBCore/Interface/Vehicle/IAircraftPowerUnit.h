#ifndef IAircraftPowerUnit_h__
#define IAircraftPowerUnit_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehiclePowerUnit.h>

namespace fb
{

    /** Interface for an aircraft attachment class. */
    class IAircraftPowerUnit : public IVehiclePowerUnit
    {
    public:
        ~IAircraftPowerUnit() override = default;

        virtual SmartPtr<IAircraftPropeller> &getPropeller() = 0;
        virtual const SmartPtr<IAircraftPropeller> &getPropeller() const = 0;
        virtual void setPropeller( SmartPtr<IAircraftPropeller> val ) = 0;

        virtual bool isElectric() const = 0;
        virtual void setElectric( bool val ) = 0;

        virtual real_Num getThrustMultiplier() const = 0;
        virtual void setThrustMultiplier( real_Num val ) = 0;

        virtual real_Num getTorqueMultiplier() const = 0;
        virtual void setTorqueMultiplier( real_Num val ) = 0;

        virtual real_Num getPeakPowerW() const = 0;
        virtual void setPeakPowerW( real_Num peak_power_w ) = 0;

        virtual real_Num getMoi() const = 0;
        virtual void setMoi( real_Num val ) = 0;
    };

}  // end namespace fb

#endif  // IAircraftPowerUnit_h__
