#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionSphere.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionSphere, Collision );

    CollisionSphere::CollisionSphere() = default;

    CollisionSphere::~CollisionSphere() = default;

    void CollisionSphere::load( SmartPtr<ISharedObject> data )
    {
    }

    void CollisionSphere::unload( SmartPtr<ISharedObject> data )
    {
    }
}  // namespace fb::scene
