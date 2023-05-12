#ifndef __IPhysicsSoftBody2__H
#define __IPhysicsSoftBody2__H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include "FBCore/Math/AABB2.h"
#include "FBCore/Math/Vector2.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Base/StringTypes.h"

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        class IPhysicsSoftBody2 : public ISharedObject
        {
        public:
            ~IPhysicsSoftBody2() override = default;

            /** */
            virtual void setPosition( const Vector2F &position ) = 0;

            /** */
            virtual const Vector2F &getPosition() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace physics
}  // end namespace fb

#endif
