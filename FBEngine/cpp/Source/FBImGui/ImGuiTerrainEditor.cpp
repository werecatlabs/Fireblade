#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTerrainEditor.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTerrainEditor, CImGuiElement<IUITerrainEditor> );

        ImGuiTerrainEditor::ImGuiTerrainEditor( int width, int height ) :
            m_width( width ),
            m_height( height )
        {
            m_heightmap.resize( m_width * m_height );
        }

        void ImGuiTerrainEditor::update()
        {
            //ImGui::Begin( "Terrain Editor" );

            // Tabs
            if( ImGui::BeginTabBar( "Tabs" ) )
            {
                if( ImGui::BeginTabItem( "Heightmap" ) )
                {
                    ShowEditor();
                    ImGui::EndTabItem();
                }
                if( ImGui::BeginTabItem( "Layermap" ) )
                {
                    ShowLayermapTab();
                    ImGui::EndTabItem();
                }
                if( ImGui::BeginTabItem( "Trees" ) )
                {
                    ShowTreesTab();
                    ImGui::EndTabItem();
                }
                if( ImGui::BeginTabItem( "Grass" ) )
                {
                    ShowGrassTab();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            //ImGui::End();
        }

        void ImGuiTerrainEditor::ShowEditor()
        {
            // Heightmap data
            ImGui::Text( "Heightmap" );
            ImGui::InputInt( "Width", &m_width );
            ImGui::InputInt( "Height", &m_height );
            if( ImGui::Button( "Resize" ) )
                ResizeHeightmap();

            // Toolbar
            ImGui::Separator();
            ImGui::Text( "Tools" );
            if( ImGui::RadioButton( "Raise", m_tool == Tool::Raise ) )
                m_tool = Tool::Raise;
            ImGui::SameLine();
            if( ImGui::RadioButton( "Lower", m_tool == Tool::Lower ) )
                m_tool = Tool::Lower;
            ImGui::SameLine();
            if( ImGui::RadioButton( "Smooth", m_tool == Tool::Smooth ) )
                m_tool = Tool::Smooth;
        }

        void ImGuiTerrainEditor::ShowLayermapTab()
        {
            ImGui::Text( "Layermap" );

            if( ImGui::InputInt( "Num Layers", &m_num_layers ) )
            {
                ResizeLayermap();
            }

            ImGui::SameLine();
            if( ImGui::Button( "Resize" ) )
            {
                ResizeLayermap();
            }

            for( int i = 0; i < m_num_layers; i++ )
            {
                auto layer = "Layer " + StringUtil::toString( i );
                if( ImGui::CollapsingHeader( layer.c_str() ) )
                {
                    ImGui::PushID( i );
                    ImGui::Text( "Layer %d", i );
                    ImGui::SameLine();
                    if( ImGui::Button( "Select Texture" ) )
                    {
                        SelectTexture( i );
                    }

                    //ImGui::Text( "%s", m_layer_textures[i].getInfo().filename.c_str() );
                    ImGui::PopID();
                }
            }
        }

        SmartPtr<scene::IComponent> ImGuiTerrainEditor::getTerrain() const
        {
            return m_terrainRenderer;
        }

        void ImGuiTerrainEditor::setTerrain( SmartPtr<scene::IComponent> terrainRenderer )
        {
            m_terrainRenderer = terrainRenderer;

            if( m_terrainRenderer )
            {
                auto numLayers = m_terrainRenderer->calculateNumLayers();

                m_num_layers = numLayers;
                m_layer_textures.resize( m_num_layers );
            }
        }

        void ImGuiTerrainEditor::ResizeHeightmap()
        {
            m_heightmap.resize( m_width * m_height );
        }

        void ImGuiTerrainEditor::SelectTexture( int layer )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto listeners = getObjectListeners();
            for( auto listener : listeners )
            {
                Array<Parameter> arguments;
                arguments.resize( 1 );

                auto subComponents = m_terrainRenderer->getSubComponentsByType<scene::TerrainLayer>();

                if( layer < subComponents.size() )
                {
                    arguments[0].object = subComponents[layer];
                }

                listener->handleEvent( IEvent::Type::UI, ui::IUITerrainEditor::selectTerrainTextureHash,
                                       arguments, this, this, nullptr );
            }
        }

        void ImGuiTerrainEditor::ResizeLayermap()
        {
            //m_layermap.resize( m_num_layers );
            m_layer_textures.resize( m_num_layers );

            if( m_terrainRenderer )
            {
                m_terrainRenderer->setNumLayers( m_num_layers );
                m_terrainRenderer->resizeLayermap();
            }
        }

        void ImGuiTerrainEditor::ShowTreesTab()
        {
            ImGui::Text( "Trees" );

            if( ImGui::InputInt( "Num Trees", &m_num_trees ) )
            {
                ResizeTrees();
            }

            ImGui::SameLine();

            if( ImGui::Button( "Resize" ) )
            {
                ResizeTrees();
            }

            for( int i = 0; i < m_num_trees; i++ )
            {
                auto layer = "Trees " + StringUtil::toString( i );
                if( ImGui::CollapsingHeader( layer.c_str() ) )
                {
                    ImGui::PushID( i );
                    ImGui::Text( "Tree %d", i );
                    ImGui::InputFloat3( "Position", &m_tree_positions[i][0] );
                    ImGui::PopID();
                }
            }
        }

        void ImGuiTerrainEditor::ResizeTrees()
        {
            m_tree_positions.resize( m_num_trees );
        }

        void ImGuiTerrainEditor::ShowGrassTab()
        {
            ImGui::Text( "Grass" );

            if( ImGui::InputInt( "Num Blades", &m_num_grass_blades ) )
            {
                ResizeGrass();
            }

            ImGui::SameLine();
            if( ImGui::Button( "Resize" ) )
            {
                ResizeGrass();
            }

            for( int i = 0; i < m_num_grass_blades; i++ )
            {
                auto layer = "Grass " + StringUtil::toString( i );
                if( ImGui::CollapsingHeader( layer.c_str() ) )
                {
                    ImGui::PushID( i );
                    ImGui::Text( "Blade %d", i );
                    ImGui::InputFloat2( "Position", &m_grass_blade_positions[i][0] );
                    ImGui::InputFloat( "Height", &m_grass_blade_heights[i] );
                    ImGui::PopID();
                }
            }
        }

        void ImGuiTerrainEditor::ResizeGrass()
        {
            m_grass_blade_positions.resize( m_num_grass_blades );
            m_grass_blade_heights.resize( m_num_grass_blades );
        }
    }  // end namespace ui
}  // end namespace fb
