#ifndef IBoxShape3_h__
#define IBoxShape3_h__

#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a box collision shape.
         *
         * This class represents a box-shaped collision shape that can be used in 3D physics simulations.
         * It is an abstract base class that defines the basic interface for a box shape. This includes
         * methods for calculating the size of the box, as well as for setting and getting the axis-aligned
         * bounding box (AABB) of the shape.
         *
         * The `IBoxShape3` class is intended to be subclassed to create specific types of box shapes with
         * different sizes and aspect ratios. Each subclass should implement the pure virtual methods
         * defined in this class to provide the specific functionality for the shape.
         *
         * @see IPhysicsShape3, IBoxShape2
         */
        class IBoxShape3 : public IPhysicsShape3
        {
        public:
            /** Destructor. */
            ~IBoxShape3() override = default;

            /**
             * @brief Gets the extents of the box shape.
             *
             * @return The extents of the box shape.
             */
            virtual Vector3<real_Num> getExtents() const = 0;

            /**
             * @brief Sets the extents of the box shape.
             *
             * @param extents The extents of the box shape.
             */
            virtual void setExtents( const Vector3<real_Num> &extents ) = 0;

            /**
             * @brief Gets the axis aligned bounding box of this shape.
             *
             * @return The axis aligned bounding box of this shape.
             */
            virtual AABB3<real_Num> getAABB() const = 0;

            /**
             * @brief Sets the axis aligned bounding box of this shape.
             *
             * @param box The axis aligned bounding box of this shape.
             */
            virtual void setAABB( const AABB3<real_Num> &box ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IBoxShape3_h__
