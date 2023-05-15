#ifndef CAnimationControllerListener_h__
#define CAnimationControllerListener_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IAnimationControllerListener.h>

namespace fb
{
    namespace render
    {

        class CAnimationControllerListener : public IAnimationControllerListener
        {
        public:
            CAnimationControllerListener();
            ~CAnimationControllerListener() override;

            void update() override;

            void OnAnimationEnd( const String &name ) override;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CAnimationControllerListener_h__
