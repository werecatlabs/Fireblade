
#pragma once

#include <FBImGui/FBImGuiPrerequisites.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#include "OgrePrerequisites.h"
#include "Compositor/Pass/OgreCompositorPass.h"

#include "OgreHeaderPrefix.h"

namespace fb
{
	class CompositorPassUiDef;

	/** @ingroup Api_Backend
	@class CompositorPassColibriGui
		Implementation of CompositorPass
	@author
		Matias N. Goldberg
	@version
		1.0
	*/
	class CompositorPassUI : public Ogre::CompositorPass
	{
	protected:
		Ogre::SceneManager    *mSceneManager;

		void setResolutionToColibri( u32 width, u32 height );

	public:
		CompositorPassUI( const CompositorPassUiDef *definition,
								  Ogre::SceneManager *sceneManager,
								  const Ogre::RenderTargetViewDef *rtv, Ogre::CompositorNode *parentNode );

		virtual void execute( const Ogre::Camera *lodCamera );

		virtual bool notifyRecreated( const Ogre::TextureGpu *channel );

	private:
		CompositorPassUiDef const *mDefinition;
	};
}

#include "OgreHeaderSuffix.h"

#endif
