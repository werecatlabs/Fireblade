#ifndef SaracenModelData_h__
#define SaracenModelData_h__

#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    class PhysicsStateData
    {
    public:
        PhysicsStateData();
        PhysicsStateData( const PhysicsStateData &other );

        PhysicsStateData &operator=( const PhysicsStateData &other );

        VectorData position;
        VectorData orientation;
        VectorData scale;
        VectorData linearVelocity;
        VectorData angularVelocity;
        int isVisible = 0;
        int modelType = 0;
        int modelState = 0;
        int modelSubState = 0;
        int isReady = 0;
    };

    extern PhysicsStateData MakeSaracenModelData();
}  // namespace fb

#endif  // SaracenModelData_h__
