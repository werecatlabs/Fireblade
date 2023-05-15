#ifndef FBPhysxConstraintFixed3_h__
#define FBPhysxConstraintFixed3_h__

#include <FBPhysx/FBPhysxConstraint.h>
#include <FBCore/Interface/Physics/IConstraintFixed3.h>

namespace fb
{
    namespace physics
    {
        class PhysxConstraintFixed3 : public PhysxConstraint<IConstraintFixed3>
        {
        public:
            PhysxConstraintFixed3();
            ~PhysxConstraintFixed3() override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            RawPtr<physx::PxFixedJoint> getJoint() const;
            void setJoint( RawPtr<physx::PxFixedJoint> joint );

        private:
            RawPtr<physx::PxFixedJoint> m_joint;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxConstraintFixed3_h__
