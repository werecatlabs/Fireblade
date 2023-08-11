//
// Created by Zane Desir on 31/10/2021.
//

#ifndef FB_CRIGIDDYNAMIC_H
#define FB_CRIGIDDYNAMIC_H

#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Physics/CRigidBody3.h>

namespace fb
{
    namespace physics
    {
        class CRigidDynamic : public CRigidBody3<IRigidDynamic3>
        {
        public:
            CRigidDynamic() = default;
            ~CRigidDynamic() = default;

            void setKinematicTarget( const Transform3<real_Num> &destination ) override;

            bool getKinematicTarget( Transform3<real_Num> &target ) override;

            bool isKinematic() const override;

            void setKinematic( bool kinematic ) override;

            void setLinearDamping( real_Num damping ) override;

            real_Num getLinearDamping() const override;

            void setAngularDamping( real_Num damping ) override;

            real_Num getAngularDamping() const override;

            void setMaxAngularVelocity( real_Num maxAngVel ) override;

            real_Num getMaxAngularVelocity() const override;

            bool isSleeping() const override;

            void setSleepThreshold( real_Num threshold ) override;

            real_Num getSleepThreshold() const override;

            void setStabilizationThreshold( real_Num threshold ) override;

            real_Num getStabilizationThreshold() const override;

            void setWakeCounter( real_Num wakeCounterValue ) override;

            real_Num getWakeCounter() const override;

            void wakeUp() override;

            void putToSleep() override;

            void setSolverIterationCounts( u32 minPositionIters, u32 minVelocityIters = 1 ) override;

            void getSolverIterationCounts( u32 &minPositionIters, u32 &minVelocityIters ) const override;

            real_Num getContactReportThreshold() const override;

            void setContactReportThreshold( real_Num threshold ) override;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FB_CRIGIDDYNAMIC_H
