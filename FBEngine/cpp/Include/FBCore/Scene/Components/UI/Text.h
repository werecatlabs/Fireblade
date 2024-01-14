#ifndef TextComponent_h__
#define TextComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {
        /** Text component.
         */
        class Text : public UIComponent
        {
        public:
            /** Constructor. */
            Text();

            /** Destructor. */
            ~Text() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc UIComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc UIComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc UIComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void updateElementState() override;

            SmartPtr<ui::IUIText> getTextObject() const;
            void setTextObject( SmartPtr<ui::IUIText> textObject );

            String getText() const;
            void setText( const String &text );

            FB_CLASS_REGISTER_DECL;

        protected:
            void createUI() override;

            u32 m_size = 12;
            ColourF m_colour = ColourF::Black;
            u8 m_verticalAlignment = 0;
            u8 m_horizontalAlignment = 0;
            String m_text;
            SmartPtr<ui::IUIText> m_textObject;
        };
    }  // namespace scene
}  // namespace fb

#endif  // TextComponent_h__
