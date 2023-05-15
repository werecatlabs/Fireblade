#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiPropertyGrid.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiUtil.h"

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiPropertyGrid, IUIPropertyGrid );
        u32 ImGuiPropertyGrid::m_childWindowCount = 0;

        ImGuiPropertyGrid::ImGuiPropertyGrid()
        {
            m_name = "PropertyGrid_" + StringUtil::toString( m_childWindowCount++ );
        }

        ImGuiPropertyGrid::~ImGuiPropertyGrid()
        {
        }

        SmartPtr<Properties> ImGuiPropertyGrid::getProperties() const
        {
            return m_properties;
        }

        void ImGuiPropertyGrid::setProperties( SmartPtr<Properties> properties )
        {
            m_properties = properties;
        }

        void ImGuiPropertyGrid::createElement( SmartPtr<IUIElement> element )
        {
            auto propertyGrid = fb::static_pointer_cast<IUIPropertyGrid>( element );

            if( auto properties = propertyGrid->getProperties() )
            {
                auto propertyGridName = propertyGrid->getName();
                if( StringUtil::isNullOrEmpty( propertyGridName ) )
                {
                    propertyGridName = "PropertyGrid_" + StringUtil::toString( m_childWindowCount++ );
                }

                auto flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable;

                if( ImGui::BeginTable( propertyGridName.c_str(), 2, flags ) )
                {
                    auto count = 0;
                    auto propertiesArray = properties->getPropertiesAsArray();
                    for( auto &property : propertiesArray )
                    {
                        auto name = property.getName();
                        auto value = property.getValue();
                        auto type = property.getType();
                        auto typeLower = StringUtil::make_lower( type );

                        auto attributes = property.getAttributes();

                        ImGui::PushID( count );

                        // Text and Tree nodes are less high than framed widgets, using
                        // AlignTextToFramePadding() we add vertical spacing to make the tree
                        // lines equal high.
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex( 0 );
                        ImGui::AlignTextToFramePadding();
                        // bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
                        // ImGui::TableSetColumnIndex(1);
                        ImGui::Text( name.c_str() );

                        // ImGui::TableNextRow();
                        // ImGui::TableSetColumnIndex(0);
                        // ImGui::AlignTextToFramePadding();
                        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf |
                                                   ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                                   ImGuiTreeNodeFlags_Bullet;
                        // ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

                        ImGui::TableSetColumnIndex( 1 );
                        ImGui::SetNextItemWidth( -FLT_MIN );

                        if( typeLower == "enum" )
                        {
                            auto enumValue = property.getAttribute( "enum" );
                            auto enumArray = Array<String>();
                            StringUtil::parseArray( enumValue, enumArray );

                            auto display_mode = 0;
                            auto it = std::find( enumArray.begin(), enumArray.end(), value );
                            if( it != enumArray.end() )
                            {
                                display_mode = (int)std::distance( enumArray.begin(), it );
                            }

                            auto enumStr = String( "" );
                            for( auto &val : enumArray )
                            {
                                enumStr = enumStr + val + '\0';
                            }

                            if( ImGui::Combo( "Display Mode", &display_mode, enumStr.c_str() ) )
                            {
                                auto str = enumArray[display_mode];
                                properties->setProperty( name, str );

                                handlePropertyChanged( propertyGrid, name, str );
                            }
                        }
                        else if( typeLower == ( "vector2" ) || typeLower == ( "vector2f" ) ||
                                 typeLower == ( "vector2d" ) )
                        {
                            auto vector = StringUtil::parseVector2<f32>( value );

                            f32 value[2] = { 0 };

                            value[0] = vector.X();
                            value[1] = vector.Y();

                            if( ImGui::InputFloat2( name.c_str(), value ) )
                            {
                                auto newVector = Vector2F( value[0], value[1] );
                                auto str = StringUtil::toString( newVector );

                                properties->setProperty( name, newVector );

                                handlePropertyChanged( propertyGrid, name, str );
                            }
                        }
                        else if( typeLower == ( "vector3" ) || typeLower == ( "vector3f" ) ||
                                 typeLower == ( "vector3d" ) )
                        {
                            auto vector = StringUtil::parseVector3<f32>( value );

                            f32 value[3] = { 0 };

                            value[0] = vector.X();
                            value[1] = vector.Y();
                            value[2] = vector.Z();

                            if( ImGui::InputFloat3( name.c_str(), value ) )
                            {
                                auto newVector = Vector3F( value[0], value[1], value[2] );
                                auto str = StringUtil::toString( newVector );

                                properties->setProperty( name, newVector );

                                handlePropertyChanged( propertyGrid, name, str );
                            }
                        }
                        if( type == String( "colourf" ) )
                        {
                            auto colour = StringUtil::parseColourf( value );

                            float col2[4];
                            col2[0] = colour.r;
                            col2[1] = colour.g;
                            col2[2] = colour.b;
                            col2[3] = colour.a;

                            if( ImGui::ColorEdit4( name.c_str(), col2 ) )
                            {
                                auto newColour = ColourF( col2[0], col2[1], col2[2], col2[3] );
                                auto str = StringUtil::toString( newColour );

                                properties->setProperty( name, newColour );

                                handlePropertyChanged( propertyGrid, name, str );
                            }
                        }
                        else if( type == "resource" )
                        {
                            auto label = value;
                            if( StringUtil::isNullOrEmpty( label ) )
                            {
                                label = "None";
                            }

                            if( ImGui::Button( label.c_str() ) )
                            {
                                handlePropertyButtonClicked( propertyGrid, name, value );
                            }

                            // auto selectButtonLabel = String( "Select" );
                            // if( ImGui::Button( selectButtonLabel.c_str() ) )
                            //{
                            //     auto listeners = propertyGrid->getElementListeners();
                            //     for( auto listener : listeners )
                            //     {
                            //         listener->handlePropertyButtonClick( name, value );
                            //     }
                            // }
                        }
                        else if( type == "button" )
                        {
                            if( ImGui::Button( name.c_str() ) )
                            {
                                handlePropertyButtonClicked( propertyGrid, name, value );
                            }
                        }
                        else if( type == "int" )
                        {
                            auto fValue = StringUtil::parseInt( value );
                            auto v = fValue;
                            if( ImGui::InputInt( value.c_str(), &v, 1 ) )
                            {
                                if( v != fValue )
                                {
                                    properties->setProperty( name, v );

                                    auto str = StringUtil::toString( v );
                                    handlePropertyChanged( propertyGrid, name, str );
                                }
                            }
                        }
                        else if( type == "float" )
                        {
                            auto fValue = StringUtil::parseFloat( value );
                            auto v = fValue;
                            if( ImGui::InputFloat( value.c_str(), &v, 0.1f ) )
                            {
                                if( v != fValue )
                                {
                                    properties->setProperty( name, v );

                                    auto str = StringUtil::toString( v );
                                    handlePropertyChanged( propertyGrid, name, str );
                                }
                            }
                        }
                        else if( type == "bool" )
                        {
                            auto bValue = StringUtil::parseBool( value );
                            bool v = bValue;

                            if( ImGuiUtil::ToggleButton( name.c_str(), &v ) )
                            {
                                if( v != bValue )
                                {
                                    properties->setProperty( name, v );

                                    auto str = StringUtil::toString( v );
                                    handlePropertyChanged( propertyGrid, name, str );
                                }
                            }
                        }
                        else
                        {
                            constexpr int BUFFER_SIZE = 4096;
                            char buffer[BUFFER_SIZE];
                            StringUtil::toBuffer( value, buffer, BUFFER_SIZE );

                            if( ImGui::InputText( name.c_str(), (c8 *)buffer, BUFFER_SIZE, 0 ) )
                            {
                                String val = buffer;

                                if( ImGui::IsItemDeactivatedAfterEdit() )
                                //if( value != String( buffer.c_str() ) )
                                {
                                    properties->setProperty( name, val );

                                    handlePropertyChanged( propertyGrid, name, val );
                                }
                            }
                        }

                        ImGui::NextColumn();

                        ImGui::PopID();

                        ++count;
                    }

                    ImGui::EndTable();
                }
            }

            // bool p_open = false;
            // ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
            // if (!ImGui::Begin("Example: Property editor", &p_open))
            //{
            //	ImGui::End();
            //	return;
            // }
            ////IMGUI_DEMO_MARKER("Examples/Property Editor");

            ////HelpMarker(
            ////	"This example shows how you may implement a property editor using two
            /// columns.\n" /	"All objects/fields data are dummies here.\n" /	"Remember
            /// that in many simple cases, you can use ImGui::SameLine(xxx) to position\n" /
            ///"your cursor horizontally instead of using the Columns() API.");

            // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            // if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter |
            // ImGuiTableFlags_Resizable))
            //{
            //	// Iterate placeholder objects (all the same data)
            //	for (int obj_i = 0; obj_i < 4; obj_i++)
            //	{
            //		ShowPlaceholderObject("Object", obj_i);
            //		//ImGui::Separator();
            //	}
            //	ImGui::EndTable();
            // }
            // ImGui::PopStyleVar();
            // ImGui::End();

            // if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter |
            // ImGuiTableFlags_Resizable))
            //{
            //	// Iterate placeholder objects (all the same data)
            //	for (int obj_i = 0; obj_i < 4; obj_i++)
            //	{
            //		//ShowPlaceholderObject("Object", obj_i);
            //		//ImGui::Separator();

            //					// Use object uid as identifier. Most commonly you could also
            // use the object pointer as a base ID. 		ImGui::PushID(obj_i);

            //		// Text and Tree nodes are less high than framed widgets, using
            // AlignTextToFramePadding() we add vertical spacing to make the tree lines equal
            // high. 		ImGui::TableNextRow(); 		ImGui::TableSetColumnIndex(0);
            //		ImGui::AlignTextToFramePadding();
            //		//bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
            //		//ImGui::TableSetColumnIndex(1);
            //		ImGui::Text("my sailor is rich");

            //		//ImGui::TableNextRow();
            //		//ImGui::TableSetColumnIndex(0);
            //		//ImGui::AlignTextToFramePadding();
            //		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf |
            // ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
            //		//ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

            //		ImGui::TableSetColumnIndex(1);
            //		ImGui::SetNextItemWidth(-FLT_MIN);
            //
            //		static float v = 3.0f;
            //		ImGui::InputFloat("##value", &v, 1.0f);

            //		ImGui::NextColumn();

            //		ImGui::PopID();
            //	}

            //	ImGui::EndTable();
            //}
        }

        void ImGuiPropertyGrid::handlePropertyButtonClicked( SmartPtr<IUIPropertyGrid> propertyGrid,
                                                             const String &name, const String &value )
        {
            auto listeners = propertyGrid->getObjectListeners();
            for( auto listener : listeners )
            {
                auto args = Array<Parameter>();
                args.resize( 2 );

                args[0].str = name;
                args[1].str = value;

                listener->handleEvent( IEvent::Type::Object, IEvent::handlePropertyButtonClick, args,
                                       propertyGrid, propertyGrid, nullptr );
            }
        }

        void ImGuiPropertyGrid::handlePropertyChanged( SmartPtr<IUIPropertyGrid> propertyGrid,
                                                       const String &name, const String &str )
        {
            auto listeners = propertyGrid->getObjectListeners();
            for( auto listener : listeners )
            {
                auto args = Array<Parameter>();
                args.resize( 2 );

                args[0].str = name;
                args[1].str = str;

                listener->handleEvent( IEvent::Type::Object, IEvent::handlePropertyChanged, args,
                                       propertyGrid, propertyGrid, nullptr );
            }
        }

    }  // end namespace ui
}  // end namespace fb
