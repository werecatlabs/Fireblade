#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIEvent.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{

    CUIEvent::CUIEvent()
    {
        setType( String( "Event" ) );
    }

    CUIEvent::~CUIEvent() = default;

    void CUIEvent::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
    {
    }

    void CUIEvent::setEventType( const String &eventType )
    {
        m_eventType = eventType;
    }

    auto CUIEvent::getEventType() const -> const String &
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
}  // namespace fb::ui
