#ifndef _CGUIEVENT_H
#define _CGUIEVENT_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIEvent : public CUIElement<IUIElement>
        {
        public:
            CUIEvent();
            ~CUIEvent() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            void setEventType( const String &eventType );
            const String &getEventType() const;

            //
            // Callbacks
            //
            void OnActivateCallback();
            void OnSelectCallback();
            void OnDeselectCallback();

        private:
            String m_eventType;
        };
    } // end namespace ui
}     // end namespace fb

#endif
