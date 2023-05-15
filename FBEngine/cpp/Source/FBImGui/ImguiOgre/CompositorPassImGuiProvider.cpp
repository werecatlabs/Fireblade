
#include "FBImGui/ImguiOgre/CompositorPassImGuiProvider.h"
#include "FBImGui/ImguiOgre/CompositorPassImGuiDef.h"
#include "FBImGui/ImguiOgre/CompositorPassImGui.h"

#if FB_GRAPHICS_SYSTEM_OGRENEXT

namespace Ogre
{


	CompositorPassImGuiProvider::CompositorPassImGuiProvider(  ) 
	{
	}
	//-------------------------------------------------------------------------
	CompositorPassDef* CompositorPassImGuiProvider::addPassDef(
			CompositorPassType passType, IdString customId,
			CompositorTargetDef *parentTargetDef, CompositorNodeDef *parentNodeDef )
	{
		if( customId == "colibri_gui" )
			return OGRE_NEW CompositorPassImGuiDef( parentTargetDef );

		return 0;
	}
	//-------------------------------------------------------------------------
	CompositorPass* CompositorPassImGuiProvider::addPass( const CompositorPassDef *definition,
															   Camera *defaultCamera,
															   CompositorNode *parentNode,
															   const RenderTargetViewDef *rtvDef,
															   SceneManager *sceneManager )
	{
		FB_ASSERT( dynamic_cast<const CompositorPassImGuiDef*>( definition ) );
		const CompositorPassImGuiDef *colibriGuiDef =
				static_cast<const CompositorPassImGuiDef*>( definition );
		return OGRE_NEW CompositorPassImGui( colibriGuiDef, sceneManager, rtvDef,
												  parentNode );
	}
}


#endif