#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FrameGrabberStandard.h>
#include <FBCore/Interface/Video/IVideoStream.h>
#include <FBCore/Interface/System/IStateMessage.h>

namespace fb
{
    
    FrameGrabberStandard::FrameGrabberStandard()
    {
        // auto engine = core::IApplicationManager::instance();
        // SmartPtr<IVideoManager>& videoMgr = engine->getVideoManager();
        // m_videoStream = videoMgr->createVideoStream();
    }

    
    FrameGrabberStandard::~FrameGrabberStandard()
    {
    }

    
    void FrameGrabberStandard::update()
    {
        // auto engine = core::IApplicationManager::instance();
        // SmartPtr<IGraphicsSystem>& gfxSystem = engine->getGraphicsSystem();
        // SmartPtr<IWindow> window = gfxSystem->getRenderWindow();

        // Vector2F windowSize = window->getSize();
        // u32 w = windowSize.X();
        // u32 h = windowSize.Y();
        // u32 colourDepth = window->getColourDepth();
        // u32 size = w * h * colourDepth;

        // StateFrameDataPtr data(new StateFrameData);
        // data->setVideoBufferSize(size);

        // window->copyContentsToMemory(data->getVideoBuffer(), size);

        // SmartPtr<ISoundManager> soundMgr = engine->getSoundManager();
        // u32 soundBufferSize = soundMgr->getBufferSize();
        // data->setSoundBufferSize(soundBufferSize);

        // soundMgr->copyContentsToMemory(data->getSoundBuffer(), soundBufferSize);

        // m_videoStream->addFrame(data);
    }

    
    void FrameGrabberStandard::addFrame( SmartPtr<IStateMessage> message )
    {
        // m_videoStream->addFrame(message);
    }

    
    SmartPtr<IStateMessage> FrameGrabberStandard::popFrame() const
    {
        return nullptr;
    }
} // end namespace fb
