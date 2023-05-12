#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/TransformState.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TransformState, BaseState );

    SmartPtr<IState> TransformState::clone()
    {
        return nullptr;
    }
}  // namespace fb
