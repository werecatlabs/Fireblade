#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector2.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb::ui, ImGuiVector2, IUIVector2 );

    ImGuiVector2::ImGuiVector2() = default;

    ImGuiVector2::~ImGuiVector2() = default;

    auto ImGuiVector2::getValue() const -> Vector2<real_Num>
    {
        return m_value;
    }

    void ImGuiVector2::setValue( const Vector2<real_Num> &value )
    {
        m_value = value;
    }

    auto ImGuiVector2::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiVector2::setLabel( const String &label )
    {
        m_label = label;
    }
}  // namespace fb::ui
