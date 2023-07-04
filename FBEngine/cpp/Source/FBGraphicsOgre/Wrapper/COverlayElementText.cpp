#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementText.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

namespace fb
{
    namespace render
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
                setLoadingState( LoadingState::Loading );

                auto overlayManager = Ogre::OverlayManager::getSingletonPtr();

                static const auto typeName = String( "TextArea" );
                const auto instanceName = getName();
                // Ogre::TextAreaOverlayElement* overlayElement =
                // (Ogre::TextAreaOverlayElement*)m_overlayMgr->createOverlayElement(typeName.c_str(),
                // instanceName.c_str());
                auto text = static_cast<Ogre::TextAreaOverlayElement *>(
                    overlayManager->createOverlayElement( typeName, instanceName ) );
                m_elementText = text;

                // text->setMetricsMode(Ogre::GuiMetricsMode::GMM_RELATIVE);
                ////text->setMetricsMode(Ogre::GuiMetricsMode::GMM_PIXELS);
                // text->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_CENTER);
                // text->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_CENTER);
                //
                // setFontName("DebugFont");
                //
                // text->setCharHeight(18);
                // text->setCaption("Test");
                // text->setSpaceWidth(9);
                // text->setColour(Ogre::ColourValue::White);

                text->setMetricsMode( Ogre::GuiMetricsMode::GMM_PIXELS );
                //text->setMetricsMode( Ogre::GuiMetricsMode::GMM_RELATIVE_ASPECT_ADJUSTED );
                //text->setHorizontalAlignment( Ogre::GuiHorizontalAlignment::GHA_LEFT );
                //text->setVerticalAlignment( Ogre::GuiVerticalAlignment::GVA_TOP );

                text->setHorizontalAlignment( Ogre::GuiHorizontalAlignment::GHA_CENTER );
                text->setVerticalAlignment( Ogre::GuiVerticalAlignment::GVA_CENTER );

                //text->setFontName( "SdkTrays/Caption" );
                text->setFontName( "default" );
                text->setCharHeight( 18 );
                // text->setCaption("Test");
                text->setSpaceWidth( 9 );

                setColour( ColourF::White );

                text->_setLeft( 100.0f / 1280.0f );
                text->_setTop( 0.0f );
                text->_setWidth( 0.2f );
                text->_setHeight( 0.1f );

                text->initialise();

                // text->setHorizontalAlignment((Ogre::GuiHorizontalAlignment)m_horizontalAlignment);
                // text->setVerticalAlignment((Ogre::GuiVerticalAlignment)m_verticalAlignment);

                setElement( text );

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

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    // if (m_state)
                    //{
                    //	if (m_stateObject)
                    //	{
                    //		m_stateObject->setState(nullptr);
                    //	}

                    //	m_state->unload(nullptr);
                    //	m_state = nullptr;
                    //}

                    if( m_stateObject )
                    {
                        if( m_stateListener )
                        {
                            m_stateObject->removeStateListener( m_stateListener );
                        }

                        stateManager->removeStateObject( m_stateObject );
                        m_stateObject->unload( nullptr );
                        m_stateObject = nullptr;
                    }

                    if( m_stateListener )
                    {
                        m_stateListener->unload( nullptr );
                        m_stateListener = nullptr;
                    }

                    auto overlayMgr = Ogre::OverlayManager::getSingletonPtr();
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

        bool COverlayElementText::isContainer() const
        {
            return true;
        }

        void COverlayElementText::_getObject( void **ppObject ) const
        {
            *ppObject = m_element;
        }

        void COverlayElementText::setFontName( const String &fontName )
        {
            try
            {
                auto fontManager = Ogre::FontManager::getSingletonPtr();
                auto font = fontManager->getByName( fontName );
                //if( !font )
                //{
                //    auto grp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
                //    auto fontResource = fontManager->load( fontName, grp );
                //    font = Ogre::dynamic_pointer_cast<Ogre::Font>(fontResource);
                //}

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

        void COverlayElementText::setVerticalAlignment( u8 alignment )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto overlayTextState = stateObject->getStateByType<OverlayTextState>() )
                {
                    overlayTextState->setAlignment( alignment );
                }
            }
        }

        u8 COverlayElementText::getVerticalAlignment() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto overlayTextState = stateObject->getStateByType<OverlayTextState>() )
                {
                    return overlayTextState->getAlignment();
                }
            }

            return 0;
        }

        void COverlayElementText::setHorizontalAlignment( u8 alignment )
        {
            
        }

        u8 COverlayElementText::getHorizontalAlignment() const
        {
            return 0;
        }

        void COverlayElementText::setSpaceWidth( f32 width )
        {
            if( m_elementText )
            {
                m_elementText->setSpaceWidth( width );
            }
        }

        f32 COverlayElementText::getSpaceWidth() const
        {
            return 0;
        }

        void COverlayElementText::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto overlayTextState = fb::static_pointer_cast<OverlayTextState>( state );

            if( auto owner =
                    fb::static_pointer_cast<COverlayElementText>( StateListenerOgre::getOwner() ) )
            {
                auto alignment = overlayTextState->getAlignment();

                if( auto elementText = owner->getElementText() )
                {
                    elementText->setAlignment(
                        static_cast<Ogre::TextAreaOverlayElement::Alignment>( alignment ) );
                }
            }

            StateListenerOgre::handleStateChanged( state );
        }

        void COverlayElementText::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner =
                    fb::static_pointer_cast<COverlayElementText>( StateListenerOgre::getOwner() ) )
            {
                auto applicationManager = core::IApplicationManager::instance();
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

        bool COverlayElementText::isValid() const
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

        Ogre::TextAreaOverlayElement *COverlayElementText::getElementText() const
        {
            return m_elementText;
        }

        void COverlayElementText::setElementText( Ogre::TextAreaOverlayElement *elementText )
        {
            m_elementText = elementText;
        }

        void COverlayElementText::createStateContext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            m_stateObject = stateManager->addStateObject();

            auto listener = factoryManager->make_ptr<StateListener>();
            listener->setOwner( this );
            m_stateListener = listener;
            m_stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<OverlayTextState>();
            m_stateObject->setState( state );

            m_stateObject->setOwner( this );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

    }  // end namespace render
}  // end namespace fb
