#include "Application.h"
#include <FBCore/FBCore.h>
#include <FBOISInput/FBOISInput.h>

#if FB_BUILD_PHYSX
#    include <FBPhysx/FBPhysx.h>
#endif

#include <FBSQLite/FBSQLite.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#    include <FBGraphicsOgreNext/FBGraphicsOgreNext.h>
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <FBGraphicsOgre/FBGraphicsOgre.h>
#endif

#if FB_BUILD_IMGUI
#    include <FBImGui/FBImGui.h>
#endif


namespace fb
{

    Application::Application()
    {
    }

    Application::~Application()
    {
        unload( nullptr );
    }

    void Application::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = fb::make_ptr<core::ApplicationManager>();
        core::ApplicationManager::setInstance( applicationManager );

        core::Application::load( data );
    }

    void Application::unload( SmartPtr<ISharedObject> data )
    {
        if( isLoaded() )
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( m_frameStatistics )
            {
                m_frameStatistics->unload( nullptr );
                m_frameStatistics = nullptr;
            }

            graphicsSystem->unload( nullptr );
            graphicsSystem = nullptr;

            Application::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
    }

    void Application::createPlugins()
    {
        try
        {
            FB_DEBUG_TRACE;

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

#ifdef _FB_STATIC_LIB_
            auto corePlugin = fb::make_ptr<FBCore>();
            corePlugin->load( nullptr );
#endif

#ifdef _FB_STATIC_LIB_
            //auto applicationPlugin = fb::make_ptr<ApplicationPlugin>();
            //applicationManager->addPlugin( applicationPlugin );
#endif



#ifdef _FB_STATIC_LIB_
            auto databasePlugin = fb::make_ptr<SQLitePlugin>();
            applicationManager->addPlugin( databasePlugin );
#endif

#ifdef _FB_STATIC_LIB_
            auto inputPlugin = fb::make_ptr<OISInput>();
            applicationManager->addPlugin( inputPlugin );
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_GRAPHICS_SYSTEM_CLAW
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsClaw>();
            applicationManager->addPlugin( graphicsPlugin );
#    elif FB_GRAPHICS_SYSTEM_OGRENEXT
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsOgreNext>();
            applicationManager->addPlugin( graphicsPlugin );
#    elif FB_GRAPHICS_SYSTEM_OGRE
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsOgre>();
            applicationManager->addPlugin( graphicsPlugin );
#    endif
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_BUILD_PHYSX
            auto physxPlugin = fb::make_ptr<physics::FBPhysx>();
            applicationManager->addPlugin( physxPlugin );
#    elif FB_BUILD_ODE
#    endif
#endif

            ApplicationUtil::createFactories();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createScene()
    {
        auto applicationManager = core::ApplicationManager::instance();

        auto uiManager = applicationManager->getUI();
        if( uiManager )
        {
            auto menuBar = uiManager->addElementByType<ui::IUIMenubar>();

            auto fileMenu = uiManager->addElementByType<ui::IUIMenu>();
            fileMenu->setLabel( "File" );
            menuBar->addMenu( fileMenu );

            Util::addMenuItem( fileMenu, static_cast<s32>( ElementId::Open ), "Open", "Open",
                                          ui::IUIMenuItem::Type::Normal );
            Util::addMenuItem( fileMenu, static_cast<s32>( ElementId::Exit ), "Exit", "Exit",
                                          ui::IUIMenuItem::Type::Normal );

            m_application->setMenubar( menuBar );
        }

        if( auto camera = m_camera )
        {
            camera->setRenderUI( true );
        }

        if( auto vp = m_viewport )
        {
            vp->setOverlaysEnabled( false );
            vp->setEnableUI( true );
            vp->setEnableSceneRender( false );
        }
    }

    void Application::createUI()
    {
#if FB_BUILD_IMGUI
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto uiManager = ui::FBImGui::createUI();
        FB_ASSERT( uiManager );
        uiManager->load( nullptr );
        applicationManager->setUI( uiManager );

        auto application = uiManager->addApplication();
        uiManager->setApplication( application );
        m_application = application;
#endif
    }

    void Application::createRenderWindow()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        m_renderWindow = ui->addElementByType<ui::IUIRenderWindow>();
        //FB_ASSERT( m_renderWindow->getWindow() );
    }

}  // namespace fb
