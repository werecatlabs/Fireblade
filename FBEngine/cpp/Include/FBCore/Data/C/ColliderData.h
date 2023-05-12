#ifndef SaracenColliderData_h__
#define SaracenColliderData_h__

#include "FBCore/Data/C/TransformData.h"

namespace fb
{
    class SaracenLayerData
    {
    public:
        SaracenLayerData();

        char name[60] = { 0 };
        int value = -1;
    };

    inline SaracenLayerData::SaracenLayerData()
    {
        name[0] = 0;
        value = -1;

        constexpr int size = sizeof( SaracenLayerData );
    }

    class SaracenSphereColliderData
    {
    public:
        SaracenSphereColliderData()
        {
            constexpr int size = sizeof( SaracenSphereColliderData );
        }

        VectorData center;
        float radius = 0.0f;
        int reserved = 0;
        int reserved0 = 0;
        int reserved1 = 0;
    };

    class SaracenPlaneColliderData
    {
    public:
        SaracenPlaneColliderData()
        {
            constexpr int structSize = sizeof( SaracenPlaneColliderData );
        }

        VectorData normal;
        float distance = 0.0f;
        int reserved = 0;
        int reserved0 = 0;
        int reserved1 = 0;
    };

    class SaracenBoxColliderData
    {
    public:
        SaracenBoxColliderData()
        {
            constexpr int structSize = sizeof( SaracenBoxColliderData );
        }

        VectorData center;
        VectorData size;
    };

    class SaracenMeshColliderData
    {
    public:
        SaracenMeshColliderData()
        {
            constexpr int structSize = sizeof( SaracenMeshColliderData );
        }

        char filePath[252];
        int isConvex = false;
    };

    class SaracenTerrainColliderData
    {
    public:
        SaracenTerrainColliderData()
        {
            constexpr int structSize = sizeof( SaracenTerrainColliderData );
        }

        char filePath[256] = { 0 };
        VectorData heightScale;
        VectorData terrainSize;
        VectorData size;
    };

    class SaracenPhysicsMaterial
    {
    public:
        SaracenPhysicsMaterial()
        {
            staticFriction = 1.0f;
            dynamicFriction = 1.0f;
            restitution = 0.1f;
            roughness = 0.0f;
        }

        float staticFriction = 0.0f;
        float dynamicFriction = 0.0f;
        float restitution = 0.0f;
        float roughness = 0.0f;
    };

    class ColliderData
    {
    public:
        enum SaracenColliderType
        {
            SPHERE,
            PLANE,
            BOX,
            MESH,
            TERRAIN,

            COUNT
        };

        ColliderData()
        {
            colliderType = 0;
            isTrigger = 0;
            constexpr int size = sizeof( ColliderData );
        }

        SaracenSphereColliderData sphereCollider;
        SaracenPlaneColliderData planeCollider;
        SaracenBoxColliderData boxCollider;
        SaracenMeshColliderData meshCollider;
        SaracenTerrainColliderData terrainCollider;

        TransformData transform;
        TransformData localTransform;
        TransformData worldTransform;

        int colliderType = 0;
        int isTrigger = 0;
    };
}  // namespace fb

#endif  // SaracenColliderData_h__
