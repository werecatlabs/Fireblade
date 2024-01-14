#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiText.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiText, CImGuiElement<IUIText> );

    ImGuiText::ImGuiText() = default;

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

    auto ImGuiText::getTextSize() const -> f32
    {
        return m_textSize;
    }

    void ImGuiText::setVerticalAlignment( u8 alignment )
    {
    }

    auto ImGuiText::getVerticalAlignment() const -> u8
    {
        return 0;
    }

    void ImGuiText::setHorizontalAlignment( u8 alignment )
    {
    }

    auto ImGuiText::getHorizontalAlignment() const -> u8
    {
        return 0;
    }

    auto ImGuiText::getText() const -> String
    {
        return m_text;
    }

}  // namespace fb::ui
