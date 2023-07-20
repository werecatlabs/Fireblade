#ifndef IAircraftPropWash_h__
#define IAircraftPropWash_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    class IAircraftPropWash : public IVehicleComponent
    {
    public:
        ~IAircraftPropWash() override = default;

        virtual Vector3<real_Num> getPropWash() = 0;
        virtual Vector3<real_Num> getPropWash( s32 section ) = 0;

        virtual SmartPtr<IAircraftPropellerUnit> &getPropellerUnit() = 0;
        virtual const SmartPtr<IAircraftPropellerUnit> &getPropellerUnit() const = 0;
        virtual void setPropellerUnit( SmartPtr<IAircraftPropellerUnit> val ) = 0;

        virtual Array<bool> &getAffectedSections() = 0;
        virtual const Array<bool> &getAffectedSections() const = 0;
        virtual void setAffectedSections( const Array<bool> &val ) = 0;

        virtual Array<float> &getSectionMultipliers() = 0;
        virtual const Array<float> &getSectionMultipliers() const = 0;
        virtual void setSectionMultipliers( const Array<float> &val ) = 0;

        virtual real_Num getStrength() const = 0;
        virtual void setStrength( real_Num val ) = 0;
    };

}  // end namespace fb

#endif  // IAircraftPropWash_h__
