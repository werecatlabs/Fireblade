//
// Created by Zane Desir on 31/10/2021.
//
#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/CRigidDynamic.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {

        void CRigidDynamic::setKinematicTarget( const Transform3<real_Num> &destination )
        {
        }

        bool CRigidDynamic::getKinematicTarget( Transform3<real_Num> &target )
        {
            return false;
        }

        bool CRigidDynamic::isKinematic() const
        {
            return false;
        }

        void CRigidDynamic::setKinematic( bool kinematic )
        {
        }

        void CRigidDynamic::setLinearDamping( real_Num damping )
        {
        }

        real_Num CRigidDynamic::getLinearDamping() const
        {
            return 0;
        }

        void CRigidDynamic::setAngularDamping( real_Num damping )
        {
        }

        real_Num CRigidDynamic::getAngularDamping() const
        {
            return 0;
        }

        void CRigidDynamic::setMaxAngularVelocity( real_Num maxAngVel )
        {
        }

        real_Num CRigidDynamic::getMaxAngularVelocity() const
        {
            return 0;
        }

        bool CRigidDynamic::isSleeping() const
        {
            return false;
        }

        void CRigidDynamic::setSleepThreshold( real_Num threshold )
        {
        }

        real_Num CRigidDynamic::getSleepThreshold() const
        {
            return 0;
        }

        void CRigidDynamic::setStabilizationThreshold( real_Num threshold )
        {
        }

        real_Num CRigidDynamic::getStabilizationThreshold() const
        {
            return 0;
        }

        void CRigidDynamic::setWakeCounter( real_Num wakeCounterValue )
        {
        }

        real_Num CRigidDynamic::getWakeCounter() const
        {
            return 0;
        }

        void CRigidDynamic::wakeUp()
        {
        }

        void CRigidDynamic::putToSleep()
        {
        }

        void CRigidDynamic::setSolverIterationCounts( u32 minPositionIters,
                                                      u32 minVelocityIters /*= 1 */ )
        {
        }

        void CRigidDynamic::getSolverIterationCounts( u32 &minPositionIters,
                                                      u32 &minVelocityIters ) const
        {
        }

        real_Num CRigidDynamic::getContactReportThreshold() const
        {
            return 0;
        }

        void CRigidDynamic::setContactReportThreshold( real_Num threshold )
        {
        }

    }  // end namespace physics
}  // end namespace fb
