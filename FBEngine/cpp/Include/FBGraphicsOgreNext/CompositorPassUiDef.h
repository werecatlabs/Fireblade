
#pragma once


#include <FBImGui/FBImGuiPrerequisites.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#include "OgreHeaderPrefix.h"
#include "Compositor/Pass/OgreCompositorPassDef.h"

namespace fb
{
	/** @ingroup Api_Backend
	@class CompositorPassColibriGuiDef
	*/
	class CompositorPassUiDef : public Ogre::CompositorPassDef
	{
	public:
		enum AspectRatioMode
		{
			/// Never change the Canvas resolution
			ArNone,
			/// Change the canvas' height if the aspect ratio of the window changes.
			/// For example if the original canvas size was 1920x1080 and the window
			/// is resized to a resolution with AR = 4:3, now the virtual canvas
			/// will be 1920x1440
			ArKeepWidth,
			/// Change the canvas' width if the aspect ratio of the window changes.
			/// For example if the original canvas size was 1920x1080 and the window
			/// is resized to a resolution with AR = 4:3, now the virtual canvas
			/// will be 1440x1080
			ArKeepHeight,
		};

    public:
		bool mRenderSceneUI = false;
		bool mSetsResolution;
		AspectRatioMode mAspectRatioMode;

	
		CompositorPassUiDef( Ogre::CompositorTargetDef *parentTargetDef ) :
			CompositorPassDef( Ogre::PASS_CUSTOM, parentTargetDef ),
			mSetsResolution( true ),
			mAspectRatioMode( ArNone )
		{
			mProfilingId = "Colibri Gui";
		}
	};
}

#include "OgreHeaderSuffix.h"

#endif
