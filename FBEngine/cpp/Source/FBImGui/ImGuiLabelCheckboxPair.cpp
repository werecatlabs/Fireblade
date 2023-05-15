#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiLabelCheckboxPair.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiLabelCheckboxPair, CImGuiElement<IUILabelCheckboxPair> );

        ImGuiLabelCheckboxPair::ImGuiLabelCheckboxPair()
        {
        }

        ImGuiLabelCheckboxPair::~ImGuiLabelCheckboxPair()
        {
            unload( nullptr );
        }

        void ImGuiLabelCheckboxPair::load( SmartPtr<ISharedObject> data )
        {
        }

        void ImGuiLabelCheckboxPair::unload( SmartPtr<ISharedObject> data )
        {
        }

        String ImGuiLabelCheckboxPair::getLabel() const
        {
            return m_label;
        }

        void ImGuiLabelCheckboxPair::setLabel( const String &val )
        {
            m_label = val;
        }

        bool ImGuiLabelCheckboxPair::getValue() const
        {
            return m_value;
        }

        void ImGuiLabelCheckboxPair::setValue( bool val )
        {
            m_value = val;
        }
    } // end namespace ui
}     // end namespace fb
