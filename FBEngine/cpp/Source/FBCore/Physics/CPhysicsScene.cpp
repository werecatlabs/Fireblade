//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/CPhysicsScene.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace physics
    {

        CPhysicsScene::CPhysicsScene()
        {
        }

        CPhysicsScene::~CPhysicsScene()
        {
        }

        void CPhysicsScene::clear()
        {
        }

        void CPhysicsScene::addActor( SmartPtr<IPhysicsBody3> body )
        {
        }

        void CPhysicsScene::removeActor( SmartPtr<IPhysicsBody3> body )
        {
        }

        void CPhysicsScene::setSize( const Vector3<real_Num> &size )
        {
        }

        Vector3<real_Num> CPhysicsScene::getSize() const
        {
            return Vector3<real_Num>::zero();
        }

        bool CPhysicsScene::rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                                     Vector3F &hitNormal, u32 collisionType /*= 0*/,
                                     u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool CPhysicsScene::intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                                        Vector3F &hitNormal, SmartPtr<ISharedObject> &object,
                                        u32 collisionType /*= 0*/, u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool CPhysicsScene::castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                                     Array<SmartPtr<RaycastHit>> &hits )
        {
            return false;
        }

        bool CPhysicsScene::castRay( const Ray3<real_Num> &ray, RaycastHit &hit )
        {
            return false;
        }

        bool CPhysicsScene::castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> &hit )
        {
            return false;
        }

        void CPhysicsScene::setGravity( const Vector3<real_Num> &vec )
        {
        }

        Vector3<real_Num> CPhysicsScene::getGravity() const
        {
            return Vector3<real_Num>::zero();
        }

        void CPhysicsScene::simulate( real_Num elapsedTime, void *scratchMemBlock,
                                      u32 scratchMemBlockSize, bool controlSimulation )
        {
        }

        bool CPhysicsScene::fetchResults( bool block, u32 *errorState )
        {
            return false;
        }

        SmartPtr<IStateContext> CPhysicsScene::getStateObject() const
        {
            static SmartPtr<IStateContext> s;
            return s;
        }

        void CPhysicsScene::setStateObject( SmartPtr<IStateContext> stateObject )
        {
        }

        bool CPhysicsScene::castRayDynamic( const Ray3<real_Num> &ray, RaycastHit &hit )
        {
            return false;
        }

    }  // end namespace physics
}  // end namespace fb
