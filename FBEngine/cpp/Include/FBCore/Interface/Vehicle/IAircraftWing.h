#ifndef IAircraftWing_h__
#define IAircraftWing_h__

#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{

    /** Interface for an aircraft wing class. */
    class IAircraftWing : public IVehicleComponent
    {
    public:
        ~IAircraftWing() override = default;

        virtual SmartPtr<IAircraftControlSurface> &getAttachedControlSurface() = 0;
        virtual const SmartPtr<IAircraftControlSurface> &getAttachedControlSurface() const = 0;
        virtual void setAttachedControlSurface( SmartPtr<IAircraftControlSurface> controlSurface ) = 0;

        virtual SmartPtr<IAircraftPropWash> &getAttachedPropWash() = 0;
        virtual const SmartPtr<IAircraftPropWash> &getAttachedPropWash() const = 0;
        virtual void setAttachedPropWash( SmartPtr<IAircraftPropWash> propWash ) = 0;

        virtual bool isControlSurface() const = 0;
        virtual void setControlSurface( bool bIsControlSurface ) = 0;

        virtual bool useCombinedControlSurface() const = 0;
        virtual void setUserCombinedControlSurface( bool bUseCombinedControlSurface ) = 0;
    };
}  // end namespace fb

#endif  // IAircraftWing_h__
