#include <FBCore/FBCorePCH.h>
#include <FBCore/System/WindowMessageData.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, WindowMessageData, ISharedObject );

    auto WindowMessageData::getWindowHandle() const -> void *
    {
        return m_windowHandle;
    }

    void WindowMessageData::setWindowHandle( void *windowHandle )
    {
        m_windowHandle = windowHandle;
    }

    auto WindowMessageData::getMessage() const -> u32
    {
        return m_message;
    }

    void WindowMessageData::setMessage( u32 message )
    {
        m_message = message;
    }

    auto WindowMessageData::getWParam() const -> size_t
    {
        return m_wParam;
    }

    void WindowMessageData::setWParam( size_t wParam )
    {
        m_wParam = wParam;
    }

    auto WindowMessageData::getLParam() const -> size_t
    {
        return m_lParam;
    }

    void WindowMessageData::setLParam( size_t lParam )
    {
        m_lParam = lParam;
    }

    auto WindowMessageData::getEvent() const -> void *
    {
        return m_event;
    }

    void WindowMessageData::setEvent( void *event )
    {
        m_event = event;
    }

    auto WindowMessageData::getSelf() const -> void *
    {
        return m_self;
    }

    void WindowMessageData::setSelf( void *self )
    {
        m_self = self;
    }

}  // end namespace fb
