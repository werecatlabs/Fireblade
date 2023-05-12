#ifndef _ISound3D_H
#define _ISound3D_H

#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    /** An interface for a 3d sound. */
    class ISound3 : public ISound
    {
    public:
        /** Virtual destructor. */
        ~ISound3() override = default;

        /** Sets the position of the sound.
        @param
        position The position of the 3d sound.
        */
        virtual void setPosition( const Vector3F &position ) = 0;

        /** Return the position of the sound.
        @return
        Returns the position of the 3d sound.
        */
        virtual Vector3F getPosition() const = 0;

        /** Sets the minimum and maximum distance of the sound. */
        virtual void setMinMaxDistance( f32 minDistance, f32 maxDistance ) = 0;

        /** Gets the minimum and maximum distance of the sound. */
        virtual void getMinMaxDistance( f32 &minDistance, f32 &maxDistance ) = 0;
    };
}  // end namespace fb

#endif
