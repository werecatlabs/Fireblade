#ifndef IMassData2_h__
#define IMassData2_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace physics
    {

        /// This holds the mass data computed for a shape.
        class IMassData2 : public ISharedObject
        {
        public:
            ~IMassData2() override = default;

            /// The mass of the shape, usually in kilograms.
            virtual void setMass( f32 val ) = 0;

            /// The mass of the shape, usually in kilograms.
            virtual f32 getMass() const = 0;

            /// The position of the shape's centroid relative to the shape's origin.
            virtual void setCenter( Vector2F val ) = 0;

            /// The position of the shape's centroid relative to the shape's origin.
            virtual Vector2F getCenter() const = 0;

            /// The rotational inertia of the shape about the local origin.
            virtual void setInertia( f32 val ) = 0;

            /// The rotational inertia of the shape about the local origin.
            virtual f32 getInertia() const = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IMassData2_h__
