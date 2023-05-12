#ifndef ModelStateData_h__
#define ModelStateData_h__

#include "FBCore/Math/Vector3.h"
#include "FBCore/Math/Quaternion.h"

namespace fb
{
    class VehicleStateData
    {
    public:
        VehicleStateData() = default;
        VehicleStateData( const VehicleStateData &other );

        VehicleStateData &operator=( const VehicleStateData &other );

        //SaracenModelData toSaracenModelData() const;

        Vector3<real_Num> position;
        Quaternion<real_Num> orientation;
        Vector3<real_Num> scale;

        Vector3<real_Num> linearVelocity;
        Vector3<real_Num> angularVelocity;

        Vector3<real_Num> forcePosition;
        Vector3<real_Num> force;
        Vector3<real_Num> torque;

        Vector3<real_Num> derivedLinearVelocity;
        Vector3<real_Num> derivedAngularVelocity;

        int isVisible = 0;
        int modelType = 0;
        int modelState = 0;
        int modelSubState = 0;
        int isReady = 0;
    };
}  // namespace fb

#endif  // SaracenModelData_h__
