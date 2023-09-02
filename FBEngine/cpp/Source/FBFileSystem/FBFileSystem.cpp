#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/FBFileSystem.h>
#include <FBCore/FBCore.h>
#include <FBFileSystem/CFileSystem.h>
#include <FBFileSystem/CFileSystemArchive.h>
#include <FBFileSystem/FileDataStream.h>
#include <FBFileSystem/ZipArchive.h>
#include <FBFileSystem/ZipFile.h>

namespace fb
{
    void FBFileSystem::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            FactoryUtil::addFactory<CFileList>();
            FactoryUtil::addFactory<CFileSystem>();
            FactoryUtil::addFactory<FileDataStream>();
            FactoryUtil::addFactory<CFileSystemArchive>();

            FactoryUtil::addFactory<ZipArchive>();
            FactoryUtil::addFactory<ZipFile>();

            factoryManager->setPoolSizeByType<CFileList>( 32 );
            factoryManager->setPoolSizeByType<CFileSystemArchive>( 32 );
            factoryManager->setPoolSizeByType<FileDataStream>( 4 );
            factoryManager->setPoolSizeByType<ZipArchive>( 8 );
            factoryManager->setPoolSizeByType<ZipFile>( 4 );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FBFileSystem::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<IFileSystem> FBFileSystem::createFileSystem()
    {
        return fb::make_ptr<CFileSystem>();
    }
}  // end namespace fb
