#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/BitUtil.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, CameraController, Component );

    CameraController::CameraController() = default;

    CameraController::~CameraController()
    {
        unload( nullptr );
    }

    void CameraController::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto eventListener = fb::make_ptr<EventListener>();
            eventListener->setOwner( this );
            m_eventListener = eventListener;

            if( auto stateContext = applicationManager->getStateContext() )
            {
                stateContext->addEventListener( eventListener );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CameraController::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateContext = applicationManager->getStateContext() )
            {
                stateContext->removeEventListener( m_eventListener );
            }

            if( m_eventListener )
            {
                m_eventListener->unload( nullptr );
                m_eventListener = nullptr;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CameraController::reload( SmartPtr<ISharedObject> data )
    {
        unload( data );
        load( data );
    }

    void CameraController::setFlag( u32 flag, bool value )
    {
        m_flags = BitUtil::setFlagValue( static_cast<u32>( m_flags ), flag, value );
    }

    auto CameraController::getFlag( u32 flag ) const -> bool
    {
        return BitUtil::getFlagValue( static_cast<u32>( m_flags ), flag );
    }

    void CameraController::setViewportId( u32 viewportId )
    {
        m_viewportId = viewportId;
    }

    auto CameraController::getViewportId() const -> u32
    {
        return m_viewportId;
    }

    void CameraController::handleSetActive( bool active )
    {
        //FB_ASSERT(isLoaded());
        FB_ASSERT( isValid() );

        if( auto actor = getActor() )
        {
            auto cameraComponent = actor->getComponent<Camera>();
            FB_ASSERT( cameraComponent );

            if( cameraComponent )
            {
                cameraComponent->setActive( active );
            }
        }
    }

    auto CameraController::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Component::getChildObjects();
        return objects;
    }

    auto CameraController::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setProperty( "viewportId", m_viewportId );
        return properties;
    }

    void CameraController::setProperties( SmartPtr<Properties> properties )
    {
        Component::setProperties( properties );

        u32 viewportId = m_viewportId;
        properties->getPropertyValue( "viewportId", viewportId );
        m_viewportId = viewportId;
    }

    void CameraController::focusSelection()
    {
    }

    auto CameraController::EventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                       const Array<Parameter> &arguments,
                                                       SmartPtr<ISharedObject> sender,
                                                       SmartPtr<ISharedObject> object,
                                                       SmartPtr<IEvent> event ) -> Parameter
    {
        static const auto focusSelectionHash = StringUtil::getHash( "focus_selection" );

        auto owner = getOwner();

        if( focusSelectionHash == eventValue )
        {
            owner->focusSelection();
        }

        return {};
    }

    void CameraController::EventListener::setOwner( CameraController *owner )
    {
        m_cameraController = owner;
    }

    auto CameraController::EventListener::getOwner() const -> CameraController *
    {
        return m_cameraController;
    }
}  // namespace fb::scene
