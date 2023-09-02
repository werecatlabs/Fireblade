#ifndef SSEffectRenderer_h__
#define SSEffectRenderer_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>

#include <FBCore/Interface/Graphics/IScreenSpaceEffectRenderer.h>
#include <OgrePrerequisites.h>
#include <OgreString.h>

namespace fb
{
    namespace render
    {

        class SSEffectRenderer : public IScreenSpaceEffectRenderer
        {
        public:
            SSEffectRenderer( Ogre::SceneManager *sceneMgr, Ogre::Viewport *viewport );
            virtual ~SSEffectRenderer();

            void ensureQuad();
            void fsquad( Ogre::Pass *p, Ogre::Viewport *vp ) const;

            Ogre::SceneManager *sceneMgr() const;
            Ogre::Viewport *viewport() const;
            Ogre::Camera *camera() const;
            Ogre::RenderSystem *renderSys() const;

            Ogre::ManualObject *quad() const;

        protected:
            Ogre::SceneManager *_sceneMgr;
            Ogre::Viewport *_viewport;
            Ogre::Camera *_camera;
            Ogre::ManualObject *_quad;

            static int m_nameExt;

            void destroyQuad();
            void buildQuad( Ogre::Camera *cam );
        };

    }  // namespace render
}  // namespace fb

#endif  // SSEffectRenderer_h__
