#ifndef _IOverlayElement_H
#define _IOverlayElement_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for an overlay element.
         */
        class IOverlayElement : public ISharedObject
        {
        public:
            /**
             * @brief Enumeration of metrics modes that can be used for positioning and sizing.
             */
            enum GuiMetricsMode
            {
                /// 'left', 'top', 'height' and 'width' are parametrics from 0.0 to 1.0
                GMM_RELATIVE,
                /// Positions & sizes are in absolute pixels
                GMM_PIXELS,
                /// Positions & sizes are in virtual pixels
                GMM_RELATIVE_ASPECT_ADJUSTED
            };

            /**
             * @brief Enum describing where '0' is in relation to the parent in the horizontal dimension.
             * Affects how 'left' is interpreted.
             */
            enum GuiHorizontalAlignment
            {
                GHA_LEFT,
                GHA_CENTER,
                GHA_RIGHT
            };

            /**
             * @brief Enum describing where '0' is in relation to the parent in the vertical dimension.
             * Affects how 'top' is interpreted.
             */
            enum GuiVerticalAlignment
            {
                GVA_TOP,
                GVA_CENTER,
                GVA_BOTTOM
            };

            /**
             * @brief Enum for horizontal alignment.
             */
            enum class HorizontalAlignment
            {
                LEFT,
                CENTER,
                RIGHT,
                COUNT
            };

            /**
             * @brief Enum for vertical alignment.
             */
            enum class VerticalAlignment
            {
                TOP,
                CENTER,
                BOTTOM,
                COUNT
            };

            /**
             * @brief Hash for the left property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_LEFT;

            /**
             * @brief Hash for the top property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_TOP;

            /**
             * @brief Hash for the width property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_WIDTH;

            /**
             * @brief Hash for the height property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_HEIGHT;

            /**
             * @brief Hash for the metrics mode property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_METRICSMODE;

            /**
             * @brief Hash for the horizontal alignment property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_ALIGN_HORIZONTAL;

            /**
             * @brief Hash for the vertical alignment property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_ALIGN_VERTICAL;

            /**
             * @brief Hash for the text property of an overlay element.
             */
            static const hash_type STATE_MESSAGE_TEXT;

            /**
             * @brief Hash for adding a child overlay element to a parent.
             */
            static const hash_type STATE_MESSAGE_ADDCHILD;

            /**
             * @brief Hash for removing a child overlay element from a parent.
             */
            static const hash_type STATE_MESSAGE_REMOVECHILD;

            /**
             * @brief Hash for attaching an object to an overlay element.
             */
            static const hash_type STATE_MESSAGE_ATTACH_OBJECT;

            /**
             * @brief Hash for detaching an object from an overlay element.
             */
            static const hash_type STATE_MESSAGE_DETACH_OBJECT;

            /**
             * @brief Hash for detaching all objects from an overlay element.
             */
            static const hash_type STATE_MESSAGE_DETACH_ALL_OBJECTS;

            /**
             * @brief Virtual destructor.
             */
            ~IOverlayElement() override = default;

            /**
             * @brief Sets the overlay element name.
             * @param name The name to set.
             */
            virtual void setName( const String &name ) = 0;

            /**
             * Gets the name of the overlay element.
             *
             * @return The name of the overlay element.
             */
            virtual String getName() const = 0;

            /**
             * Sets the material for this overlay element.
             *
             * @param material The material to set.
             */
            virtual void setMaterial( SmartPtr<IMaterial> material ) = 0;

            /**
             * Gets the material of this overlay element.
             *
             * @return The material of this overlay element.
             */
            virtual SmartPtr<IMaterial> getMaterial() const = 0;

            /**
             * Sets the caption or text of this overlay element.
             *
             * @param text The caption to set.
             */
            virtual void setCaption( const String &text ) = 0;

            /**
             * Gets the caption or text of this overlay element.
             *
             * @return The caption or text of this overlay element.
             */
            virtual String getCaption() const = 0;

            /**
             * Sets the visibility of this overlay element.
             *
             * @param visible Whether the overlay element should be visible or not.
             */
            virtual void setVisible( bool visible ) = 0;

            /**
             * Gets the visibility of this overlay element.
             *
             * @return True if the overlay element is visible, false otherwise.
             */
            virtual bool isVisible() const = 0;

            /**
             * Gets the position of this overlay element.
             *
             * @return The position of this overlay element.
             */
            virtual Vector2F getPosition() const = 0;

            /**
             * Sets the position of this overlay element.
             *
             * @param position The position to set.
             */
            virtual void setPosition( const Vector2F &position ) = 0;

            /**
             * Gets the size of this overlay element.
             *
             * @return The size of this overlay element.
             */
            virtual Vector2F getSize() const = 0;

            /**
             * Sets the size of this overlay element.
             *
             * @param size The size to set.
             */
            virtual void setSize( const Vector2F &size ) = 0;

            /**
             * Gets the Z-order of this overlay element relative to its siblings.
             * @return The Z-order of the element.
             */
            virtual u32 getZOrder() const = 0;

            /**
             * Sets the Z-order of this overlay element relative to its siblings.
             * @param zOrder The new Z-order of the element.
             */
            virtual void setZOrder( u32 zOrder ) = 0;

            /**
             * Sets the color of this overlay element.
             * @param colour The new color to set.
             */
            virtual void setColour( const ColourF &colour ) = 0;

            /**
             * Gets the color of this overlay element.
             * @return The color of the element.
             */
            virtual ColourF getColour() const = 0;

            /**
             * Sets the metrics mode for this overlay element.
             * @param metricsMode The new metrics mode to set.
             */
            virtual void setMetricsMode( u8 metricsMode ) = 0;

            /**
             * Gets the metrics mode for this overlay element.
             * @return The metrics mode of the element.
             */
            virtual u8 getMetricsMode() const = 0;

            /**
             * Sets the horizontal alignment for this overlay element.
             * @param gha The new horizontal alignment to set.
             */
            virtual void setHorizontalAlignment( u8 gha ) = 0;

            /**
             * Gets the horizontal alignment for this overlay element.
             * @return The horizontal alignment of the element.
             */
            virtual u8 getHorizontalAlignment() const = 0;

            /**
             * Sets the vertical alignment for this overlay element.
             * @param gva The new vertical alignment to set.
             */
            virtual void setVerticalAlignment( u8 gva ) = 0;

            /**
             * Gets the vertical alignment for this overlay element.
             * @return The vertical alignment of the element.
             */
            virtual u8 getVerticalAlignment() const = 0;

            /**
             * Checks whether this overlay element is a container.
             * @return True if this element is a container, false otherwise.
             */
            virtual bool isContainer() const = 0;

            /**
             * Gets the overlay associated with this element.
             *
             * @return SmartPtr<IOverlay> Smart pointer to the overlay.
             */
            virtual SmartPtr<IOverlay> getOverlay() const = 0;

            /**
             * Sets the overlay associated with this element.
             *
             * @param overlay SmartPtr<IOverlay> Smart pointer to the overlay.
             */
            virtual void setOverlay( SmartPtr<IOverlay> overlay ) = 0;

            /**
             * Gets the parent element of this element.
             *
             * @return SmartPtr<IOverlayElement> Smart pointer to the parent element.
             */
            virtual SmartPtr<IOverlayElement> getParent() const = 0;

            /**
             * Sets the parent element of this element.
             *
             * @param element SmartPtr<IOverlayElement> Smart pointer to the parent element.
             */
            virtual void setParent( SmartPtr<IOverlayElement> element ) = 0;

            /**
             * Adds a child element to this element.
             *
             * @param element SmartPtr<IOverlayElement> Smart pointer to the child element.
             */
            virtual void addChild( SmartPtr<IOverlayElement> element ) = 0;

            /**
             * Removes a child element from this element.
             *
             * @param element SmartPtr<IOverlayElement> Smart pointer to the child element.
             */
            virtual void removeChild( SmartPtr<IOverlayElement> element ) = 0;

            /**
             * Gets the children elements of this element.
             *
             * @return Array<SmartPtr<IOverlayElement>> An array of smart pointers to the children elements.
             */
            virtual Array<SmartPtr<IOverlayElement>> getChildren() const = 0;

            /**
             * Gets the properties associated with this element.
             *
             * @return SmartPtr<Properties> Smart pointer to the properties.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /**
             * Sets the properties associated with this element.
             *
             * @param properties SmartPtr<Properties> Smart pointer to the properties.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            /**
             * Gets a pointer to the underlying object. This is dependent on the graphics library used.
             *
             * @param ppObject void** Pointer to the object.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
