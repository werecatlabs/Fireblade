#ifndef ISoundPlayer_h__
#define ISoundPlayer_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    /**
     * @brief An interface for a sound player.
     */
    class ISoundPlayer : public ISharedObject
    {
    public:
        /**
         * @brief Destructor for ISoundPlayer.
         */
        ~ISoundPlayer() override = default;

        /**
         * @brief Starts playing the sound.
         */
        virtual void play() = 0;

        /**
         * @brief Stops playing the sound.
         */
        virtual void stop() = 0;

        /**
         * @brief Pauses the sound.
         */
        virtual void pause() = 0;

        /**
         * @brief Skips forward in the sound.
         */
        virtual void forward() = 0;

        /**
         * @brief Skips backward in the sound.
         */
        virtual void rewind() = 0;

        /**
         * @brief Restarts the sound from the beginning.
         */
        virtual void restart() = 0;

        /**
         * @brief Skips to the next section of the sound.
         */
        virtual void skip() = 0;

        /**
         * @brief Sets whether the sound should repeat.
         * @param repeat True to repeat, false to play only once.
         */
        virtual void setRepeat( bool repeat ) = 0;

        /**
         * @brief Gets whether the sound is set to repeat.
         * @return True if the sound is set to repeat, false otherwise.
         */
        virtual bool getRepeat() const = 0;

        /**
         * @brief Sets whether there should be a delay between beeps in the sound.
         * @param beepDelay True to enable the delay, false to disable.
         */
        virtual void setBeepDelay( bool beepDelay ) = 0;

        /**
         * @brief Gets whether there is a delay between beeps in the sound.
         * @return True if there is a delay, false otherwise.
         */
        virtual bool getBeepDelay() const = 0;

        /**
         * @brief Sets the volume of the sound.
         * @param volume The new volume level.
         */
        virtual void setVolume( f32 volume ) = 0;

        /**
         * @brief Gets the volume of the sound.
         * @return The volume level.
         */
        virtual f32 getVolume() const = 0;

        /**
         * @brief Gets the length of the sound track in milliseconds.
         * @return The length of the sound track in milliseconds.
         */
        virtual s32 getTrackLength() = 0;

        /**
         * @brief Gets the current playback time in milliseconds.
         * @return The current playback time in milliseconds.
         */
        virtual s32 getPlaybackTime() = 0;

        /**
         * @brief Sets the current playback time in milliseconds.
         * @param playbackTime The new playback time in milliseconds.
         */
        virtual void setPlaybackTime( s32 playbackTime ) = 0;

        /**
         * @brief Sets the delay time between beeps in the sound.
         * @param delayTime The new delay time in milliseconds.
         */
        virtual void setDelayTime( s32 delayTime ) = 0;

        /**
         * @brief Gets the delay time between beeps in the sound.
         * @return The delay time in milliseconds.
         */
        virtual s32 getDelayTime() const = 0;

        /**
         * @brief Gets the ID tag of the sound.
         * @return The ID tag of the sound.
         */
        virtual String getIdTag() = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // ISoundPlayer_h__
