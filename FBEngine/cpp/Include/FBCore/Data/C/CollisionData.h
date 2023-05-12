#ifndef SaracenCollision_h__
#define SaracenCollision_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Data/C/InterfaceConfigData.h>
#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    struct CollisionData
    {
        VectorData contactPoint;
        VectorData positionA;
        VectorData positionB;
        VectorData normal;
        VectorData impulse;
        float separation = 0.0f;
        int id = 0;
    };
}  // namespace fb

#endif  // SaracenCollision_h__
