#ifndef SaracenRigidBodyData_h__
#define SaracenRigidBodyData_h__

#include <FBCore/Data/C/VectorData.h>
#include "FBCore/Data/C/ColliderData.h"

namespace fb
{
    class SaracenBoundsData
    {
    public:
        VectorData minExtents;
        VectorData maxExtents;
    };

    class SaracenRigidBodyCreationData
    {
    public:
        SaracenRigidBodyCreationData()
        {
            actorId = 0;
            numSolverIterations = 10;
            mass = 1.0f;
            drag = 0.0f;
            angularDrag = 0.05f;
            useGravity = true;
            isKinematic = false;
            isStatic = true;
            autoWind = true;
            autoBouyancy = true;
        }

        int actorId = 0;
        int componentId = 0;
        int numSolverIterations = 0;
        float mass = 0.0f;
        float drag = 0.0f;
        float angularDrag = 0.0f;
        bool useGravity = true;
        bool isKinematic = false;
        bool isStatic = false;
        bool autoWind = true;
        bool autoBouyancy = true;

        SaracenBoundsData bounds;
        ColliderData collider;
        SaracenLayerData layer;
        SaracenLayerData collidableLayers[32];
    };

    struct RigidBodyData
    {
        RigidBodyData();

        VectorData position;
        VectorData orientation;
        VectorData scale;
        int id = 0;
        int reserved1 = 0;
        int reserved2 = 0;
        int reserved3 = 0;
    };

    struct SaracenRigidBodies
    {
        SaracenRigidBodies();

        RigidBodyData rigidBodies[2048];
        int numRigidBodies = 0;
    };
}  // namespace fb

#endif  // SaracenRigidBodyData_h__
