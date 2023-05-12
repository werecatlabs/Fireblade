#ifndef MaterialTextureState_h__
#define MaterialTextureState_h__

#include <FBCore/State/States/BaseState.h>

namespace fb
{

    class MaterialTextureState : public BaseState
    {
    public:
        MaterialTextureState() = default;
        ~MaterialTextureState() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // MaterialTextureState_h__
