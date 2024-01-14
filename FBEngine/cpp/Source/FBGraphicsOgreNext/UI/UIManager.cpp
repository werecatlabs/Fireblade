#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UIManager.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreHlms.h>

#include "UIButton.h"
#include "UIElement.h"
#include "UIImage.h"
#include "UILayout.h"
#include "UISlider.h"
#include "UIText.h"
#include "UIToggle.h"
#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/ColibriCheckbox.h"
#include "ColibriGui/ColibriEditbox.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriProgressbar.h"
#include "ColibriGui/ColibriRadarChart.h"
#include "ColibriGui/ColibriSlider.h"
#include "ColibriGui/ColibriSpinner.h"
#include "ColibriGui/ColibriToggleButton.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/Text/ColibriShaperManager.h"

#include "ColibriGui/Layouts/ColibriLayoutLine.h"
#include "ColibriGui/Layouts/ColibriLayoutMultiline.h"
#include "ColibriGui/Layouts/ColibriLayoutTableSameSize.h"

Colibri::Window *mainWindow = nullptr;
Colibri::Window *vertWindow = nullptr;
Colibri::Window *fullWindow = nullptr;
Colibri::Button *button0 = nullptr;
Colibri::Button *button1 = nullptr;
Colibri::Spinner *spinner0 = nullptr;
Colibri::Checkbox *checkbox0 = nullptr;
Colibri::ToggleButton *toggleButton = nullptr;
Colibri::Editbox *editbox0 = nullptr;
Colibri::Progressbar *progressBar0 = nullptr;
Colibri::Progressbar *progressBar1 = nullptr;
Colibri::Slider *slider1 = nullptr;
Colibri::Slider *slider2 = nullptr;
Colibri::Label *sliderLabel = nullptr;

Colibri::Window *overlapWindow1 = nullptr;
Colibri::Window *overlapWindow2 = nullptr;
Colibri::Button *orderButtonBack = nullptr;
Colibri::Button *orderButtonFront = nullptr;
Colibri::Window *innerOverlapWindow1 = nullptr;
Colibri::Window *innerOverlapWindow2 = nullptr;
Colibri::Button *innerOverlapButton1 = nullptr;
Colibri::Button *innerOverlapButton2 = nullptr;
Colibri::Button *innerToggleOrder = nullptr;

class DemoWidgetListener : public Colibri::WidgetActionListener
{
public:
    virtual ~DemoWidgetListener()
    {
    }

    void notifyWidgetAction( Colibri::Widget *widget, Colibri::Action::Action action ) override
    {
        if( action == Colibri::Action::Action::PrimaryActionPerform )
        {
            if( widget == orderButtonBack )
                overlapWindow1->setZOrder( 3 );
            else if( widget == orderButtonFront )
                overlapWindow1->setZOrder( 5 );
            else if( widget == innerToggleOrder )
            {
                uint8_t oldOrder = innerOverlapWindow1->getZOrder();
                innerOverlapWindow1->setZOrder( innerOverlapWindow2->getZOrder() );
                innerOverlapButton1->setZOrder( innerOverlapWindow2->getZOrder() );
                innerOverlapWindow2->setZOrder( oldOrder );
                innerOverlapButton2->setZOrder( oldOrder );
            }
        }
    }
};

DemoWidgetListener *demoActionListener;

class ColibriLogListener final : public Colibri::LogListener
{
    void log( const char *text, Colibri::LogSeverity::LogSeverity severity ) override
    {
        Ogre::LogManager::getSingleton().logMessage( text );
    }
};

class ColibriListener final : public Colibri::ColibriListener
{
    void setClipboardText( const char *text ) override
    {
        //SDL_SetClipboardText( text );
    }

    bool getClipboardText( char *colibri_nonnull *const colibri_nullable outText ) override
    {
        //*outText = SDL_GetClipboardText();
        return *outText != nullptr;
    }

    void freeClipboardText( char *colibri_nullable text ) override
    {
        free( text );
    }
};

static ColibriLogListener g_colibriLogListener;
static ColibriListener g_colibriListener;

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb::ui, UIManager, IUIManager );

        UIManager::UIManager()
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto inputListener = factoryManager->make_ptr<InputListener>();
            inputListener->setOwner( this );
            m_inputListener = inputListener;
        }

        UIManager::~UIManager()
        {
        }

        void UIManager::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            auto factoryManager = fb::make_ptr<FactoryManager>();
            setFactoryManager( factoryManager );

            ApplicationUtil::addFactory<ui::UILayout>( factoryManager );
            ApplicationUtil::addFactory<ui::UIText>( factoryManager );
            ApplicationUtil::addFactory<ui::UIImage>( factoryManager );
            ApplicationUtil::addFactory<ui::UISlider>( factoryManager );
            ApplicationUtil::addFactory<ui::UIButton>( factoryManager );
            ApplicationUtil::addFactory<ui::UIToggle>( factoryManager );

            factoryManager->setPoolSizeByType<ui::UIButton>( 4 );
            factoryManager->setPoolSizeByType<ui::UIImage>( 4 );

            struct ShaperSettings
            {
                const char *locale;
                const char *fullpath;
                hb_script_t script;
                Colibri::HorizReadingDir::HorizReadingDir horizReadingDir;
                bool useKerning;
                bool allowsVerticalLayout;

                ShaperSettings( const char *_locale, const char *_fullpath, hb_script_t _script,
                                bool _useKerning = false,
                                Colibri::HorizReadingDir::HorizReadingDir _horizReadingDir =
                                    Colibri::HorizReadingDir::LTR,
                                bool _allowsVerticalLayout = false ) :
                    locale( _locale ),
                    fullpath( _fullpath ),
                    script( _script ),
                    horizReadingDir( _horizReadingDir ),
                    useKerning( _useKerning ),
                    allowsVerticalLayout( _allowsVerticalLayout )
                {
                }
            };

            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto sceneManager = graphicsSystem->getGraphicsScene();

            Ogre::String dataPath = applicationManager->getRenderMediaPath() + "/";

            m_colibriManager = new Colibri::ColibriManager( &g_colibriLogListener, &g_colibriListener );
            m_colibriManager->setCanvasSize( Ogre::Vector2( 1920, 1080 ), Ogre::Vector2( 1920, 1080 ) );

            ShaperSettings shaperSettings[3] = {
                ShaperSettings( "en", "Fonts/DejaVuSerif.ttf", HB_SCRIPT_LATIN, true ),
                ShaperSettings( "ar", "Fonts/amiri-0.104/amiri-regular.ttf", HB_SCRIPT_ARABIC, false,
                                Colibri::HorizReadingDir::RTL ),
                ShaperSettings( "ch", "Fonts/fireflysung-1.3.0/fireflysung.ttf", HB_SCRIPT_HAN, false,
                                Colibri::HorizReadingDir::LTR, true )
            };

            Colibri::ShaperManager *shaperManager = m_colibriManager->getShaperManager();

            for( size_t i = 0; i < sizeof( shaperSettings ) / sizeof( shaperSettings[0] ); ++i )
            {
                Colibri::Shaper *shaper;
                const Ogre::String fullPath = dataPath + shaperSettings[i].fullpath;
                shaper = shaperManager->addShaper( shaperSettings[i].script, fullPath.c_str(),
                                                   shaperSettings[i].locale );
                if( shaperSettings[i].useKerning )
                    shaper->addFeatures( Colibri::Shaper::KerningOn );
            }

            size_t defaultFont = 0;  //"en"
            shaperManager->setDefaultShaper( defaultFont + 1u,
                                             shaperSettings[defaultFont].horizReadingDir,
                                             shaperSettings[defaultFont].allowsVerticalLayout );

            shaperManager->addBmpFont( ( dataPath + "Fonts/ExampleBmpFont.fnt" ).c_str() );
            shaperManager->setDefaultBmpFontForRaster( 0u );

            //mCameraController = new CameraController( mGraphicsSystem, false );

            auto canvasSize = Vector2I( 1280, 720 );

            auto root = Ogre::Root::getSingletonPtr();
            //Ogre::Window *window = mGraphicsSystem->getRenderWindow();

            auto resourcePath = applicationManager->getRenderMediaPath() + "/";

            const float aspectRatioColibri =
                static_cast<float>( canvasSize.y ) / static_cast<float>( canvasSize.x );
            m_colibriManager->setCanvasSize( Ogre::Vector2( 1920.0f, 1920.0f * aspectRatioColibri ),
                                             Ogre::Vector2( static_cast<Ogre::Real>( canvasSize.x ),
                                                            static_cast<Ogre::Real>( canvasSize.y ) ) );

            Ogre::SceneManager *smgr = nullptr;
            sceneManager->_getObject( (void **)&smgr );

            //colibriManager = new Colibri::ColibriManager();
            m_colibriManager->setOgre( root, root->getRenderSystem()->getVaoManager(), smgr );
            m_colibriManager->loadSkins(
                ( resourcePath + "Materials/ColibriGui/Skins/DarkGloss/Skins.colibri.json" ).c_str() );

            //createScene01();
            //createScene02();

            m_layoutWindow = m_colibriManager->createWindow( nullptr );
            m_layoutWindow->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );
            m_layoutWindow->setSkin( "EmptyBg" );
            m_layoutWindow->setVisualsEnabled( false );
            //m_layoutWindow->setColour( true, Ogre::ColourValue( 0, 0, 0, 0 ) );
            m_layoutWindow->setHidden( false );
            m_layoutWindow->setKeyboardNavigable( false );
            m_layoutWindow->m_breadthFirst = true;

            auto inputMgr = applicationManager->getInputDeviceManager();
            if( inputMgr )
            {
                inputMgr->addListener( m_inputListener );
            }

            setLoadingState( LoadingState::Loaded );
        }

        void UIManager::reload( SmartPtr<ISharedObject> data )
        {
        }

        void UIManager::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );

            setLoadingState( LoadingState::Unloaded );
        }

        bool UIManager::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            auto task = Thread::getCurrentTask();
            if( task == Thread::Task::Render )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto colibriManager = getColibriManager();

                if( event->getEventType() == IInputEvent::EventType::Mouse )
                {
                    if( auto mouseState = event->getMouseState() )
                    {
                        auto absolutePosition = mouseState->getAbsolutePosition();
                        auto x = absolutePosition.X();
                        auto y = absolutePosition.Y();
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
                                    x = ( absolutePosition.X() - sceneWindowPosition.X() ) / size.X();
                                    y = ( absolutePosition.Y() - sceneWindowPosition.Y() ) / size.Y();

                                    Ogre::Vector2 mousePos( x, y );
                                    auto canvasSize = colibriManager->getCanvasSize();
                                    auto canvasPoint = mousePos / canvasSize;

                                    if( mouseState->getEventType() == IMouseState::Event::Moved )
                                    {
                                        colibriManager->setMouseCursorMoved( mousePos );
                                    }

                                    if( mouseState->getEventType() == IMouseState::Event::LeftPressed )
                                    {
                                        colibriManager->setMouseCursorMoved( mousePos );
                                        colibriManager->setMouseCursorPressed( true, false );
                                    }

                                    if( mouseState->getEventType() == IMouseState::Event::LeftReleased )
                                    {
                                        colibriManager->setMouseCursorReleased();
                                    }
                                }
                            }
                        }
                        else
                        {
                            if( mouseState->getEventType() == IMouseState::Event::Moved )
                            {
                                Ogre::Vector2 mousePos( x, y );
                                colibriManager->setMouseCursorMoved( mousePos *
                                                                     colibriManager->getCanvasSize() );
                            }

                            if( mouseState->getEventType() == IMouseState::Event::LeftPressed )
                            {
                                Ogre::Vector2 mousePos( x, y );
                                colibriManager->setMouseCursorMoved( mousePos *
                                                                     colibriManager->getCanvasSize() );
                                colibriManager->setMouseCursorPressed( true, false );
                            }

                            if( mouseState->getEventType() == IMouseState::Event::LeftReleased )
                            {
                                colibriManager->setMouseCursorReleased();
                            }
                        }
                    }
                }

                auto elements = getElements();
                for( auto &element : elements )
                {
                    if( element )
                    {
                        element->handleEvent( event );
                    }
                }
            }

            return false;
        }

        void UIManager::_getObject( void **ppObject )
        {
            *ppObject = m_colibriManager;
        }

        void UIManager::update()
        {
            if( m_colibriManager )
            {
                m_layoutWindow->updateZOrderDirty();
                m_colibriManager->update( 1.0 / 60.0 );
            }
        }

        size_t UIManager::messagePump( SmartPtr<ISharedObject> data )
        {
            return 0;
        }

        SmartPtr<IUIApplication> UIManager::addApplication()
        {
            return nullptr;
        }

        void UIManager::removeApplication( SmartPtr<IUIApplication> application )
        {
        }

        SmartPtr<IUIApplication> UIManager::getApplication() const
        {
            return nullptr;
        }

        void UIManager::setApplication( SmartPtr<IUIApplication> application )
        {
        }

        SmartPtr<IUIElement> UIManager::addElement( hash64 type )
        {
            auto factoryManager = getFactoryManager();

            RecursiveMutex::ScopedLock lock( m_mutex );
            auto element = factoryManager->make_object<IUIElement>( type );
            m_elements.push_back( element );
            return element;
        }

        void UIManager::removeElement( SmartPtr<IUIElement> element )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( !m_elements.empty() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                graphicsSystem->unloadObject( element );

                auto it = std::remove( m_elements.begin(), m_elements.end(), element );
                if( it != m_elements.end() )
                {
                    m_elements.erase( it, m_elements.end() );
                }
            }
        }

        void UIManager::clear()
        {
        }

        SmartPtr<IUICursor> UIManager::getCursor() const
        {
            return nullptr;
        }

        SmartPtr<IUIElement> UIManager::findElement( const String &id ) const
        {
            return nullptr;
        }

        bool UIManager::isDragging() const
        {
            return m_dragging;
        }

        void UIManager::setDragging( bool dragging )
        {
            m_dragging = dragging;
        }

        SmartPtr<IUIWindow> UIManager::getMainWindow() const
        {
            return m_mainWindow;
        }

        void UIManager::setMainWindow( SmartPtr<IUIWindow> uiWindow )
        {
            m_mainWindow = uiWindow;
        }

        void UIManager::createScene02()
        {
            mainWindow = m_colibriManager->createWindow( nullptr );
        }

        void UIManager::createScene01()
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto sceneManager = graphicsSystem->getGraphicsScene();

            ////mCameraController = new CameraController( mGraphicsSystem, false );

            //Vector2I canvasSize = Vector2I( 1280, 720 );

            //auto root = Ogre::Root::getSingletonPtr();
            ////Ogre::Window *window = mGraphicsSystem->getRenderWindow();

            //auto resourcePath = applicationManager->getRenderMediaPath() + "/";

            //const float aspectRatioColibri =
            //    static_cast<float>( canvasSize.y ) / static_cast<float>( canvasSize.x );
            //colibriManager->setCanvasSize( Ogre::Vector2( 1920.0f, 1920.0f * aspectRatioColibri ),
            //                               Ogre::Vector2( canvasSize.x, canvasSize.y ) );

            //Ogre::SceneManager *smgr = nullptr;
            //sceneManager->_getObject( (void **)&smgr );

            ////colibriManager = new Colibri::ColibriManager();
            //colibriManager->setOgre( root, root->getRenderSystem()->getVaoManager(), smgr );
            //colibriManager->loadSkins(
            //    ( resourcePath + "Materials/ColibriGui/Skins/DarkGloss/Skins.colibri.json" ).c_str() );

            // colibriManager->setTouchOnlyMode( true );

            fullWindow = m_colibriManager->createWindow( nullptr );
            mainWindow = m_colibriManager->createWindow( nullptr );
            //mainWindow->setVisualsEnabled( false );
            vertWindow = m_colibriManager->createWindow( nullptr );

            mainWindow->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 450, 0 ) );
            vertWindow->setTransform( Ogre::Vector2( m_colibriManager->getCanvasSize().x - 64, 0 ),
                                      Ogre::Vector2( 64, 450 ) );

            //When m_breadthFirst is set to true, it can cause significant performance
            //increases for UI-heavy applications. But be sure you understand it i.e.
            //it may not render correctly if your widgets have children and they overlap.
            mainWindow->m_breadthFirst = true;
            vertWindow->m_breadthFirst = true;

            {
                fullWindow->setSize( m_colibriManager->getCanvasSize() );
                fullWindow->setSkin( "EmptyBg" );
                fullWindow->setVisualsEnabled( false );
                Colibri::Label *emojiText = m_colibriManager->createWidget<Colibri::Label>( fullWindow );
                emojiText->setText(
                    "By defining a BMP font you can easily use the private area\n"
                    "to draw emoji-like icons. Big Coffe cup: \uE000 Pizza: \uE001\n"
                    "This is a new line after emojis." );
                emojiText->sizeToFit();
                emojiText->setTopLeft( fullWindow->getSize() - emojiText->getSize() );
            }

            auto layout = new Colibri::LayoutLine( m_colibriManager );
            //layout->addCell( &Colibri::LayoutSpacer::c_DefaultBlankSpacer );

            Colibri::RadarChart *radarChart =
                m_colibriManager->createWidget<Colibri::RadarChart>( mainWindow );
            radarChart->m_minSize = Ogre::Vector2( 512, 512 );
            const std::vector<Colibri::RadarChart::DataEntry> dataSeries = {
                { 0.55f, 0.95f, "STR\n10" },
                { 0.41f, 0.81f, "DEF\n5" },
                { 0.65f, 0.7f, "HP\n100.00" },
                { 0.4f, 0.9f, "MP\n50.00" },
                { 0.75f, 0.85f, "CRT\n11" }
            };
            radarChart->setDataSeries( dataSeries, Colibri::RadarChart::LabelDisplayName );

            button0 = m_colibriManager->createWidget<Colibri::Button>( mainWindow );
            button0->m_minSize = Ogre::Vector2( 350, 64 );
            button0->getLabel()->setText( "This \uE001 is a button" );
            button0->sizeToFit();
            layout->addCell( button0 );

            toggleButton = m_colibriManager->createWidget<Colibri::ToggleButton>( mainWindow );
            toggleButton->m_minSize = Ogre::Vector2( 350, 64 );
            toggleButton->getLabel()->setText( "Toggle Button: Click me" );
            layout->addCell( toggleButton );

            checkbox0 = m_colibriManager->createWidget<Colibri::Checkbox>( mainWindow );
            checkbox0->m_minSize = Ogre::Vector2( 350, 64 );
            checkbox0->getButton()->getLabel()->setText( "This is a checkbox" );
            layout->addCell( checkbox0 );

            checkbox0 = m_colibriManager->createWidget<Colibri::Checkbox>( mainWindow );
            checkbox0->m_minSize = Ogre::Vector2( 350, 64 );
            checkbox0->getButton()->getLabel()->setText( "Left-aligned checkbox" );
            checkbox0->getButton()->getLabel()->setTextHorizAlignment(
                Colibri::TextHorizAlignment::Natural );
            layout->addCell( checkbox0 );

            checkbox0 = m_colibriManager->createWidget<Colibri::Checkbox>( mainWindow );
            checkbox0->m_minSize = Ogre::Vector2( 350, 64 );
            checkbox0->setStateMode( Colibri::Checkbox::TriState );
            checkbox0->getButton()->getLabel()->setText( "This is a tri-state checkbox" );
            //		checkbox0->sizeToFit();
            //		checkbox0->setSize( checkbox0->getSize() + Ogre::Vector2( 0, 32 ) );
            layout->addCell( checkbox0 );

            checkbox0 = m_colibriManager->createWidget<Colibri::Checkbox>( mainWindow );
            checkbox0->m_minSize = Ogre::Vector2( 350, 64 );
            checkbox0->getButton()->getLabel()->setText( "This checkbox has the tickmark outside" );
            checkbox0->setCheckboxMode( Colibri::Checkbox::TickButton );
            layout->addCell( checkbox0 );

            // By setting a Checkbox into Checkbox::NoState mode, it turns into
            // a Button that has an icon on the left or right (depending on Checkbox::setHorizWidgetDir)
            checkbox0 = m_colibriManager->createWidget<Colibri::Checkbox>( mainWindow );
            checkbox0->m_minSize = Ogre::Vector2( 350, 64 );
            checkbox0->getButton()->getLabel()->setText( "This button has an image" );
            checkbox0->setStateMode( Colibri::Checkbox::NoState );
            checkbox0->setTickmarkSkin( 0u, "EmptyBg" );
            layout->addCell( checkbox0 );

#if 1
            spinner0 = m_colibriManager->createWidget<Colibri::Spinner>( mainWindow );
            spinner0->setTopLeft( Ogre::Vector2::ZERO );
            spinner0->m_minSize = Ogre::Vector2( 350, 64 );
            spinner0->getLabel()->setText( "Options" );
            {
                std::vector<std::string> options;
                options.push_back( "Test" );
                options.push_back( "Low" );
                options.push_back( "Medium" );
                options.push_back( "High" );
                //options.push_back( "Double\nLine" );
                //options.push_back( "Triple\nLine\n3x" );
                spinner0->setOptions( options );
            }
            //spinner0->sizeToFit();
            //spinner0->setFixedWidth( true, 0 );
            layout->addCell( spinner0 );

            spinner0 = m_colibriManager->createWidget<Colibri::Spinner>( mainWindow );
            spinner0->setTopLeft( Ogre::Vector2::ZERO );
            spinner0->m_minSize = Ogre::Vector2( 350, 64 );
            spinner0->getLabel()->setText( "Numeric Spinner" );
            layout->addCell( spinner0 );

            spinner0 = m_colibriManager->createWidget<Colibri::Spinner>( mainWindow );
            spinner0->setTopLeft( Ogre::Vector2::ZERO );
            spinner0->m_minSize = Ogre::Vector2( 350, 64 );
            spinner0->setFixedWidths( true, 0.0f, 0.0f );
            spinner0->setHorizWidgetDir( Colibri::HorizWidgetDir::AutoRTL );
            spinner0->getLabel()->setText( "This spinner is on the other side" );
            layout->addCell( spinner0 );

            editbox0 = m_colibriManager->createWidget<Colibri::Editbox>( mainWindow );
            editbox0->m_minSize = Ogre::Vector2( 350, 64 );
            editbox0->setText( "You can edit this text" );
            editbox0->setPlaceholder( "Tooltip visible when empty" );
            editbox0->m_expand[0] = true;
            layout->addCell( editbox0 );

            progressBar0 = m_colibriManager->createWidget<Colibri::Progressbar>( mainWindow );
            progressBar0->m_minSize = Ogre::Vector2( 350, 64 );
            progressBar0->setProgress( 0.75f );
            layout->addCell( progressBar0 );

            progressBar1 = m_colibriManager->createWidget<Colibri::Progressbar>( mainWindow );
            progressBar1->m_minSize = Ogre::Vector2( 350, 64 );
            progressBar1->setProgress( 0.75f );
            progressBar1->setVertical( true );
            progressBar1->getProgressLayer()->setColour( true, Ogre::ColourValue( 0.0f, 0.7f, 0.2f ) );
            layout->addCell( progressBar1 );

            slider1 = m_colibriManager->createWidget<Colibri::Slider>( mainWindow );
            slider1->m_minSize = Ogre::Vector2( 350, 32 );
            layout->addCell( slider1 );

            sliderLabel = m_colibriManager->createWidget<Colibri::Label>( mainWindow );
            sliderLabel->sizeToFit();
            sliderLabel->m_minSize = Ogre::Vector2( 350, 32 );
            layout->addCell( sliderLabel );

            layout->addCell( radarChart );

            {
                const Colibri::LayoutCellVec &cells = layout->getCells();
                auto itor = cells.begin();
                auto end = cells.end();

                while( itor != end )
                {
                    // Leave a margin between adjacents cells so they're not too tight each other
                    ( *itor )->m_margin = 5.0f;
                    // Expend the width of the widget to cover all of the width that is left
                    // available to us. In this case the layout is vertical, thus each widget
                    // is not competing for horizontal space with other widgets, so it will
                    // expand to cover the whole width of the layout
                    //
                    // Note that if we expand, then m_gridLocation left/center/right settings become
                    // useless
                    //
                    // Watch https://www.youtube.com/watch?v=4c9KD9-asaQ for better understanding
                    // of how layouts work and how layouts can be nested to achieve what you want.
                    // If video tutorials are too boring for you, adjust the playback speed to
                    // 1.5x or 2.0x
                    //
                    // Colibri closely follows wxWidgets' layout models (not 100% exact though)
                    // thus what is done in wxFormBuilder can easily be reproduced in Colibri.
                    // Notice that wxFormBuilder has m_proportion and m_expand flags just like
                    // Colibri, although Colibri extends these flags to both X and Y axes, while
                    // wxWidgets alternates the meaning depending on whether the layout they belong to
                    // is vertical or horizontal (which can be very counterintuitive on grid-like layouts)
                    //
                    // That also means sometimes the settings are ignored. e.g. (*itor)->m_proportion[0]
                    // is ignored in a vertical LineLayout.
                    ( *itor )->m_expand[0] = true;

                    ++itor;
                }
            }

            // Do not put slider2 in the layout
            slider2 = m_colibriManager->createWidget<Colibri::Slider>( vertWindow );
            slider2->m_minSize = Ogre::Vector2( 32, 320 );
            slider2->setSize( slider2->m_minSize );
            slider2->setVertical( true );

            layout->setAdjustableWindow( mainWindow );
            layout->m_hardMaxSize = m_colibriManager->getCanvasSize();

            auto layoutW = new Colibri::LayoutLine( m_colibriManager );
            layoutW->setCellSize( m_colibriManager->getCanvasSize() );
            layoutW->addCell( &Colibri::LayoutSpacer::c_DefaultBlankSpacer );
            layoutW->addCell( layout );
            layoutW->layout();

            //Overlapping windows
            demoActionListener = new DemoWidgetListener();

            auto layoutOverlapping = new Colibri::LayoutLine( m_colibriManager );
            overlapWindow1 = m_colibriManager->createWindow( nullptr );
            overlapWindow1->setTransform( Ogre::Vector2( 650, 250 ), Ogre::Vector2( 300, 300 ) );
            overlapWindow1->setZOrder( 3 );

            orderButtonBack = m_colibriManager->createWidget<Colibri::Button>( overlapWindow1 );
            orderButtonBack->m_minSize = Ogre::Vector2( 350, 64 );
            orderButtonBack->getLabel()->setText( "Send to back" );
            orderButtonBack->sizeToFit();

            orderButtonBack->addActionListener( demoActionListener );
            orderButtonFront = m_colibriManager->createWidget<Colibri::Button>( overlapWindow1 );
            orderButtonFront->m_minSize = Ogre::Vector2( 350, 64 );
            orderButtonFront->getLabel()->setText( "Bring to front" );
            orderButtonFront->sizeToFit();
            orderButtonFront->addActionListener( demoActionListener );

            overlapWindow2 = m_colibriManager->createWindow( nullptr );
            overlapWindow2->setTransform( Ogre::Vector2( 750, 450 ), Ogre::Vector2( 300, 300 ) );
            overlapWindow2->setZOrder( 4 );

            layoutOverlapping->addCell( orderButtonBack );
            layoutOverlapping->addCell( orderButtonFront );
            layoutOverlapping->layout();

            {
                innerOverlapWindow1 = m_colibriManager->createWindow( overlapWindow2 );
                innerOverlapWindow1->setTransform( Ogre::Vector2( 10, 10 ), Ogre::Vector2( 100, 100 ) );
                innerOverlapWindow1->setZOrder( 1 );
                innerOverlapWindow2 = m_colibriManager->createWindow( overlapWindow2 );
                innerOverlapWindow2->setTransform( Ogre::Vector2( 20, 20 ), Ogre::Vector2( 100, 100 ) );
                innerOverlapWindow2->setZOrder( 4 );

                innerOverlapButton1 = m_colibriManager->createWidget<Colibri::Button>( overlapWindow2 );
                innerOverlapButton1->getLabel()->setText( "First" );
                innerOverlapButton1->setTransform( Ogre::Vector2( 150, 10 ), Ogre::Vector2( 100, 100 ) );
                innerOverlapButton1->setZOrder( 1 );
                innerOverlapButton2 = m_colibriManager->createWidget<Colibri::Button>( overlapWindow2 );
                innerOverlapButton2->setTransform( Ogre::Vector2( 160, 20 ), Ogre::Vector2( 100, 100 ) );
                innerOverlapButton2->setZOrder( 2 );
                innerOverlapButton2->getLabel()->setText( "Second" );

                innerToggleOrder = m_colibriManager->createWidget<Colibri::Button>( overlapWindow2 );
                innerToggleOrder->m_minSize = Ogre::Vector2( 350, 64 );
                innerToggleOrder->getLabel()->setText( "Switch order" );
                innerToggleOrder->setTopLeft( Ogre::Vector2( 0, 120 ) );
                innerToggleOrder->addActionListener( demoActionListener );
                innerToggleOrder->sizeToFit();
            }

#    if 0
		//Colibri::LayoutLine *layout = new Colibri::LayoutLine( colibriManager );
		Colibri::LayoutMultiline *layout = new Colibri::LayoutMultiline( colibriManager );
		//Colibri::LayoutTableSameSize *layout = new Colibri::LayoutTableSameSize( colibriManager );

		layout->m_numLines = 2;
		//layout->m_numColumns = 3;
		//layout->m_transpose = false;
		//layout->m_softMaxSize = mainWindow->getSizeAfterClipping();
		//layout->m_evenMarginSpaceAtEdges = false;

		//layout->m_vertical = false;

		for( int i=0 ;i<4; ++i )
		{
			button1 = colibriManager->createWidget<Colibri::Button>( mainWindow );
			button1->setTopLeft( Ogre::Vector2( 192, 192 + 192 + 480 + i * 480 ) );
			//button1->setSize( i == 1 ? Ogre::Vector2( 288, 180 ) : Ogre::Vector2( 96, 54 ) );
			button1->setSize( Ogre::Vector2( 288, 180 ) );
			button1->getLabel()->setText( "Button " + Ogre::StringConverter::toString( i ) );

			//button1->setState( Colibri::States::Disabled );

			/*button1->m_proportion[0] = 1;
			button1->m_proportion[1] = 1;*/
			/*button1->m_expand[0] = true;
			button1->m_expand[1] = true;*/

			//layout->addCell( &Colibri::LayoutSpacer::c_DefaultBlankSpacer );
			//layout->addCell( new Colibri::LayoutSpacer() );
			button1->m_margin = Ogre::Vector2( 38 );
			//button1->m_gridLocation = Colibri::GridLocations::BottomRight;
			//button1->m_gridLocation = Colibri::GridLocations::TopRight;
			//button1->m_gridLocation = Colibri::GridLocations::Top;
			//button1->m_gridLocation = Colibri::GridLocations::TopLeft;
			//button1->m_gridLocation = Colibri::GridLocations::Center;
			layout->addCell( button1 );
		}

		//layout->addCell( &Colibri::LayoutSpacer::c_DefaultBlankSpacer );

		layout->layout();
#    endif

#    if 0
		Colibri::Label *label = colibriManager->createWidget<Colibri::Label>( mainWindow );
		label->setText( "The path of the righteous man is beset on all sides by the iniquities\n"
						"of the selfish and the tyranny of evil men. Blessed is he who, in the\n"
						"name of charity and good will, shepherds the weak through the valley \n"
						"of darkness, for he is truly his brother's keeper and the finder of \n"
						"lost children. And I will strike down upon thee with great vengeance \n"
						"and furious anger those who would attempt to poison and destroy My \n"
						"brothers. And you will know My name is the Lord when I lay My \n"
						"vengeance upon thee." );
		/*label->setText( "The path of the righteous man is beset on all sides by the iniquities "
						"of the selfish and the tyranny of evil men. Blessed is he who, in the "
						"name of charity and good will, shepherds the weak through the valley "
						"of darkness, for he is truly his brother's keeper and the finder of "
						"lost children. And I will strike down upon thee with great vengeance "
						"and furious anger those who would attempt to poison and destroy My "
						"brothers. And you will know My name is the Lord when I lay My "
						"vengeance upon thee." );*/
//		label->setText( "The path of the righteous man is beset on all sides by the iniquities "
//						"of the selfish and the tyranny of evil men. Blessed is he who, in the" );
		//label->setText( "The path of the righteous man is beset on all sides by the iniquities" );
		//label->setText( "\tHola qué tal?\t\tHola2\n  Hola qué tal?\tHola2\n" );
		//label->setText( "H\nH" );
		//label->setText( "\tHola\t\tQ\nQue Tal\nHola2\nHola3" );
		//label->setText( "懶惰的姜貓懶惰的姜貓懶惰97\t的姜貓懶惰的姜貓懶惰的姜貓Hola\n懶惰的姜貓Hello\n懶惰的姜貓" );
		//label->setText( "H\tH" );
		//label->setText( "aaa\naaa\n\naga\nagaaaaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" );
		//label->setText( "Hello\t\tTest\nHello2\t\tTest" );
		//label->setText( "\tHello\n    asd" );
		//label->setText( "من أنا لاستجواب أولئك الذين يكتبونh\nola" );
//		label->setText( "من أنا لاستجواب ""\n\r"
//						"أولئك الذين يكتبونhola" );
		{
			/*std::ifstream file( "/home/matias/Desktop/Text2", std::ios::in|std::ios::binary );
			file.seekg( 0, std::ios::end );
			const size_t fileSize = file.tellg();
			file.seekg( 0, std::ios::beg );
			std::string text;
			text.resize( fileSize );
			file.read( &text[0], fileSize );*/
			//label->setText( text );
			//label->setText( "Hola\nQue tal sin paragraph?" );
			//label->setText( "Hola\u2029\nQue tal?" );
			//label->setText( "تجوستجوستجThisتجوستجوستج is a ستجو word ستجوستجوستجوستج" );
			//label->setText( "الذي يحيي ذكرى احتجاجات مواطنين فلسطينيين من مدن" );
		}
		//label->setTextHorizAlignment( Colibri::TextHorizAlignment::Left );
//		label->setTextVertAlignment( Colibri::TextVertAlignment::Center );
		//label->setTextHorizAlignment( Colibri::TextHorizAlignment::Left );
		//label->setText( "The path of the righteous man is beset on all sides by" );
		label->setSize( mainWindow->getSizeAfterClipping() );
		//label->setVertReadingDir( Colibri::VertReadingDir::ForceTTBLTR );
//		label->sizeToFit( Colibri::States::Idle, 0.5f,
//						  Colibri::TextHorizAlignment::Center, Colibri::TextVertAlignment::Center );
		label->setShadowOutline( true, Ogre::ColourValue::Black, Ogre::Vector2( 1.0f ) );
#    endif
#endif

            mainWindow->sizeScrollToFit();
        }

        void UIManager::destroyScene()
        {
            m_colibriManager->destroyWindow( fullWindow );
            m_colibriManager->destroyWindow( mainWindow );
            m_colibriManager->destroyWindow( vertWindow );
            m_colibriManager->destroyWindow( overlapWindow1 );
            m_colibriManager->destroyWindow( overlapWindow2 );
            delete demoActionListener;
            delete m_colibriManager;
        }

        Colibri::ColibriManager *UIManager::getColibriManager() const
        {
            return m_colibriManager;
        }

        void UIManager::setColibriManager( Colibri::ColibriManager *colibriManager )
        {
            m_colibriManager = colibriManager;
        }

        Colibri::Window *UIManager::getLayoutWindow() const
        {
            return m_layoutWindow;
        }

        void UIManager::setLayoutWindow( Colibri::Window *layoutWindow )
        {
            m_layoutWindow = layoutWindow;
        }

        fb::SmartPtr<fb::IFactoryManager> UIManager::getFactoryManager() const
        {
            return m_factoryManager;
        }

        void UIManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
        {
            m_factoryManager = factoryManager;
        }

        fb::Array<fb::SmartPtr<fb::ui::IUIElement>> UIManager::getElements() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_elements;
        }

        void UIManager::setElements( Array<SmartPtr<IUIElement>> elements )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_elements = elements;
        }

        Parameter UIManager::InputListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                         const Array<Parameter> &arguments,
                                                         SmartPtr<ISharedObject> sender,
                                                         SmartPtr<ISharedObject> object,
                                                         SmartPtr<IEvent> event )
        {
            if( Thread::getTaskFlag( Thread::Render_Flag ) )
            {
                if( auto owner = getOwner() )
                {
                    owner->handleEvent( event );
                }
            }

            return Parameter();
        }

        SmartPtr<UIManager> UIManager::InputListener::getOwner() const
        {
            return m_owner;
        }

        void UIManager::InputListener::setOwner( SmartPtr<UIManager> owner )
        {
            m_owner = owner;
        }
    }  // namespace ui
}  // namespace fb
