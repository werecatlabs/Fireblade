#ifndef _FB_ISound_H
#define _FB_ISound_H

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    /** An interface for a sound. */
    class ISound : public IResource
    {
    public:
        /** Virtual destructor. */
        ~ISound() override = default;

        /** Plays the sound. */
        virtual void play() = 0;

        /** Stops the sound from playing. */
        virtual void stop() = 0;

        /** Returns true if the sound is currently playing and false if not.
        @return
        Returns a boolean value indicating whether or not the sound is playing.
        Returns true if the sound is currently playing and false if not.
        */
        virtual bool isPlaying() const = 0;

        /** Sets the volume of the sound.
        @param
        volume The volume level of the sound. This should be a value ranging from 0.0 to 1.0.
        */
        virtual void setVolume( f32 volume ) = 0;

        /** Sets the volume of the sound.
        @return
        Return the volume level. This is a value ranging from 0.0 to 1.0.
        */
        virtual f32 getVolume() const = 0;

        /** Set whether the sound should loop.
        @param
        loop A boolean indicating whether the sound should loop.
        */
        virtual void setLoop( bool loop ) = 0;

        /** Returns true if the sound loops.
        @return
        Returns a boolean indicating whether the sound should loop.
        */
        virtual bool getLoop() const = 0;

        /** Gets the name of the sound.
        @return
        Returns the name of the sound.
        */
        virtual String getName() const = 0;

        /** Gets the spectrum from the sound.
        @param
        spectrum The spectrum data.
        @param
        numValues The number of value to get in the spectrum data.
        @remarks
        This will depend on the sound library used.
        */
        virtual void getSpectrum( Array<f32> &spectrum, u32 numValues ) const = 0;

        /**
         * @brief Sets the pan of the sound.
         * @param pan The new pan of the sound.
         */
        virtual void setPan( f32 pan ) = 0;

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
