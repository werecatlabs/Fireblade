#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabItem.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiApplication.h"

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTabItem, CImGuiElement<IUITabItem> );

        ImGuiTabItem::ImGuiTabItem()
        {
        }

        ImGuiTabItem::~ImGuiTabItem()
        {
        }

        void ImGuiTabItem::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto ui = applicationManager->getUI();
            auto uiApplication = fb::static_pointer_cast<ImGuiApplication>( ui->getApplication() );

            auto name = getLabel();
            if( ImGui::BeginTabItem( name.c_str() ) )
            {
                if( auto p = getChildren() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        uiApplication->createElement( child );
                    }
                }

                ImGui::EndTabItem();
            }
        }

        String ImGuiTabItem::getLabel() const
        {
            return m_label;
        }

        void ImGuiTabItem::setLabel( const String &label )
        {
            m_label = label;
        }
    }  // end namespace ui
}  // end namespace fb
