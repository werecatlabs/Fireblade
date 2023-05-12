#ifndef _IBillboard_H
#define _IBillboard_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Base/ColourF.h>

namespace fb
{
    namespace render
    {

        /** An interface for a billboard.
         */
        class IBillboard : public ISharedObject
        {
        public:
            ~IBillboard() override = default;

            /** Sets the position of the billboard. */
            virtual void setPosition( const Vector3F &position ) = 0;

            /** Gets the position of the billboard. */
            virtual Vector3F getPosition() const = 0;

            /** Sets the orientation of the billboard. */
            virtual void setOrientation( const QuaternionF &orientation ) = 0;

            /** Gets the orientation of the billboard. */
            virtual QuaternionF getOrientation() const = 0;

            /** Sets the dimensions of the billboard. */
            virtual void setScale( const Vector3F &dimensions ) = 0;

            /** Gets the dimensions of the billboard. */
            virtual Vector3F getScale() const = 0;

            /** Gets the transformation cached for the render system. */
            virtual void *_getRenderSystemTransform() const = 0;

            /** Sets the colour of this billboard.
             */
            virtual void setColour( const ColourF &colour ) = 0;

            /** Gets the colour of this billboard.
             */
            virtual ColourF getColour() const = 0;

            virtual void *getRenderData() const = 0;
            virtual void setRenderData( void *renderData ) = 0;

            /** Gets a pointer to the underlying object. This is dependent on the graphics library used.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
