#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector4.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb::ui, ImGuiVector4, IUIVector4 );

    ImGuiVector4::ImGuiVector4() = default;

    ImGuiVector4::~ImGuiVector4() = default;

    auto ImGuiVector4::getValue() const -> Vector4<real_Num>
    {
        return m_value;
    }

    void ImGuiVector4::setValue( const Vector4<real_Num> &value )
    {
        m_value = value;
    }

    auto ImGuiVector4::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiVector4::setLabel( const String &label )
    {
        m_label = label;
    }

}  // namespace fb::ui
