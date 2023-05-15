#ifndef SSEffectClear_h__
#define SSEffectClear_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphicsOgre/ScreenSpaceEffect.h>

namespace fb
{
    namespace render
    {

        class SSEffectClear : public ScreenSpaceEffect
        {
        public:
            SSEffectClear( SSEffectRenderer *r );

            void update() const;
            void clear() const;

        protected:
        };

    }  // namespace render
}  // end namespace fb

#endif  // SSEffectClear_h__
