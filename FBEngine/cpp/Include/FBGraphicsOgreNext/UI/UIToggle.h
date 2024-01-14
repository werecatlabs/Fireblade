#ifndef UIToggle_h__
#define UIToggle_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUIToggle.h>

namespace fb
{
    namespace ui
    {
        class UIToggle : public UIElement<IUIToggle>
        {
        public:
            UIToggle();
            ~UIToggle() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setLabel( const String &text ) override;
            String getLabel() const override;

            void setToggled( bool checked ) override;
            bool isToggled() const override;

            void handleStateChanged( SmartPtr<IState> &state ) override;

            void setTextSize( f32 textSize ) override;

            f32 getTextSize() const override;

            ToggleType getToggleType() const;

            void setToggleType( ToggleType toggleType );

            ToggleState getToggleState() const;

            void setToggleState( ToggleState toggleState );

            bool getShowLabel() const;

            void setShowLabel( bool showLabel );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IUIText> m_label;
            SmartPtr<IUIImage> m_bgImage;
            SmartPtr<IUIImage> m_toggleImage;

            Colibri::Checkbox *m_checkbox = nullptr;

            String m_text;

            bool m_checked = false;
            bool m_showLabel = true;

            ToggleType m_toggleType = ToggleType::CheckBox;
            ToggleState m_toggleState = ToggleState::Off;
        };
    }  // namespace ui
}  // namespace fb

#endif  // UIToggle_h__
