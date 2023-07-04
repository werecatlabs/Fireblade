#ifndef __FBOgreGFX__H
#define __FBOgreGFX__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Memory/SharedObject.h>

/*
#ifdef _DEBUG
    #pragma comment(lib, "OgreMain_d.lib")
    #pragma comment(lib, "OgreOverlay_d.lib")
    #pragma comment(lib, "Plugin_ParticleFX_d.lib")
    #pragma comment(lib, "ParticleUniverse_d.lib")
    #pragma comment(lib, "OgreTerrain_d.lib")
    #pragma comment(lib, "OgrePaging_d.lib")
    #pragma comment(lib, "MeshSplitter_d.lib")
    #pragma comment(lib, "assimp_d.lib")
    //#pragma comment(lib, "PagedGeometry.lib")
#elif NDEBUG
    #pragma comment(lib, "OgreMain.lib")
    #pragma comment(lib, "OgreOverlay.lib")
    #pragma comment(lib, "Plugin_ParticleFX.lib")
    #pragma comment(lib, "ParticleUniverse.lib")
    #pragma comment(lib, "OgreTerrain.lib")
    #pragma comment(lib, "OgrePaging.lib")
    #pragma comment(lib, "MeshSplitter.lib")
    #pragma comment(lib, "assimp.lib")
    #pragma comment(lib, "PagedGeometry.lib")
#else
    #pragma comment(lib, "OgreMain.lib")
    #pragma comment(lib, "OgreOverlay.lib")
    #pragma comment(lib, "Plugin_ParticleFX.lib")
    #pragma comment(lib, "ParticleUniverse.lib")
    #pragma comment(lib, "OgreTerrain.lib")
    #pragma comment(lib, "OgrePaging.lib")
    #pragma comment(lib, "MeshSplitter.lib")
    #pragma comment(lib, "assimp.lib")
    #pragma comment(lib, "PagedGeometry.lib")
#endif
*/

//#ifndef FB_EXPORTS
//	#include "FBOgreGfxAutolink.h"
//#endif

namespace fb
{
    namespace render
    {

        class FBGraphicsOgre : public SharedObject<ISharedObject>
        {
        public:
            FBGraphicsOgre() = default;
            ~FBGraphicsOgre() = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            static SmartPtr<render::IGraphicsSystem> createGraphicsOgre();
        };

    }  // end namespace render
}  // end namespace fb

#endif
