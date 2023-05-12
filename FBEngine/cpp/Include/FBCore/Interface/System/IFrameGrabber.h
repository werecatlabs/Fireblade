#ifndef IFrameGrabber_h__
#define IFrameGrabber_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    //---------------------------------------------
    class IFrameGrabber : public ISharedObject
    {
    public:
        ~IFrameGrabber() override = default;

        virtual void addFrame( SmartPtr<IStateMessage> message ) = 0;
        virtual SmartPtr<IStateMessage> popFrame() const = 0;
    };
}  // end namespace fb

#endif  // IFrameGrabber_h__
