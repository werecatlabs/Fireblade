#ifndef ImGuiButton_h__
#define ImGuiButton_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIButton.h>

namespace fb
{
    namespace ui
    {
        class ImGuiButton : public CImGuiElement<IUIButton>
        {
        public:
            ImGuiButton();
            ~ImGuiButton() override;

            String getLabel() const override;
            void setLabel( const String &label ) override;

            void setTextSize( f32 textSize ) override;
            f32 getTextSize() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiButton_h__
