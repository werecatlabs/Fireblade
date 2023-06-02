#ifndef _IOverlay_H
#define _IOverlay_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for an overlay.
         */
        class IOverlay : public ISharedObject
        {
        public:
            static const hash_type STATE_MESSAGE_ATTACH_OBJECT;
            static const hash_type STATE_MESSAGE_DETACH_OBJECT;
            static const hash_type STATE_MESSAGE_DETACH_ALL_OBJECTS;

            /** Virtual destructor. */
            ~IOverlay() override = default;

            /** Gets the name of the overlay.
             * @return A string containing the name of the overlay.
             */
            virtual String getName() const = 0;

            /** Sets the name of the overlay.
             * @param name A string containing the new name of the overlay.
             */
            virtual void setName( const String &name ) = 0;

            /** Adds an overlay element to the overlay.
             * @param element The overlay element to add.
             */
            virtual void addElement( SmartPtr<IOverlayElement> element ) = 0;

            /** Removes an overlay element from the overlay.
             * @param element The overlay element to remove.
             * @return True if the element was removed, false otherwise.
             */
            virtual bool removeElement( SmartPtr<IOverlayElement> element ) = 0;

            /** Gets an array of all the overlay elements in the overlay.
             * @return An array of overlay elements.
             */
            virtual Array<SmartPtr<IOverlayElement>> getElements() const = 0;

            /** Adds a scene node to the overlay.
             * @param sceneNode The scene node to add.
             */
            virtual void addSceneNode( SmartPtr<ISceneNode> sceneNode ) = 0;

            /** Removes a scene node from the overlay.
             * @param sceneNode The scene node to remove.
             * @return True if the scene node was removed, false otherwise.
             */
            virtual bool removeSceneNode( SmartPtr<ISceneNode> sceneNode ) = 0;

            /** Sets the visibility of the overlay.
             * @param visible Whether the overlay should be visible or not.
             */
            virtual void setVisible( bool visible ) = 0;

            /** Gets the visibility of the overlay.
             * @return True if the overlay is visible, false otherwise.
             */
            virtual bool isVisible() const = 0;

            /** Sets the Z-order of the overlay.
             * @param zorder The new Z-order value.
             */
            virtual void setZOrder( u32 zorder ) = 0;

            /** Gets the Z-order of the overlay.
             * @return The Z-order value.
             */
            virtual u32 getZOrder() const = 0;

            /** Updates the z orders.
             */
            virtual void updateZOrder() = 0;

            /** Gets the absolute resolution of the overlay.
             * @return The absolute resolution as a vector of integers.
             */
            virtual Vector2I getAbsoluteResolution() const = 0;

            /** Sets the absolute resolution of the overlay.
             * @param val The new absolute resolution as a vector of integers.
             */
            virtual void setAbsoluteResolution( const Vector2I &val ) = 0;

            /** Gets a pointer to the underlying object. This is dependent on the graphics library used.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Gets the data associated with the overlay as a properties object.
             * @return A properties object containing the overlay data.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data associated with the overlay as a properties object.
             * @param properties A properties object containing the overlay data to set.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
