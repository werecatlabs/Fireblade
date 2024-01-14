#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIButton.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBRenderUI/CUICursor.h>
#include <FBRenderUI/CUILayout.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{

    CUIButton::CUIButton()
    {
        createStateContext();
        m_type = "Button";
    }

    CUIButton::~CUIButton()
    {
        unload( nullptr );
    }

    void CUIButton::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            IUIManager::ScopedLock lock( renderUI );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CUIButton::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            removeAllChildren();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            //if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
            //{
            //    auto overlayManager = graphicsSystem->getOverlayManager();
            //    FB_ASSERT( overlayManager );

            //    if( m_container )
            //    {
            //        overlayManager->removeElement( m_container );
            //    }
            //}

            CUIElement<IUIButton>::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CUIButton::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        if( auto enabled = isEnabled() )
        {
            switch( auto eventType = event->getEventType() )
            {
            case IInputEvent::EventType::Mouse:
            {
                //Mouse event
                auto mouseState = event->getMouseState();
                auto mouseEventType = mouseState->getEventType();

                if( mouseEventType == IMouseState::Event::LeftPressed )
                {
                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto ui = fb::static_pointer_cast<CUIManager>( applicationManager->getUI() );
                    if( ui )
                    {
                        auto relativeMousePosition = mouseState->getRelativePosition();

                        if( auto uiWindow = ui->getMainWindow() )
                        {
                            if( auto mainWindow = applicationManager->getWindow() )
                            {
                                auto mainWindowSize = mainWindow->getSize();
                                auto mainWindowSizeF = Vector2F( static_cast<f32>( mainWindowSize.x ),
                                                                 static_cast<f32>( mainWindowSize.y ) );

                                auto pos = uiWindow->getPosition() / mainWindowSizeF;
                                auto size = uiWindow->getSize() / mainWindowSizeF;

                                auto aabb = AABB2F( pos, size, true );
                                if( aabb.isInside( relativeMousePosition ) )
                                {
                                    auto postion = getAbsolutePosition();
                                    auto rect = AABB2F( postion, postion + getSize() );

                                    auto point = ( relativeMousePosition - pos ) / size;

                                    if( rect.isInside( point ) )
                                    {
                                        if( auto pLayout = getLayout() )
                                        {
                                            auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                                            layout->onActivate( this );
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if( auto mainWindow = applicationManager->getWindow() )
                            {
                                auto mainWindowSize = mainWindow->getSize();
                                auto mainWindowSizeF = Vector2F( static_cast<f32>( mainWindowSize.x ),
                                                                 static_cast<f32>( mainWindowSize.y ) );
                                auto postion = getAbsolutePosition();
                                auto rect = AABB2F( postion, postion + getSize() );

                                auto point = ( relativeMousePosition - postion ) / mainWindowSizeF;

                                if( rect.isInside( relativeMousePosition ) )
                                {
                                    if( auto pLayout = getLayout() )
                                    {
                                        auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                                        layout->onActivate( this );
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        auto relativeMousePosition = mouseState->getRelativePosition();

                        if( auto mainWindow = applicationManager->getWindow() )
                        {
                            auto mainWindowSize = mainWindow->getSize();
                            auto mainWindowSizeF = Vector2F( static_cast<f32>( mainWindowSize.x ),
                                                             static_cast<f32>( mainWindowSize.y ) );
                            auto postion = getAbsolutePosition();
                            auto rect = AABB2F( postion, postion + getSize() );

                            auto point = ( relativeMousePosition - postion ) / mainWindowSizeF;

                            if( rect.isInside( relativeMousePosition ) )
                            {
                                if( auto pLayout = getLayout() )
                                {
                                    auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                                    layout->onActivate( this );
                                    return true;
                                }
                            }
                        }
                    }
                }
                else if( mouseEventType == IMouseState::Event::LeftReleased )
                {
                }
                else if( mouseEventType == IMouseState::Event::MiddleReleased )
                {
                }
                else if( mouseEventType == IMouseState::Event::Moved )
                {
                    Vector2F point( event->getMouseState()->getRelativePosition().X(),
                                    event->getMouseState()->getRelativePosition().Y() );
                    Vector2F postion = getAbsolutePosition();
                    AABB2F rect( postion, postion + getSize() );
                    if( rect.isInside( point ) )
                    {
                        if( m_hoverMaterial.length() > 0 )
                        {
                            if( m_container )
                            {
                                //m_container->setMaterialName( m_hoverMaterial );
                            }
                        }

                        setHighlighted( true );
                    }
                    else
                    {
                        if( m_container )
                        {
                            //m_container->setMaterialName( m_defaultMaterial );
                        }

                        setHighlighted( false );
                    }
                }
            }
            break;
            case IInputEvent::EventType::User:
            case IInputEvent::EventType::Key:
            {
                if( !isInFocus() )
                {
                    return false;
                }

                if( event->getKeyboardState()->isPressedDown() )
                {
                    if( event->getKeyboardState()->getKeyCode() ==
                        static_cast<u32>( KeyCodes::KEY_RETURN ) )
                    {
                        if( auto pLayout = getLayout() )
                        {
                            auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                            layout->onActivate( this );
                            return true;
                        }
                    }
                    else if( event->getKeyboardState()->getKeyCode() ==
                             static_cast<u32>( KeyCodes::KEY_BACK ) )
                    {
                        onDeactivate();
                        return true;
                    }
                }
            }
            break;
            case IInputEvent::EventType::Joystick:
            {
                if( !isInFocus() )
                {
                    return false;
                }

                auto actionId0 = StringUtil::getHash( "Action0" );
                auto actionId1 = StringUtil::getHash( "Action1" );

                auto joystickState = event->getJoystickState();
                auto joystickEventType =
                    static_cast<IJoystickState::Type>( joystickState->getEventType() );

                switch( joystickEventType )
                {
                case IJoystickState::Type::ButtonPressed:
                {
                    auto gameInputState = event->getGameInputState();
                    if( gameInputState->getAction() == actionId0 )
                    {
                        if( auto pLayout = getLayout() )
                        {
                            auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                            layout->onActivate( this );
                            return true;
                        }
                    }
                    else if( gameInputState->getAction() == actionId1 )
                    {
                        onDeactivate();
                        return true;
                    }
                }
                break;
                default:
                {
                }
                };
            }
            break;
            default:
            {
            }
            };
        }

        return false;
    }

    void CUIButton::setPosition( const Vector2F &position )
    {
        //if( m_container )
        //{
        //    m_container->setLeft( position.X() );
        //    m_container->setTop( position.Y() );
        //}

        CUIElement::setPosition( position );
    }

    auto CUIButton::getLabel() const -> String
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<UIElementState>() )
            {
                return state->getCaption();
            }
        }

        return "";
    }

    void CUIButton::setLabel( const String &label )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<UIElementState>() )
            {
                state->setCaption( label );
            }
        }
    }

    void CUIButton::setTextSize( f32 textSize )
    {
#if FB_USE_MYGUI
            textSize *= 3.0f;
#endif

            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    state->setTextSize( static_cast<u32>( textSize ) );
                }
            }
        }

        auto CUIButton::getTextSize() const -> f32
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return static_cast<f32>( state->getTextSize() );
                }
            }

            return 0.0f;
        }

        auto CUIButton::isSimpleButton() const -> bool
        {
            return m_isSimpleButton;
        }

        void CUIButton::setSimpleButton( bool simpleButton )
        {
            m_isSimpleButton = simpleButton;
        }

        void CUIButton::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto elementState = fb::static_pointer_cast<UIElementState>( state );

            auto caption = elementState->getCaption();
        }

        void CUIButton::handleButtonClick()
        {
            if( auto pLayout = getLayout() )
            {
                auto layout = fb::static_pointer_cast<CUILayout>( pLayout );
                layout->onActivate( this );
            }
        }

}  // namespace fb::ui
