#ifndef CameraState_h__
#define CameraState_h__

#include <FBCore/State/States/GraphicsObjectState.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    class CameraState : public GraphicsObjectState
    {
    public:
        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // CameraState_h__
