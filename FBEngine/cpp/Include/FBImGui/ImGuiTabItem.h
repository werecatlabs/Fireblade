#ifndef ImGuiTabItem_h__
#define ImGuiTabItem_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITabItem.h>

namespace fb
{
    namespace ui
    {

        class ImGuiTabItem : public CImGuiElement<IUITabItem>
        {
        public:
            ImGuiTabItem();
            ~ImGuiTabItem() override;

            void update();

            String getLabel() const;

            void setLabel( const String &label );

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiTabItem_h__
