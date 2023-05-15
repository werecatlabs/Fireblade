#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/CanvasTransform.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/Scene/Components/UI/UIComponentUtil.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/CSceneManager.h>
#include <FBCore/Scene/Components/UI/Text.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Math/Core/MathUtil.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, CanvasTransform, IComponent );

        CanvasTransform::CanvasTransform()
        {
        }

        CanvasTransform::~CanvasTransform()
        {
        }

        void CanvasTransform::updateComponents()
        {
            /*
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );

            auto registry = sceneManager->getRegistry();

            auto view = registry->view<canvas_transform>();

            view.each( []( const auto entity, auto &t ) {
                auto &transform = static_cast<canvas_transform &>( t );
                if( transform.dirty )
                {
                    auto referenceSize = Vector2I( 1920, 1080 );

                    auto canvasTransform = transform.owner;
                    if( auto actor = canvasTransform->getActor() )
                    {
                        //if( auto actorTransform = actor->getTransform() )
                        //{
                        //    auto pos = actorTransform->getPosition();
                        //    transform.position = Vector2F( pos.X(), pos.Y() );
                        //}

                        auto parentAbsolutePos = Vector2F::zero();
                        auto parentAbsoluteSize = Vector2F::zero();

                        SmartPtr<CanvasTransform> parentCanvasTransform;

                        if( auto parent = actor->getParent() )
                        {
                            parentCanvasTransform = parent->getComponent<CanvasTransform>();
                            while( !parentCanvasTransform && parent )
                            {
                                parent = parent->getParent();

                                if( parent )
                                {
                                    parentCanvasTransform = parent->getComponent<CanvasTransform>();
                                }
                            }

                            if( parentCanvasTransform )
                            {
                                parentAbsolutePos = parentCanvasTransform->getAbsolutePosition();
                                parentAbsoluteSize = parentCanvasTransform->getAbsoluteSize();
                            }
                        }

                        auto uiComponents = actor->getComponentsByType<UIComponent>();
                        for( auto uiComponent : uiComponents )
                        {
                            if( !parentCanvasTransform )
                            {
                                auto canvas =
                                    fb::static_pointer_cast<Layout>( uiComponent->getCanvas() );
                                auto referenceSize = Vector2I( 1920, 1080 );
                                if( canvas )
                                {
                                    referenceSize = canvas->getReferenceSize();

                                    parentAbsoluteSize =
                                        Vector2F( referenceSize.X(), referenceSize.Y() );
                                }
                            }

                            auto useSize = uiComponent->getTypeInfo() != Text::typeInfo();
                            auto screenPos = Vector2F::zero();
                            auto &position = transform.position;
                            auto &size = transform.size;

                            auto &horizontalAlignment = transform.horizontalAlignment;

                            switch( horizontalAlignment )
                            {
                            case HorizontalAlignment::LEFT:
                            {
                                screenPos.X() = ( parentAbsoluteSize.X() * 0.0f ) + position.X();

                                if( useSize )
                                    screenPos.X() -= size.X() * 0.0f;
                            }
                            break;
                            case HorizontalAlignment::CENTER:
                            {
                                screenPos.X() = ( parentAbsoluteSize.X() * 0.5f ) + position.X();

                                if( useSize )
                                    screenPos.X() -= size.X() * 0.5f;
                            }
                            break;
                            case HorizontalAlignment::RIGHT:
                            {
                                screenPos.X() = ( parentAbsoluteSize.X() * 1.0f ) - position.X();

                                if( useSize )
                                    screenPos.X() -= size.X() * 1.0f;
                            }
                            break;
                            default:
                            {
                            }
                            }

                            auto &verticalAlignment = transform.verticalAlignment;
                            switch( verticalAlignment )
                            {
                            case VerticalAlignment::TOP:
                            {
                                screenPos.Y() = ( parentAbsoluteSize.Y() * 0.0f ) + position.Y();

                                if( useSize )
                                    screenPos.Y() -= size.Y() * 0.0f;
                            }
                            break;
                            case VerticalAlignment::CENTER:
                            {
                                screenPos.Y() = ( parentAbsoluteSize.Y() * 0.5f ) + position.Y();

                                if( useSize )
                                    screenPos.Y() -= size.Y() * 0.5f;
                            }
                            break;
                            case VerticalAlignment::BOTTOM:
                            {
                                screenPos.Y() = ( parentAbsoluteSize.Y() * 1.0f ) - position.Y();

                                if( useSize )
                                    screenPos.Y() -= size.Y() * 1.0f;
                            }
                            break;
                            default:
                            {
                            }
                            }

                            auto relativePos =
                                Vector2F( screenPos.X() / static_cast<f32>( referenceSize.X() ),
                                          screenPos.Y() / static_cast<f32>( referenceSize.Y() ) );
                            auto relativeSize =
                                Vector2F( size.X() / static_cast<f32>( referenceSize.X() ),
                                          size.Y() / static_cast<f32>( referenceSize.Y() ) );

                            if( uiComponent )
                            {
                                if( auto element = uiComponent->getElement() )
                                {
                                    auto parentRelativePos =
                                        parentAbsolutePos /
                                        Vector2F( referenceSize.X(), referenceSize.Y() );
                                    auto relativePosition = parentRelativePos + relativePos;

                                    transform.absolutePosition =
                                        relativePosition *
                                        Vector2F( referenceSize.X(), referenceSize.Y() );
                                    transform.absoluteSize = size;

                                    element->setPosition( relativePosition );
                                    element->setSize( relativeSize );
                                }
                            }
                        }

                        auto components = actor->getComponentsInChildren<CanvasTransform>();
                        for( auto &component : components )
                        {
                            component->updateTransform();
                        }
                    }

                    transform.dirty = false;  // commented out as a workaround
                }
            } );
            */
        }

        void CanvasTransform::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );

            /*
            if( auto actor = getActor() )
            {
                auto entity = (entt::registry::entity_type)actor->getEntity();

                auto registry = sceneManager->getRegistry();
                auto &component = registry->emplace<canvas_transform>( entity );
                component.owner = this;

                setDataPtr( &component );
            }
            */

            setLoadingState( LoadingState::Loaded );
        }

        void CanvasTransform::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );

            /*
            if( auto actor = getActor() )
            {
                auto entity = (entt::registry::entity_type)actor->getEntity();

                auto registry = sceneManager->getRegistry();
                registry->remove<canvas_transform>( entity );
            }
            */

            setLoadingState( LoadingState::Unloaded );
        }

        void CanvasTransform::updateTransform()
        {
            //if( auto actor = getActor() )
            //{
            //    if( auto actorTransform = actor->getTransform() )
            //    {
            //        auto pos = actorTransform->getPosition();

            //        auto data = getDataPtrByType<canvas_transform>();
            //        data->position = Vector2F( pos.X(), pos.Y() );
            //    }
            //}

            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->dirty = true;
            }

            if( auto actor = getActor() )
            {
                auto components = actor->getComponentsInChildren<CanvasTransform>();
                for( auto component : components )
                {
                    component->updateTransform();
                }
            }

            /*
            if(auto actor = getActor())
            {
                if(auto transform = actor->getTransform())
                {
                    auto pos = transform->getPosition();
                    setPosition( Vector2F( pos.X(), pos.Y() ) );
                }

                auto uiComponents = actor->getComponentsByType<UIComponent>();
                for(auto uiComponent : uiComponents)
                {
                    auto canvas = fb::static_pointer_cast<CanvasComponent>( uiComponent->getCanvas() );
                    auto referenceSize = Vector2I( 1920, 1080 );
                    if(canvas)
                    {
                        referenceSize = canvas->getReferenceSize();
                    }

                    auto screenPos = Vector2F::zero();
                    auto position = getPosition();
                    auto size = getSize();

                    auto horizontalAlignment = getHorizontalAlignment();
                    auto verticalAlignment = getVerticalAlignment();

                    switch(horizontalAlignment)
                    {
                    case HorizontalAlignment::LEFT:
                    {
                        screenPos.X() = ( referenceSize.X() * 0.0f ) + position.X();
                        screenPos.X() -= size.X() * 0.0f;
                    }
                    break;
                    case HorizontalAlignment::CENTER:
                    {
                        screenPos.X() = ( referenceSize.X() * 0.5f ) + position.X();
                        screenPos.X() -= size.X() * 0.5f;
                    }
                    break;
                    case HorizontalAlignment::RIGHT:
                    {
                        screenPos.X() = ( referenceSize.X() * 1.0f ) - position.X();
                        screenPos.X() -= size.X() * 1.0f;
                    }
                    break;
                    default:
                    {
                    }
                    }

                    switch(verticalAlignment)
                    {
                    case VerticalAlignment::TOP:
                    {
                        screenPos.Y() = ( referenceSize.Y() * 0.0f ) + position.Y();
                        screenPos.Y() -= size.Y() * 0.0f;
                    }
                    break;
                    case VerticalAlignment::CENTER:
                    {
                        screenPos.Y() = ( referenceSize.Y() * 0.5f ) + position.Y();
                        screenPos.Y() -= size.Y() * 0.5f;
                    }
                    break;
                    case VerticalAlignment::BOTTOM:
                    {
                        screenPos.Y() = ( referenceSize.Y() * 1.0f ) - position.Y();
                        screenPos.Y() -= size.Y() * 1.0f;
                    }
                    break;
                    default:
                    {
                    }
                    }

                    auto parentRelativePos = Vector2F::zero();
                    auto parentRelativeSize = Vector2F::zero();

                    if(auto parent = actor->getParent())
                    {
                        if(auto parentCanvasTransform = parent->getComponent<CanvasTransform>())
                        {
                            parentRelativePos = parentCanvasTransform->getAbsolutePosition();
                        }
                    }

                    auto relativePos = Vector2F( screenPos.X() / static_cast<f32>(referenceSize.X()),
                                                 screenPos.Y() / static_cast<f32>(referenceSize.Y()) );
                    auto relativeSize = Vector2F( size.X() / static_cast<f32>(referenceSize.X()),
                                                  size.Y() / static_cast<f32>(referenceSize.Y()) );

                    if(uiComponent)
                    {
                        if(auto element = uiComponent->getElement())
                        {
                            auto absolutePosition = parentRelativePos + relativePos;
                            setAbsolutePosition( absolutePosition );

                            element->setPosition( absolutePosition );
                            element->setSize( relativeSize );
                        }
                    }
                }

                auto components = actor->getComponentsInChildren<CanvasTransform>();
                for(auto &component : components)
                {
                    component->updateTransform();
                }
            }
            */
        }

        void CanvasTransform::reset()
        {
        }

        void CanvasTransform::awake()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                // auto canvasTransformManager =
                //     applicationManager->getComponentManager<CanvasTransformManager>();
                // if( canvasTransformManager )
                //{
                //     canvasTransformManager->addCanvasTransform( this );
                // }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CanvasTransform::destroy()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                // auto canvasTransformManager =
                //     applicationManager->getComponentManager<CanvasTransformManager>();
                // if( canvasTransformManager )
                //{
                //     canvasTransformManager->removeCanvasTransform( this );
                // }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CanvasTransform::play()
        {
        }

        void CanvasTransform::edit()
        {
        }

        Vector2F CanvasTransform::getPosition() const
        {
            auto data = getDataPtrByType<canvas_transform>();
            return data->position;
        }

        void CanvasTransform::setPosition( const Vector2F &position )
        {
            auto data = getDataPtrByType<canvas_transform>();
            data->position = position;
            data->absolutePosition = position;
        }

        Vector2F CanvasTransform::getSize() const
        {
            auto data = getDataPtrByType<canvas_transform>();
            return data->size;
        }

        void CanvasTransform::setSize( const Vector2F &size )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->size = size;
                data->absoluteSize = size;
            }
        }

        Vector2F CanvasTransform::getAnchor() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->anchor;
            }

            return Vector2F::zero();
        }

        void CanvasTransform::setAnchor( const Vector2F &anchor )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->anchor = anchor;
            }
        }

        Vector2F CanvasTransform::getPivot() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->pivot;
            }

            return Vector2F::zero();
        }

        void CanvasTransform::setPivot( const Vector2F &pivot )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->pivot = pivot;
            }
        }

        SmartPtr<Properties> CanvasTransform::getProperties() const
        {
            try
            {
                auto properties = Component::getProperties();

                static const auto horizontalStr = String( "Horizontal" );
                static const auto verticalStr = String( "Vertical" );

                auto data = getDataPtrByType<canvas_transform>();
                if( data )
                {
                    properties->setProperty( "Position", data->position );
                    properties->setProperty( "Size", data->size );
                    properties->setProperty( "Anchor", data->anchor );
                    properties->setProperty( "Pivot", data->pivot );
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

        void CanvasTransform::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                Component::setProperties( properties );

                auto data = getDataPtrByType<canvas_transform>();

                static const auto horizontalStr = String( "Horizontal" );
                static const auto verticalStr = String( "Vertical" );

                Vector2F position;
                Vector2F size;

                properties->getPropertyValue( "Position", position );
                properties->getPropertyValue( "Size", size );

                if( data )
                {
                    properties->getPropertyValue( "Anchor", data->anchor );
                    properties->getPropertyValue( "Pivot", data->pivot );
                }

                String horizontalAlignmentStr;
                properties->getPropertyValue( horizontalStr, horizontalAlignmentStr );

                String verticalAlignmentStr;
                properties->getPropertyValue( verticalStr, verticalAlignmentStr );

                auto horizontalAlignment =
                    UIComponentUtil::getHorizontalAlignment( horizontalAlignmentStr );
                auto verticalAlignment = UIComponentUtil::getVerticalAlignment( verticalAlignmentStr );

                setHorizontalAlignment( horizontalAlignment );
                setVerticalAlignment( verticalAlignment );

                auto dirty = false;

                if( data )
                {
                    if( !MathUtil<f32>::equals( data->position, position ) )
                    {
                        data->position = position;
                        dirty = true;
                    }

                    if( !MathUtil<f32>::equals( data->size, size ) )
                    {
                        data->size = size;
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

        void CanvasTransform::setHorizontalAlignment( HorizontalAlignment gha )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->horizontalAlignment = gha;
            }
        }

        CanvasTransform::HorizontalAlignment CanvasTransform::getHorizontalAlignment() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->horizontalAlignment;
            }

            return CanvasTransform::HorizontalAlignment::CENTER;
        }

        void CanvasTransform::setVerticalAlignment( VerticalAlignment gva )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->verticalAlignment = gva;
            }
        }

        CanvasTransform::VerticalAlignment CanvasTransform::getVerticalAlignment() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->verticalAlignment;
            }

            return CanvasTransform::VerticalAlignment::CENTER;
        }

        Vector2F CanvasTransform::getAbsolutePosition() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->absolutePosition;
            }

            return Vector2F::zero();
        }

        void CanvasTransform::setAbsolutePosition( const Vector2F &absolutePosition )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->absolutePosition = absolutePosition;
            }
        }

        Vector2F CanvasTransform::getAbsoluteSize() const
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                return data->absoluteSize;
            }

            return Vector2F::zero();
        }

        void CanvasTransform::setAbsoluteSize( const Vector2F &absoluteSize )
        {
            if( auto data = getDataPtrByType<canvas_transform>() )
            {
                data->absoluteSize = absoluteSize;
            }
        }

        IFSM::ReturnType CanvasTransform::handleComponentEvent( u32 state, IFSM::Event eventType )
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
    }  // namespace scene
}  // namespace fb
