#ifndef _CGUIToggleButton_H
#define _CGUIToggleButton_H

#include <FBCore/FBCorePrerequisites.h>
#include "CUIButton.h"
#include "CUIToggleGroup.h"
#include <FBRenderUI/CUIElement.h>
#include <FBCore/Interface/UI/IUIToggleButton.h>

namespace fb
{
    namespace ui
    {
        
        class CUIToggleButton : public CUIElement<IUIToggleButton>
        {
        public:
            CUIToggleButton();
            ~CUIToggleButton() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void setToggled( bool toggled ) override;
            bool isToggled() const override;

        private:
            SmartPtr<CUIToggleGroup> m_toggleGroup;
            String m_toggleMaterial;
            bool m_isToggled;
        };
    } // end namespace ui
}     // end namespace fb

#endif
