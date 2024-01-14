//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/PhysicsScene.h>
#include <FBCore/FBCore.h>

namespace fb::physics
{

    PhysicsScene::PhysicsScene() = default;

    PhysicsScene::~PhysicsScene() = default;

    void PhysicsScene::clear()
    {
    }

    void PhysicsScene::addActor( SmartPtr<IPhysicsBody3> body )
    {
    }

    void PhysicsScene::removeActor( SmartPtr<IPhysicsBody3> body )
    {
    }

    void PhysicsScene::setSize( const Vector3<real_Num> &size )
    {
    }

    auto PhysicsScene::getSize() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto PhysicsScene::rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                                Vector3F &hitNormal, u32 collisionType /*= 0*/,
                                u32 collisionMask /*= 0 */ ) -> bool
    {
        return false;
    }

    auto PhysicsScene::intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                                   Vector3F &hitNormal, SmartPtr<ISharedObject> &object,
                                   u32 collisionType /*= 0*/, u32 collisionMask /*= 0 */ ) -> bool
    {
        return false;
    }

    auto PhysicsScene::castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                                Array<SmartPtr<IRaycastHit>> &hits ) -> bool
    {
        return false;
    }

    auto PhysicsScene::castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) -> bool
    {
        return false;
    }

    void PhysicsScene::setGravity( const Vector3<real_Num> &vec )
    {
    }

    auto PhysicsScene::getGravity() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    void PhysicsScene::simulate( real_Num elapsedTime, void *scratchMemBlock, u32 scratchMemBlockSize,
                                 bool controlSimulation )
    {
    }

    auto PhysicsScene::fetchResults( bool block, u32 *errorState ) -> bool
    {
        return false;
    }

    auto PhysicsScene::getStateContext() const -> SmartPtr<IStateContext>
    {
        static SmartPtr<IStateContext> s;
        return s;
    }

    void PhysicsScene::setStateContext( SmartPtr<IStateContext> stateContext )
    {
    }

    auto PhysicsScene::castRayDynamic( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) -> bool
    {
        return false;
    }

}  // namespace fb::physics
