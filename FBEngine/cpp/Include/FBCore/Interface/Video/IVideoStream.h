#ifndef IVideoStream_h__
#define IVideoStream_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Video/IVideo.h>

namespace fb
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

}  // end namespace fb

#endif  // IVideoStream_h__
