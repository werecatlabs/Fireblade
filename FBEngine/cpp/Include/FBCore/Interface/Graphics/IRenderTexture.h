#ifndef IRenderTexture_h__
#define IRenderTexture_h__

#include <FBCore/Interface/Graphics/IRenderTarget.h>

namespace fb
{
    namespace render
    {

        /** Interface to manage a render texture. */
        class IRenderTexture : public IRenderTarget
        {
        public:
            /** Virtual destructor. */
            ~IRenderTexture() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // namespace fb

#endif  // IRenderTexture_h__
