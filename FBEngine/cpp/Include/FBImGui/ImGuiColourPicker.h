#ifndef ImGuiColourPicker_h__
#define ImGuiColourPicker_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIColourPicker.h>

namespace fb
{
    namespace ui
    {

        class ImGuiColourPicker : public CImGuiElement<IUIColourPicker>
        {
        public:
            ImGuiColourPicker();
            ~ImGuiColourPicker() override;

            void update() override;

            FB_CLASS_REGISTER_DECL;

            void setGradient( const ColourF &startColour, const ColourF &endColour ) override;

            Pair<ColourF, ColourF> getGradient() const override;

            void setColourFormat( ColourFormat format ) override;

            ColourFormat getColourFormat() const override;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiColourPicker_h__
