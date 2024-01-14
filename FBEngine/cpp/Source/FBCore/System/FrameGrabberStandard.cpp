#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FrameGrabberStandard.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Video/IVideoManager.h>
#include <FBCore/Interface/Video/IVideoStream.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/State/Messages/StateFrameData.h>

namespace fb
{

    FrameGrabberStandard::FrameGrabberStandard()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto videoManager = applicationManager->getVideoManager();
        m_videoStream = videoManager->createVideoStream();
    }

    FrameGrabberStandard::~FrameGrabberStandard() = default;

    void FrameGrabberStandard::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto window = graphicsSystem->getRenderWindow();

        auto windowSize = window->getSize();
        u32 w = windowSize.X();
        u32 h = windowSize.Y();
        u32 colourDepth = window->getColourDepth();
        u32 size = w * h * colourDepth;

        auto data = fb::make_ptr<StateFrameData>();
        data->setVideoBufferSize( size );

        window->copyContentsToMemory( data->getVideoBuffer(), size );

        //auto soundMgr = engine->getSoundManager();
        //u32 soundBufferSize = soundMgr->getBufferSize();
        //data->setSoundBufferSize(soundBufferSize);
        //soundMgr->copyContentsToMemory(data->getSoundBuffer(), soundBufferSize);

        m_videoStream->addFrame( data );
    }

    void FrameGrabberStandard::addFrame( SmartPtr<IStateMessage> message )
    {
        m_videoStream->addFrame( message );
    }

    auto FrameGrabberStandard::popFrame() const -> SmartPtr<IStateMessage>
    {
        return nullptr;
    }
}  // end namespace fb
