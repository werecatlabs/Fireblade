#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector4.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiVector4, IUIVector4 );

        ImGuiVector4::ImGuiVector4()
        {
        }

        ImGuiVector4::~ImGuiVector4()
        {
        }

        Vector4<real_Num> ImGuiVector4::getValue() const
        {
            return m_value;
        }

        void ImGuiVector4::setValue( const Vector4<real_Num> &value )
        {
            m_value = value;
        }

        //---------------------------------------------
        String ImGuiVector4::getLabel() const
        {
            return m_label;
        }

        //---------------------------------------------
        void ImGuiVector4::setLabel( const String &label )
        {
            m_label = label;
        }
    } // end namespace ui
}     // end namespace fb
