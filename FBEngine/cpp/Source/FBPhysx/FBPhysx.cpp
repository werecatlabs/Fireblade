#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysx.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBCore/FBCore.h>

namespace fb::physics
{

    auto createPhysxManager( const SmartPtr<Properties> &properties ) -> SmartPtr<IPhysicsManager>
    {
        auto physxMgr = fb::make_ptr<PhysxManager>();
        physxMgr->load( nullptr );
        return physxMgr;
    }

    void FBPhysx::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<PhysxManager>();
    }

    void FBPhysx::unload( SmartPtr<ISharedObject> data )
    {
    }

}  // namespace fb::physics
