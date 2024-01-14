#ifndef FBPhysxMaterial_h__
#define FBPhysxMaterial_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>

namespace fb
{
    namespace physics
    {

        /** Physics material implementation. */
        class PhysxMaterial : public IPhysicsMaterial3
        {
        public:
            /** Default constructor. */
            PhysxMaterial();

            /** Destructor. */
            ~PhysxMaterial() override;

            /** @copydoc IPhysicsMaterial3::setFriction */
            void setFriction( f32 friction, int direction ) override;

            /** @copydoc IPhysicsMaterial3::setRestitution */
            void setRestitution( f32 restitution ) override;

            /** @copydoc IPhysicsMaterial3::getContactPosition */
            Vector3<real_Num> getContactPosition() const override;

            /** @copydoc IPhysicsMaterial3::getContactNormal */
            Vector3<real_Num> getContactNormal() const override;

            /** @copydoc IPhysicsMaterial3::getPhysicsBodyA */
            SmartPtr<IRigidBody3> getPhysicsBodyA() const override;

            /** @copydoc IPhysicsMaterial3::getPhysicsBodyB */
            SmartPtr<IRigidBody3> getPhysicsBodyB() const override;

            RawPtr<physx::PxMaterial> getMaterial() const;
            void setMaterial( RawPtr<physx::PxMaterial> material );

            /** @copydoc IPhysicsMaterial3::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IPhysicsMaterial3::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

        protected:
            /** The PhysX material. */
            RawPtr<physx::PxMaterial> m_material;
        };

    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxMaterial_h__
