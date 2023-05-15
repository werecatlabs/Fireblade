#ifndef ShadowFrameListener_h__
#define ShadowFrameListener_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <OgreFrameListener.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        class ShadowFrameListener : public Ogre::FrameListener
        {
        public:
            ShadowFrameListener( ScreenSpaceEffect *effectClear, Ogre::TexturePtr tex,
                                 u32 nextUpdateInterval );
            ~ShadowFrameListener() override;

            bool frameStarted( const Ogre::FrameEvent &evt ) override;

        protected:
            Ogre::TexturePtr m_tex;
            Ogre::PixelBox *box;
            float *data;
            u32 m_nextUpdate;
            u32 m_nextUpdateInterval;
            ScreenSpaceEffect *m_effectClear;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // ShadowFrameListener_h__
