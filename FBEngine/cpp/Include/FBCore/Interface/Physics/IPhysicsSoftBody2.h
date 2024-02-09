#ifndef __IPhysicsSoftBody2__H
#define __IPhysicsSoftBody2__H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include "FBCore/Math/AABB2.h"
#include "FBCore/Math/Vector2.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Core/StringTypes.h"

namespace fb
{
    namespace physics
    {

        class IPhysicsSoftBody2 : public ISharedObject
        {
        public:
            ~IPhysicsSoftBody2() override = default;

            /** */
            virtual void setPosition( const Vector2<real_Num> &position ) = 0;

            /** */
            virtual const Vector2<real_Num> &getPosition() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace physics
}  // namespace fb

#endif
