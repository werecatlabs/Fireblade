//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/RigidDynamic.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb::physics
{

    void RigidDynamic::setKinematicTarget( const Transform3<real_Num> &destination )
    {
    }

    auto RigidDynamic::getKinematicTarget( Transform3<real_Num> &target ) -> bool
    {
        return false;
    }

    auto RigidDynamic::isKinematic() const -> bool
    {
        return false;
    }

    void RigidDynamic::setKinematic( bool kinematic )
    {
    }

    void RigidDynamic::setLinearDamping( real_Num damping )
    {
    }

    auto RigidDynamic::getLinearDamping() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::setAngularDamping( real_Num damping )
    {
    }

    auto RigidDynamic::getAngularDamping() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::setMaxAngularVelocity( real_Num maxAngVel )
    {
    }

    auto RigidDynamic::getMaxAngularVelocity() const -> real_Num
    {
        return 0;
    }

    auto RigidDynamic::isSleeping() const -> bool
    {
        return false;
    }

    void RigidDynamic::setSleepThreshold( real_Num threshold )
    {
    }

    auto RigidDynamic::getSleepThreshold() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::setStabilizationThreshold( real_Num threshold )
    {
    }

    auto RigidDynamic::getStabilizationThreshold() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::setWakeCounter( real_Num wakeCounterValue )
    {
    }

    auto RigidDynamic::getWakeCounter() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::wakeUp()
    {
    }

    void RigidDynamic::putToSleep()
    {
    }

    void RigidDynamic::setSolverIterationCounts( u32 minPositionIters, u32 minVelocityIters /*= 1 */ )
    {
    }

    void RigidDynamic::getSolverIterationCounts( u32 &minPositionIters, u32 &minVelocityIters ) const
    {
    }

    auto RigidDynamic::getContactReportThreshold() const -> real_Num
    {
        return 0;
    }

    void RigidDynamic::setContactReportThreshold( real_Num threshold )
    {
    }

}  // namespace fb::physics
