#ifndef CAnimationControllerListener_h__
#define CAnimationControllerListener_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Graphics/IAnimationControllerListener.h"

namespace fb
{
    namespace render
    {

        //--------------------------------------------------------------------
        class CAnimationControllerListener : public IAnimationControllerListener
        {
        public:
            CAnimationControllerListener();
            ~CAnimationControllerListener();

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            void OnAnimationEnd( const String &name );
        };

    }  // namespace render
}  // end namespace fb

#endif  // CAnimationControllerListener_h__
