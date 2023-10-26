#include <FBGraphics/GraphicsUtil.h>
#include <FBCore/Core/Handle.h>

namespace fb
{
    namespace render
    {

        String GraphicsUtil::getTerrainTextureType( IMaterial::TerrainTextureTypes terrainTextureType )
        {
            switch( terrainTextureType )
            {
            case IMaterial::TerrainTextureTypes::Base:
                return String( "Base" );
            case IMaterial::TerrainTextureTypes::Splat:
                return String( "Splat" );
            case IMaterial::TerrainTextureTypes::Diffuse1:
                return String( "Layer1Diffuse" );
            case IMaterial::TerrainTextureTypes::Normal1:
                return String( "Layer1Normal" );
            case IMaterial::TerrainTextureTypes::Diffuse2:
                return String( "Layer2Diffuse" );
            case IMaterial::TerrainTextureTypes::Normal2:
                return String( "Layer2Normal" );
            case IMaterial::TerrainTextureTypes::Diffuse3:
                return String( "Layer3Diffuse" );
            case IMaterial::TerrainTextureTypes::Normal3:
                return String( "Layer3Normal" );
            case IMaterial::TerrainTextureTypes::Diffuse4:
                return String( "Layer4Diffuse" );
            case IMaterial::TerrainTextureTypes::Normal4:
                return String( "Layer4Normal" );
            default:
            {
            }
            break;
            }

            return String( "" );
        }

        String GraphicsUtil::getPbsTextureType( IMaterial::PbsTextureTypes pbsTextureType )
        {
            switch( pbsTextureType )
            {
            case IMaterial::PbsTextureTypes::PBSM_DIFFUSE:
                return String( "Diffuse" );
            case IMaterial::PbsTextureTypes::PBSM_NORMAL:
                return String( "Normal" );
                // case IMaterial::PbsTextureTypes::PBSM_SPECULAR:
                //	break;
            case IMaterial::PbsTextureTypes::PBSM_METALLIC:
                return String( "Metallic" );
            case IMaterial::PbsTextureTypes::PBSM_ROUGHNESS:
                return String( "Roughness" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL_WEIGHT:
                return String( "Detail Weight" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL0:
                return String( "Detail0" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL1:
                return String( "Detail1" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL2:
                return String( "Detail2" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL3:
                return String( "Detail3" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL0_NM:
                return String( "Detail0_nm" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL1_NM:
                return String( "Detail1_nm" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL2_NM:
                return String( "Detail2_nm" );
            case IMaterial::PbsTextureTypes::PBSM_DETAIL3_NM:
                return String( "Detail3_nm" );
            case IMaterial::PbsTextureTypes::PBSM_EMISSIVE:
                return String( "Emissive" );
            case IMaterial::PbsTextureTypes::PBSM_REFLECTION:
                return String( "Reflection" );
            default:
                break;
            };

            return String( "" );
        }

        String GraphicsUtil::getSkyboxTextureType( IMaterial::SkyboxTextureTypes skyboxTextureType )
        {
            switch( skyboxTextureType )
            {
            case IMaterial::SkyboxTextureTypes::Front:
                return String( "Front" );
            case IMaterial::SkyboxTextureTypes::Back:
                return String( "Back" );
            case IMaterial::SkyboxTextureTypes::Left:
                return String( "Left" );
            case IMaterial::SkyboxTextureTypes::Right:
                return String( "Right" );
            case IMaterial::SkyboxTextureTypes::Up:
                return String( "Up" );
            case IMaterial::SkyboxTextureTypes::Down:
                return String( "Down" );
            //case IMaterial::SkyboxTextureTypes::Cube:
            //    return String( "Cube" );
            default:
            {
                FB_ASSERT( false );
            }
            break;
            };

            return String( "" );
        }

        String GraphicsUtil::getSkyboxCubeTextureType(
            IMaterial::SkyboxCubeTextureTypes skyboxTextureType )
        {
            switch( skyboxTextureType )
            {
            case IMaterial::SkyboxCubeTextureTypes::Cube:
                return String( "Base" );
            default:
            {
                FB_ASSERT( false );
            }
            break;
            };

            return String( "" );
        }

        String GraphicsUtil::getMaterialType( IMaterial::MaterialType materialType )
        {
            switch( materialType )
            {
            case IMaterial::MaterialType::Standard:
                return String( "Standard" );
            case IMaterial::MaterialType::StandardSpecular:
                return String( "StandardSpecular" );
            case IMaterial::MaterialType::StandardTriPlanar:
                return String( "StandardTriPlanar" );
            case IMaterial::MaterialType::TerrainStandard:
                return String( "TerrainStandard" );
            case IMaterial::MaterialType::TerrainSpecular:
                return String( "TerrainSpecular" );
            case IMaterial::MaterialType::TerrainDiffuse:
                return String( "TerrainDiffuse" );
            case IMaterial::MaterialType::Skybox:
                return String( "Skybox" );
            case IMaterial::MaterialType::SkyboxCubemap:
                return String( "SkyboxCubemap" );
            case IMaterial::MaterialType::UI:
                return String( "UI" );
            case IMaterial::MaterialType::Custom:
                return String( "Custom" );
            default:
                break;
            }

            return String( "" );
        }

        Array<String> GraphicsUtil::getMaterialTypes()
        {
            Array<String> enumValues;
            enumValues.reserve( (size_t)IMaterial::MaterialType::Count );

            for( size_t i = 0; i < (size_t)IMaterial::MaterialType::Count; ++i )
            {
                auto eMaterialType = (IMaterial::MaterialType)i;
                auto eMaterialTypeStr = GraphicsUtil::getMaterialType( eMaterialType );
                enumValues.push_back( eMaterialTypeStr );
            }

            return enumValues;
        }

        String GraphicsUtil::getMaterialTypesString()
        {
            auto enumValues = String( "" );
            for( size_t i = 0; i < (size_t)IMaterial::MaterialType::Count; ++i )
            {
                auto eMaterialType = (IMaterial::MaterialType)i;
                auto eMaterialTypeStr = GraphicsUtil::getMaterialType( eMaterialType );
                enumValues += eMaterialTypeStr + "; ";
            }

            return enumValues;
        }

    }  // end namespace render
}  // end namespace fb
