#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector3.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb::ui, ImGuiVector3, IUIVector3 );

    ImGuiVector3::ImGuiVector3() = default;

    ImGuiVector3::~ImGuiVector3() = default;

    auto ImGuiVector3::getValue() const -> Vector3<real_Num>
    {
        return m_value;
    }

    void ImGuiVector3::setValue( const Vector3<real_Num> &value )
    {
        m_value = value;
    }

    auto ImGuiVector3::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiVector3::setLabel( const String &label )
    {
        m_label = label;
    }
}  // namespace fb::ui
