#ifndef FrameGrabberStandard_h__
#define FrameGrabberStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IFrameGrabber.h>

namespace fb
{

    class FrameGrabberStandard : public IFrameGrabber
    {
    public:
        FrameGrabberStandard();
        ~FrameGrabberStandard() override;

        void update() override;

        void addFrame( SmartPtr<IStateMessage> message ) override;
        SmartPtr<IStateMessage> popFrame() const override;

    protected:
        SmartPtr<render::IVideoStream> m_videoStream;
    };
}  // end namespace fb

#endif  // FrameGrabberStandard_h__
