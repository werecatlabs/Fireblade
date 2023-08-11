//
// Created by Zane Desir on 31/10/2021.
//

#ifndef FB_CPHYSICSSCENE_H
#define FB_CPHYSICSSCENE_H

#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace physics
    {
        class CPhysicsScene : public SharedObject<IPhysicsScene3>
        {
        public:
            CPhysicsScene();
            ~CPhysicsScene() override;

            void clear() override;

            void addActor( SmartPtr<IPhysicsBody3> body ) override;

            void removeActor( SmartPtr<IPhysicsBody3> body ) override;

            void setSize( const Vector3<real_Num> &size ) override;

            Vector3<real_Num> getSize() const override;

            bool rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                          Vector3F &hitNormal, u32 collisionType = 0, u32 collisionMask = 0 ) override;

            bool intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                             Vector3F &hitNormal, SmartPtr<ISharedObject> &object, u32 collisionType = 0,
                             u32 collisionMask = 0 ) override;

            bool castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                          Array<SmartPtr<RaycastHit>> &hits ) override;

            bool castRay( const Ray3<real_Num> &ray, RaycastHit &hit ) override;

            bool castRayDynamic( const Ray3<real_Num> &ray, RaycastHit &hit ) override;

            bool castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> &hit ) override;

            void setGravity( const Vector3<real_Num> &vec ) override;

            Vector3<real_Num> getGravity() const override;

            void simulate( real_Num elapsedTime, void *scratchMemBlock, u32 scratchMemBlockSize,
                           bool controlSimulation ) override;

            bool fetchResults( bool block, u32 *errorState ) override;

            SmartPtr<IStateContext> getStateObject() const override;

            void setStateObject( SmartPtr<IStateContext> stateObject ) override;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FB_CPHYSICSSCENE_H
