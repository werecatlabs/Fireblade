#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiText.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiText, CImGuiElement<IUIText> );

        ImGuiText::ImGuiText()
        {
        }

        ImGuiText::~ImGuiText()
        {
            unload( nullptr );
        }

        void ImGuiText::update()
        {
            auto str = getText();
            if( StringUtil::isNullOrEmpty( str ) )
            {
                str = "";
            }

            ImGui::Text( str.c_str() );
        }

        void ImGuiText::setText( const String &text )
        {
            m_text = text;
        }

        void ImGuiText::setTextSize( f32 textSize )
        {
            m_textSize = textSize;
        }

        f32 ImGuiText::getTextSize() const
        {
            return m_textSize;
        }

        void ImGuiText::setVerticalAlignment( u8 alignment )
        {
        }

        u8 ImGuiText::getVerticalAlignment() const
        {
            return 0;
        }

        void ImGuiText::setHorizontalAlignment( u8 alignment )
        {
        }

        u8 ImGuiText::getHorizontalAlignment() const
        {
            return 0;
        }

        String ImGuiText::getText() const
        {
            return m_text;
        }

    }  // end namespace ui
}  // end namespace fb
