#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/WheelController.h>
#include <FBCore/FBCore.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, WheelController, VehicleComponent<IWheelController> );

    WheelController::WheelController()
    {
        // m_suspendedMassFraction = 1.0f;
        // m_radius = 0.02f;
        // m_wheelDamping = 0.25f;
        // m_suspensionDistance = 0.1f;
        // m_springRate = 10.0f;
        // m_suspensionDamper = 0;
        // m_targetPosition = 0;
        // m_forwardExtremumSlip = 0.4f;
        // m_forwardExtrememValue = 1.0f;
        // m_forwardAsymptoteSlip = 0.8f;
        // m_forwardAsymptoteValue = 0.5f;
        // m_forwardStiffness = 1.0f;
        // m_sidewaysExtremumSlip = 0.2f;
        // m_sidewaysExtrememValue = 1.0f;
        // m_sidewaysAsymptoteSlip = 0.5f;
        // m_sidewaysAsymptoteValue = 0.5f;
        // m_sidewaysStiffness = 1.0f;
        // m_isSteeringWheel = false;
    }

    WheelController::~WheelController()
    {
        unload( nullptr );
    }

    void WheelController::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void WheelController::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );
                m_wheelController = nullptr;
                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto WheelController::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();

        properties->setProperty( "massFraction", m_massFraction );
        properties->setProperty( "radius", m_radius );
        properties->setProperty( "wheelDamping", m_wheelDamping );
        properties->setProperty( "suspensionDistance", m_suspensionDistance );
        properties->setProperty( "springRate", m_springRate );
        properties->setProperty( "suspensionDamper", m_suspensionDamper );
        properties->setProperty( "targetPosition", m_targetPosition );
        properties->setProperty( "forwardExtremumSlip", m_forwardExtremumSlip );
        properties->setProperty( "forwardExtrememValue", m_forwardExtrememValue );
        properties->setProperty( "forwardAsymptoteSlip", m_forwardAsymptoteSlip );
        properties->setProperty( "forwardAsymptoteValue", m_forwardAsymptoteValue );
        properties->setProperty( "forwardStiffness", m_forwardStiffness );
        properties->setProperty( "sidewaysExtremumSlip", m_sidewaysExtremumSlip );
        properties->setProperty( "sidewaysExtrememValue", m_sidewaysExtrememValue );
        properties->setProperty( "sidewaysAsymptoteSlip", m_sidewaysAsymptoteSlip );
        properties->setProperty( "sidewaysAsymptoteValue", m_sidewaysAsymptoteValue );
        properties->setProperty( "sidewaysStiffness", m_sidewaysStiffness );
        properties->setProperty( "isSteeringWheel", m_isSteeringWheel );
        properties->setProperty( "Reset", "ResetButton", "button", false );

        return properties;
    }

    void WheelController::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "massFraction", m_massFraction );
        properties->getPropertyValue( "radius", m_radius );
        properties->getPropertyValue( "wheelDamping", m_wheelDamping );
        properties->getPropertyValue( "suspensionDistance", m_suspensionDistance );
        properties->getPropertyValue( "springRate", m_springRate );
        properties->getPropertyValue( "suspensionDamper", m_suspensionDamper );
        properties->getPropertyValue( "targetPosition", m_targetPosition );
        properties->getPropertyValue( "forwardExtremumSlip", m_forwardExtremumSlip );
        properties->getPropertyValue( "forwardExtrememValue", m_forwardExtrememValue );
        properties->getPropertyValue( "forwardAsymptoteSlip", m_forwardAsymptoteSlip );
        properties->getPropertyValue( "forwardAsymptoteValue", m_forwardAsymptoteValue );
        properties->getPropertyValue( "forwardStiffness", m_forwardStiffness );
        properties->getPropertyValue( "sidewaysExtremumSlip", m_sidewaysExtremumSlip );
        properties->getPropertyValue( "sidewaysExtrememValue", m_sidewaysExtrememValue );
        properties->getPropertyValue( "sidewaysAsymptoteSlip", m_sidewaysAsymptoteSlip );
        properties->getPropertyValue( "sidewaysAsymptoteValue", m_sidewaysAsymptoteValue );
        properties->getPropertyValue( "sidewaysStiffness", m_sidewaysStiffness );
        properties->getPropertyValue( "isSteeringWheel", m_isSteeringWheel );

        if( properties->hasProperty( "Reset" ) )
        {
            auto &resetButton = properties->getPropertyObject( "Reset" );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                m_radius = 0.29;
                m_suspensionDistance = 0.1;
                m_wheelDamping = 12000;
            }
        }
    }

    auto WheelController::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( 5 );

        objects.emplace_back( m_wheelController );

        return objects;
    }

    auto WheelController::getWheelController() const -> SmartPtr<IWheelController>
    {
        return m_wheelController;
    }

    void WheelController::setWheelController( SmartPtr<IWheelController> wheelController )
    {
        m_wheelController = wheelController;
    }

    auto WheelController::getMassFraction() const -> f32
    {
        return m_massFraction;
    }

    void WheelController::setMassFraction( f32 val )
    {
        m_massFraction = val;
    }

    auto WheelController::getRadius() const -> f32
    {
        return m_radius;
    }

    void WheelController::setRadius( f32 val )
    {
        m_radius = val;
    }

    auto WheelController::getWheelDamping() const -> f32
    {
        return m_wheelDamping;
    }

    void WheelController::setWheelDamping( f32 val )
    {
        m_wheelDamping = val;
    }

    auto WheelController::getSuspensionDistance() const -> f32
    {
        return m_suspensionDistance;
    }

    void WheelController::setSuspensionDistance( f32 val )
    {
        m_suspensionDistance = val;
    }

    void WheelController::reset()
    {
        m_massFraction = 0.05f;
        m_radius = 0.29f;
        m_wheelDamping = 9000.0f;
        m_suspensionDistance = 0.1f;
        m_springRate = 1.0f;
        m_suspensionDamper = 1.0f;
        m_targetPosition = 0.0f;
        m_forwardExtremumSlip = 10.0f;
        m_forwardExtrememValue = 10.0f;
        m_forwardAsymptoteSlip = 10.0f;
        m_forwardAsymptoteValue = 10.0f;
        m_forwardStiffness = 10.0f;
        m_sidewaysExtremumSlip = 1.0f;
        m_sidewaysExtrememValue = 1.0f;
        m_sidewaysAsymptoteSlip = 1.0f;
        m_sidewaysAsymptoteValue = 1.0f;
        m_sidewaysStiffness = 1.0f;
    }
}  // namespace fb::scene
