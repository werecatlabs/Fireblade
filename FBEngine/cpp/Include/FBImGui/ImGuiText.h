#ifndef __ImGuiText_h__
#define __ImGuiText_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIText.h>

namespace fb
{
    namespace ui
    {

        class ImGuiText : public CImGuiElement<IUIText>
        {
        public:
            ImGuiText();
            ~ImGuiText() override;

            /** @copydoc IUIText::setText */
            void setText( const String &text ) override;

            /** @copydoc IUIText::getText */
            String getText() const override;

            /** @copydoc IUIText::setTextSize */
            void setTextSize( f32 textSize ) override;

            /** @copydoc IUIText::getTextSize */
            f32 getTextSize() const override;

            void setVerticalAlignment( u8 alignment );
            u8 getVerticalAlignment() const;

            
            void setHorizontalAlignment( u8 alignment )
            {
            }

            u8 getHorizontalAlignment() const
            {
                return 0;
            }

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Text string. */
            String m_text;
            f32 m_textSize = 10.0f;
        };

        inline String ImGuiText::getText() const
        {
            return m_text;
        }

    }  // end namespace ui
}  // end namespace fb

#endif  // CEGUIText_h__
