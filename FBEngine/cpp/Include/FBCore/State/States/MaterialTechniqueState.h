#ifndef MaterialTechniqueState_h__
#define MaterialTechniqueState_h__

#include <FBCore/State/States/BaseState.h>

namespace fb
{

    class MaterialTechniqueState : public BaseState
    {
    public:
        MaterialTechniqueState() = default;
        ~MaterialTechniqueState() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // MaterialTechniqueState_h__
