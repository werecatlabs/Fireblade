#include "FBImGui/ImguiOgre/CompositorPassImGui.h"
#include "FBImGui/ImguiOgre/CompositorPassImGuiDef.h"
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/FBCoreHeaders.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT

#include "Compositor/OgreCompositorNode.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorWorkspaceListener.h"
#include "OgrePixelFormatGpuUtils.h"
#include "OgreRenderSystem.h"
#include "OgreSceneManager.h"



namespace Ogre
{



	CompositorPassImGui::CompositorPassImGui( const CompositorPassImGuiDef *definition,
														SceneManager *sceneManager,
														const RenderTargetViewDef *rtv,
														CompositorNode *parentNode ) :
		CompositorPass( definition, parentNode ),
		mSceneManager( sceneManager ),
		mDefinition( definition )
	{
		TextureGpu *texture = mParentNode->getDefinedTexture( rtv->colourAttachments[0].textureName );
		setResolutionToColibri( texture->getWidth(), texture->getHeight() );
	}



	//-----------------------------------------------------------------------------------
	void CompositorPassImGui::execute( const Camera *lodCamera )
	{
		using namespace fb;

		//Execute a limited number of times?
		if( mNumPassesLeft != std::numeric_limits<uint32>::max() )
		{
			if (!mNumPassesLeft)
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
		FB_ASSERT(applicationManager);

		auto ui = applicationManager->getUI();
		FB_ASSERT(ui);

		auto application = ui->getApplication();
		FB_ASSERT(application);

		application->update();

		notifyPassPosExecuteListeners();

		profilingEnd();
	}



	//-----------------------------------------------------------------------------------
	void CompositorPassImGui::setResolutionToColibri( uint32 width, uint32 height )
	{
		using namespace fb;

		auto applicationManager = core::IApplicationManager::instance();
		FB_ASSERT(applicationManager);

		auto ui = applicationManager->getUI();
		FB_ASSERT(ui);

		auto application = ui->getApplication();
		FB_ASSERT(application);

		application->setWindowSize(Vector2I(width, height));


		if( !mDefinition->mSetsResolution )
			return;

		const Vector2 resolution( width, height );
		const Vector2 halfResolution( resolution / 2.0f );

		//if( fabsf( halfResolution.x - m_colibriManager->getHalfWindowResolution().x ) > 1e-6f ||
		//	fabsf( halfResolution.y - m_colibriManager->getHalfWindowResolution().y ) > 1e-6f )
		//{
		//	Ogre::Vector2 canvasSize = m_colibriManager->getCanvasSize();

		//	if( mDefinition->mAspectRatioMode == CompositorPassColibriGuiDef::ArKeepWidth )
		//	{
		//		const float newAr = resolution.y / resolution.x;
		//		canvasSize.y = round( canvasSize.x * newAr );
		//	}
		//	else if( mDefinition->mAspectRatioMode == CompositorPassColibriGuiDef::ArKeepHeight )
		//	{
		//		const float newAr = resolution.x / resolution.y;
		//		canvasSize.x = round( canvasSize.y * newAr );
		//	}

		//	m_colibriManager->setCanvasSize( canvasSize,
		//									 resolution );
		//}
	}



	//-----------------------------------------------------------------------------------
	bool CompositorPassImGui::notifyRecreated( const TextureGpu *channel )
	{
		bool usedByUs = CompositorPass::notifyRecreated( channel );

		if( usedByUs &&
			!PixelFormatGpuUtils::isDepth( channel->getPixelFormat() ) &&
			!PixelFormatGpuUtils::isStencil( channel->getPixelFormat() ) )
		{
			setResolutionToColibri( channel->getWidth(), channel->getHeight() );
		}

		return usedByUs;
	}


}


#endif