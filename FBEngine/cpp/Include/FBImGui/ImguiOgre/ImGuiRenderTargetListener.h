#ifndef ImGuiRenderTargetListener_h__
#define ImGuiRenderTargetListener_h__

#include <FBImGui/FBImGuiPrerequisites.h>

#    if FB_GRAPHICS_SYSTEM_OGRENEXT
#    elif FB_GRAPHICS_SYSTEM_OGRE
#include <OgreRenderTargetListener.h>

class ImGuiRenderTargetListener : public Ogre::RenderTargetListener
{
public:
    ImGuiRenderTargetListener();
    ~ImGuiRenderTargetListener();

    void preViewportUpdate( const Ogre::RenderTargetViewportEvent &evt );
};

#endif

#endif // ImGuiRenderTargetListener_h__



