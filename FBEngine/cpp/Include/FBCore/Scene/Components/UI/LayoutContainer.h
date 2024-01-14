#ifndef LayoutContainer_h__
#define LayoutContainer_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** A container that can be used to layout other components.
         *  @note This component is not meant to be used directly, but rather as a base class for other
         * components.
         *  @note This component is not meant to be used directly, but rather as a base class for other
         * components.
         */
        class LayoutContainer : public Component
        {
        public:
            /** Constructor. */
            LayoutContainer() = default;

            /** Destructor. */
            ~LayoutContainer() override = default;

            /** @copydoc IComponent::updateDirty */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // namespace fb

#endif  // LayoutContainer_h__
