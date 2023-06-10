#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Systems/UI/CanvasTransformSystem.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/Scene/Components/UI/Text.h>
#include "FBCore/State/States/UITransformState.h"
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/Scene/ISceneManager.h>

namespace fb
{
    namespace scene
    {

        CanvasTransformSystem::CanvasTransformSystem()
        {
            auto typeInfo = UITransformState::typeInfo();
            setStateType( typeInfo );
        }

        CanvasTransformSystem::~CanvasTransformSystem()
        {
        }

        Vector2F CanvasTransformSystem::calculateElementPosition(
            const Vector2F &parentPosition, const Vector2F &parentSize, const Vector2F &position,
            const Vector2F &size, const Vector2F &anchor, const Vector2F &anchorMin,
            const Vector2F &anchorMax )
        {
            Vector2F elementPosition;

            // Calculate the absolute position of the element within the parent's coordinate system
            elementPosition.x = parentPosition.x + ( parentSize.x * anchorMin.x ) + position.x;
            elementPosition.y = parentPosition.y + ( parentSize.y * anchorMin.y ) + position.y;

            // Calculate the size of the element relative to the parent's size
            Vector2F relativeSize;
            relativeSize.x = size.x + ( parentSize.x * ( anchorMax.x - anchorMin.x ) );
            relativeSize.y = size.y + ( parentSize.y * ( anchorMax.y - anchorMin.y ) );

            // Adjust the element's position based on the anchor point
            elementPosition.x -= relativeSize.x * anchor.x;
            elementPosition.y -= relativeSize.y * anchor.y;

            return elementPosition;
        }

        void CanvasTransformSystem::calculateElementPositionAndSize(
            const Vector2F &parentPosition, const Vector2F &parentSize, const Vector2F &position,
            const Vector2F &size, const Vector2F &anchor, const Vector2F &anchorMin,
            const Vector2F &anchorMax, f32 &left, f32 &right, f32 &top, f32 &bottom )
        {
            // Calculate the absolute position of the element within the parent's coordinate system
            Vector2F elementPosition;
            elementPosition.x = parentPosition.x + ( parentSize.x * anchorMin.x ) + position.x;
            elementPosition.y = parentPosition.y + ( parentSize.y * anchorMin.y ) + position.y;

            // Calculate the size of the element relative to the parent's size
            Vector2F relativeSize;
            relativeSize.x = size.x + ( parentSize.x * ( anchorMax.x - anchorMin.x ) );
            relativeSize.y = size.y + ( parentSize.y * ( anchorMax.y - anchorMin.y ) );

            // Calculate the left, right, top, and bottom values
            left = elementPosition.x - ( relativeSize.x * anchor.x );
            right = elementPosition.x + ( ( 1.0f - anchor.x ) * relativeSize.x );
            top = elementPosition.y - ( relativeSize.y * anchor.y );
            bottom = elementPosition.y + ( ( 1.0f - anchor.y ) * relativeSize.y );
        }

        void CanvasTransformSystem::calculateElementPositionAndSize(
            const Vector2F &parentPosition, const Vector2F &parentSize, const Vector2F &position,
            const Vector2F &size, const Vector2F &anchor, const Vector2F &anchorMin,
            const Vector2F &anchorMax, Vector2F &elementPosition, Vector2F &elementSize )
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

        void CanvasTransformSystem::calculateElementPositionAndSize(
            const Vector2F &position, const Vector2F &size, const Vector2F &anchor,
            const Vector2F &anchorMin, const Vector2F &anchorMax, Vector2F &elementPosition,
            Vector2F &elementSize )
        {
            // Calculate the absolute position of the element
            elementPosition.x = position.x - ( anchor.x * size.x );
            elementPosition.y = position.y - ( anchor.y * size.y );

            // Calculate the size of the element
            elementSize.x = ( anchorMax.x - anchorMin.x ) * size.x;
            elementSize.y = ( anchorMax.y - anchorMin.y ) * size.y;
        }

        void CanvasTransformSystem::update()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();

            auto referenceSize = Vector2F( 1920, 1080 );

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

                                    auto parentAbsolutePos = Vector2F::zero();
                                    auto parentAbsoluteSize = referenceSize;

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
                                            parentAbsolutePos =
                                                parentCanvasTransform->getAbsolutePosition();
                                            parentAbsoluteSize =
                                                parentCanvasTransform->getAbsoluteSize();
                                        }
                                    }

                                    auto screenPos = Vector2F::zero();
                                    auto position = transform.getPosition();
                                    auto size = transform.getSize();

                                    auto horizontalAlignment = (LayoutTransform::HorizontalAlignment)
                                                                   transform.getHorizontalAlignment();

                                    auto verticalAlignment = (LayoutTransform::VerticalAlignment)
                                                                 transform.getVerticalAlignment();
                                    
                                    auto relativePos = Vector2F(
                                        screenPos.X() / static_cast<f32>( referenceSize.X() ),
                                        screenPos.Y() / static_cast<f32>( referenceSize.Y() ) );
                                    auto relativeSize =
                                        Vector2F( size.X() / static_cast<f32>( referenceSize.X() ),
                                                  size.Y() / static_cast<f32>( referenceSize.Y() ) );

                                    auto anchor = transform.getAnchor();
                                    auto anchorMin = transform.getAnchorMin();
                                    auto anchorMax = transform.getAnchorMax();

                                    auto elementPos = Vector2F::zero();
                                    auto elementSize = Vector2F::zero();

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
                                            parentAbsolutePos, parentAbsoluteSize, position, size,
                                            anchor, anchorMin, anchorMax, left, right, top, bottom );

                                        elementPos = Vector2F( left, top );
                                        elementSize = Vector2F( right, bottom ) - elementPos;
                                    }
                                    else
                                    {
                                        calculateElementPositionAndSize(
                                            Vector2F::zero(), Vector2F( 1920.0f, 1080.0f ), position, size,
                                            anchor, anchorMin, anchorMax, left, right, top, bottom );

                                        elementPos = Vector2F( left, top );
                                        elementSize = Vector2F( right, bottom ) - elementPos;
                                    }

                                    relativePos = Vector2F(
                                        elementPos.X() / static_cast<f32>( referenceSize.X() ),
                                        elementPos.Y() / static_cast<f32>( referenceSize.Y() ) );
                                    relativeSize = Vector2F(
                                        elementSize.X() / static_cast<f32>( referenceSize.X() ),
                                        elementSize.Y() / static_cast<f32>( referenceSize.Y() ) );

                                    

                                    auto absolutePosition =
                                        relativePos * Vector2F( referenceSize.X(), referenceSize.Y() );
                                    auto absoluteSize =
                                        relativeSize * Vector2F( referenceSize.X(), referenceSize.Y() );

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

    }  // namespace scene
}  // namespace fb
