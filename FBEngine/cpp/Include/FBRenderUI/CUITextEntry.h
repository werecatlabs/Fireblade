#ifndef _CGUITextEntry_H
#define _CGUITextEntry_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUITextEntry.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {

        class CUITextEntry : public CUIElement<IUITextEntry>
        {
        public:
            CUITextEntry();
            ~CUITextEntry() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void update() override;

            void setText( const String &text ) override;
            String getText() const override;

            void setTextSize( f32 textSize ) override;
            f32 getTextSize() const override;

            //
            // Callbacks
            //
            void onToggleVisibility() override;
            void onToggleHighlight() override;

            void setVerticalAlignment( u8 alignment ) override;

            u8 getVerticalAlignment() const override;

            void setHorizontalAlignment( u8 alignment ) override;

            u8 getHorizontalAlignment() const override;

        private:
            String m_text;
            SmartPtr<render::IOverlayElementText> m_overlayTxt;

            String m_defaultMaterial;
            String m_hoverMaterial;

            u32 m_nextCursorFlash;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
