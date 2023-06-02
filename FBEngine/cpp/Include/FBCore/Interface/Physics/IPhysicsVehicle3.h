#ifndef __IPhysicsVehicle__H
#define __IPhysicsVehicle__H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include "FBCore/Math/AABB3.h"
#include <FBCore/Math/Vector3.h>
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Math/Transform3.h"

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        /** */
        class IPhysicsVehicle3 : public ISharedObject
        {
        public:
            /** */
            ~IPhysicsVehicle3() override = default;

            /** */
            virtual IPhysicsVehicleWheel3 *addWheel() = 0;

            /** */
            virtual IPhysicsVehicleWheel3 *getWheel( u32 wheelIndex ) const = 0;

            /** */
            virtual u32 getNumWheels() const = 0;

            /** */
            virtual void finalize() = 0;

            /** */
            virtual void applyEngineForce( f32 engineForce, u32 wheelIndex ) = 0;

            /** */
            virtual void setBrake( f32 brakeForce, u32 wheelIndex ) = 0;

            /** */
            virtual void setSteeringValue( f32 steeringValue, u32 wheelIndex ) = 0;

            /** */
            virtual void setPosition( const Vector3F &position ) = 0;

            /** */
            virtual Vector3F getPosition() const = 0;

            /** */
            virtual void setOrientation( const QuaternionF &orientation ) = 0;

            /** */
            virtual QuaternionF getOrientation() const = 0;

            /** */
            virtual void setVelocity( const Vector3F &velocity ) = 0;

            /** */
            virtual Vector3F getVelocity() const = 0;

            /** */
            virtual void setMaterialId( u32 materialId ) = 0;

            /** */
            virtual u32 getMaterialId() const = 0;

            /** */
            virtual AABB3F getLocalAABB() const = 0;

            /** */
            virtual AABB3F getWorldAABB() const = 0;

            /** */
            virtual void setEnabled( bool enabled ) = 0;

            /** */
            virtual bool isEnabled() const = 0;

            /** */
            virtual SmartPtr<IPhysicsVehicleInput3> &getVehicleInput() = 0;

            /** */
            virtual const SmartPtr<IPhysicsVehicleInput3> &getVehicleInput() const = 0;

            /** */
            virtual Array<Transform3F> getWheelTransformations() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
