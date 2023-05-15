#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Core/WindowEventListener.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        
        bool WindowEventListener::windowClosing( Ogre::RenderWindow *rw )
        {
            auto engine = core::IApplicationManager::instance();
            engine->setRunning( false );
            return false;
        }

    }  // end namespace render
}  // end namespace fb