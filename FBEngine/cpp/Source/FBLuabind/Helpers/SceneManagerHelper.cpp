#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuaBind/Helpers/SceneManagerHelper.h"

namespace fb
{

    SmartPtr<IGraphicsObject> SceneManagerHelper::_addGraphicsObject( IGraphicsScene *smgr,
                                                                      const char *type )
    {
        return smgr->addGraphicsObject( type );
    }

    SmartPtr<IGraphicsObject> SceneManagerHelper::_addGraphicsObjectNamed( IGraphicsScene *smgr,
                                                                           const char *name,
                                                                           const char *type )
    {
        return smgr->addGraphicsObject( name, type );
    }

    SmartPtr<IGraphicsMesh> SceneManagerHelper::_addMesh( IGraphicsScene *smgr, const char *meshFile )
    {
        return smgr->addMesh( meshFile );
    }

    SmartPtr<IGraphicsMesh> SceneManagerHelper::_addMeshNamed( IGraphicsScene *smgr, const char *name,
                                                               const char *meshFile )
    {
        return smgr->addMesh( name, meshFile );
    }

    ISceneNode *SceneManagerHelper::_getRootSceneNode( IGraphicsScene *smgr )
    {
        return smgr->getRootSceneNode().get();
    }

    fb::SmartPtr<IParticleSystem> SceneManagerHelper::_addParticleSystem( IGraphicsScene *smgr,
                                                                          const char *name,
                                                                          const char *templateName )
    {
        return smgr->addParticleSystem( name, templateName );
    }

    SmartPtr<IGraphicsObject> SceneManagerHelper::_getParticleSystem( IGraphicsScene *smgr,
                                                                      const char *name )
    {
        return smgr->getParticleSystem( name );
    }

    SmartPtr<ISceneNode> SceneManagerHelper::_addSceneNode( IGraphicsScene *smgr )
    {
        return smgr->addSceneNode();
    }

    void SceneManagerHelper::_loadSceneFile( IGraphicsScene *smgr, const char *filePath,
                                             SmartPtr<ISceneNode> parent )
    {
        //smgr->loadSceneFile( filePath, parent );
    }

}  // end namespace fb
