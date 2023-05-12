#ifndef GraphicsMeshState_h__
#define GraphicsMeshState_h__

#include <FBCore/State/States/GraphicsObjectState.h>

namespace fb
{
    class GraphicsMeshState : public GraphicsObjectState
    {
    public:
        GraphicsMeshState();
        ~GraphicsMeshState() override;

        SmartPtr<IState> clone() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
    };
}  // end namespace fb

#endif  // GraphicsMeshState_h__
