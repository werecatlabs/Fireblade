//
// Created by Zane Desir on 31/10/2021.
//

#ifndef FB_CRIGIDSTATIC_H
#define FB_CRIGIDSTATIC_H

#include <FBCore/Interface/Physics/IRigidStatic3.h>
#include <FBCore/Interface/System/IStateListener.h>

#include <FBCore/Physics/CRigidBody3.h>

namespace fb
{
    namespace physics
    {
        class CRigidStatic : public CRigidBody3<IRigidStatic3>
        {
        public:
            CRigidStatic() = default;
            ~CRigidStatic() = default;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FB_CRIGIDSTATIC_H
