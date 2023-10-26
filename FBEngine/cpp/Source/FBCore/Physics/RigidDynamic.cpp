//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/RigidDynamic.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {

        void RigidDynamic::setKinematicTarget( const Transform3<real_Num> &destination )
        {
        }

        bool RigidDynamic::getKinematicTarget( Transform3<real_Num> &target )
        {
            return false;
        }

        bool RigidDynamic::isKinematic() const
        {
            return false;
        }

        void RigidDynamic::setKinematic( bool kinematic )
        {
        }

        void RigidDynamic::setLinearDamping( real_Num damping )
        {
        }

        real_Num RigidDynamic::getLinearDamping() const
        {
            return 0;
        }

        void RigidDynamic::setAngularDamping( real_Num damping )
        {
        }

        real_Num RigidDynamic::getAngularDamping() const
        {
            return 0;
        }

        void RigidDynamic::setMaxAngularVelocity( real_Num maxAngVel )
        {
        }

        real_Num RigidDynamic::getMaxAngularVelocity() const
        {
            return 0;
        }

        bool RigidDynamic::isSleeping() const
        {
            return false;
        }

        void RigidDynamic::setSleepThreshold( real_Num threshold )
        {
        }

        real_Num RigidDynamic::getSleepThreshold() const
        {
            return 0;
        }

        void RigidDynamic::setStabilizationThreshold( real_Num threshold )
        {
        }

        real_Num RigidDynamic::getStabilizationThreshold() const
        {
            return 0;
        }

        void RigidDynamic::setWakeCounter( real_Num wakeCounterValue )
        {
        }

        real_Num RigidDynamic::getWakeCounter() const
        {
            return 0;
        }

        void RigidDynamic::wakeUp()
        {
        }

        void RigidDynamic::putToSleep()
        {
        }

        void RigidDynamic::setSolverIterationCounts( u32 minPositionIters,
                                                      u32 minVelocityIters /*= 1 */ )
        {
        }

        void RigidDynamic::getSolverIterationCounts( u32 &minPositionIters,
                                                      u32 &minVelocityIters ) const
        {
        }

        real_Num RigidDynamic::getContactReportThreshold() const
        {
            return 0;
        }

        void RigidDynamic::setContactReportThreshold( real_Num threshold )
        {
        }

    }  // end namespace physics
}  // end namespace fb
