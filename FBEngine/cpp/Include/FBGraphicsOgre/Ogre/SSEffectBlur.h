#ifndef SSEffectBlur_h__
#define SSEffectBlur_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include "ScreenSpaceEffect.h"

namespace fb
{
    namespace render
    {

        class SSBlurEffect : public ScreenSpaceEffect
        {
        public:
            SSBlurEffect( SSEffectRenderer *r );
            virtual ~SSBlurEffect();

            void update() const;
        };

    }  // namespace render
}  // namespace fb

#endif  // SSEffectBlur_h__
