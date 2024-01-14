#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiOutputConsole.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{

    void ImGuiOutputConsole::Clear()
    {
        m_log.clear();
    }

    void ImGuiOutputConsole::Show()
    {
        ImGui::Begin( "Output Console" );

        //ImGui::BeginChild( "ScrollingRegion", ImVec2( 0, -ImGui::GetItemsLineHeightWithSpacing() ),
        //                   false, ImGuiWindowFlags_HorizontalScrollbar );

        for( const std::string &message : m_log )
        {
            ImGui::Text( "%s", message.c_str() );
        }

        ImGui::EndChild();

        if( ImGui::Button( "Clear" ) )
        {
            Clear();
        }

        ImGui::End();
    }

}  // namespace fb::ui
