#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiAbout.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiAbout, CImGuiElement<IUIAbout> );

        ImGuiAbout::ImGuiAbout()
        {
        }

        ImGuiAbout::~ImGuiAbout()
        {
            unload( nullptr );
        }

        void ImGuiAbout::update()
        {
            //ImGui::Begin( "About" );

            ImGui::Text( "Fireblade" );
            ImGui::Separator();
            ImGui::Text( "Version 1.0.0" );
            ImGui::Text( "Copyright (C) 2023 Your Company" );
            ImGui::Separator();
            ImGui::Text( "This application is designed for games." );
            ImGui::Text( "For more information, visit our website at werecat" );

            //ImGui::End();
        }

    }  // namespace ui
}  // namespace fb
