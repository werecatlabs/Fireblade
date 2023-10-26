#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/FBGraphicsOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialPassOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTechniqueOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBCore/FBCore.h>

#include "FBGraphicsOgreNext/UI/UIManager.h"

namespace fb
{
    namespace render
    {

        void FBGraphicsOgreNext::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            FactoryUtil::addFactory<CGraphicsSystemOgreNext>();
            FactoryUtil::addFactory<GraphicsSystemState>();

            FactoryUtil::addFactory<Compositor>();

            FactoryUtil::addFactory<CMaterialOgreNext>();
            FactoryUtil::addFactory<CMaterialPassOgreNext>();
            FactoryUtil::addFactory<CMaterialTechniqueOgreNext>();
            FactoryUtil::addFactory<CMaterialTextureOgreNext>();

            FactoryUtil::addFactory<CTextureOgreNext>();

            FactoryUtil::addFactory<CSceneNodeOgreNext>();


            FactoryUtil::addFactory<ui::UIManager>();

            factoryManager->setPoolSizeByType<Compositor>( 4 );
            factoryManager->setPoolSizeByType<CMaterialOgreNext>( 1024 );
            factoryManager->setPoolSizeByType<CMaterialPassOgreNext>( 1024 );
            factoryManager->setPoolSizeByType<CMaterialTechniqueOgreNext>( 1024 );
            factoryManager->setPoolSizeByType<CMaterialTextureOgreNext>( 1024 );
            factoryManager->setPoolSizeByType<CTextureOgreNext>( 1024 );
            factoryManager->setPoolSizeByType<CSceneNodeOgreNext>( 1024 );
        }

        void FBGraphicsOgreNext::unload( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<IGraphicsSystem> FBGraphicsOgreNext::createGraphicsOgre()
        {
            return fb::make_ptr<CGraphicsSystemOgreNext>();
        }

    }  // end namespace render
}  // end namespace fb
