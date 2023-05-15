#ifndef ImGuiInputManager_h__
#define ImGuiInputManager_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIInputManager.h>

namespace fb
{
    namespace ui
    {
        class ImGuiInputManager : public CImGuiElement<IUIInputManager>
        {
        public:
            ImGuiInputManager();
            ~ImGuiInputManager() override;

            void update() override;
            
        protected:
            void CaptureInput();
            void ChangeKeyBinding( int &key );
            void ChangeJoyBinding( int &button );

            //void ShowJoystickInformation( OIS::JoyStick *joystick );

            std::map<u32, std::string> m_key_map;
            u32 *m_change_key = nullptr;
            bool m_changing_key = false;

            std::map<int, std::string> m_joy_map;
            int *m_change_joy = nullptr;
            bool m_changing_joy = false;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiInputManager_h__
