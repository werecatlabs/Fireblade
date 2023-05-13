#ifndef GraphicsUtil_h__
#define GraphicsUtil_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        class GraphicsUtil
        {
        public:
            static String getTerrainTextureType( IMaterial::TerrainTextureTypes terrainTextureType );

            static String getPbsTextureType( IMaterial::PbsTextureTypes pbsTextureType );
            static String getSkyboxTextureType( IMaterial::SkyboxTextureTypes skyboxTextureType );
            static String getSkyboxCubeTextureType(
                IMaterial::SkyboxCubeTextureTypes skyboxTextureType );

            static String getMaterialType( IMaterial::MaterialType materialType );

            static Array<String> getMaterialTypes();

            static String getMaterialTypesString();
        };

    }  // end namespace render
}  // end namespace fb

#endif  // GraphicsUtil_h__
