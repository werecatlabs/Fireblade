#ifndef ImGuiDropdown_h__
#define ImGuiDropdown_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIDropdown.h>

namespace fb
{
    namespace ui
    {
        class ImGuiDropdown : public CImGuiElement<IUIDropdown>
        {
        public:
            ImGuiDropdown();
            ~ImGuiDropdown() override;

            Array<String> getOptions() const override;
            void setOptions( const Array<String> &options ) override;

            u32 getSelectedOption() const override;
            void setSelectedOption( u32 selectedOption ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<String> m_options;
            u32 m_selectedOption = 0;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiDropdown_h__
