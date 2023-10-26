#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, CameraController, Component );

        CameraController::CameraController()
        {
        }

        CameraController::~CameraController()
        {
            unload( nullptr );
        }

        void CameraController::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto eventListener = fb::make_ptr<EventListener>();
                eventListener->setOwner( this );
                m_eventListener = eventListener;

                if( auto stateObject = applicationManager->getStateObject() )
                {
                    stateObject->addEventListener( eventListener );
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
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto stateObject = applicationManager->getStateObject() )
                {
                    stateObject->removeEventListener( m_eventListener );
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

        bool CameraController::getFlag( u32 flag ) const
        {
            return BitUtil::getFlagValue( static_cast<u32>( m_flags ), flag );
        }

        void CameraController::setViewportId( u32 viewportId )
        {
            m_viewportId = viewportId;
        }

        u32 CameraController::getViewportId() const
        {
            return m_viewportId;
        }

        bool CameraController::isInFrustum( const AABB3<real_Num> &box ) const
        {
            return true;
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

        Array<SmartPtr<ISharedObject>> CameraController::getChildObjects() const
        {
            auto objects = Component::getChildObjects();
            return objects;
        }

        SmartPtr<Properties> CameraController::getProperties() const
        {
            auto properties = Component::getProperties();
            properties->setProperty( "viewportId", m_viewportId );
            return properties;
        }

        void CameraController::setProperties( SmartPtr<Properties> properties )
        {
            u32 viewportId = m_viewportId;
            properties->getPropertyValue( "viewportId", viewportId );
            m_viewportId = viewportId;
        }

        void CameraController::focusSelection()
        {
        }

        fb::Parameter CameraController::EventListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            static const auto focusSelectionHash = StringUtil::getHash( "focus_selection" );

            auto owner = getOwner();

            if( focusSelectionHash == eventValue )
            {
                owner->focusSelection();
            }

            return Parameter();
        }

        void CameraController::EventListener::setOwner( CameraController *owner )
        {
            m_cameraController = owner;
        }

        CameraController *CameraController::EventListener::getOwner() const
        {
            return m_cameraController;
        }

    }  // namespace scene
}  // end namespace fb
