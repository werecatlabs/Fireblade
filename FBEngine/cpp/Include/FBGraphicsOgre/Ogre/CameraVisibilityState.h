#ifndef __CameraVisibilityState_h__
#define __CameraVisibilityState_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Core/HashMap.h>
#include <FBCore/Core/Pool.h>
#include <OgreHardwareOcclusionQuery.h>
#include <OgreVector.h>
#include <OgrePlane.h>
#include <list>

namespace fb
{
    namespace render
    {

        class GraphicsObjectVisibility
        {
        public:
            GraphicsObjectVisibility();

            void update();

            Ogre::HardwareOcclusionQuery *m_occlusionQuery;
            u32 m_nextUpdate;
            u32 m_pixelDrawn;
            bool m_isCulled;
            bool m_queryObject;
        };

        typedef HashMap<u32, GraphicsObjectVisibility *> VisibilityStates;

        class RenderQueueVisibilityState
        {
        public:
            Ogre::SceneNode *m_sceneNode;
        };

        typedef std::list<RenderQueueVisibilityState *> RenderQueueVisibilityStates;

        class CameraVisibilityState
        {
        public:
            enum
            {
                STATE_READY,
                STATE_QUEUED,
            };

            CameraVisibilityState();
            ~CameraVisibilityState();

            inline bool getUpdateCulling() const
            {
                return m_updateCulling;
            }
            inline void setUpdateCulling( bool val )
            {
                m_updateCulling = val;
            }

            bool isVisible( const Ogre::AxisAlignedBox &bound ) const;

            Ogre::Plane *m_planes;

            Ogre::Vector3 m_lastPosition;
            Ogre::Quaternion m_lastOrientation;

            bool m_updateCulling;

            u32 m_nextUpdate;

            typedef HashMap<u32, VisibilityStates> CameraSettingsStates;
            typedef HashMap<u32, CameraSettingsStates> PassStates;
            typedef HashMap<u32, PassStates> CameraViewportStates;
            CameraViewportStates m_states;

            typedef HashMap<u32, Ogre::RenderQueue *> RenderQueues;
            typedef HashMap<u32, RenderQueues> RenderQueueStates;
            RenderQueueStates m_renderQueueStates;

            Ogre::RenderQueue *m_renderQueue;

            inline Ogre::RenderQueue *getRenderQueue() const
            {
                return m_renderQueue;
            }
            void setRenderQueue( Ogre::RenderQueue *val )
            {
                m_renderQueue = val;
            }

            Pool<RenderQueueVisibilityState> m_renderQueueVisibilityStatePool;
            RenderQueueVisibilityStates m_renderQueueVisibilityStates;

            atomic_s32 m_state;

            s32 getState() const
            {
                return m_state;
            }
            void setState( s32 val )
            {
                m_state = val;
            }
        };

    }  // end namespace render
}  // end namespace fb

#endif  // CameraVisibilityState_h__
