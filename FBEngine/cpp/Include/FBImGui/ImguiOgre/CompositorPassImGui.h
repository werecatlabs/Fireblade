
#pragma once

#include <FBImGui/FBImGuiPrerequisites.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#include "OgrePrerequisites.h"
#include "Compositor/Pass/OgreCompositorPass.h"

#include "OgreHeaderPrefix.h"

namespace Ogre
{
	class CompositorPassImGuiDef;

	/** @ingroup Api_Backend
	@class CompositorPassColibriGui
		Implementation of CompositorPass
	@author
		Matias N. Goldberg
	@version
		1.0
	*/
	class CompositorPassImGui : public CompositorPass
	{
	protected:
		SceneManager    *mSceneManager;

		void setResolutionToColibri( uint32 width, uint32 height );

	public:
		CompositorPassImGui( const CompositorPassImGuiDef *definition,
								  SceneManager *sceneManager,
								  const RenderTargetViewDef *rtv, CompositorNode *parentNode );

		virtual void execute( const Camera *lodCamera );

		virtual bool notifyRecreated( const TextureGpu *channel );

	private:
		CompositorPassImGuiDef const *mDefinition;
	};
}

#include "OgreHeaderSuffix.h"

#endif
