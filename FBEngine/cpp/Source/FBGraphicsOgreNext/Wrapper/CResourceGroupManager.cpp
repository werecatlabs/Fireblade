#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CResourceGroupManager.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/ResourceGroupHelper.h>
#include <FBGraphicsOgreNext/FBOgreDataStream.h>
#include "FBGraphicsOgreNext/FBOgreArchive.h"
#include <FBCore/FBCore.h>
#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreMaterialManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreGpuProgramManager.h>
#include <OgreConfigFile.h>
#include <OgreRoot.h>
#include <OgreHlms.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlit.h>
#include <OgreHlmsDiskCache.h>
#include <OgreTextureGpuManager.h>
#include <OgreArchiveManager.h>
#include <OgreArchive.h>
#include <OgreLogManager.h>
#include <Terra/Hlms/OgreHlmsTerra.h>
#include <fstream>

#include "Ogre/OgreHlmsColibri.h"

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CResourceGroupManager, IResourceGroupManager );

    const u32 CResourceGroupManager::RGMID_UNLOADRESOURCEGROUP = 0;  // = StateManagerTBB::getNewId();

    CResourceGroupManager::CResourceGroupManager()
    {
        /*StateManagerPtr& stateManager = fb::IApplicationManager::instance()->getStateManager();
        m_stateListener = new StateListenerAdapter<CResourceGroupManager>(this);
        m_stateListener->setTaskNotifyId(TI_GraphicsTaskId);
        stateManager->addStateListener(m_stateListener);
        stateManager->registerObserver(this, m_stateListener);*/
    }

    CResourceGroupManager::~CResourceGroupManager()
    {
        /*if(m_stateListener)
        {
            StateManagerPtr& stateManager = fb::IApplicationManager::instance()->getStateManager();
            m_stateListener->setListener(NULL);
            stateManager->removeStateListener(m_stateListener);
            stateManager->unregisterObserver(this, m_stateListener);
            FB_SAFE_REMOVE(m_stateListener);
        }*/
    }

    void CResourceGroupManager::loadResourceFile()
    {
        try
        {
            using namespace Ogre;

            auto resourceGroupManager = ResourceGroupManager::getSingletonPtr();

            ConfigFile cf;

            try
            {
                cf.load( "resources2.cfg" );
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }

            // Go through all sections & settings in the file
            ConfigFile::SectionIterator seci = cf.getSectionIterator();

            String secName, typeName, archName;
            while( seci.hasMoreElements() )
            {
                secName = seci.peekNextKey();
                ConfigFile::SettingsMultiMap *settings = seci.getNext();
                ConfigFile::SettingsMultiMap::iterator i;
                for( i = settings->begin(); i != settings->end(); ++i )
                {
                    try
                    {
                        typeName = i->first;
                        archName = i->second;

                        resourceGroupManager->addResourceLocation( archName, typeName, secName, false );
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription() );
        }
    }

    void CResourceGroupManager::baseRegisterHlms()
    {
        try
        {
            using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // Note:  macBundlePath works for iOS too. It's misnamed.
            const String resourcePath = Path::macBundlePath() + "/Contents/Resources/";
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
            const String resourcePath = Ogre::macBundlePath() + "/";
#else
            String resourcePath = "";
#endif

            ConfigFile cf;

            try
            {
                cf.load( resourcePath + "resources2.cfg" );
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }

            auto applicationManager = core::ApplicationManager::instance();

            auto workingDirectory = Path::getWorkingDirectory();
            auto mediaPath = applicationManager->getRenderMediaPath();
            //auto rootHlmsFolder = Path::lexically_normal( workingDirectory, mediaPath );
            auto rootHlmsFolder = mediaPath;

            // At this point rootHlmsFolder should be a valid path to the Hlms data folder
            auto root = Root::getSingletonPtr();
            auto hlmsManager = root->getHlmsManager();

            Ogre::HlmsColibri *hlmsColibri = nullptr;
            HlmsUnlit *hlmsUnlit = nullptr;
            HlmsPbs *hlmsPbs = nullptr;

            // For retrieval of the paths to the different folders needed
            String mainFolderPath;
            StringVector libraryFoldersPaths;
            StringVector::const_iterator libraryFolderPathIt;
            StringVector::const_iterator libraryFolderPathEn;

            ArchiveManager &archiveManager = ArchiveManager::getSingleton();

            {
                //Create & Register HlmsColibri
                //Get the path to all the subdirectories used by HlmsColibri
                Ogre::HlmsColibri::getDefaultPaths( mainFolderPath, libraryFoldersPaths );

                auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
                //Ogre::Archive *archiveUnlit =
                //     archiveManager.load( rootHlmsFolder + mainFolderPath, "FileSystem", true );
                auto archiveUnlit = new FBOgreArchive( archivePath, "FileSystem" );

                Ogre::ArchiveVec archiveUnlitLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while( libraryFolderPathIt != libraryFolderPathEn )
                {
                    auto archiveLibraryPath =
                        Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );
                    //Ogre::Archive *archiveLibrary = archiveManager.load(
                    //    rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
                    auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
                    archiveUnlitLibraryFolders.push_back( archiveLibrary );
                    ++libraryFolderPathIt;
                }

                //Create and register the unlit Hlms
                hlmsColibri = OGRE_NEW Ogre::HlmsColibri( archiveUnlit, &archiveUnlitLibraryFolders,
                                                          Ogre::HlmsTypes::HLMS_USER0, "ui" );
                hlmsManager->registerHlms( hlmsColibri );
            }

            {
                // Create & Register HlmsUnlit
                // Get the path to all the subdirectories used by HlmsUnlit
                HlmsUnlit::getDefaultPaths( mainFolderPath, libraryFoldersPaths );

                auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
                //Archive *archiveUnlit =
                //    archiveManager.load( archivePath, "FileSystem", true );
                auto archiveUnlit = new FBOgreArchive( archivePath, "FileSystem" );

                ArchiveVec archiveUnlitLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while( libraryFolderPathIt != libraryFolderPathEn )
                {
                    auto archiveLibraryPath =
                        Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );
                    //Archive *archiveLibrary = archiveManager.load(
                    //    rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
                    auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
                    archiveUnlitLibraryFolders.push_back( archiveLibrary );
                    ++libraryFolderPathIt;
                }

                // Create and register the unlit Hlms
                hlmsUnlit = new HlmsUnlit( archiveUnlit, &archiveUnlitLibraryFolders );
                hlmsManager->registerHlms( hlmsUnlit );
            }

            {
                //Create & Register HlmsPbs
                // Do the same for HlmsPbs:
                HlmsPbs::getDefaultPaths( mainFolderPath, libraryFoldersPaths );

                auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
                //auto archivePbs = archiveManager.load( archivePath, "FileSystem", true );
                auto archivePbs = new FBOgreArchive( archivePath, "FileSystem" );

                // Get the library archive(s)
                ArchiveVec archivePbsLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while( libraryFolderPathIt != libraryFolderPathEn )
                {
                    auto archiveLibraryPath =
                        Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );
                    //Archive *archiveLibrary =
                    //    archiveManager.load( archiveLibraryPath, "FileSystem", true );

                    auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
                    archivePbsLibraryFolders.push_back( archiveLibrary );
                    ++libraryFolderPathIt;
                }

                // Create and register
                hlmsPbs = new HlmsPbs( archivePbs, &archivePbsLibraryFolders );
                hlmsManager->registerHlms( hlmsPbs );
            }

            RenderSystem *renderSystem = Root::getSingletonPtr()->getRenderSystem();
            if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
            {
                // Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
                // and below to avoid saturating AMD's discard limit (8MB) or
                // saturate the PCIE bus in some low end machines.
                bool supportsNoOverwriteOnTextureBuffers;
                renderSystem->getCustomAttribute( "MapNoOverwriteOnDynamicBufferSRV",
                                                  &supportsNoOverwriteOnTextureBuffers );

                if( !supportsNoOverwriteOnTextureBuffers )
                {
                    hlmsPbs->setTextureBufferDefaultSize( 512 * 1024 );
                    hlmsUnlit->setTextureBufferDefaultSize( 512 * 1024 );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto isAndroid() -> bool
    {
        return false;
    }

    auto getMediaReadArchiveType() -> const char *
    {
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
        return "FileSystem";
#else
        return "APKFileSystem";
#endif
    }

    void CResourceGroupManager::registerHlms()
    {
        try
        {
            auto mRoot = Ogre::Root::getSingletonPtr();
            Ogre::String mResourcePath;

            Ogre::ConfigFile cf;

            try
            {
                cf.load( "resources2.cfg" );
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }

            auto applicationManager = core::ApplicationManager::instance();

            auto workingDirectory = Path::getWorkingDirectory();
            auto mediaPath = applicationManager->getRenderMediaPath();
            //auto rootHlmsFolder = Path::lexically_normal( workingDirectory, mediaPath );
            auto rootHlmsFolder = mediaPath;

#if FB_FINAL
            rootHlmsFolder = String( "" );
#endif

            Ogre::RenderSystem *renderSystem = mRoot->getRenderSystem();

            Ogre::String shaderSyntax = "GLSL";
            if( renderSystem->getName() == "OpenGL ES 2.x Rendering Subsystem" )
            {
                shaderSyntax = "GLSLES";
            }
            if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
            {
                shaderSyntax = "HLSL";
            }
            else if( renderSystem->getName() == "Metal Rendering Subsystem" )
            {
                shaderSyntax = "Metal";
            }

            Ogre::String mainFolderPath;
            Ogre::StringVector libraryFoldersPaths;
            Ogre::StringVector::const_iterator libraryFolderPathIt;
            Ogre::StringVector::const_iterator libraryFolderPathEn;

            Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();

            Ogre::HlmsTerra *hlmsTerra = nullptr;
            Ogre::HlmsManager *hlmsManager = mRoot->getHlmsManager();

            {
                // Create & Register HlmsTerra
                // Get the path to all the subdirectories used by HlmsTerra
                Ogre::HlmsTerra::getDefaultPaths( mainFolderPath, libraryFoldersPaths );

                auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
                //Ogre::Archive *archiveTerra = archiveManager.load( archivePath, "FileSystem", true );
                auto archiveTerra = new FBOgreArchive( archivePath, "FileSystem" );

                Ogre::ArchiveVec archiveTerraLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while( libraryFolderPathIt != libraryFolderPathEn )
                {
                    auto archiveLibraryPath =
                        Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );

                    //Ogre::Archive *archiveLibrary = archiveManager.load(
                    //    rootHlmsFolder + *libraryFolderPathIt, getMediaReadArchiveType(), true );

                    auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
                    archiveTerraLibraryFolders.push_back( archiveLibrary );
                    ++libraryFolderPathIt;
                }

                // Create and register the terra Hlms
                hlmsTerra = OGRE_NEW Ogre::HlmsTerra( archiveTerra, &archiveTerraLibraryFolders );
                hlmsManager->registerHlms( hlmsTerra );
            }

            // Add Terra's piece files that customize the PBS implementation.
            // These pieces are coded so that they will be activated when
            // we set the HlmsPbsTerraShadows listener and there's an active Terra
            //(see Tutorial_TerrainGameState::createScene01)
            Ogre::Hlms *hlmsPbs = hlmsManager->getHlms( Ogre::HLMS_PBS );
            Ogre::Archive *archivePbs = hlmsPbs->getDataFolder();
            Ogre::ArchiveVec libraryPbs = hlmsPbs->getPiecesLibraryAsArchiveVec();
            //libraryPbs.push_back( Ogre::ArchiveManager::getSingletonPtr()->load(
            //    rootHlmsFolder + "Hlms/Terra/" + shaderSyntax + "/PbsTerraShadows", "FileSystem",
            //    true ) );

            auto archiveLibrary = new FBOgreArchive(
                rootHlmsFolder + "Hlms/Terra/" + shaderSyntax + "/PbsTerraShadows", "FileSystem" );
            libraryPbs.push_back( archiveLibrary );

            hlmsPbs->reloadFrom( archivePbs, &libraryPbs );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    //        void CResourceGroupManager::baseRegisterHlms()
    //        {
    //            try
    //            {
    //                using namespace Ogre;
    //
    //                auto applicationManager = core::ApplicationManager::instance();
    //
    //                auto workingDirectory = Path::getWorkingDirectory();
    //                auto mediaPath = applicationManager->getMediaPath();
    //                //auto rootHlmsFolder = Path::lexically_normal( workingDirectory, mediaPath );
    //                auto rootHlmsFolder = mediaPath;
    //
    //#if FB_FINAL
    //                rootHlmsFolder = String( "" );
    //#endif
    //
    //                auto root = Root::getSingletonPtr();
    //                auto hlmsManager = root->getHlmsManager();
    //
    //                // At this point rootHlmsFolder should be a valid path to the Hlms data folder
    //
    //                HlmsUnlit *hlmsUnlit = nullptr;
    //                HlmsPbs *hlmsPbs = nullptr;
    //
    //                // For retrieval of the paths to the different folders needed
    //                String mainFolderPath;
    //                StringVector libraryFoldersPaths;
    //                StringVector::const_iterator libraryFolderPathIt;
    //                StringVector::const_iterator libraryFolderPathEn;
    //
    //                ArchiveManager &archiveManager = ArchiveManager::getSingleton();
    //
    //                {
    //                    // Create & Register HlmsUnlit
    //                    // Get the path to all the subdirectories used by HlmsUnlit
    //                    HlmsUnlit::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
    //
    //                    auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
    //                    //auto archiveUnlit = archiveManager.load( archivePath, "FileSystem", true );
    //                    auto archiveUnlit = new FBOgreArchive( archivePath, "FileSystem" );
    //
    //                    ArchiveVec archiveUnlitLibraryFolders;
    //                    libraryFolderPathIt = libraryFoldersPaths.begin();
    //                    libraryFolderPathEn = libraryFoldersPaths.end();
    //
    //                    while( libraryFolderPathIt != libraryFolderPathEn )
    //                    {
    //                        auto archiveLibraryPath =
    //                            Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );
    //                        //Archive *archiveLibrary =
    //                        //    archiveManager.load( archiveLibraryPath, "FileSystem", true );
    //
    //                        auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
    //                        archiveUnlitLibraryFolders.push_back( archiveLibrary );
    //                        ++libraryFolderPathIt;
    //                    }
    //
    //                    // Create and register the unlit Hlms
    //                    hlmsUnlit = new HlmsUnlit( archiveUnlit, &archiveUnlitLibraryFolders );
    //                    hlmsManager->registerHlms( hlmsUnlit );
    //                }
    //
    //                {
    //                    // Create & Register HlmsPbs
    //                    // Do the same for HlmsPbs:
    //                    HlmsPbs::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
    //
    //                    auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
    //                    //auto archivePbs = archiveManager.load( archivePath, "FileSystem", true );
    //                    auto archivePbs = new FBOgreArchive( archivePath, "FileSystem" );
    //
    //                    // Get the library archive(s)
    //                    ArchiveVec archivePbsLibraryFolders;
    //                    libraryFolderPathIt = libraryFoldersPaths.begin();
    //                    libraryFolderPathEn = libraryFoldersPaths.end();
    //                    while( libraryFolderPathIt != libraryFolderPathEn )
    //                    {
    //                        auto archiveLibraryPath =
    //                            Path::lexically_normal( rootHlmsFolder, *libraryFolderPathIt );
    //                        //Archive *archiveLibrary =
    //                        //    archiveManager.load( archiveLibraryPath, "FileSystem", true );
    //
    //                        auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
    //                        archivePbsLibraryFolders.push_back( archiveLibrary );
    //                        ++libraryFolderPathIt;
    //                    }
    //
    //                    // Create and register
    //                    hlmsPbs = new HlmsPbs( archivePbs, &archivePbsLibraryFolders );
    //                    hlmsManager->registerHlms( hlmsPbs );
    //                }
    //
    //                auto renderSystem = root->getRenderSystem();
    //                if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
    //                {
    //                    // Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
    //                    // and below to avoid saturating AMD's discard limit (8MB) or
    //                    // saturate the PCIE bus in some low end machines.
    //                    bool supportsNoOverwriteOnTextureBuffers;
    //                    renderSystem->getCustomAttribute( "MapNoOverwriteOnDynamicBufferSRV",
    //                                                      &supportsNoOverwriteOnTextureBuffers );
    //
    //                    if( !supportsNoOverwriteOnTextureBuffers )
    //                    {
    //                        hlmsPbs->setTextureBufferDefaultSize( 512 * 1024 );
    //                        hlmsUnlit->setTextureBufferDefaultSize( 512 * 1024 );
    //                    }
    //                }
    //            }
    //            catch( std::exception &e )
    //            {
    //                FB_LOG_EXCEPTION( e );
    //            }
    //        }
    //
    //        void CResourceGroupManager::registerHlms( void )
    //        {
    //            try
    //            {
    //                auto applicationManager = core::ApplicationManager::instance();
    //
    //                auto rootHlmsFolder = applicationManager->getMediaPath();
    //                FB_ASSERT( !StringUtil::isNullOrEmpty( rootHlmsFolder ) );  // media path not set
    //
    //                if( StringUtil::isNullOrEmpty( rootHlmsFolder ) )
    //                {
    //                    FB_LOG( "Warning: media path empty." );
    //                }
    //
    //#if FB_FINAL
    //                rootHlmsFolder = String( "" );
    //#endif
    //
    //                auto root = Ogre::Root::getSingletonPtr();
    //                auto renderSystem = root->getRenderSystem();
    //
    //                auto shaderSyntax = Ogre::String( "GLSL" );
    //                auto renderSystemName = renderSystem->getName();
    //
    //                if( renderSystemName == "OpenGL ES 2.x Rendering Subsystem" )
    //                {
    //                    shaderSyntax = "GLSLES";
    //                }
    //                if( renderSystemName == "Direct3D11 Rendering Subsystem" )
    //                {
    //                    shaderSyntax = "HLSL";
    //                }
    //                else if( renderSystemName == "Metal Rendering Subsystem" )
    //                {
    //                    shaderSyntax = "Metal";
    //                }
    //
    //                Ogre::String mainFolderPath;
    //                Ogre::StringVector libraryFoldersPaths;
    //                Ogre::StringVector::const_iterator libraryFolderPathIt;
    //                Ogre::StringVector::const_iterator libraryFolderPathEn;
    //
    //                Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();
    //
    //                Ogre::HlmsTerra *hlmsTerra = nullptr;
    //                Ogre::HlmsManager *hlmsManager = root->getHlmsManager();
    //
    //                {
    //                    // Create & Register HlmsTerra
    //                    // Get the path to all the subdirectories used by HlmsTerra
    //                    Ogre::HlmsTerra::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
    //
    //                    auto archivePath = Path::lexically_normal( rootHlmsFolder, mainFolderPath );
    //                    //Ogre::Archive *archiveTerra = archiveManager.load( archivePath, "FileSystem", true );
    //                    auto archiveTerra = new FBOgreArchive( archivePath, "FileSystem" );
    //
    //                    Ogre::ArchiveVec archiveTerraLibraryFolders;
    //                    libraryFolderPathIt = libraryFoldersPaths.begin();
    //                    libraryFolderPathEn = libraryFoldersPaths.end();
    //                    while( libraryFolderPathIt != libraryFolderPathEn )
    //                    {
    //                        auto archiveLibraryPath =
    //                            Path::lexically_normal( rootHlmsFolder, mainFolderPath );
    //                        //Ogre::Archive *archiveLibrary =
    //                        //   archiveManager.load( archiveLibraryPath, "FileSystem", true );
    //
    //                        auto archiveLibrary = new FBOgreArchive( archiveLibraryPath, "FileSystem" );
    //                        archiveTerraLibraryFolders.push_back( archiveLibrary );
    //                        ++libraryFolderPathIt;
    //                    }
    //
    //                    // Create and register the terra Hlms
    //                    hlmsTerra = OGRE_NEW Ogre::HlmsTerra( archiveTerra, &archiveTerraLibraryFolders );
    //                    hlmsManager->registerHlms( hlmsTerra );
    //                }
    //
    //                // Add Terra's piece files that customize the PBS implementation.
    //                // These pieces are coded so that they will be activated when
    //                // we set the HlmsPbsTerraShadows listener and there's an active Terra
    //                //(see Tutorial_TerrainGameState::createScene01)
    //                Ogre::Hlms *hlmsPbs = hlmsManager->getHlms( Ogre::HLMS_PBS );
    //                Ogre::Archive *archivePbs = hlmsPbs->getDataFolder();
    //                Ogre::ArchiveVec libraryPbs = hlmsPbs->getPiecesLibraryAsArchiveVec();
    //                //libraryPbs.push_back( Ogre::ArchiveManager::getSingletonPtr()->load(
    //                //    rootHlmsFolder + "Hlms/Terra/" + shaderSyntax + "/PbsTerraShadows", "FileSystem",
    //                //    true ) );
    //
    //                auto archiveLibrary = new FBOgreArchive(
    //                    rootHlmsFolder + "Hlms/Terra/" + shaderSyntax + "/PbsTerraShadows", "FileSystem" );
    //                libraryPbs.push_back( archiveLibrary );
    //
    //                hlmsPbs->reloadFrom( archivePbs, &libraryPbs );
    //            }
    //            catch( std::exception &e )
    //            {
    //                FB_LOG_EXCEPTION( e );
    //            }
    //        }

    void CResourceGroupManager::setupResources()
    {
        try
        {
            auto root = Ogre::Root::getSingletonPtr();
            auto hlmsManager = root->getHlmsManager();
            FB_ASSERT( hlmsManager );

            // Initialize resources for LTC area lights and accurate specular reflections (IBL)
            auto hlms = hlmsManager->getHlms( Ogre::HLMS_PBS );
            // FB_ASSERT(hlms);

            if( hlms )
            {
                OGRE_ASSERT_HIGH( dynamic_cast<Ogre::HlmsPbs *>( hlms ) );

                auto hlmsPbs = static_cast<Ogre::HlmsPbs *>( hlms );

                try
                {
                    FB_ASSERT( hlmsPbs );
                    hlmsPbs->loadLtcMatrix();
                }
                catch( Ogre::FileNotFoundException &e )
                {
                    FB_LOG_ERROR( e.getFullDescription() );
                }
            }
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription() );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::loadTextureCache()
    {
#if !OGRE_NO_JSON
        Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();
        Ogre::Archive *rwAccessFolderArchive =
            archiveManager.load( mWriteAccessFolder, "FileSystem", true );

        auto m_root = Ogre::Root::getSingletonPtr();

        try
        {
            const Ogre::String filename = "textureMetadataCache.json";
            if( rwAccessFolderArchive->exists( filename ) )
            {
                Ogre::DataStreamPtr stream = rwAccessFolderArchive->open( filename );
                Array<char> fileData;
                fileData.resize( stream->size() + 1 );
                if( !fileData.empty() )
                {
                    stream->read( &fileData[0], stream->size() );
                    // Add null terminator just in case (to prevent bad input)
                    fileData.back() = '\0';
                    Ogre::TextureGpuManager *textureManager =
                        m_root->getRenderSystem()->getTextureGpuManager();
                    textureManager->importTextureMetadataCache( stream->getName(), &fileData[0], false );
                }
            }
            else
            {
                Ogre::LogManager::getSingleton().logMessage( "[INFO] Texture cache not found at " +
                                                             mWriteAccessFolder +
                                                             "/textureMetadataCache.json" );
            }
        }
        catch( Ogre::Exception &e )
        {
            Ogre::LogManager::getSingleton().logMessage( e.getFullDescription() );
        }

        archiveManager.unload( rwAccessFolderArchive );
#endif
    }

    void CResourceGroupManager::saveTextureCache()
    {
        auto root = Ogre::Root::getSingletonPtr();

        if( root->getRenderSystem() )
        {
            Ogre::TextureGpuManager *textureManager = root->getRenderSystem()->getTextureGpuManager();
            if( textureManager )
            {
                Ogre::String jsonString;
                textureManager->exportTextureMetadataCache( jsonString );
                const Ogre::String path = mWriteAccessFolder + "/textureMetadataCache.json";
                std::ofstream file( path.c_str(), std::ios::binary | std::ios::out );
                if( file.is_open() )
                {
                    file.write( jsonString.c_str(), static_cast<std::streamsize>( jsonString.size() ) );
                }
                file.close();
            }
        }
    }

    void CResourceGroupManager::loadHlmsDiskCache()
    {
        if( !mUseMicrocodeCache && !mUseHlmsDiskCache )
        {
            return;
        }

        auto root = Ogre::Root::getSingletonPtr();

        Ogre::HlmsManager *hlmsManager = root->getHlmsManager();
        Ogre::HlmsDiskCache diskCache( hlmsManager );

        Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();

        Ogre::Archive *rwAccessFolderArchive =
            archiveManager.load( mWriteAccessFolder, "FileSystem", true );

        if( mUseMicrocodeCache )
        {
            // Make sure the microcode cache is enabled.
            Ogre::GpuProgramManager::getSingleton().setSaveMicrocodesToCache( true );
            const Ogre::String filename = "microcodeCodeCache.cache";
            if( rwAccessFolderArchive->exists( filename ) )
            {
                Ogre::DataStreamPtr shaderCacheFile = rwAccessFolderArchive->open( filename );
                Ogre::GpuProgramManager::getSingleton().loadMicrocodeCache( shaderCacheFile );
            }
        }

        if( mUseHlmsDiskCache )
        {
            for( size_t i = Ogre::HLMS_LOW_LEVEL + 1u; i < Ogre::HLMS_MAX; ++i )
            {
                Ogre::Hlms *hlms = hlmsManager->getHlms( static_cast<Ogre::HlmsTypes>( i ) );
                if( hlms )
                {
                    Ogre::String filename =
                        "hlmsDiskCache" + Ogre::StringConverter::toString( i ) + ".bin";

                    try
                    {
                        if( rwAccessFolderArchive->exists( filename ) )
                        {
                            Ogre::DataStreamPtr diskCacheFile = rwAccessFolderArchive->open( filename );
                            diskCache.loadFrom( diskCacheFile );
                            diskCache.applyTo( hlms );
                        }
                    }
                    catch( Ogre::Exception & )
                    {
                        Ogre::LogManager::getSingleton().logMessage(
                            "Error loading cache from " + mWriteAccessFolder + "/" + filename +
                            "! If you have issues, try deleting the file "
                            "and restarting the app" );
                    }
                }
            }
        }

        archiveManager.unload( mWriteAccessFolder );
    }

    void CResourceGroupManager::saveHlmsDiskCache()
    {
        auto root = Ogre::Root::getSingletonPtr();

        if( root->getRenderSystem() && Ogre::GpuProgramManager::getSingletonPtr() &&
            ( mUseMicrocodeCache || mUseHlmsDiskCache ) )
        {
            Ogre::HlmsManager *hlmsManager = root->getHlmsManager();
            Ogre::HlmsDiskCache diskCache( hlmsManager );

            Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();

            Ogre::Archive *rwAccessFolderArchive =
                archiveManager.load( mWriteAccessFolder, "FileSystem", false );

            if( mUseHlmsDiskCache )
            {
                for( size_t i = Ogre::HLMS_LOW_LEVEL + 1u; i < Ogre::HLMS_MAX; ++i )
                {
                    Ogre::Hlms *hlms = hlmsManager->getHlms( static_cast<Ogre::HlmsTypes>( i ) );
                    if( hlms )
                    {
                        diskCache.copyFrom( hlms );

                        Ogre::DataStreamPtr diskCacheFile = rwAccessFolderArchive->create(
                            "hlmsDiskCache" + Ogre::StringConverter::toString( i ) + ".bin" );
                        diskCache.saveTo( diskCacheFile );
                    }
                }
            }

            if( Ogre::GpuProgramManager::getSingleton().isCacheDirty() && mUseMicrocodeCache )
            {
                const Ogre::String filename = "microcodeCodeCache.cache";
                Ogre::DataStreamPtr shaderCacheFile = rwAccessFolderArchive->create( filename );
                Ogre::GpuProgramManager::getSingleton().saveMicrocodeCache( shaderCacheFile );
            }

            archiveManager.unload( mWriteAccessFolder );
        }
    }

    void CResourceGroupManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( Thread::getTaskFlag( Thread::Render_Flag ) );

            setLoadingState( LoadingState::Loading );

#if 0
                baseRegisterHlms();
                registerHlms();

                auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();

                Ogre::ConfigFile cf;
                cf.load( "resources2.cfg" );

                // Go through all sections & settings in the file
                Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

                String secName, typeName, archName;
                while( seci.hasMoreElements() )
                {
                    secName = seci.peekNextKey();
                    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
                    Ogre::ConfigFile::SettingsMultiMap::iterator i;
                    for( i = settings->begin(); i != settings->end(); ++i )
                    {
                        try
                        {
                            typeName = i->first;
                            archName = i->second;

                            resourceGroupManager->addResourceLocation( archName, typeName, secName,
                                                                       false );
                        }
                        catch( std::exception &e )
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }
                }

                resourceGroupManager->initialiseAllResourceGroups( true );
#else

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pGraphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( pGraphicsSystem );

            auto graphicsSystem = fb::static_pointer_cast<CGraphicsSystemOgreNext>( pGraphicsSystem );
            FB_ASSERT( graphicsSystem );

            ISharedObject::ScopedLock lock( pGraphicsSystem );

            auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
            FB_ASSERT( resourceGroupManager );

            auto currentFolder = String( "./" );
            auto locationType = String( "FileSystem" );
            resourceGroupManager->addResourceLocation( currentFolder, locationType );

            loadResourceFile();

            baseRegisterHlms();
            registerHlms();

            loadTextureCache();
            loadHlmsDiskCache();
            setupResources();

            m_resourceGroupHelper = new ResourceGroupHelper;

            //auto renderTaskId = graphicsSystem->getRenderTask();
            //auto taskManager = applicationManager->getTaskManager();
            //if( taskManager )
            //{
            //    auto renderTask = taskManager->getTask( Thread::Task::Render );

            //    auto resourceLoadJob = fb::make_ptr<ResourceLoadJob>();
            //    // resourceLoadJob->setCoroutine( true );
            //    // resourceLoadJob->execute();

            //    if( renderTask )
            //    {
            //        renderTask->queueJob( resourceLoadJob );
            //    }
            //    else
            //    {
            //        resourceLoadJob->execute();
            //    }
            //}
            //else
            {
                auto resourceLoadJob = fb::make_ptr<ResourceLoadJob>();
                resourceLoadJob->execute();
            }

            //resourceGroupManager->initialiseAllResourceGroups( true );
#endif

            auto root = Ogre::Root::getSingletonPtr();
            auto rs = root->getRenderSystem();
            auto textureManager = rs->getTextureGpuManager();
            textureManager->setStagingTextureMaxBudgetBytes( 2048u * 1024u * 1024u );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            auto message = String( e.what() );
            FB_LOG_ERROR( message );
        }
    }

    void CResourceGroupManager::initialiseAllResourceGroups()
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->initialiseAllResourceGroups( true );
    }

    void CResourceGroupManager::initialiseResourceGroup( const String &groupName )
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->initialiseResourceGroup( groupName, true );
    }

    void CResourceGroupManager::unloadResourceGroup( const String &groupName )
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->unloadResourceGroup( groupName.c_str() );
    }

    void CResourceGroupManager::clearResourceGroup( const String &groupName )
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->clearResourceGroup( groupName.c_str() );
    }

    void CResourceGroupManager::destroyResourceGroup( const String &groupName )
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->destroyResourceGroup( groupName.c_str() );
    }

    void CResourceGroupManager::_getObject( void **ppObject ) const
    {
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        *ppObject = resourceGroupManager;
    }

    void CResourceGroupManager::reloadResources( const String &groupName )
    {
        std::string messages;
        m_resourceGroupHelper->checkTimeAndReloadIfNeeded( groupName.c_str(), messages, true );
    }

    void CResourceGroupManager::parseScripts( const Array<String> &scripts )
    {
        parseScripts( Util::createSet( scripts ) );
    }

    void CResourceGroupManager::parseScripts( const Set<String> &scripts )
    {
        String defaultGrp = "General";

        for( auto fileName : scripts )
        {
            try
            {
                String extension = Path::getFileExtension( fileName );

                if( extension == ( ".program" ) )
                {
                    parseScript( Ogre::MaterialManager::getSingletonPtr(), fileName, defaultGrp );
                }
                else if( extension == ( ".material" ) )
                {
                    parseScript( Ogre::MaterialManager::getSingletonPtr(), fileName, defaultGrp );
                }
                else if( extension == ( ".compositor" ) )
                {
                    // parseScript(Ogre::CompositorManager::getSingletonPtr(), fileName, defaultGrp);
                }
                else if( extension == ( ".fontdef" ) )
                {
                    // parseScript(Ogre::FontManager::getSingletonPtr(), fileName, defaultGrp);
                }
                else if( extension == ( ".overlay" ) )
                {
                    // parseScript(Ogre::OverlayManager::getSingletonPtr(), fileName, defaultGrp);
                }
                else if( extension == ( ".pu" ) )
                {
                    // parseScripts(Ogre::ParticleSystemManager::getSingletonPtr(), ".pu",
                    // defaultGrp);
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }

    void CResourceGroupManager::parseScript( Ogre::ScriptLoader *loader, const String &fileName,
                                             const String &group )
    {
        try
        {
            FB_ASSERT( loader );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = fileSystem->open( fileName );
            if( data )
            {
                Ogre::DataStreamPtr dataStream( new FBOgreDataStream( data ) );
                loader->parseScript( dataStream, group.c_str() );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::parseScripts( const String &extension, const String &group )
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( extension ) );
            FB_ASSERT( !StringUtil::isNullOrEmpty( group ) );

            auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
            FB_ASSERT( resourceGroupManager );

            auto wildCard = "*" + extension;
            auto scriptLoader = resourceGroupManager->_findScriptLoader( wildCard );
            FB_ASSERT( scriptLoader );

            auto groupMgr = Ogre::ResourceGroupManager::getSingletonPtr();
            FB_ASSERT( groupMgr );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto scriptFiles = fileSystem->getFilesWithExtension( extension );
            if( !scriptFiles.empty() )
            {
                for( auto &file : scriptFiles )
                {
                    try
                    {
                        auto fileName = String( file.filePath.c_str() );

#if 0
                            FB_LOG( "Parse script: " + fileName );

                            if( fileName.find( "PbsMaterials.material" ) != String::npos )
                            {
                                s32 stop = 0;
                                stop++;
                            }

                            auto text = fileSystem->readAllText( fileName );
                            if( text.find( "Rocks" ) != String::npos )
                            {
                                s32 stop = 0;
                                stop++;
                            }
#endif

                        auto data = fileSystem->open( fileName );
                        if( data )
                        {
                            Ogre::DataStreamPtr dataStream( new FBOgreDataStream( data ) );
                            scriptLoader->parseScript(
                                dataStream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                        }
                    }
                    catch( Ogre::Exception &e )
                    {
                        auto message = e.getFullDescription();
                        FB_LOG_ERROR( message );
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::parseScripts( Ogre::ResourceManager *resMgr, const String &extension,
                                              const String &group )
    {
        try
        {
            FB_ASSERT( resMgr );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            Array<String> scriptFileNames;
            scriptFileNames.reserve( 32 );

            fileSystem->getFileNamesWithExtension( extension, scriptFileNames );
            if( !scriptFileNames.empty() )
            {
                auto uniqueScriptFileNames = Util::createSet( scriptFileNames );
                for( auto fileName : uniqueScriptFileNames )
                {
                    try
                    {
                        auto data = fileSystem->open( fileName );
                        if( data )
                        {
                            Ogre::DataStreamPtr dataStream( new FBOgreDataStream( data ) );
                            resMgr->parseScript( dataStream, group.c_str() );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::parseScripts( Ogre::ParticleSystemManager *resMgr,
                                              const String &extension, const String &group )
    {
        try
        {
            FB_ASSERT( resMgr );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            Array<String> scriptFileNames;
            scriptFileNames.reserve( 32 );

            fileSystem->getFileNamesWithExtension( extension, scriptFileNames );
            if( !scriptFileNames.empty() )
            {
                auto uniqueScriptFileNames = Util::createSet( scriptFileNames );
                for( auto fileName : uniqueScriptFileNames )
                {
                    try
                    {
                        auto data = fileSystem->open( fileName );
                        if( data )
                        {
                            Ogre::DataStreamPtr dataStream( new FBOgreDataStream( data ) );
                            resMgr->parseScript( dataStream, group.c_str() );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::parseScripts( Ogre::ScriptLoader *loader, const String &extension,
                                              const String &group )
    {
        try
        {
            FB_ASSERT( loader );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            Array<String> scriptFileNames;
            scriptFileNames.reserve( 32 );

            fileSystem->getFileNamesWithExtension( extension, scriptFileNames );
            if( !scriptFileNames.empty() )
            {
                auto uniqueScriptFileNames = Util::createSet( scriptFileNames );
                for( auto fileName : uniqueScriptFileNames )
                {
                    try
                    {
                        auto data = fileSystem->open( fileName );
                        if( data )
                        {
                            Ogre::DataStreamPtr dataStream( new FBOgreDataStream( data ) );
                            loader->parseScript( dataStream, group.c_str() );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    CResourceGroupManager::ResourceLoadJob::ResourceLoadJob() = default;

    CResourceGroupManager::ResourceLoadJob::~ResourceLoadJob() = default;

    void CResourceGroupManager::ResourceLoadJob::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto root = Ogre::Root::getSingletonPtr();
            auto hlmsManager = root->getHlmsManager();

            auto compositorManager = root->getCompositorManager2();
            FB_ASSERT( compositorManager );

            auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
            FB_ASSERT( resourceGroupManager );

            static const auto defaultGrp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

            auto gpuProgramManager = Ogre::GpuProgramManager::getSingletonPtr();

            //// load scripts
            try
            {
#ifdef OGRE_BUILD_RENDERSYSTEM_D3D11
#    if FB_BUILD_RENDERER_DX11
                static const auto hlslProgramExt = String( ".hlsl" );
                m_resourceGroupManager->parseScripts( gpuProgramManager, hlslProgramExt, defaultGrp );
#    endif
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GL3PLUS
#    if FB_BUILD_RENDERER_GL3PLUS
                static const auto glProgramExt = String( ".glsl" );
                m_resourceGroupManager->parseScripts( gpuProgramManager, glProgramExt, defaultGrp );
#    endif
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_METAL
                static const auto metalProgramExt = String( ".metal" );
                m_resourceGroupManager->parseScripts( gpuProgramManager, metalProgramExt, defaultGrp );
#endif
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            try
            {
                auto materialManager = Ogre::MaterialManager::getSingletonPtr();
                static const auto programExt = String( ".program" );
                m_resourceGroupManager->parseScripts( materialManager, programExt, defaultGrp );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            try
            {
                static const auto materialExt = String( ".material" );
                m_resourceGroupManager->parseScripts( materialExt, defaultGrp );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            static const auto jsonMaterialExt = String( ".json" );
            // m_resourceGroupManager->parseScripts(jsonMaterialExt, defaultGrp);

            auto path = String( "2.0/scripts" );
            auto scriptFiles = fileSystem->getFilesWithExtension( path, jsonMaterialExt );
            if( !scriptFiles.empty() )
            {
                for( auto &file : scriptFiles )
                {
                    try
                    {
                        if( auto stream = fileSystem->open( file.filePath ) )
                        {
                            auto ogreStream = Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
                            hlmsManager->parseScript( ogreStream, defaultGrp );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
            }

            try
            {
                auto fontManager = Ogre::FontManager::getSingletonPtr();
                static const auto fontExt = String( ".fontdef" );
                m_resourceGroupManager->parseScripts( fontManager, fontExt, defaultGrp );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            // m_resourceGroupManager->parseScripts(Ogre::v1::OverlayManager::getSingletonPtr(),
            // ".overlay", defaultGrp); parseScripts(Ogre::ParticleSystemManager::getSingletonPtr(),
            // ".pu", defaultGrp);

            // m_resourceGroupManager->parseScripts(Ogre::GpuProgramManager::getSingletonPtr(),
            // ".cg", defaultGrp);

            try
            {
                static const auto compositorExt = String( ".compositor" );
                m_resourceGroupManager->parseScripts( compositorExt, defaultGrp );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            try
            {
                resourceGroupManager->initialiseAllResourceGroups( false );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CResourceGroupManager::ResourceLoadJob::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
    {
        auto applicationManager = core::ApplicationManager::instance();

        static const auto defaultGrp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

        JOB_BEGIN()
        // load scripts
        m_resourceGroupManager->parseScripts( Ogre::MaterialManager::getSingletonPtr(), ".program",
                                              defaultGrp );
        m_resourceGroupManager->parseScripts( Ogre::MaterialManager::getSingletonPtr(), ".material",
                                              defaultGrp );
        // parseScripts(Ogre::CompositorManager::getSingletonPtr(), ".compositor", defaultGrp);

        auto fontManager = Ogre::FontManager::getSingletonPtr();
        static const auto fontExt = String( ".fontdef" );
        m_resourceGroupManager->parseScripts( fontManager, fontExt, defaultGrp );

        m_resourceGroupManager->parseScripts( Ogre::v1::OverlayManager::getSingletonPtr(), ".overlay",
                                              defaultGrp );
        // parseScripts(Ogre::ParticleSystemManager::getSingletonPtr(), ".pu", defaultGrp);
        JOB_YIELD()
        m_resourceGroupManager->parseScripts( Ogre::GpuProgramManager::getSingletonPtr(), ".cg",
                                              defaultGrp );
        m_resourceGroupManager->parseScripts( Ogre::GpuProgramManager::getSingletonPtr(), ".hlsl",
                                              defaultGrp );

        JOB_YIELD()
        auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceGroupManager->initialiseAllResourceGroups( false );
        JOB_END()
    }

    auto CResourceGroupManager::ResourceLoadJob::getResourceGroupManager() const
        -> CResourceGroupManager *
    {
        return m_resourceGroupManager;
    }

    void CResourceGroupManager::ResourceLoadJob::setResourceGroupManager( CResourceGroupManager *val )
    {
        m_resourceGroupManager = val;
    }
}  // namespace fb::render
