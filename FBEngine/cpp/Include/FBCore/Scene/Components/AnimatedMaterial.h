#ifndef AnimatedMaterial_h__
#define AnimatedMaterial_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        /** Animated material component. */
        class AnimatedMaterial : public Component
        {
        public:
            /** Constructor. */
            AnimatedMaterial();

            /** Destructor. */
            ~AnimatedMaterial() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // AnimatedMaterial_h__
