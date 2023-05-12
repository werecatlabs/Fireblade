#ifndef IDriveTrain_h__
#define IDriveTrain_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    
    /** Interface for a vehicle drive train. */
    class IDriveTrain : public IVehicleComponent
    {
    public:
        ~IDriveTrain() override = default;

        virtual Array<SmartPtr<IWheelController>> getWheels() const = 0;
        virtual void setWheels( Array<SmartPtr<IWheelController>> wheels ) = 0;

        virtual SmartPtr<IGearBox> getGearBox() const = 0;
        virtual void setGearBox( SmartPtr<IGearBox> gearBox ) = 0;

        virtual SmartPtr<IDifferential> getDifferential() const = 0;
        virtual void setDifferential( SmartPtr<IDifferential> differential ) = 0;
    };
}  // namespace fb

#endif  // IDriveTrain_h__
