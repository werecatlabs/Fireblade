#ifndef Animator_h__
#define Animator_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** @brief Animator component.
         */
        class Animator : public Component
        {
        public:
            /** @brief Constructor. */
            Animator();

            /** @brief Destructor. */
            ~Animator() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // namespace fb

#endif  // Animator_h__
