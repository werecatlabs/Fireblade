#ifndef ImGuiTextEntry_h__
#define ImGuiTextEntry_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITextEntry.h>

namespace fb
{
    namespace ui
    {

        class ImGuiTextEntry : public CImGuiElement<IUITextEntry>
        {
        public:
            ImGuiTextEntry();
            ~ImGuiTextEntry() override;

            void update();

            void setText( const String &text ) override;

            String getText() const override;

            void setTextSize( f32 textSize ) override;

            f32 getTextSize() const override;

            void setVerticalAlignment( u8 alignment ) override;

            u8 getVerticalAlignment() const override;

            void setHorizontalAlignment( u8 alignment ) override;

            u8 getHorizontalAlignment() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_text;
        };

    }  // namespace ui
}  // namespace fb

#endif  // ImGuiTextEntry_h__
