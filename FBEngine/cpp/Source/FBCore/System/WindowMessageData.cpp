#include <FBCore/FBCorePCH.h>
#include <FBCore/System/WindowMessageData.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, WindowMessageData, CSharedObject<ISharedObject> );

    void *WindowMessageData::getWindowHandle() const
    {
        return m_windowHandle;
    }

    void WindowMessageData::setWindowHandle( void *windowHandle )
    {
        m_windowHandle = windowHandle;
    }

    u32 WindowMessageData::getMessage() const
    {
        return m_message;
    }

    void WindowMessageData::setMessage( u32 message )
    {
        m_message = message;
    }

    size_t WindowMessageData::getWParam() const
    {
        return m_wParam;
    }

    void WindowMessageData::setWParam( size_t wParam )
    {
        m_wParam = wParam;
    }

    size_t WindowMessageData::getLParam() const
    {
        return m_lParam;
    }

    void WindowMessageData::setLParam( size_t lParam )
    {
        m_lParam = lParam;
    }

    void *WindowMessageData::getEvent() const
    {
        return m_event;
    }

    void WindowMessageData::setEvent( void *event )
    {
        m_event = event;
    }

    void *WindowMessageData::getSelf() const
    {
        return m_self;
    }

    void WindowMessageData::setSelf( void *self )
    {
        m_self = self;
    }

}  // end namespace fb
