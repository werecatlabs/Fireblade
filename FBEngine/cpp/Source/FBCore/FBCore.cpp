#include <FBCore/FBCorePCH.h>
#include <FBCore/FBCore.h>
#include <FBCore/System/EventJob.h>

namespace fb
{
    void FBCore::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<EventJob>();
        FactoryUtil::addFactory<FSMManager>();
        FactoryUtil::addFactory<Properties>();
        FactoryUtil::addFactory<StateQueueStandard>(); 
        FactoryUtil::addFactory<StateContextStandard>(); 
        FactoryUtil::addFactory<StateManagerStandard>();     
        FactoryUtil::addFactory<WorkerThread>();

        FactoryUtil::addFactory<SceneNodeState>();
        FactoryUtil::addFactory<RigidbodyState>();
        FactoryUtil::addFactory<TextureState>();

        factoryManager->setPoolSizeByType<EventJob>( 32 );
        factoryManager->setPoolSizeByType<Properties>( 32 );
        factoryManager->setPoolSizeByType<StateQueueStandard>( 32 );
        factoryManager->setPoolSizeByType<StateContextStandard>( 32 );
        factoryManager->setPoolSizeByType<WorkerThread>( 8 );

        factoryManager->setPoolSizeByType<SceneNodeState>( 32 );
        factoryManager->setPoolSizeByType<TextureState>( 32 );
        factoryManager->setPoolSizeByType<RigidbodyState>( 32 );
    }

    void FBCore::unload( SmartPtr<ISharedObject> data )
    {
    }
}  // end namespace fb
