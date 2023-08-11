//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/CPhysicsManager.h>
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
        CPhysicsManager::CPhysicsManager()
        {
        }

        CPhysicsManager::~CPhysicsManager()
        {
        }

        void CPhysicsManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void CPhysicsManager::unload( SmartPtr<ISharedObject> data )
        {
        }

        bool CPhysicsManager::getEnableDebugDraw() const
        {
            return false;
        }

        void CPhysicsManager::setEnableDebugDraw( bool enableDebugDraw )
        {
        }

        SmartPtr<IPhysicsMaterial3> CPhysicsManager::createMaterial()
        {
            return nullptr;
        }

        void CPhysicsManager::destroyMaterial( SmartPtr<IPhysicsMaterial3> material )
        {
        }

        SmartPtr<IPhysicsScene3> CPhysicsManager::createScene()
        {
            return nullptr;
        }

        void CPhysicsManager::destroyScene( SmartPtr<IPhysicsScene3> scene )
        {
        }

        bool CPhysicsManager::destroyCollisionShape( SmartPtr<IPhysicsShape3> collisionShape )
        {
            return false;
        }

        bool CPhysicsManager::destroyPhysicsBody( SmartPtr<IRigidBody3> body )
        {
            return false;
        }

        SmartPtr<ICharacterController3> CPhysicsManager::addCharacter()
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> CPhysicsManager::createRigidStatic(
            const Transform3<real_Num> &transform )
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> CPhysicsManager::createRigidStatic(
            SmartPtr<IPhysicsShape3> collisionShape )
        {
            return nullptr;
        }

        SmartPtr<IRigidStatic3> CPhysicsManager::createRigidStatic(
            SmartPtr<IPhysicsShape3> collisionShape, SmartPtr<Properties> properties )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsSoftBody3> CPhysicsManager::createSoftBody( const String &filePath )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsVehicle3> CPhysicsManager::addVehicle( SmartPtr<IRigidBody3> chassis,
                                                                const SmartPtr<Properties> &properties )
        {
            return nullptr;
        }

        SmartPtr<IPhysicsVehicle3> CPhysicsManager::addVehicle(
            SmartPtr<scene::IDirector> vehicleTemplate )
        {
            return nullptr;
        }

        bool CPhysicsManager::destroyVehicle( SmartPtr<IPhysicsVehicle3> vehicle )
        {
            return false;
        }

        bool CPhysicsManager::rayTest( const Vector3<real_Num> &start,
                                       const Vector3<real_Num> &direction, Vector3<real_Num> &hitPos,
                                       Vector3<real_Num> &hitNormal, u32 collisionType /*= 0*/,
                                       u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        bool CPhysicsManager::intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                          Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                          SmartPtr<ISharedObject> &object, u32 collisionType /*= 0*/,
                                          u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        SmartPtr<IConstraintD6> CPhysicsManager::d6JointCreate( SmartPtr<IPhysicsBody3> actor0,
                                                                const Transform3<real_Num> &localFrame0,
                                                                SmartPtr<IPhysicsBody3> actor1,
                                                                const Transform3<real_Num> &localFrame1 )
        {
            return nullptr;
        }

        SmartPtr<IConstraintFixed3> CPhysicsManager::fixedJointCreate(
            SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
            SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 )
        {
            return nullptr;
        }

        SmartPtr<IConstraintDrive> CPhysicsManager::createConstraintDrive()
        {
            return nullptr;
        }

        SmartPtr<IConstraintLinearLimit> CPhysicsManager::createConstraintLinearLimit(real_Num extent, real_Num contactDist)
        {            
            return nullptr;
        }

        SmartPtr<IRaycastHit> CPhysicsManager::createRaycastHitData()
        {
            return nullptr;
        }

        Thread::Task CPhysicsManager::getStateTask() const
        {
            return static_cast<Thread::Task>( 0 );
        }

        Thread::Task CPhysicsManager::getPhysicsTask() const
        {
            return static_cast<Thread::Task>( 0 );
        }

        void CPhysicsManager::loadObject( SmartPtr<ISharedObject> object, bool forceQueue /*= false */ )
        {
        }

        void CPhysicsManager::unloadObject( SmartPtr<ISharedObject> object,
                                            bool forceQueue /*= false */ )
        {
        }

        SmartPtr<IRigidDynamic3> CPhysicsManager::createRigidDynamic(
            const Transform3<real_Num> &transform )
        {
            return nullptr;
        }
    }  // end namespace physics
}  // end namespace fb
