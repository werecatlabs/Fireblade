#ifndef CompositorState_h__
#define CompositorState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    class CompositorState : public BaseState
    {
    public:
        CompositorState();
        ~CompositorState() override;

        bool isEnabled() const;
        void setEnabled( bool enabled );

        FB_CLASS_REGISTER_DECL;

    protected:
        bool m_enabled = false;
    };
} // namespace fb

#endif  // CompositorState_h__
