#include <FBCore/FBCorePCH.h>
#include "FBCore/Jobs/CameraManagerReset.h"
#include <FBCore/FBCore.h>

namespace fb
{

    CameraManagerReset::CameraManagerReset()
    {
    }

    CameraManagerReset::~CameraManagerReset()
    {
    }

    void CameraManagerReset::execute()
    {
        //Thread::sleep( 1.0 );

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto cameraManager = applicationManager->getCameraManager();
        if( cameraManager )
        {
            cameraManager->reset();
        }
    }

    SmartPtr<ISharedObject> CameraManagerReset::getOwner() const
    {
        return m_owner;
    }

    void CameraManagerReset::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

}  // namespace fb
