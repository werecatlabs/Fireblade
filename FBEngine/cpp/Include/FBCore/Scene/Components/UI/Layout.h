#ifndef CanvasComponent_h__
#define CanvasComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class Layout : public UIComponent
        {
        public:
            Layout();
            ~Layout() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            SmartPtr<ui::IUILayout> getLayout() const;
            void setLayout( SmartPtr<ui::IUILayout> val );

            Vector2I getReferenceSize() const;
            void setReferenceSize( const Vector2I &val );

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            u32 getElementOrder( SmartPtr<UIComponent> component ) const;

            u32 getElementOrderReversed( SmartPtr<UIComponent> component ) const;

            void childAdded( SmartPtr<IActor> child );

            void childRemoved( SmartPtr<IActor> child );

            /** @copydoc UIComponent::childAddedInHierarchy */
            virtual void childAddedInHierarchy( SmartPtr<IActor> child );

            /** @copydoc UIComponent::childRemovedInHierarchy */
            virtual void childRemovedInHierarchy( SmartPtr<IActor> child );

            u32 getZOrder() const;

            void setZOrder( u32 zOrder );

            s32 getZOrder( SmartPtr<IActor> obj );

            FB_CLASS_REGISTER_DECL;

            void levelWasLoaded( s32 level ) override;

        protected:
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            void updateVisibility() override;

            SmartPtr<ui::IUILayout> m_layout;
            Vector2I m_referenceSize = Vector2I( 1920, 1080 );

            /**
             * @brief The ui component's z order.
             */
            u32 m_zOrder = 0;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CanvasComponent_h__
