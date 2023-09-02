#ifndef RenderSystemListener_h__
#define RenderSystemListener_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>

#include <OgreRenderSystem.h>

namespace fb
{
    namespace render
    {

        class RenderSystemListener : public Ogre::RenderSystem::Listener
        {
        public:
            void eventOccurred( const Ogre::String &eventName,
                                const Ogre::NameValuePairList *parameters = nullptr ) override
            {
            }
        };

    }  // end namespace render
}  // end namespace fb

#endif  // RenderSystemListener_h__
