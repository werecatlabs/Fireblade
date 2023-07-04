#include <GameEditorPCH.h>
#include "jobs/JobCreatePackage.h"
#include "commands/AddNewScriptCmd.h"
#include "ui/ProjectWindow.h"
#include "ui/ApplicationFrame.h"
#include "editor/EditorManager.h"
#include <FBApplication/FBApplication.h>
#include <FBCore/FBCore.h>
#include <fstream>

#include "FBFileSystem/ZipUtil.h"

namespace fb
{
    namespace editor
    {

        JobCreatePackage::JobCreatePackage()
        {
        }

        JobCreatePackage::~JobCreatePackage()
        {
        }

        void JobCreatePackage::execute()
        {
            using namespace fb;
            FB_DEBUG_TRACE;

#if defined FB_PLATFORM_WIN32
            FB_LOG( "Starting packaging" );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto fileDialog = fileSystem->openFileDialog();

            fileDialog->setDialogMode( INativeFileDialog::DialogMode::Select );

            auto result = fileDialog->openDialog();
            if( result == INativeFileDialog::Result::Dialog_Okay )
            {
                auto dst = StringUtilW::toUTF8to16( fileDialog->getFilePath() );
                auto projectPath = applicationManager->getProjectPath();
                auto rootPath = PathW::getWorkingDirectory();
                auto projectPathW = StringUtil::toUTF8to16( projectPath );

                // PhysicsManagerPtr physicsManager = getPhysicsManager();
                // if (physicsManager)
                //{
                //	physicsManager->createCollisionCache();
                // }

                //Array<StringW> datafilesFiles =
                //    PathW::getFilesAsAbsolutePaths( rootPath + L"/media/datafiles/", true );
                //// ZipUtil::createZipFileFromPath(L"../Media/materials.zip", materialFiles);
                //// ZipUtil::createObfuscatedZipFileFromPath(L"../Media/media.zip", L"../../media/datafiles/",
                //// datafilesFiles);
                //ZipUtil::createObfuscatedZipFileFromPath( mediaPath + L"/media.zip", datafilesFiles );

                //Array<StringW> materialFiles =
                //    PathW::getFilesAsAbsolutePaths( rootPath + L"/media/materials/" );
                //// ZipUtil::createZipFileFromPath(L"../Media/materials.zip", materialFiles);
                //ZipUtil::createObfuscatedZipFileFromPath( mediaPath + L"/materials.zip", materialFiles );

                //Array<StringW> overlaysFiles =
                //    PathW::getFilesAsAbsolutePaths( rootPath + L"/media/overlays/" );
                //// ZipUtil::createZipFileFromPath(L"../Media/textures.zip", overlaysFiles);
                //ZipUtil::createObfuscatedZipFileFromPath( mediaPath + L"/overlays.zip", overlaysFiles );

                //Array<StringW> particleFiles =
                //    PathW::getFilesAsAbsolutePaths( rootPath + L"/media/particle/" );
                //// ZipUtil::createZipFileFromPath(L"../Media/particle.zip", particleFiles);
                //ZipUtil::createObfuscatedZipFileFromPath( mediaPath + L"/particle.zip", particleFiles );

                //auto textureFiles = fileSystem->getFiles( projectPath + "/Assets/", ".png" );

                auto scenesPath = projectPathW + L"/Assets/";
                auto sceneExt = StringW( L".fbscene" );
                auto sceneFiles = PathW::getFilesAsAbsolutePaths( scenesPath, sceneExt, true );
                // ZipUtil::createZipFileFromPath(L"../Media/textures.zip", textureFiles);
                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/scenes.fbpak", sceneFiles );

                auto meshFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/Cache/",
                                                                 StringW( L".fbmeshbin" ), true );
                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/mesh.fbpak", meshFiles );

                auto meshSourceFilesLowerCase = PathW::getFilesAsAbsolutePaths(
                    projectPathW + L"/Assets/", StringW( L".fbx" ), true );
                auto meshSourceFilesUppercase = PathW::getFilesAsAbsolutePaths(
                    projectPathW + L"/Assets/", StringW( L".FBX" ), true );

                Array<StringW> meshSourceFiles;
                meshSourceFiles.reserve( meshSourceFilesLowerCase.size() +
                                         meshSourceFilesUppercase.size() );
                std::copy( meshSourceFilesLowerCase.begin(), meshSourceFilesLowerCase.end(),
                           std::back_inserter( meshSourceFiles ) );
                std::copy( meshSourceFilesUppercase.begin(), meshSourceFilesUppercase.end(),
                           std::back_inserter( meshSourceFiles ) );

                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/meshSource.fbpak", meshSourceFiles );

                auto settingsFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/SettingsCache/",
                                                                     StringW( L".meshdata" ), true );
                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/settings.fbpak", settingsFiles );

                auto materialFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/Assets/",
                                                                     StringW( L".mat" ), true );
                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/material.fbpak", materialFiles );

                auto pythonScriptFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/Assets/",
                                                                         StringW( L".py" ), true );
                auto luaScriptFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/Assets/",
                                                                      StringW( L".lua" ), true );

                Array<StringW> scriptFiles;
                scriptFiles.reserve( pythonScriptFiles.size() + luaScriptFiles.size() );
                std::copy( pythonScriptFiles.begin(), pythonScriptFiles.end(),
                           std::back_inserter( scriptFiles ) );
                std::copy( luaScriptFiles.begin(), luaScriptFiles.end(),
                           std::back_inserter( scriptFiles ) );

                ZipUtil::createObfuscatedZipFileFromPath( dst + L"/scripts.fbpak", scriptFiles );

                bool packageTextures = false;
                if( packageTextures )
                {
                    auto textureFiles = PathW::getFilesAsAbsolutePaths( projectPathW + L"/Assets/",
                                                                        StringW( L".png" ), true );

                    auto pakCount = 0;
                    auto numTextures = 128;
                    for( size_t i = 0; i < textureFiles.size(); i += numTextures )
                    {
                        auto texs = Array<StringW>( textureFiles.begin() + i,
                                                    textureFiles.begin() + ( i + numTextures ) );

                        auto pakCountStr = StringUtil::toStringW( StringUtil::toString( pakCount ) );
                        ZipUtil::createObfuscatedZipFileFromPath(
                            dst + L"/textures_" + pakCountStr + L".fbpak", texs );
                        pakCount++;
                    }
                }

                // Array<StringW> uiFiles = FileSystemW::getFilesAsAbsolutePaths(mediaPath + L"/ui/",
                // true);
                ////ZipUtil::createZipFileFromPath(L"../Media/ui.zip", uiFiles);
                // ZipUtil::createObfuscatedZipFileFromPath(mediaPath + L"/ui.zip", mediaPath + L"/ui/",
                // uiFiles);

                // Array<StringW> collisionFiles = FileSystemW::getFilesAsAbsolutePaths(rootPath +
                // L"/media/collision/", true);
                ////ZipUtil::createZipFileFromPath(L"../Media/materials.zip", materialFiles);
                ////ZipUtil::createObfuscatedZipFileFromPath(L"../Media/media.zip", L"../../media/datafiles/",
                /// datafilesFiles);
                // ZipUtil::createObfuscatedZipFileFromPath(mediaPath + L"/collision.zip", rootPath +
                // L"/media/collision/", collisionFiles);

                FB_LOG( "Finished packaging" );
            }
#endif
        }

        StringW JobCreatePackage::getDst() const
        {
            return m_dst;
        }

        void JobCreatePackage::setDst( const StringW &dst )
        {
            m_dst = dst;
        }

    }  // namespace editor
}  // namespace fb
