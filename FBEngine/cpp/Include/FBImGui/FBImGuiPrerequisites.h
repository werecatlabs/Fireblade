#ifndef FBImGuiPrerequisites_h__
#define FBImGuiPrerequisites_h__

#include <FBCore/FBCorePrerequisites.h>

#if FB_BUILD_SDL2
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;
#endif

class ImguiManagerOgre;

namespace OIS
{
    class JoyStick;
}

namespace Ogre
{
    class MetalDevice;
    class MetalWindow;
}

namespace Ogre
{
    class RenderTargetListener;
}

namespace fb
{
    class ImGuiOverlayOgre;

    namespace ui
    {
        class ImGuiApplication;
        class ImGuiApplicationOSX;
        class ImGuiRenderWindow;

        template<class T>
        class ImGuiWindowT;

        class ImGuiMenu;
        class ImGuiMenubar;
        class ImGuiMenuItem;
    } // end namespace ui
}     // end namespace fb

#endif  // FBImGuiPrerequisites_h__
