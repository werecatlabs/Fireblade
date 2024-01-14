
#include "ColibriGui/Ogre/CompositorPassColibriGui.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/Ogre/CompositorPassColibriGuiDef.h"

#include "Compositor/OgreCompositorNode.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorWorkspaceListener.h"
#include "OgreCamera.h"
#include "OgrePixelFormatGpuUtils.h"
#include "OgreRenderSystem.h"
#include "OgreSceneManager.h"
#include "FBCore/FBCore.h"

namespace Ogre
{
    CompositorPassColibriGui::CompositorPassColibriGui( const CompositorPassColibriGuiDef *definition,
                                                        Camera *defaultCamera,
                                                        SceneManager *sceneManager,
                                                        const RenderTargetViewDef *rtv,
                                                        CompositorNode *parentNode,
                                                        Colibri::ColibriManager *colibriManager ) :
        CompositorPass( definition, parentNode ),
        mSceneManager( sceneManager ),
        mCamera( 0 ),
        m_colibriManager( colibriManager ),
        mDefinition( definition )
    {
#if OGRE_VERSION >= OGRE_MAKE_VERSION( 3, 0, 0 )
        if( !definition->mSkipLoadStoreSemantics )
        {
            initialize( rtv );
        }
#    ifdef COLIBRI_MULTIPASS_SUPPORT
        else
        {
            OGRE_EXCEPT(
                Exception::ERR_INVALIDPARAMS,
                "skip_load_store_semantics MUST be false when compiled with COLIBRI_MULTIPASS_SUPPORT",
                "CompositorPassColibriGui::CompositorPassColibriGui" );
        }
#    endif
#endif
        mCamera = defaultCamera;

        m_textureName = rtv->colourAttachments[0].textureName;
        TextureGpu *texture = mParentNode->getDefinedTexture( m_textureName );
        setResolutionToColibri( texture->getWidth(), texture->getHeight() );
    }
    //-----------------------------------------------------------------------------------
    void CompositorPassColibriGui::execute( const Camera *lodCamera )
    {
        auto applicationManager = fb::core::ApplicationManager::instance();
        auto ui = applicationManager->getRenderUI();

        Colibri::ColibriManager *colibriManager;
        ui->_getObject( (void **)&colibriManager );

        if( colibriManager )
        {
            // Execute a limited number of times?
            if( mNumPassesLeft != std::numeric_limits<uint32>::max() )
            {
                if( !mNumPassesLeft )
                    return;
                --mNumPassesLeft;
            }

            profilingBegin();

            notifyPassEarlyPreExecuteListeners();

#if OGRE_VERSION >= OGRE_MAKE_VERSION( 3, 0, 0 )
            analyzeBarriers();
            executeResourceTransitions();
            setRenderPassDescToCurrent();
#endif

            SceneManager *sceneManager = mCamera->getSceneManager();
            sceneManager->_setCamerasInProgress( CamerasInProgress( mCamera ) );
            sceneManager->_setCurrentCompositorPass( this );

            // Fire the listener in case it wants to change anything
            notifyPassPreExecuteListeners();

            colibriManager->prepareRenderCommands();

            RenderSystem *renderSystem = sceneManager->getDestinationRenderSystem();
            renderSystem->executeRenderPassDescriptorDelayedActions();

            colibriManager->render();

            sceneManager->_setCurrentCompositorPass( 0 );

            notifyPassPosExecuteListeners();

            profilingEnd();
        }
    }
    //-----------------------------------------------------------------------------------
    void CompositorPassColibriGui::setResolutionToColibri( uint32 width, uint32 height )
    {
        if( !mDefinition->mSetsResolution )
            return;

        const Vector2 resolution( (Real)width, (Real)height );
        const Vector2 halfResolution( resolution / 2.0f );

        if( fabsf( halfResolution.x - m_colibriManager->getHalfWindowResolution().x ) > 1e-6f ||
            fabsf( halfResolution.y - m_colibriManager->getHalfWindowResolution().y ) > 1e-6f )
        {
            Ogre::Vector2 canvasSize = Ogre::Vector2( 1920, 1080 );

            if( mDefinition->mAspectRatioMode == CompositorPassColibriGuiDef::ArKeepWidth )
            {
                const float newAr = resolution.y / resolution.x;
                canvasSize.y = canvasSize.x * newAr;
            }
            else if( mDefinition->mAspectRatioMode == CompositorPassColibriGuiDef::ArKeepHeight )
            {
                const float newAr = resolution.x / resolution.y;
                canvasSize.x = canvasSize.y * newAr;
            }

            auto applicationManager = fb::core::ApplicationManager::instance();
            auto ui = applicationManager->getRenderUI();

            Colibri::ColibriManager *colibriManager = nullptr;
            ui->_getObject( (void **)&colibriManager );

            if( colibriManager )
            {
                colibriManager->setCanvasSize( canvasSize, resolution );
            }
        }
    }
    //-----------------------------------------------------------------------------------
    bool CompositorPassColibriGui::notifyRecreated( const TextureGpu *channel )
    {
        bool usedByUs = CompositorPass::notifyRecreated( channel );

        if( !usedByUs )
        {
            // Because of mSkipLoadStoreSemantics, notifyRecreated may return false incorrectly
            TextureGpu *texture = mParentNode->getDefinedTexture( m_textureName );
            usedByUs = texture == channel;
        }

        if( usedByUs &&  //
            !PixelFormatGpuUtils::isDepth( channel->getPixelFormat() ) &&
            !PixelFormatGpuUtils::isStencil( channel->getPixelFormat() ) )
        {
            setResolutionToColibri( channel->getWidth(), channel->getHeight() );
        }

        return usedByUs;
    }
}  // namespace Ogre
