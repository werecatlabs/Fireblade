#include <GameEditorPCH.h>
#include "jobs/GenerateSkybox.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    GenerateSkybox::GenerateSkybox() = default;

    GenerateSkybox::~GenerateSkybox() = default;

    void GenerateSkybox::createMaterialFromFolder( SmartPtr<IFolderExplorer> folder )
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
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto materialManager = graphicsSystem->getMaterialManager();

                auto fileSystem = applicationManager->getFileSystem();

                auto resourceDatabase = applicationManager->getResourceDatabase();

                auto materialFileName = leafFolderName + ".mat";
                auto materialPath = applicationManager->getProjectPath() + "/Assets/Materials/Skybox/" +
                                    materialFileName;
                auto material = resourceDatabase->loadResourceByType<render::IMaterial>( materialPath );
                if( !material )
                {
                    auto uuid = StringUtil::getUUID();
                    auto materialResult = materialManager->createOrRetrieve( uuid, materialPath, "" );
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
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Front ) );
                    }

                    if( fileNameLower.find( "back" ) != std::string::npos )
                    {
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Back ) );
                    }

                    if( fileNameLower.find( "right" ) != std::string::npos )
                    {
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Right ) );
                    }

                    if( fileNameLower.find( "left" ) != std::string::npos )
                    {
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Left ) );
                    }

                    if( fileNameLower.find( "top" ) != std::string::npos )
                    {
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Up ) );
                    }

                    if( fileNameLower.find( "down" ) != std::string::npos )
                    {
                        material->setTexture(
                            relativePath,
                            static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Down ) );
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

    void GenerateSkybox::setupSkyboxFromFolder( SmartPtr<IFolderExplorer> folder )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto materialManager = graphicsSystem->getMaterialManager();

            auto fileSystem = applicationManager->getFileSystem();

            auto resourceDatabase = applicationManager->getResourceDatabase();
            auto selectionManager = applicationManager->getSelectionManager();

            auto folderName = folder->getFolderName();
            auto leafFolderName = Path::getFileName( folderName );

            auto files = folder->getFiles();

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                    if( auto skybox = actor->getComponent<scene::Skybox>() )
                    {
                        for( const auto &file : files )
                        {
                            auto fileNameLower = StringUtil::make_lower( file );
                            auto relativePath =
                                Path::getRelativePath( applicationManager->getProjectPath(), file );

                            if( fileNameLower.find( "front" ) != std::string::npos )
                            {
                                skybox->setTextureByName(
                                    relativePath,
                                    static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Front ) );
                            }

                            if( fileNameLower.find( "back" ) != std::string::npos )
                            {
                                skybox->setTextureByName(
                                    relativePath,
                                    static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Back ) );
                            }

                            if( !skybox->getSwapLeftRight() )
                            {
                                if( fileNameLower.find( "right" ) != std::string::npos )
                                {
                                    skybox->setTextureByName(
                                        relativePath,
                                        static_cast<u32>(
                                            render::IMaterial::SkyboxTextureTypes::Right ) );
                                }

                                if( fileNameLower.find( "left" ) != std::string::npos )
                                {
                                    skybox->setTextureByName(
                                        relativePath,
                                        static_cast<u32>(
                                            render::IMaterial::SkyboxTextureTypes::Left ) );
                                }
                            }
                            else
                            {
                                if( fileNameLower.find( "right" ) != std::string::npos )
                                {
                                    skybox->setTextureByName(
                                        relativePath,
                                        static_cast<u32>(
                                            render::IMaterial::SkyboxTextureTypes::Left ) );
                                }

                                if( fileNameLower.find( "left" ) != std::string::npos )
                                {
                                    skybox->setTextureByName(
                                        relativePath,
                                        static_cast<u32>(
                                            render::IMaterial::SkyboxTextureTypes::Right ) );
                                }
                            }

                            if( fileNameLower.find( "top" ) != std::string::npos ||
                                fileNameLower.find( "up" ) != std::string::npos )
                            {
                                skybox->setTextureByName(
                                    relativePath,
                                    static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Up ) );
                            }

                            if( fileNameLower.find( "down" ) != std::string::npos )
                            {
                                skybox->setTextureByName(
                                    relativePath,
                                    static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Down ) );
                            }
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void GenerateSkybox::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto projectPath = applicationManager->getProjectPath();
            if( !fileSystem->isExistingFolder( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto folderPath = getFolderPath();
            if( StringUtil::isNullOrEmpty( folderPath ) )
            {
                if( auto fileDialog = fileSystem->openFileDialog() )
                {
                    fileDialog->setDialogMode( INativeFileDialog::DialogMode::Select );
                    fileDialog->setFileExtension( ".fbscene" );
                    fileDialog->setFilePath( projectPath );

                    auto result = fileDialog->openDialog();
                    if( result == INativeFileDialog::Result::Dialog_Okay )
                    {
                        auto filePath = fileDialog->getFilePath();
                        folderPath = Path::getRelativePath( projectPath, filePath );
                        setFolderPath( folderPath );
                    }
                }
            }

            if( !StringUtil::isNullOrEmpty( folderPath ) )
            {
                auto folder = fileSystem->getFolderListing( folderPath );
                if( !folder )
                {
                    auto folderAbsolutePath = Path::getAbsolutePath( projectPath, folderPath );
                    folder = fileSystem->getFolderListing( folderAbsolutePath );
                }

                if( folder )
                {
                    setupSkyboxFromFolder( folder );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto GenerateSkybox::getFolderPath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_folderPath;
    }

    void GenerateSkybox::setFolderPath( const String &folderPath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_folderPath = folderPath;
    }

}  // namespace fb::editor
