#ifndef UIButton_h__
#define UIButton_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUIButton.h>

namespace fb
{
    namespace ui
    {

        class UIButton : public UIElement<ui::IUIButton>
        {
        public:
            UIButton();
            ~UIButton();

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            String getLabel() const override;

            void setLabel( const String &label ) override;

            void setTextSize( f32 textSize ) override;

            f32 getTextSize() const override;

            void handleStateChanged( SmartPtr<IState> &state );

        protected:
            Colibri::Button *m_button = nullptr;
            String m_label;
            f32 m_textSize = 1.0;
        };

    }  // namespace ui
}  // namespace fb

#endif  // UIButton_h__
