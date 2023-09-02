#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/IOSystem.h>
#include <FBAssimp/IOStream.h>
#include <FBCore/FBCore.h>

namespace fb
{

    IOSystem::IOSystem( const SmartPtr<IStream> &_source, const String &group ) :
        source( _source ),
        _group( group )
    {
    }

    IOSystem::IOSystem()
    {
    }

    bool IOSystem::Exists( const char *pFile ) const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );
        FB_ASSERT( fileSystem->isValid() );

        return fileSystem->isExistingFile( pFile );
    }

    char IOSystem::getOsSeparator() const
    {
        return '/';
    }

    Assimp::IOStream *IOSystem::Open( const char *pFile, const char *pMode )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );
        FB_ASSERT( fileSystem->isValid() );

        auto stream = fileSystem->open( pFile );
        if( stream )
        {
            streams.emplace_back( new IOStream( stream ) );
            return streams.back();
        }

        return nullptr;
    }

    void IOSystem::Close( Assimp::IOStream *pFile )
    {
        auto it = std::find( streams.begin(), streams.end(), pFile );
        if( it != streams.end() )
        {
            delete pFile;
            streams.erase( it );
        }
    }

}  // namespace fb
