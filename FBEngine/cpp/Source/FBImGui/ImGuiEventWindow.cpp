#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiEventWindow.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiEventWindow, ImGuiWindowT<IUIEventWindow> );

        ImGuiEventWindow::ImGuiEventWindow()
        {
        }

        ImGuiEventWindow::~ImGuiEventWindow()
        {
        }

        Array<SmartPtr<IEvent>> ImGuiEventWindow::getEvents() const
        {
            return m_events;
        }

        void ImGuiEventWindow::setEvents( const Array<SmartPtr<IEvent>> &events )
        {
            m_events = events;
        }

        void ImGuiEventWindow::update()
        {
            auto eventName = String("Event");
            ImGui::Text( eventName.c_str() );

            for( auto e : m_events )
            {
                auto componentEvent = fb::dynamic_pointer_cast<scene::IComponentEvent>( e );
                auto listeners = componentEvent->getListeners();
                for(auto listener : listeners)
                {
                    constexpr int size = 1024;
                    char buf[size];

                    auto component = listener->getComponent();
                    StringUtil::toBuffer( "", buf, size );

                    auto objectLabel = String( "Object" );
                    if (ImGui::InputText( objectLabel.c_str(), buf, IM_ARRAYSIZE( buf ) ))
                    {
                        //listener->setComponent(  );
                    }

                    auto objectFunctionLabel = String( "Function" );

                    auto functionName = listener->getFunction();
                    StringUtil::toBuffer( objectFunctionLabel, buf, size );

                    if (ImGui::InputText( objectFunctionLabel.c_str(), buf, IM_ARRAYSIZE( buf ) ))
                    {
                        listener->setFunction( functionName );
                    }
                }
            }
        }

    }  // end namespace ui
}  // end namespace fb
