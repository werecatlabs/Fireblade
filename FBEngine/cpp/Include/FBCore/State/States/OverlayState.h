#ifndef FB_OVERLAYSTATE_H
#define FB_OVERLAYSTATE_H

#include <FBCore/State/States/BaseState.h>

namespace fb
{
    class OverlayState : public BaseState
    {
    public:
        OverlayState();
        ~OverlayState() override;

        bool isVisible() const;

        void setVisible( bool visible );

        FB_CLASS_REGISTER_DECL;

    protected:
        atomic_bool m_visible = true;
    };
} // end namespace fb

#endif  // FB_OVERLAYSTATE_H
