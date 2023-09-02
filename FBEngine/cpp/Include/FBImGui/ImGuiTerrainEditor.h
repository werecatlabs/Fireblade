#ifndef ImGuiTerrainEditor_h__
#define ImGuiTerrainEditor_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include "FBCore/Interface/UI/IUITerrainEditor.h"

namespace fb
{
    namespace ui
    {
        class ImGuiTerrainEditor : public CImGuiElement<IUITerrainEditor>
        {
        public:
            ImGuiTerrainEditor() = default;
            ImGuiTerrainEditor( int width, int height );
            ~ImGuiTerrainEditor() override = default;

            void update() override;

            void ShowEditor();

            void ShowLayermapTab();

            SmartPtr<scene::IComponent> getTerrain() const;

            void setTerrain( SmartPtr<scene::IComponent> terrainRenderer );

            FB_CLASS_REGISTER_DECL;

        private:
            void ResizeHeightmap();

            void SelectTexture( int layer );

            void ResizeLayermap();
            void ShowTreesTab();

            void ResizeTrees();

            void ShowGrassTab();
            void ResizeGrass();

            enum class Tool
            {
                Raise,
                Lower,
                Smooth,
            };

            SmartPtr<scene::TerrainSystem> m_terrainRenderer;

            int m_num_layers = 0;
            std::vector<SmartPtr<render::ITexture>> m_layer_textures;

            int m_num_trees = 0;
            Array<Vector3F> m_tree_positions;

            int m_num_grass_blades = 0;
            Array<Vector3F> m_grass_blade_positions;
            Array<f32> m_grass_blade_heights;

            int m_width = 129;
            int m_height = 129;
            std::vector<float> m_heightmap;
            Tool m_tool = Tool::Raise;
            String selected_filename;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiTerrainEditor_h__
