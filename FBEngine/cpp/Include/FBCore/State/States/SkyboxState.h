#ifndef SkyboxState_h__
#define SkyboxState_h__

#include <FBCore/State/States/BaseState.h>

namespace fb
{
    class SkyboxState : public BaseState
    {
    public:
        SkyboxState();

        ~SkyboxState();

        FB_CLASS_REGISTER_DECL;

    protected:
    };
}  // end namespace fb

#endif  // SkyboxState_h__
