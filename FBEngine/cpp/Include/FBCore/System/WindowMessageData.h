#ifndef WindowMessageData_h__
#define WindowMessageData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/Graphics/IWindowEvent.h>

namespace fb
{
    
    class WindowMessageData : public SharedObject<render::IWindowEvent>
    {
    public:
        WindowMessageData() = default;
        ~WindowMessageData() override = default;

        void *getWindowHandle() const;
        void setWindowHandle( void *windowHandle );

        u32 getMessage() const;
        void setMessage( u32 message );

        size_t getWParam() const;
        void setWParam( size_t wParam );

        size_t getLParam() const;
        void setLParam( size_t val );

        void *getEvent() const;

        void setEvent( void *event );

        void *getSelf() const;

        void setSelf( void *self );

        FB_CLASS_REGISTER_DECL;

    private:
        void *m_windowHandle = nullptr;
        u32 m_message = 0;
        size_t m_wParam = 0;
        size_t m_lParam = 0;

        void *m_event = nullptr;
        void *m_self = nullptr;
    };
}  // end namespace fb

#endif  // WindowMessageData_h__
