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

            /** @copydoc UIComponent::getProperties
             */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc UIComponent::setProperties
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            bool isToggled() const;

            void setToggled( bool toggled );

            void updateColour();

            SmartPtr<LayoutTransform> getToggleTransform() const;

            void setToggleTransform( SmartPtr<LayoutTransform> toggleTransform );

            void updateTransform() override;

            ColourF getToggledColour() const;

            void setToggledColour( const ColourF &toggledColour );

            ColourF getUntoggledColour() const;

            void setUntoggledColour( const ColourF &untoggledColour );

            FB_CLASS_REGISTER_DECL;

        protected:
            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            auto handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType;

            SmartPtr<LayoutTransform> m_toggleTransform;

            ColourF m_toggledColour = ColourF::Green;
            ColourF m_untoggledColour = ColourF::White;

            bool m_isToggled = true;
        };
    } // namespace scene
}     // end namespace fb

#endif  // ToggleComponent_h__
