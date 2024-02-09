#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/Scene/Components/UI/UIComponentUtil.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Transform.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/UI/Text.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Scene/Systems/UI/LayoutTransformSystem.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/State/States/UITransformState.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, LayoutTransform, IComponent );

    LayoutTransform::LayoutTransform() = default;

    LayoutTransform::~LayoutTransform()
    {
        unload( nullptr );
    }

    void LayoutTransform::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );
        Component::load( data );
        setLoadingState( LoadingState::Loaded );
    }

    void LayoutTransform::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        m_uiComponent = nullptr;
        m_layout = nullptr;

        Component::unload( data );
        setLoadingState( LoadingState::Unloaded );
    }

    void LayoutTransform::updateTransform()
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setOwner( this );
            data->setUIComponent( m_uiComponent.get() );
            data->setDirty( true );
        }

        if( auto actor = getActor() )
        {
            auto components = actor->getComponentsInChildren<LayoutTransform>();
            for( auto component : components )
            {
                component->updateTransform();
            }
        }
    }

    auto LayoutTransform::getPosition() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getPosition();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setPosition( const Vector2<real_Num> &position )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setPosition( position );
            data->setAbsolutePosition( position );
        }
    }

    auto LayoutTransform::getSize() const -> Vector2<real_Num>
    {
        auto data = getComponentStateByType<UITransformState>();
        return data->getSize();
    }

    void LayoutTransform::setSize( const Vector2<real_Num> &size )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setSize( size );
            data->setAbsoluteSize( size );
        }
    }

    auto LayoutTransform::getAnchor() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getAnchor();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setAnchor( const Vector2<real_Num> &anchor )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setAnchor( anchor );
        }
    }

    auto LayoutTransform::getAnchorMin() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getAnchorMin();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setAnchorMin( const Vector2<real_Num> &anchorMin )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setAnchorMin( anchorMin );
        }
    }

    auto LayoutTransform::getAnchorMax() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getAnchorMax();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setAnchorMax( const Vector2<real_Num> &anchorMax )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setAnchorMax( anchorMax );
        }
    }

    auto LayoutTransform::getProperties() const -> SmartPtr<Properties>
    {
        try
        {
            auto properties = Component::getProperties();

            static const auto horizontalStr = String( "Horizontal" );
            static const auto verticalStr = String( "Vertical" );

            auto data = getComponentStateByType<UITransformState>();
            if( data )
            {
                auto position = data->getPosition();
                auto size = data->getSize();

                auto absolutePosition = data->getAbsolutePosition();
                auto absoluteSize = data->getAbsoluteSize();

                auto anchor = data->getAnchor();
                auto anchorMin = data->getAnchorMin();
                auto anchorMax = data->getAnchorMax();

                properties->setProperty( "Position", position );
                properties->setProperty( "Size", size );
                properties->setProperty( "AbsolutePosition", absolutePosition );
                properties->setProperty( "AbsoluteSize", absoluteSize );
                properties->setProperty( "Anchor", anchor );
                properties->setProperty( "AnchorMin", anchorMin );
                properties->setProperty( "AnchorMax", anchorMax );
            }

            properties->setProperty( horizontalStr, "" );
            properties->setProperty( verticalStr, "" );

            auto horizontalAlignment = getHorizontalAlignment();
            auto horizontalValueStr =
                UIComponentUtil::getHorizontalAlignmentString( horizontalAlignment );
            properties->setProperty( horizontalStr, horizontalValueStr );

            auto &horizontalPathProperty = properties->getPropertyObject( horizontalStr );
            horizontalPathProperty.setType( "enum" );

            auto enumValues = UIComponentUtil::getHorizontalAlignmentTypesString();
            horizontalPathProperty.setAttribute( "enum", enumValues );

            auto verticalAlignment = getVerticalAlignment();
            auto verticalValueStr = UIComponentUtil::getVerticalAlignmentString( verticalAlignment );
            properties->setProperty( verticalStr, verticalValueStr );

            auto &verticalPathProperty = properties->getPropertyObject( verticalStr );
            verticalPathProperty.setType( "enum" );

            auto verticalEnumValues = UIComponentUtil::getVerticalAlignmentTypesString();
            verticalPathProperty.setAttribute( "enum", verticalEnumValues );

            return properties;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void LayoutTransform::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            Component::setProperties( properties );

            auto data = getComponentStateByType<UITransformState>();

            static const auto horizontalStr = String( "Horizontal" );
            static const auto verticalStr = String( "Vertical" );

            auto position = Vector2<real_Num>::zero();
            auto size = Vector2<real_Num>( 1920.0f, 1080.0f );

            properties->getPropertyValue( "Position", position );
            properties->getPropertyValue( "Size", size );

            auto anchor = data->getAnchor();
            auto anchorMin = data->getAnchorMin();
            auto anchorMax = data->getAnchorMax();

            properties->getPropertyValue( "Anchor", anchor );
            properties->getPropertyValue( "AnchorMin", anchorMin );
            properties->getPropertyValue( "AnchorMax", anchorMax );

            String horizontalAlignmentStr;
            properties->getPropertyValue( horizontalStr, horizontalAlignmentStr );

            String verticalAlignmentStr;
            properties->getPropertyValue( verticalStr, verticalAlignmentStr );

            auto horizontalAlignment = UIComponentUtil::getHorizontalAlignment( horizontalAlignmentStr );
            auto verticalAlignment = UIComponentUtil::getVerticalAlignment( verticalAlignmentStr );

            if( getHorizontalAlignment() != horizontalAlignment )
            {
                setHorizontalAlignment( horizontalAlignment );

                switch( horizontalAlignment )
                {
                case HorizontalAlignment::LEFT:
                {
                    anchor.X() = 0.0f;
                }
                break;
                case HorizontalAlignment::CENTER:
                {
                    anchor.X() = 0.5f;
                }
                break;
                case HorizontalAlignment::RIGHT:
                {
                    anchor.X() = 1.0f;
                }
                break;
                }
            }

            if( getVerticalAlignment() != verticalAlignment )
            {
                setVerticalAlignment( verticalAlignment );

                switch( verticalAlignment )
                {
                case VerticalAlignment::TOP:
                {
                    anchor.Y() = 0.0f;
                }
                break;
                case VerticalAlignment::CENTER:
                {
                    anchor.Y() = 0.5f;
                }
                break;
                case VerticalAlignment::BOTTOM:
                {
                    anchor.Y() = 1.0f;
                }
                break;
                }
            }

            data->setAnchor( anchor );
            data->setAnchorMin( anchorMin );
            data->setAnchorMax( anchorMax );

            auto dirty = false;

            if( data )
            {
                if( !MathUtil<real_Num>::equals( data->getPosition(), position ) )
                {
                    data->setPosition( position );
                    dirty = true;
                }

                if( !MathUtil<real_Num>::equals( data->getSize(), size ) )
                {
                    data->setSize( size );
                    dirty = true;
                }
            }

            if( dirty )
            {
                if( auto actor = getActor() )
                {
                    if( auto transform = actor->getTransform() )
                    {
                        auto pos = getPosition();

                        auto localPosition = transform->getLocalPosition();
                        transform->setPosition(
                            Vector3<real_Num>( pos.X(), pos.Y(), localPosition.Z() ) );
                    }
                }
            }

            updateTransform();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void LayoutTransform::setHorizontalAlignment( HorizontalAlignment gha )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setHorizontalAlignment( static_cast<u8>( gha ) );
        }
    }

    auto LayoutTransform::getHorizontalAlignment() const -> LayoutTransform::HorizontalAlignment
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return static_cast<LayoutTransform::HorizontalAlignment>( data->getHorizontalAlignment() );
        }

        return HorizontalAlignment::CENTER;
    }

    void LayoutTransform::setVerticalAlignment( VerticalAlignment gva )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setVerticalAlignment( static_cast<u8>( gva ) );
        }
    }

    auto LayoutTransform::getVerticalAlignment() const -> LayoutTransform::VerticalAlignment
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return static_cast<LayoutTransform::VerticalAlignment>( data->getVerticalAlignment() );
        }

        return VerticalAlignment::CENTER;
    }

    auto LayoutTransform::getAbsolutePosition() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getAbsolutePosition();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setAbsolutePosition( const Vector2<real_Num> &absolutePosition )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setAbsolutePosition( absolutePosition );
        }
    }

    auto LayoutTransform::getAbsoluteSize() const -> Vector2<real_Num>
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            return data->getAbsoluteSize();
        }

        return Vector2<real_Num>::zero();
    }

    void LayoutTransform::setAbsoluteSize( const Vector2<real_Num> &absoluteSize )
    {
        if( auto data = getComponentStateByType<UITransformState>() )
        {
            data->setAbsoluteSize( absoluteSize );
        }
    }

    auto LayoutTransform::getLayout() const -> SmartPtr<UIComponent>
    {
        return m_layout;
    }

    void LayoutTransform::setLayout( SmartPtr<UIComponent> layout )
    {
        m_layout = layout;
    }

    auto LayoutTransform::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        Component::handleComponentEvent( state, eventType );

        switch( eventType )
        {
        case IFSM::Event::Change:
        {
        }
        break;
        case IFSM::Event::Enter:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Destroyed:
            {
            }
            break;
            case State::Edit:
            case State::Play:
            {
                if( auto actor = getActor() )
                {
                    if( auto uiComponent = actor->getComponent<UIComponent>() )
                    {
                        m_uiComponent = uiComponent;
                    }
                }

                setupCanvas();

                updateTransform();
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Leave:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Play:
            {
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Pending:
            break;
        case IFSM::Event::Complete:
            break;
        case IFSM::Event::NewState:
            break;
        case IFSM::Event::WaitForChange:
            break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void LayoutTransform::setupCanvas()
    {
        if( auto actor = getActor() )
        {
            if( auto rootActor = actor->getSceneRoot() )
            {
                auto layout = rootActor->getComponentAndInChildren<Layout>();
                setLayout( layout );
            }
        }
    }

}  // namespace fb::scene
