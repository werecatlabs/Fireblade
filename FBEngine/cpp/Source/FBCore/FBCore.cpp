#include <FBCore/FBCorePCH.h>
#include <FBCore/FBCore.h>

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

        FactoryUtil::addFactory<scene::Actor>();
        FactoryUtil::addFactory<scene::Scene>();
        FactoryUtil::addFactory<scene::SceneManager>();
        FactoryUtil::addFactory<scene::Transform>();

        FactoryUtil::addFactory<scene::AudioEmitter>();

        FactoryUtil::addFactory<scene::LayoutTransform>();
        FactoryUtil::addFactory<scene::Layout>();
        FactoryUtil::addFactory<scene::Image>();
        FactoryUtil::addFactory<scene::Text>();

        FactoryUtil::addFactory<SceneNodeState>();
        FactoryUtil::addFactory<RigidbodyState>();
        FactoryUtil::addFactory<TextureState>();
        FactoryUtil::addFactory<UITransformState>();

        FactoryUtil::addFactory<WindowMessageData>();

        factoryManager->setPoolSizeByType<scene::Actor>( 32 );
        factoryManager->setPoolSizeByType<scene::AudioEmitter>( 4 );

        factoryManager->setPoolSizeByType<EventJob>( 32 );
        factoryManager->setPoolSizeByType<Properties>( 32 );
        factoryManager->setPoolSizeByType<StateQueueStandard>( 32 );
        factoryManager->setPoolSizeByType<StateContextStandard>( 32 );
        factoryManager->setPoolSizeByType<WorkerThread>( 8 );        

        factoryManager->setPoolSizeByType<SceneNodeState>( 32 );
        factoryManager->setPoolSizeByType<TextureState>( 32 );
        factoryManager->setPoolSizeByType<RigidbodyState>( 32 );

        factoryManager->setPoolSizeByType<WindowMessageData>( 4 );
    }

    void FBCore::unload( SmartPtr<ISharedObject> data )
    {
    }

}  // end namespace fb
