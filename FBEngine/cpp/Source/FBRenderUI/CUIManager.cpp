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

namespace fb
{
    namespace ui
    {
        CUIManager::CUIManager() :
            m_itemInFocus( nullptr )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
            if(inputMgr)
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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                setLoadingState( LoadingState::Loaded );
            }
            catch(std::exception &e)
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

                if(auto root = getRoot())
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
            catch(std::exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUIManager::initialise()
        {
        }

        size_t CUIManager::messagePump( SmartPtr<ISharedObject> data )
        {
            return 0;
        }

        bool CUIManager::OnEvent( const SmartPtr<IInputEvent> &event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto elements = getElements();
            for(auto element : elements)
            {
                if(element)
                {
                    element->handleEvent( event );
                }
            }

            if(event->getEventType() == IInputEvent::EventType::Mouse)
            {
                if(auto mouseState = event->getMouseState())
                {
                    auto absolutePosition = mouseState->getAbsolutePosition();
                    auto x = static_cast<s32>(absolutePosition.X());
                    auto y = static_cast<s32>(absolutePosition.Y());
                    auto z = 0;

                    auto relativePosition = mouseState->getRelativePosition();

                    if(auto uiWindow = applicationManager->getSceneRenderWindow())
                    {
                        if(auto mainWindow = applicationManager->getWindow())
                        {
                            auto mainWindowSize = mainWindow->getSize();
                            auto mainWindowSizeF = Vector2F( static_cast<f32>(mainWindowSize.x),
                                                             static_cast<f32>(mainWindowSize.y) );

                            auto sceneWindowPosition = uiWindow->getPosition();
                            auto sceneWindowSize = uiWindow->getSize();

                            auto pos = sceneWindowPosition / mainWindowSizeF;
                            auto size = sceneWindowSize / mainWindowSizeF;

                            auto aabb = AABB2F( pos, size, true );
                            if(aabb.isInside( relativePosition ))
                            {
                                x = static_cast<s32>(( absolutePosition.X() - sceneWindowPosition.X() ) /
                                                     size.X());
                                y = static_cast<s32>(( absolutePosition.Y() - sceneWindowPosition.Y() ) /
                                                     size.Y());

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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            if(applicationManager->isEditor())
            {
                if(auto uiWindow = applicationManager->getSceneRenderWindow())
                {
                    if(auto mainWindow = applicationManager->getWindow())
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
                        //        if( auto stateContext = element->getStateObject() )
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
                if(auto mainWindow = applicationManager->getWindow())
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
                    //        if( auto stateContext = element->getStateObject() )
                    //        {
                    //            stateContext->setDirty( true );
                    //        }
                    //    }
                    //}
                }
            }

            auto task = Thread::getCurrentTask();
            if(task != Thread::Task::Application)
            {
            }

            // const Array<IGUIElement*>& children = m_root->getChildren();
            // for(u32 i=0; i<children.size(); ++i)
            //{
            //	IGUIElement* item = (IGUIElement*)children[i];
            //	item->update();
            // }
        }

        SmartPtr<IUIApplication> CUIManager::addApplication()
        {
            return nullptr;
        }

        void CUIManager::removeApplication( SmartPtr<IUIApplication> application )
        {
        }

        SmartPtr<IUIElement> CUIManager::addElement( hash64 type )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto BUTTON_TYPEINFO = IUIButton::typeInfo();
            auto LAYOUT_TYPEINFO = IUILayout::typeInfo();
            auto CONTAINER_TYPEINFO = IUIContainer::typeInfo();
            auto IMAGE_TYPEINFO = IUIImage::typeInfo();
            auto TEXT_TYPEINFO = IUIText::typeInfo();
            auto TERRAIN_TYPEINFO = IUITerrainEditor::typeInfo();

            if(type == BUTTON_TYPEINFO)
            {
                auto element = fb::make_ptr<CUIButton>();
                graphicsSystem->loadObject( element );
                addElementPtr( element );
                return element;
            }
            if(type == LAYOUT_TYPEINFO)
            {
                auto element = fb::make_ptr<CUILayout>();
                graphicsSystem->loadObject( element );
                addElementPtr( element );
                return element;
            }
            if(type == CONTAINER_TYPEINFO)
            {
                auto element = fb::make_ptr<CUIContainer>();
                graphicsSystem->loadObject( element );
                addElementPtr( element );
                return element;
            }
            if(type == IMAGE_TYPEINFO)
            {
                auto element = fb::make_ptr<CUIImage>();
                graphicsSystem->loadObject( element );
                addElementPtr( element );
                return element;
            }
            if(type == TEXT_TYPEINFO)
            {
                auto element = fb::make_ptr<CUIText>();
                graphicsSystem->loadObject( element );
                addElementPtr( element );
                return element;
            }
            if(type == TERRAIN_TYPEINFO)
            {
                //auto element = fb::make_ptr<CUITerrainEditor>();
                //graphicsSystem->loadObject( element );
                //addElementPtr( element );
                //return element;
            }

            return nullptr;
        }

        SmartPtr<IUIElement> CUIManager::addElement( SmartPtr<IUIElement> parent, u8 type )
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

        SmartPtr<IUIElement> CUIManager::addElement( SmartPtr<IUIElement> parent, u8 type,
                                                     Properties &properties )
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
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if(element)
            {
                graphicsSystem->unloadObject( element );

                auto pElements = getElementsPtr();
                if(pElements)
                {
                    auto &elements = *pElements;
                    auto it = std::find( elements.begin(), elements.end(), element );
                    if(it != elements.end())
                    {
                        elements.erase( it );
                    }
                }
            }
        }

        void CUIManager::clear()
        {
            if(auto root = getRoot())
            {
                root->removeAllChildren();
            }
        }

        SharedPtr<Array<SmartPtr<IUIElement>>> CUIManager::getElementsPtr() const
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
            if(!pElements)
            {
                pElements = fb::make_shared<Array<SmartPtr<IUIElement>>>();
                setElementsPtr( pElements );
            }

            if(pElements)
            {
                auto &elements = *pElements;
                elements.push_back( element );
            }
        }

        void CUIManager::setRoot( SmartPtr<IUIElement> root )
        {
            m_root = root;
        }

        SmartPtr<IUIElement> CUIManager::getRoot() const
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

        //bool CUIManager::load( const String &fileName )
        //{
        //    m_currentFileName = fileName;

        //    auto fileSystem = core::IApplicationManager::instance()->getFileSystem();
        //    SmartPtr<IStream> data = fileSystem->open( fileName );
        //    if( !data )
        //    {
        //        return false;
        //    }

        //    String xmlStr = data->getAsString();

        //    auto doc = new TiXmlDocument;
        //    doc->Parse( xmlStr.c_str() );

        //    if( doc->Error() )
        //    {
        //        String decription = String( "Error - Could not load xml file: " ) + fileName +
        //                            String( "Error: " ) + doc->ErrorDesc();
        //        FB_LOG_MESSAGE( "GUI", decription.c_str() );
        //        delete doc;
        //        return false;
        //    }

        //    String message = String( "Loading File: " ) + fileName;
        //    FB_LOG_MESSAGE( "GUI", message );

        //    TiXmlElement *root = doc->RootElement();
        //    if( root )
        //    {
        //        createGUIItem( m_root, root );
        //    }

        //    delete doc;
        //    return true;
        //}

        SmartPtr<IUICursor> CUIManager::getCursor() const
        {
            return m_cursor;
        }

        SmartPtr<IUIElement> CUIManager::findElement( const String &id ) const
        {
            if(auto root = getRoot())
            {
                SmartPtr<IUIElement> item = root->findChildById( id );
                if(!item)
                {
                    String message = String( "Could not find gui item: " ) + id;
                    FB_LOG_MESSAGE( "GUI", message.c_str() );
                }

                return item;
            }

            return nullptr;
        }

        SmartPtr<IUIApplication> CUIManager::getApplication() const
        {
            return nullptr;
        }

        void CUIManager::setApplication( SmartPtr<IUIApplication> application )
        {
        }

        Array<SmartPtr<IUIWindow>> CUIManager::getWindows() const
        {
            return m_windows;
        }

        void CUIManager::setWindows( Array<SmartPtr<IUIWindow>> val )
        {
            m_windows = val;
        }

        Array<SmartPtr<IUIRenderWindow>> CUIManager::getRenderWindows() const
        {
            return Array<SmartPtr<IUIRenderWindow>>();
        }

        void CUIManager::setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val )
        {
        }

        Array<SmartPtr<IUIFileBrowser>> CUIManager::getFileBrowsers() const
        {
            return Array<SmartPtr<IUIFileBrowser>>();
        }

        void CUIManager::setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val )
        {
        }

        bool CUIManager::isDragging() const
        {
            return m_dragging;
        }

        void CUIManager::setDragging( bool dragging )
        {
            m_dragging = dragging;
        }

        s32 CUIManager::setProperty( hash32 hash, void *param )
        {
            return 0;
        }

        s32 CUIManager::setProperty( hash32 hash, const Parameters &params )
        {
            return 0;
        }

        s32 CUIManager::setProperty( hash32 hash, const Parameter &param )
        {
            return 0;
        }

        s32 CUIManager::setProperty( hash32 hash, const String &value )
        {
            return 0;
        }

        s32 CUIManager::getProperty( hash32 hash, void *param ) const
        {
            return 0;
        }

        s32 CUIManager::getProperty( hash32 hash, Parameters &params ) const
        {
            return 0;
        }

        s32 CUIManager::getProperty( hash32 hash, Parameter &param ) const
        {
            return 0;
        }

        s32 CUIManager::getProperty( hash32 hash, String &value ) const
        {
            return 0;
        }

        void CUIManager::setObject( hash32 hash, SmartPtr<ISharedObject> object )
        {
        }

        void CUIManager::getObject( hash32 hash, SmartPtr<ISharedObject> &object ) const
        {
        }

        s32 CUIManager::callFunction( u32 hash, SmartPtr<ISharedObject> object, Parameters &results )
        {
            return 0;
        }

        s32 CUIManager::callFunction( u32 hash, const Parameters &params, Parameters &results )
        {
            return 0;
        }

        SmartPtr<IUIWindow> CUIManager::getMainWindow() const
        {
            return m_uiWindow;
        }

        void CUIManager::setMainWindow( SmartPtr<IUIWindow> uiWindow )
        {
            m_uiWindow = uiWindow;
        }

        void CUIManager::lock()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            graphicsSystem->lock();
        }

        void CUIManager::unlock()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            graphicsSystem->unlock();
        }

        void CUIManager::createGUIItem( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            // if(!pNode)
            //{
            //	LOG_MESSAGE("CGUIManager", "error: CGUIManager::createGUIItem - null node passed in.");
            //	return;
            // }

            // SmartPtr<IGUIElement> curParent = parent;

            // TiXmlNode* pCurItemNode = (TiXmlNode*)pNode;
            // TiXmlNode* pCurNode = pCurItemNode;
            // while( pCurNode )
            //{
            //	const String value(pCurNode->Value());

            //	//loop through the values
            //	if( value==( String("AnimatedMaterial") ))
            //	{
            //		CAnimatedMaterial* image = new CAnimatedMaterial;
            //		image->initialise(parent, pCurNode);
            //		parent->addChild(image);
            //		image->removeReference();

            //		curParent = image;
            //	}
            //	if( value==( String("Container") ))
            //	{
            //		CGUIContainer* container = new CGUIContainer;
            //		container->initialise(parent, pCurNode);
            //		parent->addChild(container);
            //		container->removeReference();

            //		curParent = container;
            //	}
            //	else if( value==(String("DialogBox")) )
            //	{
            //		CGUIDialogBox* dialogBox = new CGUIDialogBox;
            //		dialogBox->initialise(parent, pCurNode);
            //		parent->addChild(dialogBox);
            //		dialogBox->removeReference();

            //		curParent = dialogBox;
            //	}
            //	else if( value==(String("Bar")) )
            //	{
            //		CGUIBar* bar = new CGUIBar;
            //		bar->initialise(parent, pCurNode);
            //		parent->addChild(bar);
            //		bar->removeReference();

            //		curParent = bar;
            //	}
            //	else if( value==(String("ElementBar")) )
            //	{
            //		CGUIElementBar* bar = new CGUIElementBar;
            //		bar->initialise(parent, pCurNode);
            //		parent->addChild(bar);
            //		bar->removeReference();

            //		curParent = bar;
            //	}
            //	else if( value==(String("Button")) )
            //	{
            //		CGUIButton* button = new CGUIButton;
            //		button->initialise(parent, pCurNode);
            //		parent->addChild(button);
            //		button->removeReference();

            //		curParent = button;
            //	}
            //	else if( value==(String("ToggleButton")) )
            //	{
            //		CGUIToggleButton* button = new CGUIToggleButton;
            //		button->initialise(parent, pCurNode);
            //		parent->addChild(button);
            //		button->removeReference();

            //		curParent = button;
            //	}
            //	else if( value==(String("Image")) )
            //	{
            //		CGUIImage* image = new CGUIImage;
            //		image->initialise(parent, pCurNode);
            //		parent->addChild(image);
            //		image->removeReference();

            //		curParent = image;
            //	}
            //	else if( value==(String("VectorImage")) )
            //	{
            //		CGUIVector* image = new CGUIVector;
            //		image->initialise(parent, pCurNode);
            //		parent->addChild(image);
            //		image->removeReference();

            //		curParent = image;
            //	}
            //	else if( value==(String("ImageArray")) )
            //	{
            //		CGUIImageArray* imageArray = new CGUIImageArray;
            //		imageArray->initialise(parent, pCurNode);
            //		parent->addChild(imageArray);
            //		imageArray->removeReference();

            //		curParent = imageArray;
            //	}
            //	else if( value==(String("Text")) )
            //	{
            //		CGUIText* text = new CGUIText;
            //		text->initialise(parent, pCurNode);
            //		parent->addChild(text);
            //		text->removeReference();

            //		curParent = text;
            //	}
            //	else if( value==(String("TextEntry")) )
            //	{
            //		CGUITextEntry* textEntry = new CGUITextEntry;
            //		textEntry->initialise(parent, pCurNode);
            //		parent->addChild(textEntry);
            //		textEntry->removeReference();

            //		curParent = textEntry;
            //	}
            //	else if( value==(String("Layout")) )
            //	{
            //		CGUILayout* layout = new CGUILayout;
            //		layout->initialise(curParent, pCurNode);
            //		parent->addChild(layout);
            //		layout->removeReference();

            //		curParent = layout;

            //		layout->setLayout(layout);

            //		m_layoutFileMap[layout->getName()] = m_currentFileName;
            //	}
            //	else if( value==(String("Menu")) )
            //	{
            //		CGUIMenu* menu = new CGUIMenu;
            //		menu->initialise(parent, pCurNode);
            //		parent->addChild(menu);
            //		menu->removeReference();

            //		curParent = menu;
            //	}
            //	else if( value==(String("CharacterSelectMenu")) )
            //	{
            //		CGUICharacterSelectMenu* menu = new CGUICharacterSelectMenu;
            //		menu->initialise(parent, pCurNode);
            //		parent->addChild(menu);
            //		menu->removeReference();

            //		curParent = menu;
            //	}
            //	else if( value==(String("MenuItem")) )
            //	{
            //		CGUIMenuItem* menuItem = new CGUIMenuItem;
            //		menuItem->initialise(parent, pCurNode);
            //		parent->addChild(menuItem);
            //		menuItem->removeReference();

            //		curParent = menuItem;
            //	}
            //	else if( value==(String("Spinner")) )
            //	{
            //		CGUISpinner* spinner = new CGUISpinner;
            //		spinner->initialise(parent, pCurNode);
            //		parent->addChild(spinner);
            //		spinner->removeReference();

            //		curParent = spinner;
            //	}
            //	else if( value==(String("ScrollingText")) )
            //	{
            //		CGUIScrollingText* item = new CGUIScrollingText;
            //		item->initialise(parent, pCurNode);
            //		parent->addChild(item);
            //		item->removeReference();

            //		curParent = item;
            //	}
            //	else if( value==(String("ScrollingTxtElement")) )
            //	{
            //		CGUIScrollingTextElement* item = new CGUIScrollingTextElement;
            //		item->initialise(parent, pCurNode);
            //		parent->addChild(item);
            //		item->removeReference();

            //		curParent = item;
            //	}
            //	else if( value==(String("AnimatorPosition")) )
            //	{
            //		const TiXmlElement* element = pCurNode->ToElement();
            //		String name = XmlUtil::getString(element->Attribute("id"));
            //		Vector2F start =
            //StringUtil::parseVector2<f32>(XmlUtil::getString(element->Attribute("start"))); 		Vector2F
            //end = StringUtil::parseVector2<f32>(XmlUtil::getString(element->Attribute("end"))); 		f32
            //animLength = StringUtil::parseFloat(XmlUtil::getString(element->Attribute("animLength")));
            //		bool loop = StringUtil::parseBool(XmlUtil::getString(element->Attribute("loop")),
            //false);

            //		/*
            //		SmartPtr<IAnimator> animator(new PositionAnimator2<IGUIElement>(parent.get(),
            //animLength, start, end), true); 		animator->setId(StringUtil::getHashMakeLower(name));
            //		animator->setLoop(loop);
            //		((GUIElement*)parent.get())->addAnimator(animator);
            //		*/
            //	}
            //	else if( value==(String("FadeEffect")) )
            //	{
            //		CGUIFadeEffect* fadeEffect = new CGUIFadeEffect;
            //		fadeEffect->initialise(parent, pCurNode);
            //		parent->addChild(fadeEffect);
            //		fadeEffect->removeReference();

            //		curParent = fadeEffect;
            //	}
            //	else if( value==(String("ScriptListener")) )
            //	{
            //		CGUIScriptListener* listener = new CGUIScriptListener;
            //		listener->initialise(parent, pCurNode);
            //	}
            //	else if( value==(String("ScriptInputListener")) )
            //	{
            //		CGUIScriptInputListener* listener = new CGUIScriptInputListener;
            //		listener->initialise(parent, pCurNode);
            //	}
            //	else if( value==(String("ScriptMenuListener")) )
            //	{
            //		CGUIScriptMenuListener* listener = new CGUIScriptMenuListener;
            //		listener->initialise(parent, pCurNode);
            //	}
            //	else if( value==(String("Template")) )
            //	{
            //		CGUIItemTemplate* templateItem = new CGUIItemTemplate;
            //		templateItem->initialise(parent, pCurNode);
            //		parent->addChild(templateItem);
            //		templateItem->removeReference();

            //		curParent = templateItem;
            //	}
            //	else
            //	{
            //		if(value.length() > 0)
            //		{
            //			String message = String("Could not create element: ") + value;
            //			LOG_MESSAGE("GUI", message);
            //		}
            //	}

            //	//look for child nodes
            //	TiXmlNode* childNode = pCurNode->FirstChild();
            //	if(childNode)
            //	{
            //		createGUIItem(curParent, childNode);
            //	}

            //	//get the next node
            //	pCurNode = pCurNode->NextSibling();
            //}

            ////if(curParent)
            ////	((IGUIElement*)curParent.get())->_onInitialiseEnd();
        }

        Array<SmartPtr<IUIElement>> CUIManager::getElements() const
        {
            if(auto p = getElementsPtr())
            {
                auto &elements = *p;
                return Array<SmartPtr<IUIElement>>( elements.begin(), elements.end() );
            }

            return Array<SmartPtr<IUIElement>>();
        }

        CUIManager::InputListener::InputListener( CUIManager *mgr ) :
            m_mgr( mgr )
        {
        }

        Parameter CUIManager::InputListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                          const Array<Parameter> &arguments,
                                                          SmartPtr<ISharedObject> sender,
                                                          SmartPtr<ISharedObject> object,
                                                          SmartPtr<IEvent> event )
        {
            if(eventValue == IEvent::inputEvent)
            {
                m_mgr->OnEvent( event );
            }

            return Parameter();
        }
    } // end namespace ui
}     // end namespace fb
