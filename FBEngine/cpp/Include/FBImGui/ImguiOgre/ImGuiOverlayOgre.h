// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.

#ifndef FB__COMPONENTS_OVERLAY_INCLUDE_OGREIMGUIOVERLAY_H_
#define FB__COMPONENTS_OVERLAY_INCLUDE_OGREIMGUIOVERLAY_H_


#if FB_GRAPHICS_SYSTEM_OGRE && !FB_GRAPHICS_SYSTEM_OGRENEXT
#    include <FBImGui/FBImGuiPrerequisites.h>
#    include "OgreOverlay.h"
#    include "OgreOverlayPrerequisites.h"
#    include <OgreResourceGroupManager.h>
#    include <OgreSimpleRenderable.h>
#    include <OgreRenderQueueListener.h>
#    include <FBCore/Core/StringTypes.h>
#include "FBCore/Memory/SmartPtr.h"

#    include <imgui.h>

namespace fb
{
    class _OgreOverlayExport ImGuiOverlayOgre : public Ogre::Overlay
    {
    public:
        ImGuiOverlayOgre();
        ~ImGuiOverlayOgre() override;

        void load();
        void unload();

        /// add font from ogre .fontdef file
        /// must be called before first show()
        ImFont *addFont( const String &name,
                         const String &group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

        static bool NewFrame();

        void _findVisibleObjects( Ogre::Camera *cam, Ogre::RenderQueue *queue,
                                  Ogre::Viewport *vp ) override;

	    LoadingState getLoadingState() const;

        void setLoadingState( LoadingState loadingState );

    private:
        void initialise() override;

        using CodePointRange = Array<ImWchar>;
        Array<CodePointRange> mCodePointRanges;

        class RenderQueueListener : public Ogre::RenderQueueListener
        {
        public:
            RenderQueueListener() = default;
            ~RenderQueueListener() override = default;

            void renderQueueStarted( Ogre::uint8 queueGroupId, const Ogre::String &invocation,
                                     bool &skipThisInvocation ) override;

            ImGuiOverlayOgre *getOwner() const;
            void setOwner( ImGuiOverlayOgre *owner );

            SmartPtr<render::ICamera> getCamera() const;
            void setCamera( SmartPtr<render::ICamera> camera );

        private:
            ImGuiOverlayOgre *m_owner = nullptr;
            WeakPtr<render::ICamera> m_camera = nullptr;
        };

        class ImGUIRenderable : public Ogre::SimpleRenderable
        {
        public:
            ImGUIRenderable();
            ~ImGUIRenderable() override;

            void initialise();

            void updateVertexData( ImDrawData *draw_data );

            bool preRender( Ogre::SceneManager *sm, Ogre::RenderSystem *rsys ) override;

            void getWorldTransforms( Ogre::Matrix4 *xform ) const override;
            void getRenderOperation( Ogre::RenderOperation &op ) override;

            const Ogre::LightList &getLights( void ) const override;

            void createMaterial();
            void createFontTexture();

            const Ogre::MaterialPtr &getMaterial() const override;

            /// Implementation of Ogre::SimpleRenderable
            Ogre::Real getBoundingRadius( void ) const override;

            Ogre::Real getSquaredViewDepth( const Ogre::Camera * ) const override;

            void _update();

            Ogre::Matrix4 mXform;
            Ogre::TexturePtr mFontTex;
            Ogre::MaterialPtr mMaterial;
        };

        LoadingState m_loadingState = LoadingState::Allocated;
        Ogre::RenderQueueListener *m_renderQueueListener = nullptr;
        ImGUIRenderable *mRenderable = nullptr;
        Ogre::SceneNode *m_sceneNode = nullptr;
        static u32 m_frameCount;
    };
} // namespace fb

#endif

#endif /* COMPONENTS_OVERLAY_INCLUDE_OGREIMGUIOVERLAY_H_ */
