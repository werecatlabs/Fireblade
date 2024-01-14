#ifndef VerticalLayoutContainer_h__
#define VerticalLayoutContainer_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {

        /** Vertical layout container.
         *  This container will lay out its children vertically, from top to bottom.
         *  The width of the container will be the width of the widest child, and the height
         *  will be the sum of the heights of all children plus the spacing between them.
         */
        class VerticalLayout : public LayoutContainer
        {
        public:
            VerticalLayout();
            ~VerticalLayout() override;

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

#endif  // VerticalLayoutContainer_h__
