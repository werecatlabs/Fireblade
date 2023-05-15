#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiInputManager.h>
#include <FBCore/FBCore.h>
#include <imgui.h>
#include <map>

namespace fb
{
    namespace ui
    {

        ImGuiInputManager::ImGuiInputManager()
        {
        }

        ImGuiInputManager::~ImGuiInputManager()
        {
        }

        void ImGuiInputManager::CaptureInput()
        {
            //m_keyboard->capture();
            //m_joystick->capture();
            //if( m_changing_key )
            //{
            //    for( int i = 0; i < 256; i++ )
            //    {
            //        if( m_keyboard->isKeyDown( static_cast<OIS::KeyCode>( i ) ) )
            //        {
            //            *m_change_key = static_cast<OIS::KeyCode>( i );
            //            m_changing_key = false;
            //            break;
            //        }
            //    }
            //}
        }

        void ImGuiInputManager::ChangeKeyBinding( int &key )
        {
        }

        void ImGuiInputManager::ChangeJoyBinding( int &button )
        {
        }

        void ImGuiInputManager::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto inputDeviceManager = applicationManager->getInputDeviceManager();

            //ImGui::Begin( "Input Manager" );

            if( ImGui::BeginTabBar( "Tabs" ) )
            {
                if( ImGui::BeginTabItem( "Bindings" ) )
                {
                    // Key bindings
                    ImGui::Text( "Key Bindings" );
                    for( auto &[key, name] : m_key_map )
                    {
                        ImGui::PushID( &key );
                        ImGui::Text( "%s:", name.c_str() );
                        ImGui::SameLine();
                        if( ImGui::Button( "Change" ) )
                        {
                            //    ChangeKeyBinding( key );
                        }

                        ImGui::PopID();
                    }

                    // Joystick bindings
                    ImGui::Text( "Joystick Bindings" );
                    for( auto &[button, name] : m_joy_map )
                    {
                        ImGui::PushID( &button );
                        ImGui::Text( "%s:", name.c_str() );
                        ImGui::SameLine();
                        if( ImGui::Button( "Change" ) )
                        {
                            //ChangeJoyBinding( button );
                        }
                        ImGui::PopID();
                    }

                    ImGui::EndTabItem();
                }

                if( ImGui::BeginTabItem( "JoyStick" ) )
                {
                    //auto joysticks = inputDeviceManager->getJoySticks();
                    //for( auto joystick : joysticks )
                    //{
                    //    //ShowJoystickInformation( joystick );
                    //}

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ///ImGui::End();
        }

        /*
        void ImGuiInputManager::ShowJoystickInformation( OIS::JoyStick *joystick )
        {
            //ImGui::Begin( "Joystick Information" );

            ImGui::Text( "Name: %s", joystick->vendor().c_str() );
            ImGui::Text( "Axes: %d", joystick->getNumberOfComponents( OIS::OIS_Axis ) );
            ImGui::Text( "Buttons: %d", joystick->getNumberOfComponents( OIS::OIS_Button ) );

            OIS::JoyStickState state = joystick->getJoyStickState();

            ImGui::Text( "Axis Values:" );
            for( int i = 0; i < state.mAxes.size(); ++i )
            {
                ImGui::Text( "  %d: %.2f", i, state.mAxes[i].abs );
            }

            ImGui::Text( "Button States:" );
            for( int i = 0; i < state.mButtons.size(); ++i )
            {
                ImGui::Text( "  %d: %s", i, state.mButtons[i] ? "Pressed" : "Released" );
            }

            //ImGui::End();
        }
        */

    }  // end namespace ui
}  // end namespace fb
