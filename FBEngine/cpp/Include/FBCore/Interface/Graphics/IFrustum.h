#ifndef _IFrustum_H
#define _IFrustum_H

#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Math/Sphere3.h>

namespace fb
{
    namespace render
    {

        /** Interface for a frustum used in culling. */
        class IFrustum : public IGraphicsObject
        {
        public:
            /** Virtual destructor. */
            ~IFrustum() override = default;

            /** Sets the position of the near clipping plane. */
            virtual void setNearClipDistance( f32 nearDist ) = 0;

            /** Sets the position of the near clipping plane. */
            virtual f32 getNearClipDistance() const = 0;

            /** Sets the distance to the far clipping plane. */
            virtual void setFarClipDistance( f32 farDist ) = 0;

            /** Retrieves the distance from the frustum to the far clipping plane. */
            virtual f32 getFarClipDistance() const = 0;

            /** Sets the aspect ratio for the frustum viewport. */
            virtual void setAspectRatio( f32 ratio ) = 0;

            /** Retrieves the current aspect ratio. */
            virtual f32 getAspectRatio() const = 0;

            /** Tests whether the given container is visible in the Frustum. */
            virtual bool isVisible( const AABB3F &bound ) const = 0;

            /** Tests whether the given container is visible in the Frustum. */
            virtual bool isVisible( const Sphere3F &bound ) const = 0;

            /** Tests whether the given vertex is visible in the Frustum. */
            virtual bool isVisible( const Vector3<real_Num> &vert ) const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
