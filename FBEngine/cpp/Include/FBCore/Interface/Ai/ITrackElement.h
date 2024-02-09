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

        virtual Vector3<real_Num> getCenter() const = 0;
        virtual void setCenter( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getStart() const = 0;
        virtual void setStart( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getEnd() const = 0;
        virtual void setEnd( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getDirection() const = 0;
        virtual void setDirection( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getExtents() const = 0;
        virtual void setExtents( const Vector3<real_Num> &val ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ITrackElement_h__
