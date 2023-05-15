#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector2.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiVector2, IUIVector2 );

        ImGuiVector2::ImGuiVector2()
        {
        }

        ImGuiVector2::~ImGuiVector2()
        {
        }

        Vector2<real_Num> ImGuiVector2::getValue() const
        {
            return m_value;
        }

        void ImGuiVector2::setValue( const Vector2<real_Num> &value )
        {
            m_value = value;
        }

        String ImGuiVector2::getLabel() const
        {
            return m_label;
        }

        void ImGuiVector2::setLabel( const String &label )
        {
            m_label = label;
        }
    }  // end namespace ui
}  // end namespace fb
