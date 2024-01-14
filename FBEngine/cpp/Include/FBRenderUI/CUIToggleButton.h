#ifndef _CGUIToggleButton_H
#define _CGUIToggleButton_H

#include <FBCore/FBCorePrerequisites.h>
#include "CUIButton.h"
#include "CUIToggleGroup.h"
#include <FBRenderUI/CUIElement.h>
#include <FBCore/Interface/UI/IUIToggle.h>

namespace fb
{
    namespace ui
    {

        class CUIToggleButton : public CUIElement<IUIToggle>
        {
        public:
            CUIToggleButton();
            ~CUIToggleButton() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void setToggled( bool toggled ) override;
            bool isToggled() const override;

            ToggleType getToggleType() const override;

            void setToggleType( ToggleType toggleType ) override;

            ToggleState getToggleState() const override;

            void setToggleState( ToggleState toggleState ) override;

            String getLabel() const override;

            void setLabel( const String &label ) override;

            void setTextSize( f32 textSize ) override;

            f32 getTextSize() const override;

        private:
            SmartPtr<CUIToggleGroup> m_toggleGroup;
            String m_toggleMaterial;
            bool m_isToggled;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
