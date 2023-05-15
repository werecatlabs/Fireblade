#ifndef ImGuiLabelTextInputPair_h__
#define ImGuiLabelTextInputPair_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Interface/UI/IUILabelTextInputPair.h>
#include <FBImGui/CImGuiElement.h>

namespace fb
{
    namespace ui
    {
        class ImGuiLabelTextInputPair : public CImGuiElement<IUILabelTextInputPair>
        {
        public:
            ImGuiLabelTextInputPair();
            ~ImGuiLabelTextInputPair() override;

            String getLabel() const override;
            void setLabel( const String &label ) override;

            String getValue() const override;
            void setValue( const String &value ) override;

        protected:
            String m_label;
            String m_value;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiLabelTextInputPair_h__
