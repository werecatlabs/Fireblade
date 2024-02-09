#ifndef _ISOUNDMANAGER_H
#define _ISOUNDMANAGER_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceManager.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    /**
     * @brief An interface for a sound manager.
     */
    class ISoundManager : public IResourceManager
    {
    public:
        /**
         * @brief Virtual destructor for ISoundManager.
         */
        ~ISoundManager() override = default;

        /**
         * @brief Adds a sound to the sound manager.
         * @param name The name of the sound resource (can also be a file path).
         * @param loop A boolean indicating whether the sound loops.
         * @return A shared pointer to the sound.
         */
        virtual SmartPtr<ISound> addSound( const String &name, bool loop = true ) = 0;

        /** 
        * @brief Remove a sound from the sound manager.
        * @param sound The sound to remove.
        */
        virtual void removeSound( SmartPtr<ISound> sound ) = 0;

        /**
         * @brief Adds a sound listener to the sound manager.
         * @param name The name of the listener.
         * @param position The position of the listener.
         * @return A shared pointer to the sound listener.
         */
        virtual SmartPtr<ISoundListener3> addListener3(
            const String &name, const Vector3<real_Num> &position = Vector3<real_Num>::zero() ) = 0;

        /**
         * @brief Finds a sound listener by name.
         * @param name The name of the listener to find.
         * @return A shared pointer to the found sound listener.
         */
        virtual SmartPtr<ISoundListener3> findListener3( const String &name ) = 0;

        /**
         * @brief Sets the master volume of the sound manager.
         * @param volume The volume level (0.0 to 1.0).
         */
        virtual void setVolume( f32 volume ) = 0;

        /**
         * @brief Gets the master volume of the sound manager.
         * @return The volume level (0.0 to 1.0).
         */
        virtual f32 getVolume() const = 0;

        /**
         * @brief Starts recording sound.
         */
        virtual void startRecording() = 0;

        /**
         * @brief Stops recording sound.
         */
        virtual void stopRecording() = 0;

        /**
         * @brief Gets the size of the sound buffer.
         * @return The size of the sound buffer.
         */
        virtual u32 getBufferSize() const = 0;

        /**
         * @brief Copies the contents of the sound buffer to memory.
         * @param buffer The destination buffer.
         * @param size The size of the buffer.
         */
        virtual void copyContentsToMemory( void *buffer, u32 size ) = 0;

        /**
         * @brief Returns whether the sound manager is running in real-time.
         * @return True if the sound manager is running in real-time, false otherwise.
         */
        virtual bool isRealtime() const = 0;

        /**
         * @brief Returns whether the sound manager is muted.
         */
        virtual bool isMute() const = 0;

        /**
         * @brief Sets whether the sound manager is muted.
         */
        virtual void setMute( bool mute ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
