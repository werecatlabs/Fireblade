#ifndef FBPhysxVehicleWheel3_h__
#define FBPhysxVehicleWheel3_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsVehicleWheel3.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        class PhysxVehicleWheel3 : public IPhysicsVehicleWheel3
        {
        public:
            PhysxVehicleWheel3();
            ~PhysxVehicleWheel3() override;

            void initialise( const Vector3F &pos, f32 radius, f32 width, f32 suspensionRestLength,
                             f32 suspension_Ks, f32 suspension_Kd, bool powered, bool steering,
                             bool brakes ) override;

            Vector3F getPosition() const override;

            QuaternionF getOrientation() const override;

            Vector3F getVelocity() const override;

            void setMaterialId( u32 materialId ) override;

            u32 getMaterialId() const override;

            f32 getAngularVelocity() const override;

            AABB3F getLocalAABB() const override;

            AABB3F getWorldAABB() const override;

        protected:
        };

    }  // namespace physics
}  // namespace fb

// end namespace fb

#endif  // FBPhysxVehicleWheel3_h__
