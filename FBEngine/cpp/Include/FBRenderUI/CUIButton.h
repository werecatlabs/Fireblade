#ifndef _CGUIBUTTON_H
#define _CGUIBUTTON_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIButton.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {

        class CUIButton : public CUIElement<IUIButton>
        {
        public:
            CUIButton();
            ~CUIButton() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void setPosition( const Vector2F &position ) override;

            String getLabel() const override;
            void setLabel( const String &label ) override;

            void setTextSize( f32 textSize ) override;
            f32 getTextSize() const override;

            bool isSimpleButton() const;
            void setSimpleButton( bool simpleButton );

            void handleStateChanged( SmartPtr<IState> &state );

            void handleButtonClick();

        protected:
            String m_defaultMaterial;
            String m_hoverMaterial;
            bool m_isSimpleButton = true;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
