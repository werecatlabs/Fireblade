#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/CompositorPassUI.h"
#include "FBGraphicsOgreNext/CompositorPassUiDef.h"
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/FBCoreHeaders.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT

#    include "Compositor/OgreCompositorNode.h"
#    include "Compositor/OgreCompositorWorkspace.h"
#    include "Compositor/OgreCompositorWorkspaceListener.h"
#    include "OgrePixelFormatGpuUtils.h"
#    include "OgreRenderSystem.h"
#    include "OgreSceneManager.h"

namespace fb
{

    CompositorPassUI::CompositorPassUI( const CompositorPassUiDef *definition,
                                        Ogre::SceneManager *sceneManager,
                                        const Ogre::RenderTargetViewDef *rtv,
                                        Ogre::CompositorNode *parentNode ) :
        CompositorPass( definition, parentNode ),
        mSceneManager( sceneManager ),
        mDefinition( definition )
    {
        auto texture = mParentNode->getDefinedTexture( rtv->colourAttachments[0].textureName );
        setResolutionToColibri( texture->getWidth(), texture->getHeight() );
    }

    //-----------------------------------------------------------------------------------
    void CompositorPassUI::execute( const Ogre::Camera *lodCamera )
    {
        using namespace fb;

        //Execute a limited number of times?
        if( mNumPassesLeft != std::numeric_limits<u32>::max() )
        {
            if( !mNumPassesLeft )
            {
                return;
            }

            --mNumPassesLeft;
        }

        profilingBegin();

        notifyPassEarlyPreExecuteListeners();

        //analyzeBarriers();
        //executeResourceTransitions();

        //Fire the listener in case it wants to change anything
        notifyPassPreExecuteListeners();

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();

        if( mDefinition->mRenderSceneUI )
        {
            auto renderUI = applicationManager->getRenderUI();
            renderUI->update();
        }
        else
        {
            if( ui )
            {
                auto application = ui->getApplication();
                FB_ASSERT( application );

                application->update();
            }
        }

        notifyPassPosExecuteListeners();

        profilingEnd();
    }

    //-----------------------------------------------------------------------------------
    void CompositorPassUI::setResolutionToColibri( u32 width, u32 height )
    {
        using namespace fb;

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        if( ui )
        {
            auto application = ui->getApplication();
            if( application )
            {
                application->setWindowSize( Vector2I( width, height ) );
            }
        }
    }

    //-----------------------------------------------------------------------------------
    bool CompositorPassUI::notifyRecreated( const Ogre::TextureGpu *channel )
    {
        bool usedByUs = CompositorPass::notifyRecreated( channel );

        if( usedByUs && !Ogre::PixelFormatGpuUtils::isDepth( channel->getPixelFormat() ) &&
            !Ogre::PixelFormatGpuUtils::isStencil( channel->getPixelFormat() ) )
        {
            setResolutionToColibri( channel->getWidth(), channel->getHeight() );
        }

        return usedByUs;
    }

}  // namespace fb

#endif
