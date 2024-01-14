#ifndef ToggleGroup_h__
#define ToggleGroup_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class ToggleGroup : public UIComponent
        {
        public:
            ToggleGroup();
            ~ToggleGroup() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            void createUI() override;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // ToggleGroup_h__
