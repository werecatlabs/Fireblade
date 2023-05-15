#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiVector3.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiVector3, IUIVector3 );

        //---------------------------------------------
        ImGuiVector3::ImGuiVector3()
        {
        }

        //---------------------------------------------
        ImGuiVector3::~ImGuiVector3()
        {
        }

        //---------------------------------------------
        Vector3<real_Num> ImGuiVector3::getValue() const
        {
            return m_value;
        }

        //---------------------------------------------
        void ImGuiVector3::setValue( const Vector3<real_Num> &value )
        {
            m_value = value;
        }

        //---------------------------------------------
        String ImGuiVector3::getLabel() const
        {
            return m_label;
        }

        //---------------------------------------------
        void ImGuiVector3::setLabel( const String &label )
        {
            m_label = label;
        }
    } // end namespace ui
}     // end namespace fb
