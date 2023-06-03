#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FrameStatistics.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FrameStatistics, SharedObject<IFrameStatistics> );

    FrameStatistics::FrameStatistics()
    {
    }

    FrameStatistics::~FrameStatistics()
    {
        unload( nullptr );
    }

    void FrameStatistics::update()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();
        FB_ASSERT( timer );

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        switch( task )
        {
        case Thread::Task::Primary:
        {
            auto &nextFPSUpdate = m_nextUpdate[static_cast<u32>( task )];
            if( nextFPSUpdate < t )
            {
                if( m_fpsText )
                {
                    auto fps = 1.0 / dt;
                    auto fpsStr = "Primary: " + StringUtil::toString( fps );
                    m_fpsText->setCaption( fpsStr );
                }

                nextFPSUpdate = t + static_cast<time_interval>( 1.0 );
            }
        }
        break;
        case Thread::Task::Render:
        {
            auto &nextFPSUpdate = m_nextUpdate[static_cast<u32>( task )];
            if( nextFPSUpdate < t )
            {
                if( m_fpsText )
                {
                    auto fps = 1.0 / dt;
                    auto fpsStr = "Render: " + StringUtil::toString( fps );
                    m_renderText->setCaption( fpsStr );
                }

                nextFPSUpdate = t + static_cast<time_interval>( 1.0 );
            }
        }
        break;
        case Thread::Task::Physics:
        {
            auto &nextFPSUpdate = m_nextUpdate[static_cast<u32>( task )];
            if( nextFPSUpdate < t )
            {
                if( m_physicsFPSText )
                {
                    auto fps = 1.0 / dt;
                    auto fpsStr = "Physics: " + StringUtil::toString( fps );
                    m_physicsFPSText->setCaption( fpsStr );
                }

                nextFPSUpdate = t + static_cast<time_interval>( 1.0 );
            }
        }
        break;
        case Thread::Task::Application:
        {
            auto &nextFPSUpdate = m_nextUpdate[static_cast<u32>( task )];
            if( nextFPSUpdate < t )
            {
                if( m_applicationFPSText )
                {
                    auto fps = 1.0 / dt;
                    auto fpsStr = "Application: " + StringUtil::toString( fps );
                    m_applicationFPSText->setCaption( fpsStr );
                }

                nextFPSUpdate = t + static_cast<time_interval>( 1.0 );
            }
        }
        break;
        }
    }

    void FrameStatistics::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto overlayManager = graphicsSystem->getOverlayManager();
            FB_ASSERT( overlayManager );

            m_statsOverlay = overlayManager->addOverlay( "stats" );
            FB_ASSERT( m_statsOverlay );

            if( m_statsOverlay )
            {
                /*
                auto statsContainer = overlayManager->addElement( "Panel", "StatsContainer" );
                m_statsOverlay->addElement( statsContainer );
                statsContainer->setMetricsMode(
                    static_cast<u8>( render::IOverlayElement::GuiMetricsMode::GMM_PIXELS ) );
                    */

                auto referenceScreenSize = Vector2F( 1920.0f, 1080.0f );

                m_fpsText = overlayManager->addElement( "TextArea", "fpsText" );
                //m_fpsText->setFontName( "SdkTrays/Caption" );
                m_fpsText->setPosition( Vector2F( 0, 0 ) / referenceScreenSize );
                m_fpsText->setSize( Vector2F( 100, 40 ) / referenceScreenSize );
                m_fpsText->setHorizontalAlignment( 0 );
                m_fpsText->setVerticalAlignment( 0 );
                m_statsOverlay->addElement( m_fpsText );

                m_physicsFPSText = overlayManager->addElement( "TextArea", "physicsFPSText" );
                //m_physicsFPSText->setFontName( "SdkTrays/Caption" );
                m_physicsFPSText->setHorizontalAlignment( 0 );
                m_physicsFPSText->setVerticalAlignment( 0 );
                m_physicsFPSText->setPosition( Vector2F( 0, 20 ) / referenceScreenSize );
                m_statsOverlay->addElement( m_physicsFPSText );

                m_applicationFPSText = overlayManager->addElement( "TextArea", "applicationFPSText" );
                //m_applicationFPSText->setFontName( "SdkTrays/Caption" );
                m_applicationFPSText->setHorizontalAlignment( 0 );
                m_applicationFPSText->setVerticalAlignment( 0 );
                m_applicationFPSText->setPosition( Vector2F( 0, 40 ) / referenceScreenSize );
                m_statsOverlay->addElement( m_applicationFPSText );

                m_renderText = overlayManager->addElement( "TextArea", "renderText" );
                //m_renderText->setFontName( "SdkTrays/Caption" );
                m_renderText->setHorizontalAlignment( 0 );
                m_renderText->setVerticalAlignment( 0 );
                m_renderText->setPosition( Vector2F( 0, 60 ) / referenceScreenSize );
                m_statsOverlay->addElement( m_renderText );

                m_statsOverlay->setVisible( true );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FrameStatistics::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                if( m_statsOverlay )
                {
                    overlayManager->removeOverlay( m_statsOverlay );
                    m_statsOverlay = nullptr;
                }

                if( m_fpsText )
                {
                    overlayManager->removeElement( m_fpsText );
                    m_fpsText = nullptr;
                }

                if( m_physicsFPSText )
                {
                    overlayManager->removeElement( m_physicsFPSText );
                    m_physicsFPSText = nullptr;
                }

                if( m_applicationFPSText )
                {
                    overlayManager->removeElement( m_applicationFPSText );
                    m_applicationFPSText = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    bool FrameStatistics::isValid() const
    {
        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded )
        {
            if( m_statsOverlay && m_fpsText && m_physicsFPSText && m_applicationFPSText )
            {
                for( auto val : m_nextUpdate )
                {
                    auto fValue = static_cast<f64>( val );
                    if( !MathD::isFinite( fValue ) )
                    {
                        return false;
                    }
                }

                if( !( m_statsOverlay->isValid() && m_fpsText->isValid() &&
                       m_physicsFPSText->isValid() && m_applicationFPSText->isValid() ) )
                {
                    return false;
                }

                return true;
            }
        }
        else if( loadingState == LoadingState::Unloaded )
        {
            if( !( m_statsOverlay && m_fpsText && m_physicsFPSText && m_applicationFPSText ) )
            {
                return true;
            }
        }

        return false;
    }

    void FrameStatistics::setVisible( bool visible )
    {
        if( m_statsOverlay )
        {
            m_statsOverlay->setVisible( visible );
        }
    }

    bool FrameStatistics::isVisible() const
    {
        if( m_statsOverlay )
        {
            return m_statsOverlay->isVisible();
        }

        return false;
    }
}  // end namespace fb
