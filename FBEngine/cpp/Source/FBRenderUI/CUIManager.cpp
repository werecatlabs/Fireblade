#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIAnimatedMaterial.h>
#include <FBRenderUI/CUICursor.h>
#include <FBRenderUI/CUIBar.h>
#include <FBRenderUI/CUIElement.h>
#include <FBRenderUI/CUIElementBar.h>
#include <FBRenderUI/CUIButton.h>
#include <FBRenderUI/CUIFadeEffect.h>
#include <FBRenderUI/CUIToggleButton.h>
#include <FBRenderUI/CUIContainer.h>
#include <FBRenderUI/CUIDialogBox.h>
#include <FBRenderUI/CUIText.h>
#include <FBRenderUI/CUITextEntry.h>
#include <FBRenderUI/CUILayout.h>
#include <FBRenderUI/CUIImage.h>
#include <FBRenderUI/CUIImageArray.h>
#include <FBRenderUI/CUIMenu.h>
#include <FBRenderUI/CUICharacterSelectMenu.h>
#include <FBRenderUI/CUIMenuItem.h>
#include <FBRenderUI/CUISpinner.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBRenderUI/CUIScrollingText.h>
#include <FBRenderUI/CUIScrollingTextElement.h>
#include <FBRenderUI/CUIVector.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, CUIManager, IUIManager );

    CUIManager::CUIManager() : m_itemInFocus( nullptr )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto root = factoryManager->make_ptr<CUIElement<IUIElement>>();
        root->setEnabled( true );
        root->setVisible( false, false );

        setRoot( root );

        /*CGUICursor* cursor = new CGUICursor;
        cursor->initialise();
        m_root->addChild(cursor);
        cursor->removeReference();

        m_cursor = cursor;*/

        m_inputListener = factoryManager->make_ptr<InputListener>( this );

        auto inputMgr = applicationManager->getInputDeviceManager();
        if( inputMgr )
        {
            inputMgr->addListener( m_inputListener );
        }

        //m_elements.reserve( 1024 );
    }

    CUIManager::~CUIManager()
    {
        unload( nullptr );
    }

    void CUIManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto factoryManager = fb::make_ptr<FactoryManager>();
            setFactoryManager( factoryManager );

            ApplicationUtil::addFactory<CUIAnimatedMaterial>();
            ApplicationUtil::addFactory<CUICursor>();
            ApplicationUtil::addFactory<CUIBar>();
            ApplicationUtil::addFactory<CUIElementBar>();
            ApplicationUtil::addFactory<CUIButton>();
            ApplicationUtil::addFactory<CUIFadeEffect>();
            ApplicationUtil::addFactory<CUIToggleButton>();
            ApplicationUtil::addFactory<CUIContainer>();
            ApplicationUtil::addFactory<CUIDialogBox>();
            ApplicationUtil::addFactory<CUIText>();
            ApplicationUtil::addFactory<CUITextEntry>();
            ApplicationUtil::addFactory<CUILayout>();
            ApplicationUtil::addFactory<CUIImage>();
            ApplicationUtil::addFactory<CUIImageArray>();
            ApplicationUtil::addFactory<CUIMenu>();
            ApplicationUtil::addFactory<CUICharacterSelectMenu>();
            ApplicationUtil::addFactory<CUIMenuItem>();
            ApplicationUtil::addFactory<CUISpinner>();
            ApplicationUtil::addFactory<CUIItemTemplate>();
            ApplicationUtil::addFactory<CUIScrollingText>();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            auto message = String( e.what() );
            FB_LOG_ERROR( message );
        }
    }

    void CUIManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            if( auto root = getRoot() )
            {
                root->removeAllChildren();
                root = nullptr;
            }

            //if( auto gui = MyGUI::Gui::getInstancePtr() )
            //{
            //    gui->shutdown();
            //}

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CUIManager::messagePump( SmartPtr<ISharedObject> data ) -> size_t
    {
        return 0;
    }

    auto CUIManager::OnEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto elements = getElements();
        for( auto element : elements )
        {
            if( element )
            {
                element->handleEvent( event );
            }
        }

        if( event->getEventType() == IInputEvent::EventType::Mouse )
        {
            if( auto mouseState = event->getMouseState() )
            {
                auto absolutePosition = mouseState->getAbsolutePosition();
                auto x = static_cast<s32>( absolutePosition.X() );
                auto y = static_cast<s32>( absolutePosition.Y() );
                auto z = 0;

                auto relativePosition = mouseState->getRelativePosition();

                if( auto uiWindow = applicationManager->getSceneRenderWindow() )
                {
                    if( auto mainWindow = applicationManager->getWindow() )
                    {
                        auto mainWindowSize = mainWindow->getSize();
                        auto mainWindowSizeF = Vector2F( static_cast<f32>( mainWindowSize.x ),
                                                         static_cast<f32>( mainWindowSize.y ) );

                        auto sceneWindowPosition = uiWindow->getPosition();
                        auto sceneWindowSize = uiWindow->getSize();

                        auto pos = sceneWindowPosition / mainWindowSizeF;
                        auto size = sceneWindowSize / mainWindowSizeF;

                        auto aabb = AABB2F( pos, size, true );
                        if( aabb.isInside( relativePosition ) )
                        {
                            x = static_cast<s32>( ( absolutePosition.X() - sceneWindowPosition.X() ) /
                                                  size.X() );
                            y = static_cast<s32>( ( absolutePosition.Y() - sceneWindowPosition.Y() ) /
                                                  size.Y() );

                            //if( auto input = MyGUI::InputManager::getInstancePtr() )
                            //{
                            //    if( mouseState->getEventType() == IMouseState::Event::Moved )
                            //    {
                            //        input->injectMouseMove( x, y, z );
                            //    }

                            //    if( mouseState->getEventType() == IMouseState::Event::LeftPressed )
                            //    {
                            //        input->injectMousePress( x, y, MyGUI::MouseButton::Left );
                            //    }

                            //    if( mouseState->getEventType() == IMouseState::Event::LeftReleased )
                            //    {
                            //        input->injectMouseRelease( x, y, MyGUI::MouseButton::Left );
                            //    }
                            //}
                        }
                    }
                }
                else
                {
                    //if( auto input = MyGUI::InputManager::getInstancePtr() )
                    //{
                    //    if( mouseState->getEventType() == IMouseState::Event::Moved )
                    //    {
                    //        input->injectMouseMove( x, y, z );
                    //    }

                    //    if( mouseState->getEventType() == IMouseState::Event::LeftPressed )
                    //    {
                    //        input->injectMousePress( x, y, MyGUI::MouseButton::Left );
                    //    }

                    //    if( mouseState->getEventType() == IMouseState::Event::LeftReleased )
                    //    {
                    //        input->injectMouseRelease( x, y, MyGUI::MouseButton::Left );
                    //    }
                    //}
                }
            }
        }

        return false;

        //return m_root->handleEvent( event );
    }

    void CUIManager::update()
    {
        ScopedLock lock( this );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        if( applicationManager->isEditor() )
        {
            if( auto uiWindow = applicationManager->getSceneRenderWindow() )
            {
                if( auto mainWindow = applicationManager->getWindow() )
                {
                    //auto mainWindowSize = mainWindow->getSize();

                    //auto renderManager =
                    //    (MyGUI::OgreRenderManager *)MyGUI::RenderManager::getInstancePtr();

                    //auto viewSize = renderManager->getViewSize();
                    //if( viewSize.width != mainWindowSize.X() ||
                    //    viewSize.height != mainWindowSize.Y() )
                    //{
                    //    renderManager->setViewSize( mainWindowSize.X(), mainWindowSize.Y() );

                    //    auto elements = getElements();
                    //    for( auto &element : elements )
                    //    {
                    //        if( auto stateContext = element->getStateContext() )
                    //        {
                    //            stateContext->setDirty( true );
                    //        }
                    //    }
                    //}
                }
            }
        }
        else
        {
            if( auto mainWindow = applicationManager->getWindow() )
            {
                //auto mainWindowSize = mainWindow->getSize();

                //auto renderManager =
                //    (MyGUI::OgreRenderManager *)MyGUI::RenderManager::getInstancePtr();

                //auto viewSize = renderManager->getViewSize();
                //if( viewSize.width != mainWindowSize.X() || viewSize.height != mainWindowSize.Y() )
                //{
                //    renderManager->setViewSize( mainWindowSize.X(), mainWindowSize.Y() );

                //    auto elements = getElements();
                //    for( auto &element : elements )
                //    {
                //        if( auto stateContext = element->getStateContext() )
                //        {
                //            stateContext->setDirty( true );
                //        }
                //    }
                //}
            }
        }

        auto task = Thread::getCurrentTask();
        if( task == Thread::Task::Render )
        {
            // const Array<IGUIElement*>& children = m_root->getChildren();
            // for(u32 i=0; i<children.size(); ++i)
            //{
            //	IGUIElement* item = (IGUIElement*)children[i];
            //	item->update();
            // }
        }
    }

    auto CUIManager::addApplication() -> SmartPtr<IUIApplication>
    {
        return nullptr;
    }

    void CUIManager::removeApplication( SmartPtr<IUIApplication> application )
    {
    }

    auto CUIManager::addElement( hash64 type ) -> SmartPtr<IUIElement>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto BUTTON_TYPEINFO = IUIButton::typeInfo();
        auto LAYOUT_TYPEINFO = IUILayout::typeInfo();
        auto CONTAINER_TYPEINFO = IUIContainer::typeInfo();
        auto IMAGE_TYPEINFO = IUIImage::typeInfo();
        auto TEXT_TYPEINFO = IUIText::typeInfo();
        auto TERRAIN_TYPEINFO = IUITerrainEditor::typeInfo();

        if( type == BUTTON_TYPEINFO )
        {
            auto element = fb::make_ptr<CUIButton>();
            graphicsSystem->loadObject( element );
            addElementPtr( element );
            return element;
        }
        if( type == LAYOUT_TYPEINFO )
        {
            auto element = fb::make_ptr<CUILayout>();
            graphicsSystem->loadObject( element );
            addElementPtr( element );
            return element;
        }
        if( type == CONTAINER_TYPEINFO )
        {
            auto element = fb::make_ptr<CUIContainer>();
            graphicsSystem->loadObject( element );
            addElementPtr( element );
            return element;
        }
        if( type == IMAGE_TYPEINFO )
        {
            auto element = fb::make_ptr<CUIImage>();
            graphicsSystem->loadObject( element );
            addElementPtr( element );
            return element;
        }
        if( type == TEXT_TYPEINFO )
        {
            auto element = fb::make_ptr<CUIText>();
            graphicsSystem->loadObject( element );
            addElementPtr( element );
            return element;
        }
        if( type == TERRAIN_TYPEINFO )
        {
            //auto element = fb::make_ptr<CUITerrainEditor>();
            //graphicsSystem->loadObject( element );
            //addElementPtr( element );
            //return element;
        }

        return nullptr;
    }

    auto CUIManager::addElement( SmartPtr<IUIElement> parent, u8 type ) -> SmartPtr<IUIElement>
    {
        //auto eType = static_cast<ElementType>( type );
        //switch( eType )
        //{
        //case ElementType::Layout:
        //{
        //    auto element = fb::make_ptr<CUILayout>();
        //    element->load( nullptr );
        //    addElementPtr( element );
        //    return element;
        //}
        //break;
        //case ElementType::Container:
        //{
        //    auto element = fb::make_ptr<CUIContainer>();
        //    element->load( nullptr );
        //    addElementPtr( element );
        //    return element;
        //}
        //break;
        //case ElementType::Image:
        //{
        //    auto element = fb::make_ptr<CUIImage>();
        //    element->load( nullptr );
        //    addElementPtr( element );
        //    return element;
        //}
        //break;
        //case ElementType::Text:
        //{
        //    auto element = fb::make_ptr<CUIText>();
        //    element->load( nullptr );
        //    addElementPtr( element );
        //    return element;
        //}
        //break;
        //default:
        //{
        //}
        //}

        return nullptr;
    }

    auto CUIManager::addElement( SmartPtr<IUIElement> parent, u8 type, Properties &properties )
        -> SmartPtr<IUIElement>
    {
        //auto eType = static_cast<ElementType>( type );
        //switch( eType )
        //{
        //case ElementType::Image:
        //{
        //    auto element = fb::make_ptr<CUIImage>();
        //    element->initialise( parent, properties );
        //    parent->addChild( element.get() );
        //    addElementPtr( element );
        //    return element;
        //}
        //break;
        //default:
        //{
        //}
        //}

        return nullptr;
    }

    void CUIManager::removeElement( SmartPtr<IUIElement> element )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        if( element )
        {
            graphicsSystem->unloadObject( element );

            auto pElements = getElementsPtr();
            if( pElements )
            {
                auto &elements = *pElements;
                auto it = std::find( elements.begin(), elements.end(), element );
                if( it != elements.end() )
                {
                    elements.erase( it );
                }
            }
        }
    }

    void CUIManager::clear()
    {
        if( auto root = getRoot() )
        {
            root->removeAllChildren();
        }
    }

    auto CUIManager::getElementsPtr() const -> SharedPtr<Array<SmartPtr<IUIElement>>>
    {
        return m_elements;
    }

    void CUIManager::setElementsPtr( SharedPtr<Array<SmartPtr<IUIElement>>> ptr )
    {
        m_elements = ptr;
    }

    void CUIManager::setElements( const Array<SmartPtr<IUIElement>> &elements )
    {
        auto ptr = fb::make_shared<Array<SmartPtr<IUIElement>>>();
        *ptr = elements;
        setElementsPtr( ptr );
    }

    void CUIManager::addElementPtr( SmartPtr<IUIElement> element )
    {
        auto pElements = getElementsPtr();
        if( !pElements )
        {
            pElements = fb::make_shared<Array<SmartPtr<IUIElement>>>();
            setElementsPtr( pElements );
        }

        if( pElements )
        {
            auto &elements = *pElements;
            elements.push_back( element );
        }
    }

    void CUIManager::setRoot( SmartPtr<IUIElement> root )
    {
        m_root = root;
    }

    auto CUIManager::getRoot() const -> SmartPtr<IUIElement>
    {
        return m_root;
    }

    void CUIManager::reloadCurrentLayout()
    {
        // const auto& children = m_root->getChildren();
        // for (u32 childIdx = 0; childIdx < children.size(); ++childIdx)
        //{
        //	SmartPtr<IUIElement> item;// = (IGUIElement*)children[childIdx];
        //	if (item->isVisible() && item->getType() == (String("Layout")))
        //	{
        //		for (u32 listenerIdx = 0; listenerIdx < m_managerListeners.size(); ++listenerIdx)
        //		{
        //			auto listener = m_managerListeners[listenerIdx];
        //			listener->OnStartReloadScripts();
        //		}

        //		String filePath = m_layoutFileMap[item->getName()];
        //		m_currentFileName = filePath;

        //		m_root->removeChild(item.get());
        //		//item.setNull();

        //		SmartPtr<IStream> stream =
        //IApplicationManager::instance()->getFileSystem()->open(filePath);

        //		TiXmlDocument* doc = new TiXmlDocument;
        //		doc->Parse(stream->getAsString().c_str());

        //		if (doc->Error())
        //		{
        //			String decription = String("Error - Could not load xml file: ") + filePath
        //				+ String("Error: ") + doc->ErrorDesc();
        //			FB_LOG_MESSAGE("GUI", decription.c_str());
        //			//MessageBoxUtil::show(decription.c_str());
        //			return;
        //		}

        //		TiXmlElement* root = doc->RootElement();
        //		if (root)
        //		{
        //			createGUIItem(m_root, root);
        //		}

        //		//m_root->getChildren().getLast()->setVisible(true);
        //		//m_root->getChildren().getLast()->setEnabled(true);

        //		for (u32 listenerIdx = 0; listenerIdx < m_managerListeners.size(); ++listenerIdx)
        //		{
        //			auto listener = m_managerListeners[listenerIdx];
        //			listener->OnFinishReloadScripts();
        //		}

        //		printf("Finished reloading gui.");
        //	}
        //}
    }

    auto CUIManager::getCursor() const -> SmartPtr<IUICursor>
    {
        return m_cursor;
    }

    auto CUIManager::findElement( const String &id ) const -> SmartPtr<IUIElement>
    {
        if( auto root = getRoot() )
        {
            SmartPtr<IUIElement> item = root->findChildById( id );
            if( !item )
            {
                String message = String( "Could not find gui item: " ) + id;
                FB_LOG_MESSAGE( "GUI", message.c_str() );
            }

            return item;
        }

        return nullptr;
    }

    auto CUIManager::getApplication() const -> SmartPtr<IUIApplication>
    {
        return nullptr;
    }

    void CUIManager::setApplication( SmartPtr<IUIApplication> application )
    {
    }

    auto CUIManager::getWindows() const -> Array<SmartPtr<IUIWindow>>
    {
        return m_windows;
    }

    void CUIManager::setWindows( Array<SmartPtr<IUIWindow>> val )
    {
        m_windows = val;
    }

    auto CUIManager::getRenderWindows() const -> Array<SmartPtr<IUIRenderWindow>>
    {
        return {};
    }

    void CUIManager::setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val )
    {
    }

    auto CUIManager::getFileBrowsers() const -> Array<SmartPtr<IUIFileBrowser>>
    {
        return {};
    }

    void CUIManager::setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val )
    {
    }

    auto CUIManager::isDragging() const -> bool
    {
        return m_dragging;
    }

    void CUIManager::setDragging( bool dragging )
    {
        m_dragging = dragging;
    }

    auto CUIManager::getMainWindow() const -> SmartPtr<IUIWindow>
    {
        return m_uiWindow;
    }

    void CUIManager::setMainWindow( SmartPtr<IUIWindow> uiWindow )
    {
        m_uiWindow = uiWindow;
    }

    void CUIManager::lock()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        graphicsSystem->lock();
    }

    void CUIManager::unlock()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        graphicsSystem->unlock();
    }

    void CUIManager::_getObject( void **ppObject )
    {
    }

    auto CUIManager::getFactoryManager() const -> SmartPtr<IFactoryManager>
    {
        return m_factoryManager;
    }

    void CUIManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
    {
        m_factoryManager = factoryManager;
    }

    auto CUIManager::getElements() const -> Array<SmartPtr<IUIElement>>
    {
        if( auto p = getElementsPtr() )
        {
            auto &elements = *p;
            return Array<SmartPtr<IUIElement>>( elements.begin(), elements.end() );
        }

        return {};
    }

    CUIManager::InputListener::InputListener( CUIManager *mgr ) : m_mgr( mgr )
    {
    }

    auto CUIManager::InputListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                 const Array<Parameter> &arguments,
                                                 SmartPtr<ISharedObject> sender,
                                                 SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( eventValue == IEvent::inputEvent )
        {
            m_mgr->OnEvent( event );
        }

        return {};
    }
}  // namespace fb::ui
