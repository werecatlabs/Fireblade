#ifndef _OGRECONSOLE_H_
#define _OGRECONSOLE_H_

#include <FBCore/Interface/System/IConsole.h>
#include <OgreOverlaySystem.h>

namespace fb
{

    class OgreConsole : public IConsole
    {
    public:
        OgreConsole();
        ~OgreConsole();

        void initialise();

        void setVisible( bool visible );

    private:
        Ogre::Overlay *m_pOverlay;
    };

}  // end namespace fb

#endif
