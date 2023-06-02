#ifndef IDecalCursor_h__
#define IDecalCursor_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for a decal cursor used for in-world decal placement.
         */
        class IDecalCursor : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IDecalCursor() override = default;

            /** Returns whether the cursor is visible. */
            virtual bool isVisible() const = 0;

            /**
             * Sets the visibility of the cursor.
             * @param visible Whether the cursor should be visible.
             */
            virtual void setVisible( bool visible ) = 0;

            /** Returns the current position of the cursor. */
            virtual Vector3F getPosition() const = 0;

            /**
             * Sets the position of the cursor.
             * @param position The new position of the cursor.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            /** Returns the current size of the cursor. */
            virtual Vector2F getSize() const = 0;

            /**
             * Sets the size of the cursor.
             * @param size The new size of the cursor.
             */
            virtual void setSize( const Vector2F &size ) = 0;

            /**
             * Adds a debug entity to the cursor for visualization purposes.
             * @param entityName The name of the debug entity.
             * @param scale The scale of the debug entity (default: Vector3F::unit()).
             */
            virtual void addDebugEntity( const String &entityName,
                                         const Vector3F &scale = Vector3F::unit() ) = 0;

            /** Removes the debug entity from the cursor. */
            virtual void removeDebugEntity() = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IDecalCursor_h__
