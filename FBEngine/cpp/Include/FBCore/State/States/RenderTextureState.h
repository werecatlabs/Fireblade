#ifndef RenderTextureState_h__
#define RenderTextureState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class RenderTextureState : public BaseState
    {
    public:
        RenderTextureState() = default;
        ~RenderTextureState() override = default;

        SmartPtr<IState> clone();

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // RenderTextureState_h__
