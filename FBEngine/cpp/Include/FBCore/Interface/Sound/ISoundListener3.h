#ifndef _ISoundListener3D_H
#define _ISoundListener3D_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    /** Interface for a 3D sound listener. */
    class ISoundListener3 : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ISoundListener3() override = default;

        /** Set the new position of the listener. */
        virtual void setPosition( const Vector3<real_Num> &position ) = 0;

        /** Retrieves the current position of the listener. */
        virtual Vector3<real_Num> getPosition() const = 0;

        /** Sets the direction the listener is facing. */
        virtual void setForwardVector( const Vector3<real_Num> &forwardVector ) = 0;

        /** Gets the direction the listener is facing. */
        virtual Vector3<real_Num> getForwardVector() const = 0;

        /** Set the velocity of the listener. */
        virtual void setVelocity( const Vector3<real_Num> &velocity ) = 0;

        /** Retrieves the velocity of the listener. */
        virtual Vector3<real_Num> getVelocity() const = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
