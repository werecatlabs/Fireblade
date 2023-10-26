#ifndef ShadowFrameListener_h__
#define ShadowFrameListener_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <OgreFrameListener.h>
//#include <OgreTexture.h>

namespace fb
{
    namespace render
    {

        class ScreenSpaceEffect;

        class ShadowFrameListener : public Ogre::FrameListener
        {
        public:
            // ShadowFrameListener(ScreenSpaceEffect* effectClear, Ogre::TexturePtr tex, u32
            // nextUpdateInterval);
            ~ShadowFrameListener();

            bool frameStarted( const Ogre::FrameEvent &evt );

        protected:
            // Ogre::TexturePtr m_tex;
            // Ogre::PixelBox* box;
            float *data;
            u32 m_nextUpdate;
            u32 m_nextUpdateInterval;
            ScreenSpaceEffect *m_effectClear;
        };

    }  // namespace render
}  // end namespace fb

#endif  // ShadowFrameListener_h__
