#ifndef IDebug_h__
#define IDebug_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Quaternion.h>

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
            virtual void drawPoint( hash_type id, const Vector3<real_Num> &positon, u32 color ) = 0;

            /**
             * Draws a debug line.
             * @param id The unique identifier for the line.
             * @param start The start position of the line in 3D space.
             * @param end The end position of the line in 3D space.
             * @param colour The color of the line.
             */
            virtual SmartPtr<IDebugLine> drawLine( hash_type id, const Vector3<real_Num> &start,
                                                   const Vector3<real_Num> &end, u32 colour ) = 0;

            /** Draws a debug circle.
             * @param id The unique identifier for the circle.
             * @param position The position of the circle in 3D space.
             * @param radius The radius of the circle.
             * @param color The color of the circle.
             */
            virtual SmartPtr<IDebugCircle> drawCircle( hash_type id, const Vector3<real_Num> &position,
                                                       const Quaternion<real_Num> &orientation,
                                                       real_Num radius, u32 color ) = 0;

            /**
             * Draws text.
             * @param id The unique identifier for the text.
             * @param position The position of the text in 2D space.
             * @param text The text to draw.
             * @param color The color of the text.
             */
            virtual void drawText( hash_type id, const Vector2<real_Num> &position, const String &text,
                                   u32 color ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IDebug_h__
