#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIEvent.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIEvent::CUIEvent()
        {
            setType( String( "Event" ) );
        }

        CUIEvent::~CUIEvent()
        {
        }

        void CUIEvent::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
        }

        void CUIEvent::setEventType( const String &eventType )
        {
            m_eventType = eventType;
        }

        const String &CUIEvent::getEventType() const
        {
            return m_eventType;
        }

        //
        // Callbacks
        //

        void CUIEvent::OnActivateCallback()
        {
        }

        void CUIEvent::OnSelectCallback()
        {
        }

        void CUIEvent::OnDeselectCallback()
        {
        }
    }  // namespace ui
}  // end namespace fb
