#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/SSEffectClear.h"
#include "FBGraphicsOgre/SSEffectRenderer.h"
#include <Ogre.h>

namespace fb
{
    namespace render
    {

        SSEffectClear::SSEffectClear( SSEffectRenderer *r ) : ScreenSpaceEffect( r )
        {
            if( !Ogre::MaterialManager::getSingleton().resourceExists( "shadow_clear" ) )
            {
                return;
            }

            // Ogre::MaterialPtr mat = (Ogre::MaterialPtr)
            //	Ogre::MaterialManager::getSingleton().load("shadow_clear", "General");

            //_pass = mat->getTechnique(0)->getPass(0);
        }

        void SSEffectClear::update() const
        {
            ScreenSpaceEffect::update();
        }

        void SSEffectClear::clear() const
        {
            _renderer->renderSys()->_setViewport( _viewport );
            //_renderer->renderSys()->clearFrameBuffer( Ogre::FBT_DEPTH,
            //_viewport->getBackgroundColour(), 1.0);
        }

    }  // namespace render
}  // end namespace fb
