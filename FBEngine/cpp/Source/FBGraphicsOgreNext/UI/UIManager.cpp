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

            ApplicationUtil::addFactory<UILayout>( factoryManager );
            ApplicationUtil::addFactory<UIText>( factoryManager );
            ApplicationUtil::addFactory<UIImage>( factoryManager );
            ApplicationUtil::addFactory<UISlider>( factoryManager );
            ApplicationUtil::addFactory<UIButton>( factoryManager );
            ApplicationUtil::addFactory<UIToggle>( factoryManager );

            factoryManager->setPoolSizeByType<UIButton>( 4 );
            factoryManager->setPoolSizeByType<UIImage>( 4 );

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
            //auto sceneManager = graphicsSystem->getGraphicsScene();
            //FB_ASSERT( sceneManager );

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

            //Ogre::SceneManager *smgr = nullptr;
            //sceneManager->_getObject( (void **)&smgr );

            //colibriManager = new Colibri::ColibriManager();
            //m_colibriManager->setOgre( root, root->getRenderSystem()->getVaoManager(), smgr );

            m_colibriManager->loadSkins(
                ( resourcePath + "Materials/ColibriGui/Skins/DarkGloss/Skins.colibri.json" ).c_str() );

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
            //setupSceneManager();

            //createLayoutWindow();

            if( m_colibriManager )
            {
                if( m_layoutWindow )
                {
                    m_layoutWindow->updateZOrderDirty();
                }

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

        SmartPtr<IFactoryManager> UIManager::getFactoryManager() const
        {
            return m_factoryManager;
        }

        void UIManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
        {
            m_factoryManager = factoryManager;
        }

        Array<SmartPtr<IUIElement>> UIManager::getElements() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_elements;
        }

        void UIManager::setElements( Array<SmartPtr<IUIElement>> elements )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_elements = elements;
        }

        SmartPtr<render::IGraphicsScene> UIManager::getGraphicsScene() const
        {
            return m_graphicsScene;
        }

        void UIManager::setGraphicsScene( SmartPtr<render::IGraphicsScene> graphicsScene )
        {
            m_graphicsScene = graphicsScene;

            if( m_graphicsScene )
            {
                setupSceneManager( m_graphicsScene );
                createLayoutWindow();
            }
        }

        void UIManager::setupSceneManager( SmartPtr<render::IGraphicsScene> sceneManager )
        {
            if( sceneManager )
            {
                auto root = Ogre::Root::getSingletonPtr();

                Ogre::SceneManager *smgr = nullptr;
                sceneManager->_getObject( (void **)&smgr );

                if( smgr )
                {
                    auto renderSystem = root->getRenderSystem();
                    auto vaoManager = renderSystem->getVaoManager();
                    m_colibriManager->setOgre( root, vaoManager, smgr );
                }
            }
        }

        void UIManager::createLayoutWindow()
        {
            m_layoutWindow = m_colibriManager->createWindow( nullptr );
            m_layoutWindow->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );
            m_layoutWindow->setSkin( "EmptyBg" );
            m_layoutWindow->setVisualsEnabled( false );
            //m_layoutWindow->setColour( true, Ogre::ColourValue( 0, 0, 0, 0 ) );
            m_layoutWindow->setHidden( false );
            m_layoutWindow->setKeyboardNavigable( false );
            m_layoutWindow->m_breadthFirst = true;
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
