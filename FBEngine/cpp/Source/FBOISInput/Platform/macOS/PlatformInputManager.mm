#include <FBOISInput/Platform/macOS/PlatformInputManager.h>
#include <FBCore/FBCore.h>
#include <FBOISInput/FBInputEvent.h>
#include <FBOISInput/FBMouseState.h>

#import <Foundation/Foundation.h>

#if TARGET_OS_OSX
#    import <Cocoa/Cocoa.h>
#else
#    import <UIKit/UIKit.h>
#endif

namespace fb
{

    SmartPtr<render::IWindow> PlatformInputManager::getWindow() const
    {
        return m_window;
    }

    void PlatformInputManager::setWindow( SmartPtr<render::IWindow> window )
    {
        m_window = window;

        m_listener = fb::make_ptr<WindowListener>();
        m_window->addListener( m_listener );
    }

    void PlatformInputManager::WindowListener::handleEvent( SmartPtr<render::IWindowEvent> pEvent )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto inputManager = applicationManager->getInputDeviceManager();

        auto windowMessage = fb::static_pointer_cast<WindowMessageData>( pEvent );
        auto event = (NSEvent *)windowMessage->getEvent();
        auto view = (NSView *)windowMessage->getSelf();

        auto contentScalingFactor =
            ( [view respondsToSelector:@selector( wantsBestResolutionOpenGLSurface )] &&
              [(id)view wantsBestResolutionOpenGLSurface] )
                ? ( view.window.screen ?: [NSScreen mainScreen] ).backingScaleFactor
                : 1.0f;

        if( event.type == NSEventTypeLeftMouseDown || event.type == NSEventTypeRightMouseDown ||
            event.type == NSEventTypeOtherMouseDown )
        {
            int button = (int)[event buttonNumber];

            SmartPtr<IInputEvent> inputEvent( new InputEvent );
            inputEvent->setEventType( IInputEvent::EventType::Mouse );

            SmartPtr<IMouseState> mouseState( new MouseState );
            inputEvent->setMouseState( mouseState );

            if( event.type == NSEventTypeLeftMouseDown )
            {
                mouseState->setEventType( IMouseState::Event::LeftPressed );
            }
            else if( event.type == NSEventTypeRightMouseDown )
            {
                mouseState->setEventType( IMouseState::Event::RightPressed );
            }

            inputManager->postEvent( inputEvent );
        }

        if( event.type == NSEventTypeLeftMouseUp || event.type == NSEventTypeRightMouseUp ||
            event.type == NSEventTypeOtherMouseUp )
        {
            int button = (int)[event buttonNumber];

            SmartPtr<IInputEvent> inputEvent( new InputEvent );
            inputEvent->setEventType( IInputEvent::EventType::Mouse );

            SmartPtr<IMouseState> mouseState( new MouseState );
            inputEvent->setMouseState( mouseState );

            if( event.type == NSEventTypeLeftMouseUp )
            {
                mouseState->setEventType( IMouseState::Event::LeftReleased );
            }
            else if( event.type == NSEventTypeRightMouseUp )
            {
                mouseState->setEventType( IMouseState::Event::RightReleased );
            }

            inputManager->postEvent( inputEvent );
        }

        if( event.type == NSEventTypeMouseMoved || event.type == NSEventTypeLeftMouseDragged ||
            event.type == NSEventTypeRightMouseDragged || event.type == NSEventTypeOtherMouseDragged )
        {
            NSPoint mousePoint = event.locationInWindow;
            mousePoint = [view convertPoint:mousePoint fromView:nil];
            mousePoint = NSMakePoint( mousePoint.x, view.bounds.size.height - mousePoint.y );

            auto vSize = NSMakePoint(view.bounds.size.width, view.bounds.size.height);
            //io.AddMousePosEvent((float)mousePoint.x * contentScalingFactor,  (float)mousePoint.y * contentScalingFactor);

            SmartPtr<IInputEvent> inputEvent( new InputEvent );
            inputEvent->setEventType( IInputEvent::EventType::Mouse );

            SmartPtr<IMouseState> mouseState( new MouseState );
            inputEvent->setMouseState( mouseState );

            auto size = Vector2F(vSize.x, vSize.y);
            auto absolutePosition = Vector2F( (f32)mousePoint.x * contentScalingFactor,
                                              (f32)mousePoint.y * contentScalingFactor );
            auto relativeMove = absolutePosition - m_lastMousePosition;
            auto relativePosition = absolutePosition / size;

            mouseState->setRelativeMove( relativeMove );
            mouseState->setRelativePosition( relativePosition );
            mouseState->setAbsolutePosition( absolutePosition );

            mouseState->setEventType( IMouseState::Event::Moved );

            m_lastMousePosition = absolutePosition;
            inputManager->postEvent( inputEvent );
        }
    }

}  // namespace fb