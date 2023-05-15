#ifndef ImGuiLabelCheckboxPair_h__
#define ImGuiLabelCheckboxPair_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Interface/UI/IUILabelCheckboxPair.h>
#include <FBImGui/CImGuiElement.h>

namespace fb
{
    namespace ui
    {
        class ImGuiLabelCheckboxPair : public CImGuiElement<IUILabelCheckboxPair>
        {
        public:
            ImGuiLabelCheckboxPair();
            ~ImGuiLabelCheckboxPair() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            String getLabel() const override;
            void setLabel( const String &val ) override;

            bool getValue() const override;
            void setValue( bool val ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
            bool m_value = true;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiLabelCheckboxPair_h__
