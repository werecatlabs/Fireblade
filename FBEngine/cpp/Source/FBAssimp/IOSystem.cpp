#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/IOSystem.h>
#include <FBAssimp/IOStream.h>
#include <FBCore/FBCore.h>

#include <utility>

namespace fb
{

    IOSystem::IOSystem( const SmartPtr<IStream> &_source, String group ) :
        source( _source ),
        _group( std::move( group ) )
    {
    }

    IOSystem::IOSystem() = default;

    auto IOSystem::Exists( const char *pFile ) const -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );
        FB_ASSERT( fileSystem->isValid() );

        return fileSystem->isExistingFile( pFile );
    }

    auto IOSystem::getOsSeparator() const -> char
    {
        return '/';
    }

    auto IOSystem::Open( const char *pFile, const char *pMode ) -> Assimp::IOStream *
    {
        auto applicationManager = core::ApplicationManager::instance();
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
