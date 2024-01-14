#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementText.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED( fb::render, COverlayElementText,
                               COverlayElementOgre<IOverlayElementText> );

    COverlayElementText::COverlayElementText() : COverlayElementOgre<IOverlayElementText>()
    {
        createStateContext();
    }

    COverlayElementText::~COverlayElementText()
    {
        unload( nullptr );
    }

    void COverlayElementText::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            setLoadingState( LoadingState::Loading );

            auto overlayManager = Ogre::v1::OverlayManager::getSingletonPtr();

            static const auto typeName = String( "TextArea" );
            const auto instanceName = getName();
            // Ogre::TextAreaOverlayElement* overlayElement =
            // (Ogre::TextAreaOverlayElement*)m_overlayMgr->createOverlayElement(typeName.c_str(),
            // instanceName.c_str());
            auto text = (Ogre::v1::TextAreaOverlayElement *)overlayManager->createOverlayElement(
                typeName, instanceName );

            setElement( text );
            setElementText( text );

            // text->setMetricsMode(Ogre::v1::GuiMetricsMode::GMM_RELATIVE);
            ////text->setMetricsMode(Ogre::v1::GuiMetricsMode::GMM_PIXELS);
            // text->setHorizontalAlignment(Ogre::v1::GuiHorizontalAlignment::GHA_CENTER);
            // text->setVerticalAlignment(Ogre::v1::GuiVerticalAlignment::GVA_CENTER);
            //
            // setFontName("DebugFont");
            //
            // text->setCharHeight(18);
            // text->setCaption("Test");
            // text->setSpaceWidth(9);
            // text->setColour(Ogre::ColourValue::White);

            // text->setMetricsMode(Ogre::v1::GuiMetricsMode::GMM_RELATIVE);
            text->setMetricsMode( Ogre::v1::GuiMetricsMode::GMM_PIXELS );
            text->setHorizontalAlignment( Ogre::v1::GuiHorizontalAlignment::GHA_LEFT );
            text->setVerticalAlignment( Ogre::v1::GuiVerticalAlignment::GVA_TOP );
            text->setFontName( "default" );
            text->setCharHeight( 18 );
            // text->setCaption("Test");
            text->setSpaceWidth( 9 );
            text->setColour( Ogre::ColourValue::White );
            text->initialise();

            // text->setHorizontalAlignment((Ogre::v1::GuiHorizontalAlignment)m_horizontalAlignment);
            // text->setVerticalAlignment((Ogre::v1::GuiVerticalAlignment)m_verticalAlignment);

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void COverlayElementText::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( const auto &loadingState = getLoadingState(); loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                // if (m_state)
                //{
                //	if (m_stateContext)
                //	{
                //		m_stateContext->setState(nullptr);
                //	}

                //	m_state->unload(nullptr);
                //	m_state = nullptr;
                //}

                if( m_stateContext )
                {
                    if( m_stateListener )
                    {
                        m_stateContext->removeStateListener( m_stateListener );
                    }

                    stateManager->removeStateObject( m_stateContext );
                    m_stateContext->unload( nullptr );
                    m_stateContext = nullptr;
                }

                if( m_stateListener )
                {
                    m_stateListener->unload( nullptr );
                    m_stateListener = nullptr;
                }

                auto overlayMgr = Ogre::v1::OverlayManager::getSingletonPtr();
                FB_ASSERT( overlayMgr );

                if( m_element )
                {
                    overlayMgr->destroyOverlayElement( m_element );
                    m_element = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto COverlayElementText::isContainer() const -> bool
    {
        return true;
    }

    //--------------------------------------------
    void COverlayElementText::setFontName( const String &fontName )
    {
        try
        {
            auto fontManager = Ogre::FontManager::getSingletonPtr();
            auto font = fontManager->getByName( fontName );
            if( font )
            {
                if( m_elementText )
                {
                    m_elementText->setFontName( fontName.c_str() );
                }
            }
        }
        catch( Ogre::Exception &e )
        {
            auto errorMessage = e.getFullDescription();
            FB_LOG_ERROR( errorMessage );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void COverlayElementText::setCharHeight( f32 charHeight )
    {
        if( m_elementText )
        {
            m_elementText->setCharHeight( charHeight );
        }
    }

    void COverlayElementText::setAlignment( u8 alignment )
    {
        if( m_elementText )
        {
            m_elementText->setAlignment(
                static_cast<Ogre::v1::TextAreaOverlayElement::Alignment>( alignment ) );
        }
    }

    auto COverlayElementText::getAlignment() const -> u8
    {
        if( m_elementText )
        {
            return static_cast<u8>( m_elementText->getAlignment() );
        }

        return 0;
    }

    void COverlayElementText::setSpaceWidth( f32 width )
    {
        if( m_elementText )
        {
            m_elementText->setSpaceWidth( width );
        }
    }

    auto COverlayElementText::getSpaceWidth() const -> f32
    {
        return 0;
    }

    void COverlayElementText::StateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto overlayTextState = fb::static_pointer_cast<OverlayTextState>( state );

        if( auto owner = fb::static_pointer_cast<COverlayElementText>( StateListenerOgre::getOwner() ) )
        {
            auto alignment = overlayTextState->getAlignment();

            if( auto elementText = owner->getElementText() )
            {
                elementText->setAlignment(
                    static_cast<Ogre::v1::TextAreaOverlayElement::Alignment>( alignment ) );
            }
        }

        StateListenerOgre::handleStateChanged( state );
    }

    void COverlayElementText::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
        if( auto owner = fb::static_pointer_cast<COverlayElementText>( StateListenerOgre::getOwner() ) )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto renderTask = graphicsSystem->getRenderTask();
            auto currentTaskId = Thread::getCurrentTask();

            // FB_ASSERT(currentTaskId == renderTask);

            if( currentTaskId == renderTask )
            {
                if( message->isExactly<StateMessageText>() )
                {
                    auto stateMessage = fb::static_pointer_cast<StateMessageText>( message );
                    auto messageType = stateMessage->getType();
                    auto value = stateMessage->getText();

                    if( messageType == STATE_MESSAGE_TEXT )
                    {
                        owner->setCaption( value );
                    }
                }
                else if( message->isExactly<StateMessageFloatValue>() )
                {
                    auto stateMessage = fb::static_pointer_cast<StateMessageFloatValue>( message );
                    auto messageType = stateMessage->getType();
                    auto value = stateMessage->getValue();

                    if( messageType == STATE_MESSAGE_LEFT )
                    {
                        //owner->setLeft( value );
                    }
                    else if( messageType == STATE_MESSAGE_TOP )
                    {
                        //owner->setTop( value );
                    }
                    else if( messageType == STATE_MESSAGE_WIDTH )
                    {
                        //owner->setWidth( value );
                    }
                    else if( messageType == STATE_MESSAGE_HEIGHT )
                    {
                        //owner->setHeight( value );
                    }
                }
                else if( message->isExactly<StateMessageUIntValue>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );
                    FB_ASSERT( objectMessage );

                    auto messageType = objectMessage->getType();
                    auto messageValue = objectMessage->getValue();

                    if( messageType == STATE_MESSAGE_METRICSMODE )
                    {
                        owner->setMetricsMode( static_cast<u8>( messageValue ) );
                    }
                    else if( messageType == STATE_MESSAGE_ALIGN_HORIZONTAL )
                    {
                        owner->setHorizontalAlignment( static_cast<u8>( messageValue ) );
                    }
                    else if( messageType == STATE_MESSAGE_ALIGN_VERTICAL )
                    {
                        owner->setVerticalAlignment( static_cast<u8>( messageValue ) );
                    }
                }
            }
        }
    }

    void COverlayElementText::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto COverlayElementText::isValid() const -> bool
    {
        const auto &loadingState = getLoadingState();
        switch( loadingState )
        {
        case LoadingState::Unloaded:
        {
            if( m_element )
            {
                return false;
            }
        }
        break;
        case LoadingState::Loading:
        {
            if( m_element )
            {
                return false;
            }
        }
        break;
        case LoadingState::Loaded:
        {
            if( !m_element )
            {
                return false;
            }
        }
        break;
        case LoadingState::Unloading:
        {
            if( !m_element )
            {
                return false;
            }
        }
        break;
        default:
            break;
        }

        return COverlayElementOgre<IOverlayElementText>::isValid();
    }

    auto COverlayElementText::getElementText() const -> Ogre::v1::TextAreaOverlayElement *
    {
        return m_elementText;
    }

    void COverlayElementText::setElementText( Ogre::v1::TextAreaOverlayElement *elementText )
    {
        m_elementText = elementText;
    }

    void COverlayElementText::createStateContext()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        m_stateContext = stateManager->addStateObject();

        auto listener = factoryManager->make_ptr<StateListener>();
        listener->setOwner( this );
        m_stateListener = listener;
        m_stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<OverlayTextState>();
        m_stateContext->addState( state );

        m_stateContext->setOwner( this );

        auto stateTask = graphicsSystem->getStateTask();
        state->setTaskId( stateTask );
    }

}  // namespace fb::render
