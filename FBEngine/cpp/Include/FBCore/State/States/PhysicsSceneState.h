#ifndef PhysicsSceneState_h__
#define PhysicsSceneState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/State/States/RigidbodyState.h>

namespace fb
{
    class PhysicsSceneState : public BaseState
    {
    public:
        PhysicsSceneState() = default;
        ~PhysicsSceneState() override = default;

        FixedArray<RigidbodyState, 1024> m_rigidBodies;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // RigidbodyState_h__
