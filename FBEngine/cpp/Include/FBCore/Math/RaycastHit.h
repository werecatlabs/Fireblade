#ifndef RaycastHit_h__
#define RaycastHit_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{
    class RaycastHit : public SharedObject<ISharedObject>
    {
    public:
        SmartPtr<scene::IComponent> rigidbody;
        SmartPtr<scene::IComponent> collider;
        SmartPtr<scene::IActor> transform;

        Vector2F barycentricCoordinate;
        Vector2F lightmapCoord;
        Vector3<real_Num> normal;
        Vector3<real_Num> point;

        Vector2F textureCoord;
        Vector2F textureCoord2;

        f32 distance = 0.0f;

        s32 triangleIndex = 0;
    };
}  // end namespace fb

#endif  // RaycastHit_h__
