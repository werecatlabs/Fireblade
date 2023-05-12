#ifndef ITrackElement_h__
#define ITrackElement_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    class ITrackElement : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ITrackElement() override = default;

        virtual Vector3F getCenter() const = 0;
        virtual void setCenter( const Vector3F &val ) = 0;

        virtual Vector3F getStart() const = 0;
        virtual void setStart( const Vector3F &val ) = 0;

        virtual Vector3F getEnd() const = 0;
        virtual void setEnd( const Vector3F &val ) = 0;

        virtual Vector3F getDirection() const = 0;
        virtual void setDirection( const Vector3F &val ) = 0;

        virtual Vector3F getExtents() const = 0;
        virtual void setExtents( const Vector3F &val ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ITrackElement_h__
