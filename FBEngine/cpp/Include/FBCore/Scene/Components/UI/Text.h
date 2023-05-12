#ifndef TextComponent_h__
#define TextComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class Text : public UIComponent
        {
        public:
            Text();
            ~Text() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::start */
            void play();

            /** @copydoc IComponent::edit */
            void edit();

            SmartPtr<ui::IUIText> getTextObject() const;
            void setTextObject( SmartPtr<ui::IUIText> textObject );

            String getText() const;
            void setText( const String &text );

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc UIComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc UIComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void updateElementState();

            FB_CLASS_REGISTER_DECL;

        protected:
            void createUI();

            u32 m_size = 12;
            ColourF m_colour = ColourF::Black;
            String m_text;
            SmartPtr<ui::IUIText> m_textObject;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // TextComponent_h__
