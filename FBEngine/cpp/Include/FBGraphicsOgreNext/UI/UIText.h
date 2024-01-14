#ifndef UIText_h__
#define UIText_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUIText.h>

namespace fb
{
    namespace ui
    {

        class UIText : public UIElement<IUIText>
        {
        public:
            UIText();
            ~UIText() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void setText( const String &text ) override;

            String getText() const override;

            void setTextSize( f32 textSize ) override;

            f32 getTextSize() const override;

            void setVerticalAlignment( u8 alignment ) override;

            u8 getVerticalAlignment() const override;

            void setHorizontalAlignment( u8 alignment ) override;

            u8 getHorizontalAlignment() const override;

            void handleStateChanged( SmartPtr<IState> &state ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Colibri::Label *m_label = nullptr;
            String m_text;
            f32 m_textSize = 0;
            u8 m_verticalAlignment = 3;
            u8 m_horizontalAlignment = 3;
        };
    } // namespace render
}     // namespace fb

#endif  // UIText_h__
