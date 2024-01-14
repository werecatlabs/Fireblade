#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/Apple/CTextureOgreApple.h>
#include <FBCore/FBCore.h>
#include <OgreTextureGpuManager.h>
#include <OgreRoot.h>
#include <FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include/OgreMetalPrerequisites.h>
#include <FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include/OgreMetalTextureGpu.h>

namespace fb
{
    namespace render
    {

        void CTextureOgreApple::getTextureFinal( void **ppTexture ) const
        {
            auto ogreTexture = getTexture();
            if (ogreTexture)
            {
#if defined FB_PLATFORM_APPLE
#    if FB_BUILD_RENDERER_METAL
                auto glTexture = static_cast<Ogre::MetalTextureGpu *>( ogreTexture );
                auto tex = glTexture->getFinalTextureName();
#    elif FB_BUILD_RENDERER_OPENGL
                auto glTexture = (Ogre::GL3PlusTextureGpu *)ogreTexture;
                auto tex = glTexture->getFinalTextureName();
#    endif
#endif

                *static_cast<void **>( ppTexture ) = (void *)CFBridgingRetain( tex );
            }
        }

    }  // end namespace render
}  // end namespace fb
