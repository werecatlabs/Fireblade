#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTextEntry.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiTextEntry, CImGuiElement<IUITextEntry> );

    ImGuiTextEntry::ImGuiTextEntry() = default;

    ImGuiTextEntry::~ImGuiTextEntry() = default;

    void ImGuiTextEntry::update()
    {
        auto name = getName();
        auto value = getText();
        if( StringUtil::isNullOrEmpty( value ) )
        {
            value = "";
        }

        constexpr int BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        StringUtil::toBuffer( value, buffer, BUFFER_SIZE );

        if( ImGui::InputText( name.c_str(), buffer, BUFFER_SIZE, 0 ) )
        {
            String val = buffer;
            setText( val );
        }

        auto dropTarget = getDropTarget();
        if( dropTarget )
        {
            if( ImGui::BeginDragDropTarget() )
            {
                auto payload = ImGui::AcceptDragDropPayload( "_TREENODE" );
                if( payload )
                {
                    auto pData = static_cast<const char *>( payload->Data );
                    auto dataSize = payload->DataSize;
                    auto data = String( pData, dataSize );

                    auto menuItemId = getElementId();

                    auto args = Array<Parameter>();
                    args.resize( 3 );

                    args[0].setStr( data );
                    args[1].setStr( name );
                    args[2].setStr( value );

                    dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args, this, this,
                                             nullptr );
                }

                ImGui::EndDragDropTarget();
            }
        }

        if( ImGui::IsKeyReleased( ImGuiKey_Enter ) )
        {
            auto listeners = getObjectListeners();
            for( auto &listener : listeners )
            {
                auto args = Array<Parameter>();
                args.resize( 2 );

                args[0].str = getText();

                listener->handleEvent( IEvent::Type::Object, IEvent::handlePropertyChanged, args, this,
                                       this, nullptr );
            }
        }
    }

    void ImGuiTextEntry::setText( const String &text )
    {
        m_text = text;
    }

    auto ImGuiTextEntry::getText() const -> String
    {
        return m_text;
    }

    void ImGuiTextEntry::setTextSize( f32 textSize )
    {
    }

    auto ImGuiTextEntry::getTextSize() const -> f32
    {
        return 0.0f;
    }

    void ImGuiTextEntry::setVerticalAlignment( u8 alignment )
    {
    }

    auto ImGuiTextEntry::getVerticalAlignment() const -> u8
    {
        return 0;
    }

    void ImGuiTextEntry::setHorizontalAlignment( u8 alignment )
    {
    }

    auto ImGuiTextEntry::getHorizontalAlignment() const -> u8
    {
        return 0;
    }
}  // namespace fb::ui
