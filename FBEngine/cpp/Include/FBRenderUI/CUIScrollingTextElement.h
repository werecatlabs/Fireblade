#ifndef _CGUIScrollingTxtElement_H
#define _CGUIScrollingTxtElement_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {

        class CUIScrollingTextElement : public CUIElement<IUIElement>
        {
        public:
            CUIScrollingTextElement();
            ~CUIScrollingTextElement() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

        private:
            // Callbacks
            void onToggleHighlight() override;
            void onToggleVisibility() override;

            SmartPtr<render::IOverlayElementContainer> m_container;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
