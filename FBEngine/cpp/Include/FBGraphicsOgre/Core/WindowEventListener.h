#ifndef WindowEventListener_h__
#define WindowEventListener_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <OgreWindowEventUtilities.h>

namespace fb
{
    namespace render
    {

        class WindowEventListener : public Ogre::WindowEventListener
        {
        public:
            WindowEventListener() = default;
            ~WindowEventListener() override = default;

            bool windowClosing( Ogre::RenderWindow *rw ) override;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // WindowEventListener_h__
