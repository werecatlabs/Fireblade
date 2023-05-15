#ifndef SceneManagerHelper_h__
#define SceneManagerHelper_h__

#include <FBCore/Interface/Graphics/IGraphicsScene.h>

namespace fb
{
    using namespace fb::render;

    /* SceneManager functions */
    class SceneManagerHelper
    {
    public:
        static SmartPtr<IGraphicsObject> _addGraphicsObject( IGraphicsScene *smgr, const char *type );
        static SmartPtr<IGraphicsObject> _addGraphicsObjectNamed( IGraphicsScene *smgr, const char *name,
                                                                  const char *type );
        static SmartPtr<IGraphicsMesh> _addMesh( IGraphicsScene *smgr, const char *meshFile );
        static SmartPtr<IGraphicsMesh> _addMeshNamed( IGraphicsScene *smgr, const char *name,
                                                      const char *meshFile );
        static ISceneNode *_getRootSceneNode( IGraphicsScene *smgr );
        static SmartPtr<IParticleSystem> _addParticleSystem( IGraphicsScene *smgr, const char *name,
                                                             const char *templateName );
        static SmartPtr<IGraphicsObject> _getParticleSystem( IGraphicsScene *smgr, const char *name );
        static SmartPtr<ISceneNode> _addSceneNode( IGraphicsScene *smgr );

        static void _loadSceneFile( IGraphicsScene *smgr, const char *filePath,
                                    SmartPtr<ISceneNode> parent );
    };

}  // end namespace fb

#endif  // SceneManagerHelper_h__
