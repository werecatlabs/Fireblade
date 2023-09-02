#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUILayout.h>
#include <FBRenderUI/CUIContainer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, CUILayout, CUIElement<IUILayout> );

        u32 CUILayout::m_nameExt = 0;

        CUILayout::CUILayout()
        {
            createStateContext();
            m_type = "Layout";

            // auto applicationManager = core::IApplicationManager::instance();
            // FactoryPtr factory = engine->getFactory();
            // SmartPtr<IFiniteStateMachine> fsm = factory->create("FSMStandard");
            ////m_fsm->addFSMListener(this);
            // m_fsm = fsm;

            // setLayout(this);
        }

        CUILayout::~CUILayout()
        {
            removeAllChildren();
        }

        void CUILayout::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto name = String( "Layout_" ) + StringUtil::toString( m_nameExt++ );
                setName( name );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                if( !overlayManager )
                {
                    return;
                }

                auto existingOverLay = overlayManager->findOverlay( getName() );

                if( existingOverLay )
                {
                    FB_ASSERT_TRUE( existingOverLay );  // overlay already exists
                }

                m_overlay = overlayManager->addOverlay( getName() );

                s32 zOrder = 500;

                m_overlay->setZOrder( zOrder );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUILayout::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                if( m_overlay )
                {
                    overlayManager->removeOverlay( m_overlay );

                    m_overlay->unload( nullptr );
                    m_overlay = nullptr;
                }

                CUIElement<IUILayout>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool CUILayout::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            bool eventHandled = false;

            // if(!isEnabled())
            //	return eventHandled;

            // switch(event->getEventType())
            //{
            // case IInputEvent::MOUSE_INPUT_EVENT:
            // case IInputEvent::USER_INPUT_EVENT:
            // case IInputEvent::KEY_INPUT_EVENT:
            // case IInputEvent::JOYSTICK_INPUT_EVENT:
            //	{
            //		//for(u32 i=0; i<m_inputListeners.size(); ++i)
            //		//{
            //		//	IGUIItemInputListener* inputListener = m_inputListeners[i];
            //		//	if(inputListener->onEvent(event))
            //		//		return true;
            //		//}
            //		//}

            //		Parameters params;
            //		params.set_used(1);
            //		params[0].setPtr((void*)event.get());

            //		Parameters results(1);

            //		static const hash32 ON_INPUT_EVENT_HASH = StringUtil::getHash("inputEvent");

            //		SmartPtr<IScriptInvoker>& invoker = getInvoker();
            //		if(invoker)
            //			invoker->event(ON_INPUT_EVENT_HASH, params, results);

            //		eventHandled = (results.size() > 0) && (results[0].getBool() == TRUE);
            //	}
            //	break;
            // default:
            //	{
            //	}
            //};

            return eventHandled || CUIElement::handleEvent( event );
        }

        void CUILayout::update()
        {
            // getFSM()->update();
            CUIElement::update();
        }

        void CUILayout::handleStateChanged( SmartPtr<IState> &state )
        {
            if( isLoaded() )
            {
                if( m_overlay )
                {
                    auto overlayState = fb::dynamic_pointer_cast<UIElementState>( state );

                    auto visible = overlayState->isVisible();
                    m_overlay->setVisible( visible );
                }

                CUIElement<IUILayout>::handleStateChanged( state );
            }
        }

        void CUILayout::OnEnterState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnEnterIdleState();
                break;
            case FS_FADEIN:
                OnEnterFadeInState();
                break;
            case FS_FADEOUT:
                OnEnterFadeOutState();
                break;
            default:
            {
            }
            }
        }

        void CUILayout::OnUpdateState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnUpdateIdleState();
                break;
            case FS_FADEIN:
                OnUpdateFadeInState();
                break;
            case FS_FADEOUT:
                OnUpdateFadeOutState();
                break;
            default:
            {
            }
            }
        }

        void CUILayout::OnLeaveState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnLeaveIdleState();
                break;
            case FS_FADEIN:
                OnLeaveFadeInState();
                break;
            case FS_FADEOUT:
                OnLeaveFadeOutState();
                break;
            default:
            {
            }
            }
        }

        u8 CUILayout::GetStateFromName( const String &stateName ) const
        {
            if( stateName == ( "Idle" ) )
            {
                return FS_IDLE;
            }
            if( stateName == ( "FadeIn" ) )
            {
                return FS_FADEIN;
            }
            if( stateName == ( "FadeOut" ) )
            {
                return FS_FADEOUT;
            }

            return 0;
        }

        String CUILayout::GetStateNameFromId( u8 stateId ) const
        {
            switch( stateId )
            {
            case FS_IDLE:
                return String( "Idle" );
            case FS_FADEIN:
                return String( "FadeIn" );
            case FS_FADEOUT:
                return String( "FadeOut" );
            default:
            {
                return StringUtil::EmptyString;
            }
            }

            return StringUtil::EmptyString;
        }

        void CUILayout::addChild( SmartPtr<IUIElement> child )
        {
            if( isThreadSafe() )
            {
                FB_ASSERT( child );

                child->setLayout( this );

                if( child )
                {
                    render::IOverlayElement *element = nullptr;
                    child->_getObject( (void **)&element );
                    //FB_ASSERT( element );

                    if( element )
                    {
                        if( m_overlay )
                        {
                            m_overlay->addElement( element );
                        }
                    }
                }

                CUIElement<IUILayout>::addChild( child );
            }
            else
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto factoryManager = applicationManager->getFactoryManager();

                if( !child->isLoaded() )
                {
                    graphicsSystem->loadObject( child );
                }

                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_ADD_CHILD );
                message->setObject( child );
                addMessage( message );
            }
        }

        bool CUILayout::removeChild( SmartPtr<IUIElement> child )
        {
            if( isThreadSafe() )
            {
                child->setLayout( nullptr );

                FB_ASSERT( child );

                if( child )
                {
                    render::IOverlayElement *element = nullptr;
                    child->_getObject( (void **)&element );
                    // FB_ASSERT(element);

                    if( element )
                    {
                        if( m_overlay )
                        {
                            return m_overlay->removeElement( element );
                        }
                    }
                }

                return CUIElement<IUILayout>::removeChild( child );
            }
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            if( !child->isLoaded() )
            {
                graphicsSystem->loadObject( child );
            }

            auto message = factoryManager->make_ptr<StateMessageObject>();
            message->setType( STATE_MESSAGE_REMOVE_CHILD );
            message->setObject( child );
            addMessage( message );

            return false;
        }

        SmartPtr<IUIWindow> CUILayout::getUiWindow() const
        {
            return m_uiWindow;
        }

        void CUILayout::setUiWindow( SmartPtr<IUIWindow> uiWindow )
        {
            m_uiWindow = uiWindow;
        }

        void CUILayout::updateZOrder()
        {
            CUIElement<IUILayout>::updateZOrder();

            if( m_overlay )
            {
                m_overlay->updateZOrder();
            }
        }

        SmartPtr<Properties> CUILayout::getProperties() const
        {
            auto properties = CUIElement<IUILayout>::getProperties();
            properties->setProperty( "Type", "UILayout" );
            properties->setProperty( "Name", getName() );
            properties->setProperty( "Visible", isVisible() );
            properties->setProperty( "Enabled", isEnabled() );
            properties->setProperty( "Position", getPosition() );
            properties->setProperty( "Size", getSize() );
            return properties;
        }

        Array<SmartPtr<ISharedObject>> CUILayout::getChildObjects() const
        {
            auto objects = CUIElement<IUILayout>::getChildObjects();

            if( m_uiWindow )
            {
                objects.push_back( m_uiWindow );
            }

            if( m_overlay )
            {
                objects.push_back( m_overlay );
            }

            if( m_fsm )
            {
                objects.push_back( m_fsm );
            }

            return objects;
        }

        void CUILayout::OnEnterIdleState()
        {
        }

        void CUILayout::OnEnterFadeInState()
        {
            CUIElement::onEvent( String( "FadeIn" ) );
        }

        void CUILayout::OnEnterFadeOutState()
        {
            CUIElement::onEvent( String( "FadeOut" ) );
        }

        void CUILayout::OnUpdateIdleState()
        {
        }

        void CUILayout::OnUpdateFadeInState()
        {
        }

        void CUILayout::OnUpdateFadeOutState()
        {
        }

        void CUILayout::OnLeaveIdleState()
        {
        }

        void CUILayout::OnLeaveFadeInState()
        {
        }

        void CUILayout::OnLeaveFadeOutState()
        {
        }

        //
        // Callbacks
        //

        void CUILayout::OnActivateCallback()
        {
        }

        void CUILayout::OnSelectCallback()
        {
        }

        void CUILayout::OnDeselectCallback()
        {
        }

        SmartPtr<IFSM> CUILayout::getFSM()
        {
            return m_fsm;
        }

        const SmartPtr<IFSM> &CUILayout::getFSM() const
        {
            return m_fsm;
        }
    }  // end namespace ui
}  // end namespace fb
