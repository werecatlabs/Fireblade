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

namespace fb::physics
{

    PhysicsManager::PhysicsManager() = default;

    PhysicsManager::~PhysicsManager() = default;

    void PhysicsManager::load( SmartPtr<ISharedObject> data )
    {
    }

    void PhysicsManager::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto PhysicsManager::getEnableDebugDraw() const -> bool
    {
        return false;
    }

    void PhysicsManager::setEnableDebugDraw( bool enableDebugDraw )
    {
    }

    auto PhysicsManager::addMaterial() -> SmartPtr<IPhysicsMaterial3>
    {
        return nullptr;
    }

    void PhysicsManager::removeMaterial( SmartPtr<IPhysicsMaterial3> material )
    {
    }

    auto PhysicsManager::addScene() -> SmartPtr<IPhysicsScene3>
    {
        return nullptr;
    }

    void PhysicsManager::removeScene( SmartPtr<IPhysicsScene3> scene )
    {
    }

    auto PhysicsManager::removeCollisionShape( SmartPtr<IPhysicsShape3> collisionShape ) -> bool
    {
        return false;
    }

    auto PhysicsManager::removePhysicsBody( SmartPtr<IRigidBody3> body ) -> bool
    {
        return false;
    }

    auto PhysicsManager::addCharacter() -> SmartPtr<ICharacterController3>
    {
        return nullptr;
    }

    auto PhysicsManager::addRigidStatic( const Transform3<real_Num> &transform )
        -> SmartPtr<IRigidStatic3>
    {
        return nullptr;
    }

    auto PhysicsManager::addRigidStatic( SmartPtr<IPhysicsShape3> collisionShape )
        -> SmartPtr<IRigidStatic3>
    {
        return nullptr;
    }

    auto PhysicsManager::addRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                         SmartPtr<Properties> properties ) -> SmartPtr<IRigidStatic3>
    {
        return nullptr;
    }

    auto PhysicsManager::addSoftBody( const String &filePath ) -> SmartPtr<IPhysicsSoftBody3>
    {
        return nullptr;
    }

    auto PhysicsManager::addVehicle( SmartPtr<IRigidBody3> chassis,
                                     const SmartPtr<Properties> &properties )
        -> SmartPtr<IPhysicsVehicle3>
    {
        return nullptr;
    }

    auto PhysicsManager::addVehicle( SmartPtr<scene::IDirector> vehicleTemplate )
        -> SmartPtr<IPhysicsVehicle3>
    {
        return nullptr;
    }

    auto PhysicsManager::removeVehicle( SmartPtr<IPhysicsVehicle3> vehicle ) -> bool
    {
        return false;
    }

    auto PhysicsManager::rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction,
                                  Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                  u32 collisionType /*= 0*/, u32 collisionMask /*= 0 */ ) -> bool
    {
        return false;
    }

    auto PhysicsManager::intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                     Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                     SmartPtr<ISharedObject> &object, u32 collisionType /*= 0*/,
                                     u32 collisionMask /*= 0 */ ) -> bool
    {
        return false;
    }

    auto PhysicsManager::addConstraintD6( SmartPtr<IPhysicsBody3> actor0,
                                          const Transform3<real_Num> &localFrame0,
                                          SmartPtr<IPhysicsBody3> actor1,
                                          const Transform3<real_Num> &localFrame1 )
        -> SmartPtr<IConstraintD6>
    {
        return nullptr;
    }

    auto PhysicsManager::addFixedConstraint( SmartPtr<IPhysicsBody3> actor0,
                                             const Transform3<real_Num> &localFrame0,
                                             SmartPtr<IPhysicsBody3> actor1,
                                             const Transform3<real_Num> &localFrame1 )
        -> SmartPtr<IConstraintFixed3>
    {
        return nullptr;
    }

    void PhysicsManager::removeConstraint( SmartPtr<IPhysicsConstraint3> constraint )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        unloadObject( constraint );
        m_constraints.erase( std::remove( m_constraints.begin(), m_constraints.end(), constraint ),
                             m_constraints.end() );
    }

    auto PhysicsManager::addConstraintDrive() -> SmartPtr<IConstraintDrive>
    {
        return nullptr;
    }

    auto PhysicsManager::addConstraintLinearLimit( real_Num extent, real_Num contactDist )
        -> SmartPtr<IConstraintLinearLimit>
    {
        return nullptr;
    }

    auto PhysicsManager::addRaycastHitData() -> SmartPtr<IRaycastHit>
    {
        return nullptr;
    }

    void PhysicsManager::removeRaycastHitData( SmartPtr<IRaycastHit> raycastHitData )
    {
    }

    auto PhysicsManager::getStateTask() const -> Thread::Task
    {
        return static_cast<Thread::Task>( 0 );
    }

    auto PhysicsManager::getPhysicsTask() const -> Thread::Task
    {
        return static_cast<Thread::Task>( 0 );
    }

    void PhysicsManager::loadObject( SmartPtr<ISharedObject> object, bool forceQueue /*= false */ )
    {
    }

    void PhysicsManager::unloadObject( SmartPtr<ISharedObject> object, bool forceQueue /*= false */ )
    {
    }

    void PhysicsManager::lock()
    {
        m_mutex.lock();
    }

    void PhysicsManager::unlock()
    {
        m_mutex.unlock();
    }

    auto PhysicsManager::addRigidDynamic( const Transform3<real_Num> &transform )
        -> SmartPtr<IRigidDynamic3>
    {
        return nullptr;
    }
}  // namespace fb::physics
