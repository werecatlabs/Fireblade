#ifndef __FBOgreGfxAutolink__H
#define __FBOgreGfxAutolink__H

#if FB_USE_AUTO_LINK
#ifdef _DEBUG
	#pragma comment(lib, "FBGraphicsOgre_d.lib")
#elif NDEBUG
	#pragma comment(lib, "FBGraphicsOgre.lib")
#else
	#pragma comment(lib, "FBGraphicsOgre.lib")
#endif
#endif

#endif
