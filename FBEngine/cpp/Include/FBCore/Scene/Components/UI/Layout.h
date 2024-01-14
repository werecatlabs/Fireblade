#ifndef Layout_h__
#define Layout_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        /** Layout component.
         *  This component is used to layout UI components.
         *  It is used to layout UI components in a specific way.
         *  It is also used to set the reference size for the UI components.
         */
        class Layout : public UIComponent
        {
        public:
            /** Constructor. */
            Layout();

            /** Destructor. */
            ~Layout() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** Layout object. */
            SmartPtr<ui::IUILayout> getLayout() const;

            /** Layout object. */
            void setLayout( SmartPtr<ui::IUILayout> layout );

            /** Reference size. */
            Vector2I getReferenceSize() const;

            /**  Reference size. */
            void setReferenceSize( const Vector2I &referenceSize );

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateDirty */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** Get the order of the element in the layout. */
            u32 getElementOrder( SmartPtr<UIComponent> component ) const;

            /** Get the order of the element in the layout. */
            u32 getElementOrderReversed( SmartPtr<UIComponent> component ) const;

            /** Get the order of the element in the layout. */
            s32 getZOrder( SmartPtr<IActor> obj );

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @copydoc UIComponent::handleComponentEvent */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** @copydoc UIComponent::updateVisibility */
            void updateVisibility() override;

            /** Layout object. */
            SmartPtr<ui::IUILayout> m_layout;

            /** Reference size. */
            Vector2I m_referenceSize = Vector2I( 1920, 1080 );
        };
    }  // namespace scene
}  // namespace fb

#endif  // Layout_h__
