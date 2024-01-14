#ifndef ToggleComponent_h__
#define ToggleComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {
        /** Toggle component class. */
        class Toggle : public UIComponent
        {
        public:
            /** Constructor. */
            Toggle();

            /** Destructor. */
            ~Toggle() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** Gets the toggle. */
            SmartPtr<ui::IUIToggle> getToggle() const;

            /** Sets the toggle. */
            void setToggle( SmartPtr<ui::IUIToggle> toggle );

            /** @copydoc UIComponent::getProperties
             */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc UIComponent::setProperties
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            bool isToggled() const;

            void setToggled( bool toggled );

            SmartPtr<LayoutTransform> getToggleTransform() const;

            void setToggleTransform( SmartPtr<LayoutTransform> toggleTransform );

            void updateTransform() override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @copydoc UIComponent::createUI */
            void createUI() override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            // The toggle ui object.
            SmartPtr<ui::IUIToggle> m_toggle;

            SmartPtr<LayoutTransform> m_toggleTransform;

            bool m_isToggled = true;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // ToggleComponent_h__
