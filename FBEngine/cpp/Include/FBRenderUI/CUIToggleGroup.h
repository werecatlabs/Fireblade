#ifndef __CUIToggleGroup_H
#define __CUIToggleGroup_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIToggleGroup : public CUIElement<IUIElement>
        {
        public:
            CUIToggleGroup( const String &id );
            ~CUIToggleGroup() override;

            void addToggleButton( CUIToggleButton *button );
            bool removeToggleButton( CUIToggleButton *button );

            void OnSetButtonToggled( CUIToggleButton *button );

            CUIToggleButton *getCurToggledButton() const;

        private:
            Array<CUIToggleButton *> m_toggleButtons;
            CUIToggleButton *m_curToggledBtn;
        };
    } // end namespace ui
}     // end namespace fb

#endif
