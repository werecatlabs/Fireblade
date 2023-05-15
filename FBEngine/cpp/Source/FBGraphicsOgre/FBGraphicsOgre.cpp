#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/FBGraphicsOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSystemOgre.h>
#include <FBCore/FBCore.h>
#include <FBGraphicsOgre/Wrapper/CDebugLine.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CSceneNodeOgre.h>

#include "FBGraphicsOgre/Wrapper/CTextureOgre.h"
#include <FBGraphicsOgre/Wrapper/CTextureOgreStateListener.h>
#include "Wrapper/CCubemap.h"
#include "Wrapper/CDebugText.h"
#include "Wrapper/CDynamicMesh.h"
#include "Wrapper/CFont.h"
#include "Wrapper/CGraphicsMeshOgre.h"
#include "Wrapper/CLightOgre.h"
#include "Wrapper/COverlayElementContainer.h"
#include "Wrapper/COverlayElementText.h"
#include "Wrapper/COverlayOgre.h"
#include "Wrapper/CWindowOgre.h"
#include "Wrapper/CTerrainOgre.h"
#include "Wrapper/CSceneNodeOgre.h"

namespace fb
{
    namespace render
    {

        void FBGraphicsOgre::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            FactoryUtil::addFactory<CGraphicsSystemOgre>();
            FactoryUtil::addFactory<GraphicsSystemState>();

            FactoryUtil::addFactory<CCubemap>();

            FactoryUtil::addFactory<CDebugLine>();
            FactoryUtil::addFactory<CDebugText>();
            FactoryUtil::addFactory<CDynamicMesh>();
            FactoryUtil::addFactory<CFont>();

            FactoryUtil::addFactory<CGraphicsMeshOgre>();
            FactoryUtil::addFactory<CLightOgre>();

            FactoryUtil::addFactory<COverlayOgre>();
            FactoryUtil::addFactory<COverlayElementContainer>();
            FactoryUtil::addFactory<COverlayElementText>();

            FactoryUtil::addFactory<CMaterialOgre>();
            FactoryUtil::addFactory<CMaterialPassOgre>();
            FactoryUtil::addFactory<CMaterialTechniqueOgre>();

            FactoryUtil::addFactory<CTextureOgre>();
            FactoryUtil::addFactory<CTextureOgreStateListener>();

            FactoryUtil::addFactory<CSceneNodeOgre>();
            FactoryUtil::addFactory<CTerrainOgre>();
            FactoryUtil::addFactory<CWindowOgre>();

            factoryManager->setPoolSizeByType<CGraphicsMeshOgre>( 32 );
            factoryManager->setPoolSizeByType<CLightOgre>( 32 );

            factoryManager->setPoolSizeByType<COverlayOgre>( 32 );
            factoryManager->setPoolSizeByType<COverlayElementContainer>( 32 );
            factoryManager->setPoolSizeByType<COverlayElementText>( 32 );

            factoryManager->setPoolSizeByType<CDebugLine>( 32 );
            factoryManager->setPoolSizeByType<CMaterialOgre>( 32 );
            factoryManager->setPoolSizeByType<CMaterialPassOgre>( 32 );
            factoryManager->setPoolSizeByType<CMaterialTechniqueOgre>( 32 );
            factoryManager->setPoolSizeByType<CSceneNodeOgre>( 32 );

            factoryManager->setPoolSizeByType<CTextureOgre>( 32 );
            factoryManager->setPoolSizeByType<CTextureOgreStateListener>( 32 );
        }

        void FBGraphicsOgre::unload( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<IGraphicsSystem> FBGraphicsOgre::createGraphicsOgre()
        {
            return fb::make_ptr<CGraphicsSystemOgre>();
        }

    }  // namespace render
}  // end namespace fb
