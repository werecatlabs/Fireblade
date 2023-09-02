#pragma once
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    class PlatformInputManager : public ISharedObject
    {
    public:
        PlatformInputManager() = default;
        ~PlatformInputManager() = default;

        SmartPtr<render::IWindow> getWindow() const;

        void setWindow( SmartPtr<render::IWindow> window );

    protected:
        class WindowListener : public render::IWindowListener
        {
        public:
            WindowListener() = default;
            ~WindowListener() = default;

            fb::Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            void handleEvent( SmartPtr<render::IWindowEvent> event );

            Vector2F m_lastMousePosition;
        };

        SmartPtr<render::IWindow> m_window;
        SmartPtr<render::IWindowListener> m_listener;
    };

}  // namespace fb
