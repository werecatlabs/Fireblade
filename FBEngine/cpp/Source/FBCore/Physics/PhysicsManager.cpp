//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/PhysicsManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Physics/IConstraintD6.h>
#include <FBCore/Interface/Physics/IConstraintFixed3.h>
#include <FBCore/Interface/Physics/IConstraintDrive.h>
#include <FBCore/Interface/Physics/IConstraintLinearLimit.h>
#include <FBCore/Interface/Physics/IRaycastHit.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/Physics/IRigidStatic3.h>
#include <FBCore/Interface/Physics/IRigidBody3.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IPhysicsVehicle3.h>
#include <FBCore/Interface/Physics/IPhysicsSoftBody3.h>
#include <FBCore/Interface/Physics/ICharacterController3.h>
#include <FBCore/Interface/Scene/IDirector.h>

namespace fb
{
    namespace physics
    {
        PhysicsManager::PhysicsManager()
        {
        }

        PhysicsManager::~PhysicsManager()
        {
        }

        void PhysicsManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void PhysicsManager::unload( SmartPtr<ISharedObject> data )
        {
        }

        bool PhysicsManager::getEnableDebugDraw() const
        {
            return false;
        }

        void PhysicsManager::setEnableDebugDraw( bool enableDebugDraw )
        {
        }

        SmartPtr<IPhysicsMaterial3> PhysicsManager::addMaterial()
        {
            return nullptr;
        }

        void PhysicsManager::removeMaterial( SmartPtr<IPhysicsMaterial3> material )
        {
        }

        SmartPtr<IPhysicsScene3> PhysicsManager::addScene()
        {
            return nullptr;
        }

        void PhysicsManager::removeScene( SmartPtr<IPhysicsScene3> scene )
        {
        }

        bool PhysicsManager::removeCollisionShape( SmartPtr<IPhysicsShape3> collisionShape )
        {
            return false;
        }

        bool PhysicsManager::removePhysicsBody( SmartPtr<IRigidBody3> body )
        {
            return false;
        }

        SmartPtr<ICharacterController3> PhysicsManager::addCharacter()
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> PhysicsManager::addRigidStatic( const Transform3<real_Num> &transform )
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> PhysicsManager::addRigidStatic(
            SmartPtr<IPhysicsShape3> collisionShape )
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> PhysicsManager::addRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                                                 SmartPtr<Properties> properties )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsSoftBody3> PhysicsManager::addSoftBody( const String &filePath )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsVehicle3> PhysicsManager::addVehicle( SmartPtr<IRigidBody3> chassis,
                                                                const SmartPtr<Properties> &properties )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsVehicle3> PhysicsManager::addVehicle(
            SmartPtr<scene::IDirector> vehicleTemplate )
        {
            return nullptr;
        }

        bool PhysicsManager::removeVehicle( SmartPtr<IPhysicsVehicle3> vehicle )
        {
            return false;
        }

        bool PhysicsManager::rayTest( const Vector3<real_Num> &start,
                                       const Vector3<real_Num> &direction, Vector3<real_Num> &hitPos,
                                       Vector3<real_Num> &hitNormal, u32 collisionType /*= 0*/,
                                       u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool PhysicsManager::intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                          Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                          SmartPtr<ISharedObject> &object, u32 collisionType /*= 0*/,
                                          u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        SmartPtr<IConstraintD6> PhysicsManager::addJointD6( SmartPtr<IPhysicsBody3> actor0,
                                                             const Transform3<real_Num> &localFrame0,
                                                             SmartPtr<IPhysicsBody3> actor1,
                                                             const Transform3<real_Num> &localFrame1 )
        {
            return nullptr;
        }

        SmartPtr<IConstraintFixed3> PhysicsManager::addFixedJoint(
            SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
            SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 )
        {
            return nullptr;
        }

        SmartPtr<IConstraintDrive> PhysicsManager::addConstraintDrive()
        {
            return nullptr;
        }

        SmartPtr<IConstraintLinearLimit> PhysicsManager::addConstraintLinearLimit(
            real_Num extent, real_Num contactDist )
        {
            return nullptr;
        }

        SmartPtr<IRaycastHit> PhysicsManager::addRaycastHitData()
        {
            return nullptr;
        }

        void PhysicsManager::removeRaycastHitData( SmartPtr<IRaycastHit> raycastHitData )
        {
        }

        Thread::Task PhysicsManager::getStateTask() const
        {
            return static_cast<Thread::Task>( 0 );
        }

        Thread::Task PhysicsManager::getPhysicsTask() const
        {
            return static_cast<Thread::Task>( 0 );
        }

        void PhysicsManager::loadObject( SmartPtr<ISharedObject> object, bool forceQueue /*= false */ )
        {
        }

        void PhysicsManager::unloadObject( SmartPtr<ISharedObject> object,
                                            bool forceQueue /*= false */ )
        {
        }

        SmartPtr<IRigidDynamic3> PhysicsManager::addRigidDynamic(
            const Transform3<real_Num> &transform )
        {
            return nullptr;
        }
    }  // end namespace physics
}  // end namespace fb
