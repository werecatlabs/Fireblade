#ifndef ImGuiDropdown_h__
#define ImGuiDropdown_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIDropdown.h>

namespace fb
{
    namespace ui
    {
        class ImGuiOutputConsole
        {
        public:
            ImGuiOutputConsole() = default;

            void Log( const std::string &message )
            {
                m_log.push_back( message );
            }

            void Clear();

            void Show();

        private:
            std::vector<std::string> m_log;
        };

    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiDropdown_h__
