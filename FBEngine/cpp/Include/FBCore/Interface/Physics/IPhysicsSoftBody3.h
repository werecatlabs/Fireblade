#ifndef __IPhysicsSoftBody3__H
#define __IPhysicsSoftBody3__H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include "FBCore/Math/AABB3.h"
#include <FBCore/Math/Vector3.h>
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Core/StringTypes.h"

namespace fb
{
    namespace physics
    {

        class IPhysicsSoftBody3 : public ISharedObject
        {
        public:
            ~IPhysicsSoftBody3() override = default;

            /** */
            virtual void setPosition( const Vector3F &position ) = 0;

            /** */
            virtual const Vector3F &getPosition() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace physics
}  // end namespace fb

#endif
