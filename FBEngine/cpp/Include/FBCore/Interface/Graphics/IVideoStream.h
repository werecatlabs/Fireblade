#ifndef IVideoStream_h__
#define IVideoStream_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Graphics/IVideo.h>

namespace fb
{
    namespace render
    {

        /** An interface for a video stream class. */
        class IVideoStream : public IVideo
        {
        public:
            /** */
            ~IVideoStream() override = default;

            /** */
            virtual void addFrame( SmartPtr<IStateMessage> message ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // end namespace fb

#endif  // IVideoStream_h__
