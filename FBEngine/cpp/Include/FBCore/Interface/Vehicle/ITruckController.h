#ifndef ITruckController_h__
#define ITruckController_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    
    class ITruckController : public IVehicleController
    {
    public:
        enum class Input
        {
            THROTTLE,
            BRAKE,
            STEERING,

            COUNT
        };

        ~ITruckController() override = default;

        f32 getChannel( s32 idx ) const override = 0;
        void setChannel( s32 idx, f32 channel ) override = 0;

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

        virtual SmartPtr<IVehicleCallback> getCallback() const = 0;
        virtual void setCallback( SmartPtr<IVehicleCallback> callback ) = 0;

        virtual SmartPtr<IAerodymanicsWind> &getWind() = 0;
        virtual const SmartPtr<IAerodymanicsWind> &getWind() const = 0;
        virtual void setWind( SmartPtr<IAerodymanicsWind> val ) = 0;

        virtual void addWheel( SmartPtr<IWheelController> wheel ) = 0;
        virtual void removeWheel( SmartPtr<IWheelController> wheel ) = 0;

        virtual Array<SmartPtr<IWheelController>> getWheels() const = 0;
        virtual void setWheels( Array<SmartPtr<IWheelController>> wheels ) = 0;

        virtual void openCab() = 0;
        virtual void closeCab() = 0;

        virtual void hoiseDown() = 0;
        virtual void hoistUp() = 0;

        virtual RawPtr<IWheelController> getWheelController( u32 index ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ITruckController_h__
