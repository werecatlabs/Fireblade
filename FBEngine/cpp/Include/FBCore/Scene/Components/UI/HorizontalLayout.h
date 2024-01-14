#ifndef HorizontalLayoutContainer_h__
#define HorizontalLayoutContainer_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {

        /** Horizontal layout container. */
        class HorizontalLayout : public LayoutContainer
        {
        public:
            HorizontalLayout();

            void updateTransform() override;

            /** @copydoc LayoutContainer::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc LayoutContainer::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            // Spacing between child elements
            f32 m_spacing = 5.0f;
        };

    }  // namespace scene
}  // namespace fb

#endif  // HorizontalLayoutContainer_h__
