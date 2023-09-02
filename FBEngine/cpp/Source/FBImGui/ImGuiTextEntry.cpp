#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTextEntry.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTextEntry, CImGuiElement<IUITextEntry> );

        ImGuiTextEntry::ImGuiTextEntry()
        {
        }

        ImGuiTextEntry::~ImGuiTextEntry()
        {
        }

        void ImGuiTextEntry::update()
        {
            auto name = getName();
            auto value = getText();
            if(StringUtil::isNullOrEmpty( value ))
            {
                value = "";
            }

            constexpr int BUFFER_SIZE = 4096;
            char buffer[BUFFER_SIZE];
            StringUtil::toBuffer( value, buffer, BUFFER_SIZE );

            if(ImGui::InputText( name.c_str(), buffer, BUFFER_SIZE, 0 ))
            {
                String val = buffer;
                setText( val );
            }

            if(ImGui::IsKeyReleased( ImGuiKey_Enter ))
            {
                auto listeners = getObjectListeners();
                for(auto &listener : listeners)
                {
                    auto args = Array<Parameter>();
                    args.resize( 2 );

                    args[0].str = getText();

                    listener->handleEvent( IEvent::Type::Object, IEvent::handlePropertyChanged, args,
                                           this, this, nullptr );
                }
            }
        }

        void ImGuiTextEntry::setText( const String &text )
        {
            m_text = text;
        }

        String ImGuiTextEntry::getText() const
        {
            return m_text;
        }

        void ImGuiTextEntry::setTextSize( f32 textSize )
        {
        }

        f32 ImGuiTextEntry::getTextSize() const
        {
            return 0.0f;
        }

        void ImGuiTextEntry::setVerticalAlignment( u8 alignment )
        {
        }

        u8 ImGuiTextEntry::getVerticalAlignment() const
        {
            return 0;
        }

        void ImGuiTextEntry::setHorizontalAlignment( u8 alignment )
        {
        }

        u8 ImGuiTextEntry::getHorizontalAlignment() const
        {
            return 0;
        }
    } // namespace ui
}     // namespace fb
