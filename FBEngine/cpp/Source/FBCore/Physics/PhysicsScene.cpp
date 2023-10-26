//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/PhysicsScene.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace physics
    {

        PhysicsScene::PhysicsScene()
        {
        }

        PhysicsScene::~PhysicsScene()
        {
        }

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

        Vector3<real_Num> PhysicsScene::getSize() const
        {
            return Vector3<real_Num>::zero();
        }

        bool PhysicsScene::rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                                     Vector3F &hitNormal, u32 collisionType /*= 0*/,
                                     u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool PhysicsScene::intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                                        Vector3F &hitNormal, SmartPtr<ISharedObject> &object,
                                        u32 collisionType /*= 0*/, u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool PhysicsScene::castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                                           Array<SmartPtr<IRaycastHit>> &hits )
        {
            return false;
        }

        bool PhysicsScene::castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit )
        {
            return false;
        }

        void PhysicsScene::setGravity( const Vector3<real_Num> &vec )
        {
        }

        Vector3<real_Num> PhysicsScene::getGravity() const
        {
            return Vector3<real_Num>::zero();
        }

        void PhysicsScene::simulate( real_Num elapsedTime, void *scratchMemBlock,
                                      u32 scratchMemBlockSize, bool controlSimulation )
        {
        }

        bool PhysicsScene::fetchResults( bool block, u32 *errorState )
        {
            return false;
        }

        SmartPtr<IStateContext> PhysicsScene::getStateObject() const
        {
            static SmartPtr<IStateContext> s;
            return s;
        }

        void PhysicsScene::setStateObject( SmartPtr<IStateContext> stateObject )
        {
        }

        bool PhysicsScene::castRayDynamic( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit )
        {
            return false;
        }

    }  // end namespace physics
}  // end namespace fb
