#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiText.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiText, CImGuiElement<IUIText> );

        ImGuiText::ImGuiText()
        {
        }

        ImGuiText::~ImGuiText()
        {
            unload( nullptr );
        }

        void ImGuiText::setText( const String &text )
        {
            m_text = text;
        }

        void ImGuiText::setTextSize( f32 textSize )
        {
            m_textSize = textSize;
        }

        f32 ImGuiText::getTextSize() const
        {
            return m_textSize;
        }

        void ImGuiText::setVerticalAlignment( u8 alignment )
        {
            
        }

        u8 ImGuiText::getVerticalAlignment() const
        {
            return 0;
        }

    }  // end namespace ui
}  // end namespace fb
