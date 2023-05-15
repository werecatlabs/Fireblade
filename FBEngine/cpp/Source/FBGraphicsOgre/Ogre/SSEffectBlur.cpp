#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Ogre/SSEffectBlur.h"
#include "FBGraphicsOgre/Ogre/SSEffectRenderer.h"
#include <Ogre.h>

namespace fb
{
    namespace render
    {

        SSBlurEffect::SSBlurEffect( SSEffectRenderer *r ) : ScreenSpaceEffect( r )
        {
            if( !Ogre::MaterialManager::getSingleton().resourceExists( "compute_blur" ) )
            {
                return;
            }

            // Ogre::MaterialPtr mat =
            //	Ogre::MaterialManager::getSingleton().load("compute_blur",
            //"General").staticCast<Ogre::Material>();

            //_pass = mat->getTechnique(0)->getPass(0);
        }

        SSBlurEffect::~SSBlurEffect()
        {
        }

        void SSBlurEffect::update() const
        {
            clear();

            if( !_active || !_pass )
                return;

            Ogre::GpuProgramParametersSharedPtr params = _pass->getFragmentProgramParameters();

            if( params->_findNamedConstantDefinition( "texelSize" ) )
                params->setNamedConstant( "texelSize", 1.0f / _texture->getWidth() );

            ScreenSpaceEffect::update();
        }

    }  // namespace render
}  // namespace fb
