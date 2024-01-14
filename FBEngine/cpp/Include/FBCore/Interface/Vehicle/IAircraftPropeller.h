#ifndef IAircraftPropeller_h__
#define IAircraftPropeller_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>

namespace fb
{

    class IAircraftPropeller : public IVehicleComponent
    {
    public:
        ~IAircraftPropeller() override = default;

        virtual real_Num getDiameter() const = 0;
        virtual void setDiameter( real_Num val ) = 0;

        virtual Vector3<real_Num> getThrust() const = 0;
        virtual void setThrust( const Vector3<real_Num> &val ) = 0;

        virtual real_Num getThrustValue() const = 0;
        virtual void setThrustValue( real_Num val ) = 0;

        virtual real_Num getPropwash() const = 0;
        virtual void setPropwash( real_Num val ) = 0;

        virtual Vector3<real_Num> getThrustLine() const = 0;
        virtual void setThrustLine( const Vector3<real_Num> &val ) = 0;

        virtual real_Num getDownThrust() const = 0;
        virtual void setDownThrust( real_Num val ) = 0;

        virtual real_Num getSideThrust() const = 0;
        virtual void setSideThrust( real_Num val ) = 0;
    };

}  // end namespace fb

#endif  // IAircraftPropeller_h__
