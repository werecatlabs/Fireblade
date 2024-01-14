#include <FBImGui/ImguiOgre/ImGuiRenderTargetListener.h>
#include <FBCore/FBCore.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <FBImGui/ImguiOgre/ImGuiOverlayOgre.h>
#    include <OgreOverlayManager.h>
#    include <Ogre.h>

using namespace fb;

ImGuiRenderTargetListener::ImGuiRenderTargetListener()
{
}

ImGuiRenderTargetListener::~ImGuiRenderTargetListener()
{
}

void ImGuiRenderTargetListener::preViewportUpdate( const Ogre::RenderTargetViewportEvent &evt )
{
    using namespace Ogre;

    if( ImGuiOverlayOgre::NewFrame() )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        auto application = ui->getApplication();
        application->update();
    }
}

#endif
