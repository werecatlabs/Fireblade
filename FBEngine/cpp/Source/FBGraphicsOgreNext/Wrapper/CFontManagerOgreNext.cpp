#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CFontManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CFontOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreMaterialManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CFontManagerOgreNext, IMaterialManager );

        CFontManagerOgreNext::CFontManagerOgreNext()
        {
            m_fonts.reserve( 1024 );
        }

        CFontManagerOgreNext::~CFontManagerOgreNext()
        {
            unload( nullptr );
        }

        void CFontManagerOgreNext::load( SmartPtr<ISharedObject> data )
        {
        }

        void CFontManagerOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                for(auto &font : m_fonts)
                {
                    font->unload( nullptr );
                }

                m_fonts.clear();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IFont> CFontManagerOgreNext::cloneFont( const String &name, const String &clonedFontName )
        {
            return nullptr;
        }

        SmartPtr<IResource> CFontManagerOgreNext::create( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto font = factoryManager->make_ptr<CFontOgreNext>();
            FB_ASSERT( font );

            auto handle = font->getHandle();
            FB_ASSERT( handle );

            handle->setName( name );

            auto uuid = StringUtil::getUUID();
            handle->setUUID( uuid );

            m_fonts.push_back( font );

            return font;
        }

        SmartPtr<IResource> CFontManagerOgreNext::create( const String &uuid, const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto font = factoryManager->make_ptr<CFontOgreNext>();
            FB_ASSERT( font );

            auto handle = font->getHandle();
            FB_ASSERT( handle );

            handle->setName( name );

            handle->setUUID( uuid );

            m_fonts.push_back( font );

            return font;
        }

        Pair<SmartPtr<IResource>, bool> CFontManagerOgreNext::createOrRetrieve( const String &uuid,
                                                                        const String &path,
                                                                        const String &type )
        {
            auto fontResource = getById( uuid );
            if( fontResource )
            {
                auto font = fb::static_pointer_cast<CFontOgreNext>( fontResource );
                return std::pair<SmartPtr<IResource>, bool>( font, false );
            }

            auto font = create( uuid, path );
            FB_ASSERT( font );

            return Pair<SmartPtr<IResource>, bool>( font, true );
        }

        Pair<SmartPtr<IResource>, bool> CFontManagerOgreNext::createOrRetrieve( const String &path )
        {
            auto fontResource = getByName( path );
            if( fontResource )
            {
                auto font = fb::static_pointer_cast<CFontOgreNext>( fontResource );
                return std::pair<SmartPtr<IResource>, bool>( font, false );
            }

            auto font = create( path );
            FB_ASSERT( font );

            return Pair<SmartPtr<IResource>, bool>( font, true );
        }

        void CFontManagerOgreNext::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
                FB_ASSERT( resource );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto font = fb::static_pointer_cast<IFont>( resource );
                auto pData = font->toData();

                //auto mat = pData->getDataAsType<data::font_graph>();
                //auto fontStr = DataUtil::toString( mat, true );

                //fileSystem->writeAllText( filePath, fontStr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IResource> CFontManagerOgreNext::loadFromFile( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto fontName = Path::getFileNameWithoutExtension( filePath );

                for( auto font : m_fonts )
                {
                    auto handle = font->getHandle();
                    auto currentMaterialName = handle->getName();

                    if( fontName == currentMaterialName )
                    {
                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( filePath, fileInfo ) )
                        {
                            auto currentMaterialPath = String( fileInfo.filePath.c_str() );
                            auto currentFileId = font->getFileSystemId();

                            if( filePath == currentMaterialPath && fileInfo.fileId == currentFileId )
                            {
                                return font;
                            }
                        }
                    }
                }

                auto ext = Path::getFileExtension( filePath );
                if( ext == ".mat" )
                {
                    auto stream = fileSystem->open( filePath );
                    if( stream )
                    {
                        auto fontStr = stream->getAsString();

                        //data::font_graph fontData;
                        //DataUtil::parse( fontStr, &fontData );

                        auto font = create( fontName );

                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( filePath, fileInfo ) )
                        {
                            auto fileId = fileInfo.fileId;
                            font->setFileSystemId( fileId );
                        }

                        //auto data = fb::make_ptr<CData<data::font_graph>>();
                        //data->setData( &fontData );
                        //font->fromData( data );
                        //font->load( nullptr );

                        //m_fonts.push_back( font );

                        //return font;
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CFontManagerOgreNext::load( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                auto fontResource = getByName( name );
                if( fontResource )
                {
                    auto font = fb::static_pointer_cast<CFontOgreNext>( fontResource );
                    font->load( nullptr );
                    return font;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CFontManagerOgreNext::getByName( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                for( auto &font : m_fonts )
                {
                    auto handle = font->getHandle();
                    if( handle->getName() == name )
                    {
                        return font;
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CFontManagerOgreNext::getById( const String &uuid )
        {
            try
            {
                for( auto &font : m_fonts )
                {
                    auto handle = font->getHandle();
                    if( handle->getUUID() == uuid )
                    {
                        return font;
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CFontManagerOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }
    }  // end namespace render
}  // end namespace fb