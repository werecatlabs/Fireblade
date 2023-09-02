#ifndef FBPhysxMaterial_h__
#define FBPhysxMaterial_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/RawPtr.h>

namespace fb
{
    namespace physics
    {
        class PhysxMaterial : public IPhysicsMaterial3
        {
        public:
            PhysxMaterial();
            ~PhysxMaterial() override;

            void setFriction( f32 friction, int direction ) override;
            void setRestitution( f32 restitution ) override;

            Vector3<real_Num> getContactPosition() const override;
            Vector3<real_Num> getContactNormal() const override;

            SmartPtr<IRigidBody3> getPhysicsBodyA() const override;
            SmartPtr<IRigidBody3> getPhysicsBodyB() const override;

            RawPtr<physx::PxMaterial> getMaterial() const;
            void setMaterial( RawPtr<physx::PxMaterial> material );

            /** @copydoc IPhysicsMaterial3::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IPhysicsMaterial3::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

        protected:
            RawPtr<physx::PxMaterial> m_material;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxMaterial_h__
