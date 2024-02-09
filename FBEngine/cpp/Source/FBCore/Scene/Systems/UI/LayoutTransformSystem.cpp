#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Systems/UI/LayoutTransformSystem.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/State/States/UITransformState.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/Scene/ISceneManager.h>

namespace fb::scene
{

    LayoutTransformSystem::LayoutTransformSystem()
    {
        auto typeInfo = UITransformState::typeInfo();
        setStateType( typeInfo );
    }

    LayoutTransformSystem::~LayoutTransformSystem() = default;

    auto LayoutTransformSystem::calculateElementPosition(
        const Vector2<real_Num> &parentPosition, const Vector2<real_Num> &parentSize,
        const Vector2<real_Num> &position, const Vector2<real_Num> &size,
        const Vector2<real_Num> &anchor, const Vector2<real_Num> &anchorMin,
        const Vector2<real_Num> &anchorMax ) -> Vector2<real_Num>
    {
        Vector2<real_Num> elementPosition;

        // Calculate the absolute position of the element within the parent's coordinate system
        elementPosition.x = parentPosition.x + ( parentSize.x * anchorMin.x ) + position.x;
        elementPosition.y = parentPosition.y + ( parentSize.y * anchorMin.y ) + position.y;

        // Calculate the size of the element relative to the parent's size
        Vector2<real_Num> relativeSize;
        relativeSize.x = size.x + ( parentSize.x * ( anchorMax.x - anchorMin.x ) );
        relativeSize.y = size.y + ( parentSize.y * ( anchorMax.y - anchorMin.y ) );

        // Adjust the element's position based on the anchor point
        elementPosition.x -= relativeSize.x * anchor.x;
        elementPosition.y -= relativeSize.y * anchor.y;

        return elementPosition;
    }

    void LayoutTransformSystem::calculateElementPositionAndSize(
        const Vector2<real_Num> &parentPosition, const Vector2<real_Num> &parentSize,
        const Vector2<real_Num> &position, const Vector2<real_Num> &size,
        const Vector2<real_Num> &anchor, const Vector2<real_Num> &anchorMin,
        const Vector2<real_Num> &anchorMax, f32 &left, f32 &right, f32 &top, f32 &bottom )
    {
        // Calculate the absolute position of the element within the parent's coordinate system
        Vector2<real_Num> elementPosition;
        elementPosition.x = parentPosition.x + ( parentSize.x * anchorMin.x ) + position.x;
        elementPosition.y = parentPosition.y + ( parentSize.y * anchorMin.y ) + position.y;

        // Calculate the size of the element relative to the parent's size
        Vector2<real_Num> relativeSize;
        relativeSize.x = size.x + ( parentSize.x * ( anchorMax.x - anchorMin.x ) );
        relativeSize.y = size.y + ( parentSize.y * ( anchorMax.y - anchorMin.y ) );

        // Calculate the left, right, top, and bottom values
        left = elementPosition.x - ( relativeSize.x * anchor.x );
        right = elementPosition.x + ( ( 1.0f - anchor.x ) * relativeSize.x );
        top = elementPosition.y - ( relativeSize.y * anchor.y );
        bottom = elementPosition.y + ( ( 1.0f - anchor.y ) * relativeSize.y );
    }

    void LayoutTransformSystem::calculateElementPositionAndSize(
        const Vector2<real_Num> &parentPosition, const Vector2<real_Num> &parentSize,
        const Vector2<real_Num> &position, const Vector2<real_Num> &size,
        const Vector2<real_Num> &anchor, const Vector2<real_Num> &anchorMin,
        const Vector2<real_Num> &anchorMax, Vector2<real_Num> &elementPosition,
        Vector2<real_Num> &elementSize )
    {
        // Calculate the absolute position of the element within the parent's coordinate system
        elementPosition.x = parentPosition.x + ( parentSize.x * anchorMin.x ) + position.x;
        elementPosition.y = parentPosition.y + ( parentSize.y * anchorMin.y ) + position.y;

        // Calculate the size of the element relative to the parent's size
        elementSize.x = size.x + ( parentSize.x * ( anchorMax.x - anchorMin.x ) );
        elementSize.y = size.y + ( parentSize.y * ( anchorMax.y - anchorMin.y ) );

        // Adjust the element's position based on the anchor point
        elementPosition.x -= elementSize.x * anchor.x;
        elementPosition.y -= elementSize.y * anchor.y;
    }

    void LayoutTransformSystem::calculateElementPositionAndSize( const Vector2<real_Num> &position,
                                                                 const Vector2<real_Num> &size,
                                                                 const Vector2<real_Num> &anchor,
                                                                 const Vector2<real_Num> &anchorMin,
                                                                 const Vector2<real_Num> &anchorMax,
                                                                 Vector2<real_Num> &elementPosition,
                                                                 Vector2<real_Num> &elementSize )
    {
        // Calculate the absolute position of the element
        elementPosition.x = position.x - ( anchor.x * size.x );
        elementPosition.y = position.y - ( anchor.y * size.y );

        // Calculate the size of the element
        elementSize.x = ( anchorMax.x - anchorMin.x ) * size.x;
        elementSize.y = ( anchorMax.y - anchorMin.y ) * size.y;
    }

    void LayoutTransformSystem::update()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();

        auto typeInfo = LayoutTransform::typeInfo();

        for( const auto &component : m_components )
        {
            if( component )
            {
                auto &state = component->getComponentState();
                if( state )
                {
                    auto &transform = *(UITransformState *)state.get();
                    if( transform.isDirty() )
                    {
                        auto canvasTransform = (LayoutTransform *)transform.getOwner();
                        if( canvasTransform )
                        {
                            if( auto actor = canvasTransform->getActor() )
                            {
                                if( actor->getName() == "Canvas" )
                                {
                                    int stop = 0;
                                    stop = 0;
                                }
                                else if( actor->getName() == "StartMenu" )
                                {
                                    int stop = 0;
                                    stop = 0;
                                }
                                else if( actor->getName() == "Text" )
                                {
                                    int stop = 0;
                                    stop = 0;
                                }
                                else if( actor->getName() == "Panel" )
                                {
                                    int stop = 0;
                                    stop = 0;
                                }

                                auto parentAbsolutePos = Vector2<real_Num>::zero();
                                auto parentAbsoluteSize = Vector2<real_Num>( 1920.0f, 1080.0f );

                                SmartPtr<LayoutTransform> parentCanvasTransform;

                                if( auto parent = actor->getParent() )
                                {
                                    parentCanvasTransform = parent->getComponent<LayoutTransform>();
                                    while( !parentCanvasTransform && parent )
                                    {
                                        parent = parent->getParent();

                                        if( parent )
                                        {
                                            parentCanvasTransform =
                                                parent->getComponent<LayoutTransform>();
                                        }
                                    }

                                    if( parentCanvasTransform )
                                    {
                                        parentAbsolutePos = parentCanvasTransform->getAbsolutePosition();
                                        parentAbsoluteSize = parentCanvasTransform->getAbsoluteSize();
                                    }
                                }

                                auto screenPos = Vector2<real_Num>::zero();
                                auto position = transform.getPosition();
                                auto size = transform.getSize();

                                auto horizontalAlignment =
                                    static_cast<LayoutTransform::HorizontalAlignment>(
                                        transform.getHorizontalAlignment() );

                                auto verticalAlignment = static_cast<LayoutTransform::VerticalAlignment>(
                                    transform.getVerticalAlignment() );

                                auto relativePos = Vector2<real_Num>::zero();
                                auto relativeSize = Vector2<real_Num>::zero();

                                if( !( MathF::equals( parentAbsoluteSize.X(), 0.0f ) &&
                                       MathF::equals( parentAbsoluteSize.Y(), 0.0f ) ) )
                                {
                                    relativePos = Vector2<real_Num>(
                                        screenPos.X() / static_cast<f32>( parentAbsoluteSize.X() ),
                                        screenPos.Y() / static_cast<f32>( parentAbsoluteSize.Y() ) );

                                    relativeSize = Vector2<real_Num>(
                                        size.X() / static_cast<f32>( parentAbsoluteSize.X() ),
                                        size.Y() / static_cast<f32>( parentAbsoluteSize.Y() ) );
                                }

                                auto anchor = transform.getAnchor();
                                auto anchorMin = transform.getAnchorMin();
                                auto anchorMax = transform.getAnchorMax();

                                auto elementPos = Vector2<real_Num>::zero();
                                auto elementSize = Vector2<real_Num>::zero();

                                f32 left = 0.0f;
                                f32 right = 0.0f;
                                f32 top = 0.0f;
                                f32 bottom = 0.0f;

                                if( parentCanvasTransform )
                                {
                                    //calculateElementPositionAndSize(
                                    //    parentAbsolutePos, parentAbsoluteSize, position, size,
                                    //    anchor, anchorMin, anchorMax, elementPos, elementSize );

                                    calculateElementPositionAndSize(
                                        parentAbsolutePos, parentAbsoluteSize, position, size, anchor,
                                        anchorMin, anchorMax, left, right, top, bottom );

                                    elementPos = Vector2<real_Num>( left, top );
                                    elementSize = Vector2<real_Num>( right, bottom ) - elementPos;
                                }
                                else
                                {
                                    calculateElementPositionAndSize(
                                        Vector2<real_Num>::zero(), Vector2<real_Num>( 1920.0f, 1080.0f ),
                                        position, size, anchor, anchorMin, anchorMax, left, right, top,
                                        bottom );

                                    elementPos = Vector2<real_Num>( left, top );
                                    elementSize = Vector2<real_Num>( right, bottom ) - elementPos;
                                }

                                if( elementSize.x < 0.0f )
                                {
                                    elementSize.x = MathF::Abs( elementSize.x );
                                }

                                if( elementSize.y < 0.0f )
                                {
                                    elementSize.y = MathF::Abs( elementSize.y );
                                }

                                auto canvas =
                                    fb::static_pointer_cast<Layout>( canvasTransform->getLayout() );
                                if( canvas )
                                {
                                    auto canvasSize = canvas->getReferenceSize();
                                    auto fCanvasSize =
                                        Vector2<real_Num>( static_cast<f32>( canvasSize.X() ),
                                                           static_cast<f32>( canvasSize.Y() ) );

                                    if( !( MathF::equals( static_cast<f32>( canvasSize.X() ), 0.0f ) &&
                                           MathF::equals( static_cast<f32>( canvasSize.Y() ), 0.0f ) ) )
                                    {
                                        relativePos = Vector2<real_Num>(
                                            elementPos.X() / static_cast<f32>( canvasSize.X() ),
                                            elementPos.Y() / static_cast<f32>( canvasSize.Y() ) );
                                        relativeSize = Vector2<real_Num>(
                                            elementSize.X() / static_cast<f32>( canvasSize.X() ),
                                            elementSize.Y() / static_cast<f32>( canvasSize.Y() ) );
                                    }

                                    auto absolutePosition = relativePos * fCanvasSize;
                                    auto absoluteSize = relativeSize * fCanvasSize;

                                    transform.setAbsolutePosition( absolutePosition );
                                    transform.setAbsoluteSize( absoluteSize );

                                    FB_ASSERT( relativePos.isValid() );
                                    FB_ASSERT( relativeSize.isValid() );

                                    auto uiComponents = actor->getComponentsByType<UIComponent>();
                                    for( auto uiComponent : uiComponents )
                                    {
                                        if( uiComponent )
                                        {
                                            if( auto element = uiComponent->getElement() )
                                            {
                                                element->setPosition( relativePos );
                                                element->setSize( relativeSize );
                                            }
                                        }
                                    }
                                }

                                auto components = actor->getComponentsInChildren<LayoutTransform>();
                                for( auto &component : components )
                                {
                                    component->updateTransform();
                                }
                            }
                        }
                    }

                    transform.setDirty( false );
                }
            }
        }
    }

}  // namespace fb::scene
