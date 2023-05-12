#ifndef __IPhysicsVehicleWheel__H
#define __IPhysicsVehicleWheel__H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include "FBCore/Math/AABB3.h"
#include <FBCore/Math/Vector3.h>
#include "FBCore/Math/Quaternion.h"

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        /** */
        class IPhysicsVehicleWheel3 : public ISharedObject
        {
        public:
            /** */
            ~IPhysicsVehicleWheel3() override = default;

            virtual void initialise( const Vector3F &pos, f32 radius, f32 width,
                                     f32 suspensionRestLength, f32 suspension_Ks, f32 suspension_Kd,
                                     bool powered, bool steering, bool brakes ) = 0;

            /** */
            virtual Vector3F getPosition() const = 0;

            /** */
            virtual QuaternionF getOrientation() const = 0;

            /** */
            virtual Vector3F getVelocity() const = 0;

            /** */
            virtual void setMaterialId( u32 materialId ) = 0;

            /** */
            virtual u32 getMaterialId() const = 0;

            /** */
            virtual f32 getAngularVelocity() const = 0;

            /** */
            virtual AABB3F getLocalAABB() const = 0;

            /** */
            virtual AABB3F getWorldAABB() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
