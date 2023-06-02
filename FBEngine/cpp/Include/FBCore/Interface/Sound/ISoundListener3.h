#ifndef _ISoundListener3D_H
#define _ISoundListener3D_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    class ISoundListener3 : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ISoundListener3() override = default;

        /** Set the new position of the listener. */
        virtual void setPosition( const Vector3F &position ) = 0;

        /** Retrieves the current position of the listener. */
        virtual Vector3F getPosition() const = 0;

        /** Sets the direction the listener is facing. */
        virtual void setForwardVector( const Vector3F &forwardVector ) = 0;

        /** Gets the direction the listener is facing. */
        virtual Vector3F getForwardVector() const = 0;

        /** Set the velocity of the listener. */
        virtual void setVelocity( const Vector3F &velocity ) = 0;

        /** Retrieves the velocity of the listener. */
        virtual Vector3F getVelocity() const = 0;
    };
}  // end namespace fb

#endif
