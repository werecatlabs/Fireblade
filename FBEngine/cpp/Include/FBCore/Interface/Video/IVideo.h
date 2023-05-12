#ifndef IVideo_h__
#define IVideo_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    /**
     * @brief An interface for a video class.
     */
    class IVideo : public ISharedObject
    {
    public:
        /**
         * @brief Destructor for IVideo.
         */
        virtual ~IVideo() override = default;

        /**
         * @brief Gets the unique id of this video.
         * @return The unique id of this video.
         */
        virtual u32 getId() const = 0;

        /**
         * @brief Used to play the video.
         */
        virtual void play() = 0;

        /**
         * @brief Used to stop the video.
         */
        virtual void stop() = 0;

        /**
         * @brief Gets the size of the video.
         * @return The size of the video.
         */
        virtual Vector2I getSize() const = 0;

        /**
         * @brief Sets the size of the video.
         * @param size The new size of the video.
         */
        virtual void setSize( const Vector2I &size ) = 0;

        /**
         * @brief Gets the frame buffer data.
         * @return The frame buffer data.
         */
        virtual void *getCurrentFrameBuffer() const = 0;

        /**
         * @brief Sets the loop flag for the video.
         * @param loop The new loop flag.
         */
        virtual void setLoop( bool loop ) = 0;

        /**
         * @brief Gets the loop flag for the video.
         * @return The loop flag for the video.
         */
        virtual bool getLoop() const = 0;

        /**
         * @brief Sets the auto-update flag for the video.
         * @param autoUpdate The new auto-update flag.
         */
        virtual void setAutoUpdate( bool autoUpdate ) = 0;

        /**
         * @brief Gets the auto-update flag for the video.
         * @return The auto-update flag for the video.
         */
        virtual bool getAutoUpdate() const = 0;

        /**
         * @brief Gets the video texture object.
         * @return The video texture object.
         */
        virtual SmartPtr<IVideoTexture> getVideoTexture() const = 0;

        /**
         * @brief Sets the video texture object.
         * @param val The new video texture object.
         */
        virtual void setVideoTexture( SmartPtr<IVideoTexture> val ) = 0;

        /**
         * @brief Gets the object.
         * @param object The object to get.
         */
        virtual void _getObject( void **object ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IVideo_h__
