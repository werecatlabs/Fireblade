#ifndef ButtonComponent_h__
#define ButtonComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {
        class Button : public UIComponent
        {
        public:
            Button();
            ~Button() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc UIComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc UIComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void updateElementState();
            void updateCaption();

            SmartPtr<ui::IUIButton> getButton() const;

            void setButton( SmartPtr<ui::IUIButton> button );

            bool isSimpleButton() const;
            void setSimpleButton( bool simpleButton );

            String getCaption() const;
            void setCaption( const String &caption );

            FB_CLASS_REGISTER_DECL;

        protected:
            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            void createUI() override;

            SmartPtr<ui::IUIButton> m_button;
            bool m_isSimpleButton = true;
            String m_caption;
            u32 m_textSize = 12;

            SmartPtr<render::ITexture> m_image;
            SmartPtr<render::ITexture> m_highlightedImage;
            SmartPtr<render::ITexture> m_pressedImage;

            ColourF m_normalColour = ColourF::White;
            ColourF m_highlightedColour = ColourF::White;
            ColourF m_pressedColour = ColourF::White;
            ColourF m_disabledColour = ColourF::White;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // ButtonComponent_h__
