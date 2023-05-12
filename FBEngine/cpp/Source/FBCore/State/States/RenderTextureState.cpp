#include <FBCore/FBCorePCH.h>
#include "FBCore/State/States/RenderTextureState.h"
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RenderTextureState, BaseState );

    SmartPtr<IState> RenderTextureState::clone()
    {
        return nullptr;
    }
}  // namespace fb
