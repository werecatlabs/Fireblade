#ifndef Animation_h__
#define Animation_h__

#include <FBCore/Scene/Components/SubComponent.h>

namespace fb
{
    namespace scene
    {
        /** @brief Animation subcomponent.
         */
        class Animation : public SubComponent
        {
        public:
            /** @brief Constructor. */
            Animation();

            /** @brief Destructor. */
            ~Animation() override;

            /** @copydoc SubComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc SubComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            f32 getLength() const;

            void setLength( f32 length );

            FB_CLASS_REGISTER_DECL;

        protected:
            f32 m_length = 0.0f;
        };
    }  // namespace scene
}  // namespace fb

#endif  // Animation_h__
