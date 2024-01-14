#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiButton.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiButton, CImGuiElement<IUIButton> );

    ImGuiButton::ImGuiButton() = default;

    ImGuiButton::~ImGuiButton() = default;

    void ImGuiButton::update()
    {
        auto label = getLabel();
        if( StringUtil::isNullOrEmpty( label ) )
        {
            label = "Untitled";
        }

        if( ImGui::Button( label.c_str() ) )
        {
            if( auto parent = getParent() )
            {
                if( parent->isDerived<IUIToolbar>() )
                {
                    auto toolbar = fb::static_pointer_cast<IUIToolbar>( parent );
                    FB_ASSERT( toolbar );

                    auto listeners = toolbar->getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        listener->handleEvent( IEvent::Type::UI, IEvent::handleSelection, args, toolbar,
                                               this, nullptr );
                    }
                }
                else
                {
                    auto listeners = getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        listener->handleEvent( IEvent::Type::UI, IEvent::handleSelection, args, this,
                                               this, nullptr );
                    }
                }
            }
        }
    }

    auto ImGuiButton::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiButton::setLabel( const String &label )
    {
        m_label = label;
    }

    void ImGuiButton::setTextSize( f32 textSize )
    {
    }

    auto ImGuiButton::getTextSize() const -> f32
    {
        return 0.0f;
    }
}  // namespace fb::ui
