#include <GameEditorPCH.h>
#include "jobs/GenerateSkyboxMaterials.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        GenerateSkyboxMaterials::GenerateSkyboxMaterials()
        {
        }

        GenerateSkyboxMaterials::~GenerateSkyboxMaterials()
        {
        }

        void GenerateSkyboxMaterials::createMaterialFromFolder( SmartPtr<IFolderExplorer> folder )
        {
            try
            {
                auto folderName = folder->getFolderName();
                auto leafFolderName = Path::getFileName( folderName );

                auto files = folder->getFiles();

                auto createMaterial = false;

                for( const auto &file : files )
                {
                    auto fileNameLower = StringUtil::make_lower( file );
                    if( fileNameLower.find( "front" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }

                    if( fileNameLower.find( "right" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }

                    if( fileNameLower.find( "back" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }

                    if( fileNameLower.find( "left" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }

                    if( fileNameLower.find( "top" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }

                    if( fileNameLower.find( "down" ) != std::string::npos )
                    {
                        createMaterial = true;
                    }
                }

                if( createMaterial )
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    auto materialManager = graphicsSystem->getMaterialManager();

                    auto fileSystem = applicationManager->getFileSystem();

                    auto resourceDatabase = applicationManager->getResourceDatabase();

                    auto materialFileName = leafFolderName + ".mat";
                    auto materialPath = applicationManager->getProjectPath() +
                                        "/Assets/Materials/Skybox/" + materialFileName;
                    auto material =
                        resourceDatabase->loadResourceByType<render::IMaterial>( materialPath );
                    if( !material )
                    {
                        auto uuid = StringUtil::getUUID();
                        auto materialResult =
                            materialManager->createOrRetrieve( uuid, materialPath, "" );
                        material = fb::dynamic_pointer_cast<render::IMaterial>( materialResult.first );
                    }

                    material->setMaterialType( render::IMaterial::MaterialType::Skybox );
                    material->load( nullptr );

                    for( const auto &file : files )
                    {
                        auto fileNameLower = StringUtil::make_lower( file );
                        auto relativePath =
                            Path::getRelativePath( applicationManager->getProjectPath(), file );

                        if( fileNameLower.find( "front" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Front );
                        }

                        if( fileNameLower.find( "right" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Right );
                        }

                        if( fileNameLower.find( "back" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Back );
                        }

                        if( fileNameLower.find( "left" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Left );
                        }

                        if( fileNameLower.find( "top" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Up );
                        }

                        if( fileNameLower.find( "down" ) != std::string::npos )
                        {
                            material->setTexture( relativePath,
                                                  (u32)render::IMaterial::SkyboxTextureTypes::Down );
                        }
                    }

                    materialManager->saveToFile( materialPath, material );
                }

                auto subFolders = folder->getSubFolders();
                for( auto subFolder : subFolders )
                {
                    createMaterialFromFolder( subFolder );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void GenerateSkyboxMaterials::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto folderPath = applicationManager->getProjectPath() + "/Assets/Allsky";
                auto folder = fileSystem->getFolderListing( folderPath );
                if( folder )
                {
                    createMaterialFromFolder( folder );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace editor
}  // namespace fb
