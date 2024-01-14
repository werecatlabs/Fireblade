#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CVideoTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCubemapOgreNext.h>
#include <FBGraphicsOgreNext/OgreUtil.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Path.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <Ogre.h>
#include <OgreFileSystem.h>
#include <OgreImage2.h>
#include <OgrePixelFormatGpuUtils.h>
#include <OgreTextureBox.h>
#include <OgreTextureGpuManager.h>
#include <OgreLwString.h>

#if defined FB_PLATFORM_APPLE
#    include <FBGraphicsOgreNext/Wrapper/Apple/CTextureOgreApple.h>
#endif

namespace fb::render
{

    CTextureManagerOgreNext::CTextureManagerOgreNext()
    {
        m_textureListener = fb::make_ptr<TextureListener>();
    }

    CTextureManagerOgreNext::~CTextureManagerOgreNext()
    {
        unload( nullptr );
    }

    void CTextureManagerOgreNext::update()
    {
        for( auto rt : m_renderTargets )
        {
            rt->update();
        }
    }

    void CTextureManagerOgreNext::postUpdate()
    {
        for( auto rt : m_renderTargets )
        {
            rt->postUpdate();
        }
    }

    void CTextureManagerOgreNext::load( SmartPtr<ISharedObject> data )
    {
    }

    void CTextureManagerOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( isValid() );

            if( const auto &loadingState = getLoadingState(); loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( const auto pTextures = getTextures() )
                {
                    auto &textures = *pTextures;
                    for( auto &t : textures )
                    {
                        t->unload( nullptr );
                    }

                    setTextures( nullptr );
                }

                for( auto &rtt : m_renderTargets )
                {
                    rtt->unload( nullptr );
                }

                m_renderTargets.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CTextureManagerOgreNext::createManual( const String &name, const String &group, u8 texType,
                                                u32 width, u32 height, u32 depth, s32 num_mips,
                                                u8 format, s32 usage /*= TU_DEFAULT*/ )
        -> SmartPtr<ITexture>
    {
        //auto root = Ogre::Root::getSingletonPtr();
        //auto renderSystem = root->getRenderSystem();
        //auto textureManager = renderSystem->getTextureGpuManager();

        //Ogre::TexturePtr tex = textureManager->createManual(
        //    name.c_str(), group.c_str(), static_cast<Ogre::TextureType>( texType ), width, height,
        //    depth, num_mips, static_cast<Ogre::PixelFormat>( format ), usage );

        //SmartPtr<CTextureOgre> texture( new CTextureOgre );
        //texture->initialise( tex );

        //return texture
        //;

        return nullptr;
    }

    auto CTextureManagerOgreNext::create( const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto texture = factoryManager->make_ptr<CTextureOgreNext>();
            FB_ASSERT( texture );

            texture->addObjectListener( m_textureListener );

            auto handle = texture->getHandle();
            FB_ASSERT( handle );

            handle->setName( name );

            auto uuid = StringUtil::getUUID();
            handle->setUUID( uuid );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( name, fileInfo, false ) )
            {
                texture->setFileSystemId( fileInfo.fileId );
            }
            else if( fileSystem->findFileInfo( name, fileInfo, true ) )
            {
                texture->setFileSystemId( fileInfo.fileId );
            }

            texture->setFilePath( name );

            addTexture( texture );

            return texture;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::create( const String &uuid, const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto texture = factoryManager->make_ptr<CTextureOgreNext>();
            FB_ASSERT( texture );

            texture->addObjectListener( m_textureListener.get() );

            auto handle = texture->getHandle();
            FB_ASSERT( handle );

            handle->setName( name );
            handle->setUUID( uuid );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( name, fileInfo, false ) )
            {
                texture->setFileSystemId( fileInfo.fileId );
            }
            else if( fileSystem->findFileInfo( name, fileInfo, true ) )
            {
                texture->setFileSystemId( fileInfo.fileId );
            }

            texture->setFilePath( name );

            addTexture( texture );

            return texture;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::loadResource( const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            auto textureResource = getByName( name );
            if( textureResource )
            {
                auto texture = fb::static_pointer_cast<CTextureOgreNext>( textureResource );
                texture->load( nullptr );
                return texture;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::getByName( const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            auto pTextures = getTextures();
            if( pTextures )
            {
                auto textures = *pTextures;
                for( auto &texture : textures )
                {
                    auto handle = texture->getHandle();
                    if( handle->getName() == name )
                    {
                        return texture;
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::getById( const String &uuid ) -> SmartPtr<IResource>
    {
        try
        {
            if( auto pTextures = getTextures() )
            {
                auto &textures = *pTextures;
                for( auto &texture : textures )
                {
                    auto handle = texture->getHandle();
                    if( handle->getUUID() == uuid )
                    {
                        return texture;
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::createOrRetrieve( const String &uuid, const String &path,
                                                    const String &type )
        -> Pair<SmartPtr<IResource>, bool>
    {
        try
        {
            if( auto textureResource = getById( uuid ) )
            {
                auto texture = fb::static_pointer_cast<CTextureOgreNext>( textureResource );
                return std::pair<SmartPtr<IResource>, bool>( texture, false );
            }

            auto texture = create( uuid, path );
            FB_ASSERT( texture );

            return Pair<SmartPtr<IResource>, bool>( texture, true );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    auto CTextureManagerOgreNext::createOrRetrieve( const String &path )
        -> Pair<SmartPtr<IResource>, bool>
    {
        try
        {
            if( auto textureResource = getByName( path ) )
            {
                auto texture = fb::static_pointer_cast<CTextureOgreNext>( textureResource );
                return std::pair<SmartPtr<IResource>, bool>( texture, false );
            }

            auto texture = create( path );
            FB_ASSERT( texture );

            return Pair<SmartPtr<IResource>, bool>( texture, true );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>( nullptr, false );
    }

    void CTextureManagerOgreNext::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    auto CTextureManagerOgreNext::loadFromFile( const String &filePath ) -> SmartPtr<IResource>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            auto materialName = Path::getFileNameWithoutExtension( filePath );

            auto pTextures = getTextures();
            if( pTextures )
            {
                auto &textures = *pTextures;
                for( auto material : textures )
                {
                    auto handle = material->getHandle();
                    auto currentMaterialName = handle->getName();

                    if( materialName == currentMaterialName )
                    {
                        return material;
                    }
                }
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto stream = fileSystem->open( filePath );
            if( stream )
            {
                auto material = create( filePath );

                FileInfo fileInfo;
                if( fileSystem->findFileInfo( filePath, fileInfo ) )
                {
                    auto fileId = fileInfo.fileId;
                    material->setFileSystemId( fileId );
                }

                return material;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::createVideoTexture( const String &name ) -> SmartPtr<IVideoTexture>
    {
        return nullptr;
    }

    auto CTextureManagerOgreNext::addCubemap() -> SmartPtr<ICubemap>
    {
        return nullptr;
    }

    auto CTextureManagerOgreNext::createRenderTexture() -> SmartPtr<ITexture>
    {
#if defined FB_PLATFORM_WIN32
        auto texture = fb::make_ptr<CTextureOgreNext>();
        texture->setUsageFlags( ITexture::TU_RENDERTARGET );
        texture->load( nullptr );
        addTexture( texture );
        return texture;
#elif defined FB_PLATFORM_APPLE
        auto texture = fb::make_ptr<CTextureOgreApple>();
        texture->setUsageFlags( ITexture::TU_RENDERTARGET );
        texture->load( nullptr );
        addTexture( texture );
        return texture;
#endif
    }

    void CTextureManagerOgreNext::destroyRenderTexture( SmartPtr<ITexture> texture )
    {
        if( texture )
        {
            texture->unload( nullptr );

            removeTexture( texture );

            auto it = std::find( m_renderTargets.begin(), m_renderTargets.end(), texture );
            if( it != m_renderTargets.end() )
            {
                m_renderTargets.erase( it );
            }
        }
    }

    auto CTextureManagerOgreNext::createSkyBoxCubeMap( Array<String> faceNames ) -> SmartPtr<ITexture>
    {
        const uint8_t maxNumMipmaps = 2;  // (uint8_t)atoi(argv[3]) + 1u;
        const char *extension = "";       // argv[2];
        const char *outputFilename = "";  // argv[4];

        if( StringUtil::isNullOrEmpty( faceNames[0] ) )
        {
            return nullptr;
        }

        using namespace Ogre;

        Image2 faceImage;

        const bool bOpenSuccess = OgreUtil::loadIntoImage( faceNames[0], faceImage );

        if( !bOpenSuccess )
        {
            printf( "Could not determine resolution nor pixel format. Aborting\n" );
            return nullptr;
        }

        const uint32_t width = faceImage.getWidth();
        const uint32_t height = faceImage.getWidth();
        const PixelFormatGpu pixelFormat = faceImage.getPixelFormat();

        uint8_t numMipmaps = PixelFormatGpuUtils::getMaxMipmapCount( width, height );
        numMipmaps = std::min( numMipmaps, maxNumMipmaps );
        const size_t requiredBytes = PixelFormatGpuUtils::calculateSizeBytes(
            width, height, 1u, 6u, pixelFormat, numMipmaps, 4u );

        faceImage.generateMipmaps( true );

        void *data = OGRE_MALLOC_SIMD( requiredBytes, MEMCATEGORY_GENERAL );

        auto finalCubemap = new Image2();
        finalCubemap->loadDynamicImage( data, width, height, 6u, TextureTypes::TypeCube, pixelFormat,
                                        true, numMipmaps );

        for( uint8_t mip = 0u; mip < numMipmaps; ++mip )
        {
            for( uint32_t face = 0u; face < 6u; ++face )
            {
                if( mip != 0u || face != 0u )
                {
                    OgreUtil::loadIntoImage( faceNames[face], faceImage );
                }

                TextureBox dstBox = finalCubemap->getData( mip );
                if( faceImage.getWidth() != dstBox.width || faceImage.getHeight() != dstBox.height )
                {
                    faceImage.resize( dstBox.width, dstBox.height );
                }

                TextureBox srcBox = faceImage.getData( 0 );

                dstBox.sliceStart = face;
                dstBox.numSlices = 1u;
                dstBox.copyFrom( srcBox );
                dstBox.numSlices = 6u;
            }
        }

        //finalCubemap.save( outputFilename, 0u, numMipmaps );
        //auto uuid = StringUtil::getUUID() + Path::getFileExtension( faceNames[0] );
        auto uuid = StringUtil::getUUID() + ".dds";  // hack

        auto root = Ogre::Root::getSingletonPtr();
        auto renderSystem = root->getRenderSystem();
        auto textureManager = renderSystem->getTextureGpuManager();

        auto texture = textureManager->createTexture(
            uuid, Ogre::GpuPageOutStrategy::Discard, Ogre::TextureFlags::RenderToTexture,
            Ogre::TextureTypes::TypeCube, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

        texture->setResolution( faceImage.getWidth(), faceImage.getHeight(), 1u );
        texture->setPixelFormat( faceImage.getPixelFormat() );
        //texture->scheduleTransitionTo( Ogre::GpuResidency::Resident, finalCubemap );
        texture->scheduleReupload( finalCubemap );

        //faceImage.save( "test.png", 0, 1 );

        auto t = fb::static_pointer_cast<CTextureOgreNext>( create( uuid ) );
        t->setTexture( texture );
        return t;
    }

    auto CTextureManagerOgreNext::createSkyBoxCubeMap( Array<SmartPtr<ITexture>> skyboxTextures )
        -> SmartPtr<ITexture>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            const char *faceNames[6] = { "_px", "_nx", "_py", "_ny", "_pz", "_nz" };

            std::string folderPath;
            const uint8_t maxNumMipmaps = 1;  // (uint8_t)atoi(argv[3]) + 1u;
            std::string extension = "";       // argv[2];
            std::string outputFilename = Path::getWorkingDirectory() + "/output.ktx";  // argv[4];
            String filename;

            auto faceTex0 = skyboxTextures[0];
            FB_ASSERT( faceTex0 );

            auto fileId = faceTex0->getFileSystemId();
            FB_ASSERT( fileId != 0 );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileId, fileInfo ) )
            {
                folderPath = String( fileInfo.path.c_str() );
                filename = String( fileInfo.fileName.c_str() );
            }

            if( !folderPath.empty() && folderPath[folderPath.size() - 1u] != '/' )
            {
                folderPath.push_back( '/' );
            }

            using namespace Ogre;
            char tmpBuffer[256];

            // LwString filename( LwString::FromEmptyPointer( tmpBuffer, sizeof( tmpBuffer ) ) );

            Image2 faceImage;
            // filename.a( "m", 0, faceNames[0], ".", extension );

            const bool bOpenSuccess = OgreUtil::loadIntoImage( filename, faceImage );

            if( !bOpenSuccess )
            {
                printf( "Could not determine resolution nor pixel format. Aborting\n" );
                return nullptr;
            }

            const uint32_t width = faceImage.getWidth();
            const uint32_t height = faceImage.getWidth();
            const PixelFormatGpu pixelFormat = faceImage.getPixelFormat();

            uint8_t numMipmaps = PixelFormatGpuUtils::getMaxMipmapCount( width, height );
            numMipmaps = std::min( numMipmaps, maxNumMipmaps );
            const size_t requiredBytes = PixelFormatGpuUtils::calculateSizeBytes(
                width, height, 1u, 6u, pixelFormat, numMipmaps, 4u );

            void *data = OGRE_MALLOC_SIMD( requiredBytes, MEMCATEGORY_GENERAL );
            Image2 finalCubemap;
            finalCubemap.loadDynamicImage( data, width, height, 6u, TextureTypes::TypeCube, pixelFormat,
                                           true, numMipmaps );

            for( uint8_t mip = 0u; mip < numMipmaps; ++mip )
            {
                for( uint32_t face = 0u; face < 6u; ++face )
                {
                    auto currentFaceTex = skyboxTextures[face];
                    if( currentFaceTex )
                    {
                        auto currentFileId = currentFaceTex->getFileSystemId();
                        FB_ASSERT( currentFileId != 0 );

                        FileInfo currentFileInfo;
                        if( fileSystem->findFileInfo( currentFileId, currentFileInfo ) )
                        {
                            folderPath = String( currentFileInfo.path.c_str() );
                            filename = String( currentFileInfo.fileName.c_str() );
                        }

                        if( mip != 0u || face != 0u )
                        {
                            OgreUtil::loadIntoImage( filename.c_str(), faceImage );
                        }
                    }

                    TextureBox srcBox = faceImage.getData( 0u );
                    TextureBox dstBox = finalCubemap.getData( mip );

                    dstBox.sliceStart = face;
                    dstBox.numSlices = 1u;
                    dstBox.copyFrom( srcBox );
                    dstBox.numSlices = 6u;
                }
            }

            //finalCubemap.save( outputFilename, 0u, numMipmaps );

            return nullptr;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CTextureManagerOgreNext::createSkyBoxCubeMap( SmartPtr<IMaterial> material )
        -> SmartPtr<ITexture>
    {
        return nullptr;
    }

    void CTextureManagerOgreNext::_getObject( void **ppObject ) const
    {
    }

    auto CTextureManagerOgreNext::getStateContext() const -> fb::SmartPtr<fb::IStateContext>
    {
        return nullptr;
    }

    void CTextureManagerOgreNext::setStateContext( SmartPtr<IStateContext> stateContext )
    {
    }

    void CTextureManagerOgreNext::addTexture( SmartPtr<ITexture> texture )
    {
        FB_DEBUG_TRACE;

        auto pTextures = getTextures();
        if( !pTextures )
        {
            pTextures = fb::make_shared<Array<SmartPtr<ITexture>>>();
            setTextures( pTextures );
        }

        if( pTextures )
        {
            auto &textures = *pTextures;
            textures.push_back( texture );
        }
    }

    void CTextureManagerOgreNext::removeTexture( SmartPtr<ITexture> texture )
    {
        FB_DEBUG_TRACE;

        if( auto pTextures = getTextures() )
        {
            auto &textures = *pTextures;

            auto it = std::find( textures.begin(), textures.end(), texture );
            if( it != textures.end() )
            {
                textures.erase( it );
            }

            auto newTextures = fb::make_shared<Array<SmartPtr<ITexture>>>();
            *newTextures = Array<SmartPtr<ITexture>>( textures.begin(), textures.end() );
            setTextures( newTextures );
        }
    }

    void CTextureManagerOgreNext::setTextures( SharedPtr<Array<SmartPtr<ITexture>>> p )
    {
        m_textures = p;
    }

    auto CTextureManagerOgreNext::getTextures() const -> SharedPtr<Array<SmartPtr<ITexture>>>
    {
        return m_textures;
    }

    auto CTextureManagerOgreNext::TextureListener::handleEvent( IEvent::Type eventType,
                                                                hash_type eventValue,
                                                                const Array<Parameter> &arguments,
                                                                SmartPtr<ISharedObject> sender,
                                                                SmartPtr<ISharedObject> object,
                                                                SmartPtr<IEvent> event ) -> fb::Parameter
    {
        return {};
    }

    void CTextureManagerOgreNext::TextureListener::loadingStateChanged( ISharedObject *sharedObject,
                                                                        LoadingState oldState,
                                                                        LoadingState newState )
    {
        if( newState == LoadingState::Loaded )
        {
        }
    }

    auto CTextureManagerOgreNext::TextureListener::destroy( void *ptr ) -> bool
    {
        return false;
    }

}  // namespace fb::render
