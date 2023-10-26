#ifndef _CAnimationTextureCtrl_H_
#define _CAnimationTextureCtrl_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Graphics/IGraphicsScene.h"
#include "FBCore/Interface/Graphics/IGraphicsObject.h"
#include "FBCore/Interface/Graphics/IAnimationTextureControl.h"
#include <OgreMaterial.h>
#include <OgreTextureUnitState.h>
#include <OgreController.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        class CAnimationTextureControl : public IAnimationTextureControl
        {
        public:
            CAnimationTextureControl();
            ~CAnimationTextureControl();

            void initialise( Ogre::TextureUnitState *textureUnit );

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            bool setAnimationEnabled( bool enabled );
            bool setAnimationEnabled( bool enabled, f32 timePosition );
            bool isAnimationEnabled();

            bool hasAnimationEnded();

            void setAnimationLoop( bool loop );
            bool isAnimationLooping() const;

            void setAnimationReversed();
            bool isAnimationReversed();

            bool setTimePosition( f32 timePosition );
            f32 getTimePosition() const;

        private:
            Ogre::TextureUnitState *m_textureUnit;
            Ogre::Controller<f32> *m_animController;

            u32 m_prevFrame;

            bool m_enabled;
        };

        typedef SmartPtr<CAnimationTextureControl> CAnimationTextureCtrlPtr;

    }  // namespace render
}  // end namespace fb

#endif
