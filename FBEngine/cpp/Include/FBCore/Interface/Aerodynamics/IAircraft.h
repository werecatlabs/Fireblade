#ifndef IAircraft_h__
#define IAircraft_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleController.h>

#include "FBCore/Interface/Aerodynamics/IAircraftCallback.h"
#include "FBCore/Interface/Aerodynamics/IAerodymanicsWind.h"
#include "FBCore/Interface/Aerodynamics/IAircraftPropellerUnit.h"

#include "FBCore/Interface/Vehicle/IVehicleBody.h"
#include "FBCore/Interface/Vehicle/IBatteryPack.h"
#include "FBCore/Interface/Vehicle/IWheelController.h"

namespace fb
{

    /** Interface for an aircraft class. */
    class IAircraft : public IVehicleController
    {
    public:
        ~IAircraft() override = default;

        virtual SmartPtr<Properties> &getProperties() = 0;
        virtual const SmartPtr<Properties> &getProperties() const = 0;
        virtual void setProperties( SmartPtr<Properties> val ) = 0;

        virtual bool isElectric() const = 0;
        virtual void setElectric( bool bIsElectric ) = 0;

        virtual bool getEnablePowerUnit() const = 0;
        virtual void setEnablePowerUnit( bool val ) = 0;

        virtual bool getEmulateBattery() const = 0;
        virtual void setEmulateBattery( bool val ) = 0;

        virtual real_Num getAirDensity() const = 0;
        virtual void setAirDensity( real_Num val ) = 0;

        virtual SmartPtr<IBatteryPack> &getBatteryPack() = 0;
        virtual const SmartPtr<IBatteryPack> &getBatteryPack() const = 0;
        virtual void setBatteryPack( SmartPtr<IBatteryPack> val ) = 0;

        virtual SmartPtr<IAircraftCallback> getCallback() const = 0;
        virtual void setCallback( SmartPtr<IAircraftCallback> callback ) = 0;

        virtual SmartPtr<IAerodymanicsWind> &getWind() = 0;
        virtual const SmartPtr<IAerodymanicsWind> &getWind() const = 0;
        virtual void setWind( SmartPtr<IAerodymanicsWind> val ) = 0;

        virtual void addPropellerUnit( SmartPtr<IAircraftPropellerUnit> propellerUnit ) = 0;
        virtual void removePropellerUnit( SmartPtr<IAircraftPropellerUnit> propellerUnit ) = 0;

        virtual Array<SmartPtr<IAircraftPropellerUnit>> getPropellerUnits() const = 0;
        virtual void setPropellerUnits( Array<SmartPtr<IAircraftPropellerUnit>> val ) = 0;

        virtual void addWheel( SmartPtr<IWheelController> wheel ) = 0;
        virtual void removeWheel( SmartPtr<IWheelController> wheel ) = 0;

        virtual Array<SmartPtr<IWheelController>> getWheels() const = 0;
        virtual void setWheels( Array<SmartPtr<IWheelController>> wheels ) = 0;

        virtual real_Num getEngineRPM( int idx ) const = 0;
        virtual real_Num getThrust( int idx ) const = 0;

        virtual void setControlAngle( int id, float angle ) = 0;

        virtual real_Num getSectionMultiplier() const = 0;
        virtual void setSectionMultiplier( real_Num sectionMultiplier ) = 0;

        virtual String getModelDataFilePath() const = 0;
        virtual void setModelDataFilePath( const String &val ) = 0;

        virtual Transform3<real_Num> getBodyTransform() const = 0;
        virtual void setBodyTransform( Transform3<real_Num> val ) = 0;

        virtual real_Num getRollwiseDamping() const = 0;
        virtual void setRollwiseDamping( real_Num rollwiseDamping ) = 0;
    };

}  // end namespace fb

#endif  // IAircraft_h__
