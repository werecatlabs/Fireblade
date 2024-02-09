#ifndef FB_OverlayTextState_H
#define FB_OverlayTextState_H

#include <FBCore/State/States/OverlayElementState.h>

namespace fb
{
    class OverlayTextState : public OverlayElementState
    {
    public:
        OverlayTextState();
        ~OverlayTextState() override;

        String getText() const;
        void setText( const String &text );

        u32 getAlignment() const;
        void setAlignment( u32 alignment );

        FB_CLASS_REGISTER_DECL;

    protected:
        String m_text;
        u32 m_alignment = 0;
    };
}  // end namespace fb

#endif  // FB_OVERLAYSTATE_H
