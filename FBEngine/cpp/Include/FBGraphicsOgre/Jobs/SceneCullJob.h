#ifndef SceneCullJob_h__
#define SceneCullJob_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include "FBCore/Interface/System/IJob.h"
#include <OgreSceneManager.h>
#include <FBGraphicsOgre/CameraVisibilityState.h>  //todo remove

namespace fb
{
    namespace render
    {

        class SceneCullJob : public IJob
        {
        public:
            SceneCullJob();
            ~SceneCullJob();

            void update();

            void findVisible();

            u32 getProgress() const;
            void setProgress( u32 val );

            s32 getPriority() const;
            void setPriority( s32 val );

            CameraVisibilityState *getVisibilityState() const;
            void setVisibilityState( CameraVisibilityState *val );

            Ogre::SceneManager *getSceneManager() const;
            void setSceneManager( Ogre::SceneManager *val );

            Ogre::Camera *getCamera() const;
            void setCamera( Ogre::Camera *val );

            Ogre::VisibleObjectsBoundsInfo *getVisibleBounds() const;
            void setVisibleBounds( Ogre::VisibleObjectsBoundsInfo *val );

            bool getOnlyShadowCasters() const;
            void setOnlyShadowCasters( bool val );

            Ogre::RenderQueue *getRenderQueue() const;
            void setRenderQueue( Ogre::RenderQueue *val );

        protected:
            void addVisibleObjects( Ogre::SceneNode *sceneNode, Ogre::Camera *cam,
                                    Ogre::VisibleObjectsBoundsInfo *visibleBounds,
                                    bool onlyShadowCasters, CameraVisibilityState *visibilityState );

            void _addToRenderQueue( Ogre::SceneNode *sceneNode, Ogre::Camera *cam,
                                    Ogre::RenderQueue *queue, bool onlyShadowCasters,
                                    Ogre::VisibleObjectsBoundsInfo *visibleBounds );

            CameraVisibilityState *m_visibilityState;
            Ogre::SceneManager *m_sceneManager;
            Ogre::Camera *m_camera;
            Ogre::RenderQueue *m_renderQueue;

            Ogre::VisibleObjectsBoundsInfo *m_visibleBounds;
            bool m_onlyShadowCasters;
        };

        typedef SmartPtr<SceneCullJob> SceneCullJobPtr;

    }  // namespace render
}  // end namespace fb

#endif  // SceneCullJob_h__
