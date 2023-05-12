#ifndef IDebug_h__
#define IDebug_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for debug rendering.
         */
        class IDebug : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IDebug() override = default;

            /**
             * Clears the debug output.
             */
            virtual void clear() = 0;

            /**
             * Draws a debug point.
             * @param id The unique identifier for the point.
             * @param positon The position of the point in 3D space.
             * @param color The color of the point.
             */
            virtual void drawPoint( s32 id, const Vector3<real_Num> &positon, u32 color ) = 0;

            /**
             * Draws a debug line.
             * @param id The unique identifier for the line.
             * @param start The start position of the line in 3D space.
             * @param end The end position of the line in 3D space.
             * @param colour The color of the line.
             */
            virtual void drawLine( s32 id, const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                   u32 colour ) = 0;

            /**
             * Draws text.
             * @param id The unique identifier for the text.
             * @param position The position of the text in 2D space.
             * @param text The text to draw.
             * @param color The color of the text.
             */
            virtual void drawText( s32 id, const Vector2<real_Num> &position, const String &text,
                                   u32 color ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IDebug_h__
