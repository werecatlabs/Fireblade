#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/CameraManagerReset.h>
#include <FBCore/Scene/CameraManager.h>
#include <FBCore/Interface/IApplicationManager.h>

namespace fb
{
    CameraManagerReset::CameraManagerReset() = default;

    CameraManagerReset::~CameraManagerReset() = default;

    void CameraManagerReset::execute()
    {
        auto delayTime = getDelayTime();
        if( delayTime > 0.0f )
        {
            Thread::sleep( delayTime );
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto cameraManager = applicationManager->getCameraManager();
        if( cameraManager )
        {
            cameraManager->reset();
        }
    }

    auto CameraManagerReset::getOwner() const -> SmartPtr<ISharedObject>
    {
        return m_owner;
    }

    void CameraManagerReset::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    auto CameraManagerReset::getDelayTime() const -> f32
    {
        return m_delayTime;
    }

    void CameraManagerReset::setDelayTime( f32 delayTime )
    {
        m_delayTime = delayTime;
    }

}  // namespace fb
