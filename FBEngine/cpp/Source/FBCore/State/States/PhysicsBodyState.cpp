#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/PhysicsBodyState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, PhysicsBodyState, BaseState );

    PhysicsBodyState::PhysicsBodyState() = default;

    PhysicsBodyState::~PhysicsBodyState() = default;
}  // end namespace fb
