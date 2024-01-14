#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/CompositorPassProvider.h"

#include "FBGraphicsOgreNext/ColibriGui/Ogre/CompositorPassColibriGui.h"
#include "FBGraphicsOgreNext/ColibriGui/Ogre/CompositorPassColibriGuiDef.h"
#include "FBGraphicsOgreNext/CompositorPassUiDef.h"
#include "FBGraphicsOgreNext/CompositorPassUI.h"
#include <FBCore/FBCore.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT

namespace fb
{

    CompositorPassProvider::CompositorPassProvider() = default;

    //-------------------------------------------------------------------------
    auto CompositorPassProvider::addPassDef( Ogre::CompositorPassType passType, Ogre::IdString customId,
                                             Ogre::CompositorTargetDef *parentTargetDef,
                                             Ogre::CompositorNodeDef *parentNodeDef )
        -> Ogre::CompositorPassDef *
    {
        if( customId == "app_gui" )
        {
            return OGRE_NEW CompositorPassUiDef( parentTargetDef );
        }

        if( customId == "scene_gui" )
        {
            auto p = OGRE_NEW Ogre::CompositorPassColibriGuiDef( parentTargetDef );
            p->mSkipLoadStoreSemantics = true;
            //p->mRenderSceneUI = true;
            return p;
        }

        return nullptr;
    }

    //-------------------------------------------------------------------------
    auto CompositorPassProvider::addPass( const Ogre::CompositorPassDef *definition,
                                          Ogre::Camera *defaultCamera, Ogre::CompositorNode *parentNode,
                                          const Ogre::RenderTargetViewDef *rtvDef,
                                          Ogre::SceneManager *sceneManager ) -> Ogre::CompositorPass *
    {
        if( dynamic_cast<const CompositorPassUiDef *>( definition ) )
        {
            auto colibriGuiDef = static_cast<const CompositorPassUiDef *>( definition );
            return OGRE_NEW CompositorPassUI( colibriGuiDef, sceneManager, rtvDef,

                                              parentNode );
        }

        if( dynamic_cast<const Ogre::CompositorPassColibriGuiDef *>( definition ) )
        {
            auto applicationManager = fb::core::ApplicationManager::instance();
            auto ui = applicationManager->getRenderUI();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto sceneManager = graphicsSystem->getGraphicsScene();
            auto camera = sceneManager->getCamera();

            Ogre::SceneManager *smgr = nullptr;
            sceneManager->_getObject( reinterpret_cast<void **>( &smgr ) );

            Ogre::Camera *ogreCamera = nullptr;
            camera->_getObject( reinterpret_cast<void **>( &ogreCamera ) );

            Colibri::ColibriManager *colibriManager;
            ui->_getObject( reinterpret_cast<void **>( &colibriManager ) );

            auto colibriGuiDef = static_cast<const Ogre::CompositorPassColibriGuiDef *>( definition );
            return OGRE_NEW Ogre::CompositorPassColibriGui( colibriGuiDef, ogreCamera, smgr, rtvDef,
                                                            parentNode, colibriManager );
        }

        return nullptr;
    }
}  // namespace fb

#endif
