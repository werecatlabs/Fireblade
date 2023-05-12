#ifndef ShapeState_h__
#define ShapeState_h__

#include <FBCore/State/States/BaseState.h>

namespace fb
{

    class ShapeState : public BaseState
    {
    public:
        ShapeState();
        ~ShapeState();

        FB_CLASS_REGISTER_DECL;

    protected:
    };

}  // end namespace fb

#endif // ShapeState_h__
