#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/MeshManager.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Mesh/Mesh.h>
#include <FBCore/Mesh/MeshResource.h>
#include <FBCore/Mesh/MeshSerializer.h>
#include "FBCore/ApplicationUtil.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MeshManager, IMeshManager );

    MeshManager::MeshManager() = default;

    MeshManager::~MeshManager() = default;

    void MeshManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( auto p = getMeshesPtr() )
            {
                auto &meshes = *p;
                for( auto mesh : meshes )
                {
                    mesh->unload( nullptr );
                }

                meshes.clear();
            }

            if( auto p = getMeshResourcesPtr() )
            {
                auto &meshResources = *p;
                for( auto meshResources : meshResources )
                {
                    meshResources->unload( nullptr );
                }

                meshResources.clear();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshManager::addMesh( SmartPtr<IMesh> mesh )
    {
        auto p = getMeshesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IMesh>>>();
            setMeshesPtr( p );
        }

        if( p )
        {
            auto &meshes = *p;
            meshes.push_back( mesh );
        }
    }

    void MeshManager::removeMesh( SmartPtr<IMesh> mesh )
    {
        auto p = getMeshesPtr();
        if( p )
        {
            auto meshes = Array<SmartPtr<IMesh>>( p->begin(), p->end() );
            meshes.erase( std::remove( meshes.begin(), meshes.end(), mesh ), meshes.end() );

            auto newMeshes =
                fb::make_shared<ConcurrentArray<SmartPtr<IMesh>>>( meshes.begin(), meshes.end() );
            setMeshesPtr( newMeshes );
        }
    }

    void MeshManager::addMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        auto p = getMeshResourcesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IMeshResource>>>();
            setMeshResourcesPtr( p );
        }

        if( p )
        {
            p->push_back( meshResource );
        }
    }

    void MeshManager::removeMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        auto p = getMeshResourcesPtr();
        if( p )
        {
            auto meshResources = Array<SmartPtr<IMeshResource>>( p->begin(), p->end() );
            meshResources.erase( std::remove( meshResources.begin(), meshResources.end(), meshResource ),
                                 meshResources.end() );

            auto newMeshResources = fb::make_shared<ConcurrentArray<SmartPtr<IMeshResource>>>(
                meshResources.begin(), meshResources.end() );
            setMeshResourcesPtr( newMeshResources );
        }
    }

    SmartPtr<IMesh> MeshManager::findMesh( const String &name )
    {
        if( auto p = getMeshesPtr() )
        {
            auto &meshes = *p;
            for( auto &mesh : meshes )
            {
                auto meshName = mesh->getName();
                if( meshName == name )
                {
                    return mesh;
                }
            }
        }

        return nullptr;
    }

    SmartPtr<IMesh> MeshManager::loadMesh( const String &filePath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileId = StringUtil::getHash64( filePath );

        if( auto p = getMeshResourcesPtr() )
        {
            auto &meshResources = *p;
            for( auto meshResource : meshResources )
            {
                auto meshResourceFileId = meshResource->getFileSystemId();
                if( meshResourceFileId == fileId )
                {
                    return meshResource;
                }
            }
        }

        auto ext = Path::getFileExtension( filePath );

        if( ext == ".fbmeshbin" || ext == ".FBMESHBIN" )
        {
            auto fileSystem = applicationManager->getFileSystem();
            auto stream = fileSystem->open( filePath );
            if( stream )
            {
                MeshSerializer serializer;
                auto mesh = serializer.loadMesh( stream );

                auto meshResource = fb::make_ptr<MeshResource>();
                meshResource->setFilePath( filePath );
                meshResource->setMesh( mesh );

                addMeshResource( meshResource );
                return mesh;
            }
        }

        if( ApplicationUtil::isSupportedMesh( filePath ) )
        {
            auto meshLoader = applicationManager->getMeshLoader();
            FB_ASSERT( meshLoader );

            if( meshLoader )
            {
                auto mesh = meshLoader->loadMesh( filePath );

                auto meshResource = fb::make_ptr<MeshResource>();
                meshResource->setFilePath( filePath );
                meshResource->setMesh( mesh );

                addMeshResource( meshResource );
                return mesh;
            }
        }

        return nullptr;
    }

    void MeshManager::saveMesh( SmartPtr<IMesh> mesh, const String &filePath )
    {
        if( mesh )
        {
            MeshSerializer serializer;
            serializer.exportMesh( static_cast<Mesh *>( mesh.get() ), filePath );
        }
    }

    void MeshManager::saveMeshToFile( SmartPtr<IMesh> mesh, const String &filePath )
    {
    }

    SmartPtr<render::ISceneNode> MeshManager::loadMeshes( const String &fileName )
    {
        return nullptr;
    }

    SmartPtr<IResource> MeshManager::create( const String &uuid )
    {
        return nullptr;
    }

    SmartPtr<IResource> MeshManager::create( const String &uuid, const String &name )
    {
        return nullptr;
    }

    Pair<SmartPtr<IResource>, bool> MeshManager::createOrRetrieve( const String &uuid,
                                                                   const String &path,
                                                                   const String &type )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( path );
            auto fileId = StringUtil::getHash64( sFilePath );

            if( auto p = getMeshResourcesPtr() )
            {
                auto &meshResources = *p;
                for( auto meshResource : meshResources )
                {
                    auto meshResourceFileId = meshResource->getFileSystemId();
                    if( meshResourceFileId == fileId )
                    {
                        return Pair<SmartPtr<IResource>, bool>( meshResource, false );
                    }
                }
            }

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( sFilePath, fileInfo ) )
            {
                auto meshResource = fb::make_ptr<MeshResource>();

                if( auto handle = meshResource->getHandle() )
                {
                    if( !StringUtil::isNullOrEmpty( uuid ) )
                    {
                        handle->setUUID( uuid );
                    }
                    else
                    {
                        handle->setUUID( StringUtil::getUUID() );
                    }
                }

                meshResource->setFileSystemId( fileId );
                meshResource->setFilePath( sFilePath );

                auto projectPath = applicationManager->getProjectPath();
                auto settingsCachePath = applicationManager->getSettingsPath();
                auto filePathHash = StringUtil::getHash64( sFilePath );
                auto fileDataPath =
                    settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
                fileDataPath = Path::getRelativePath( projectPath, fileDataPath );

                FileInfo dataFileInfo;
                if( fileSystem->findFileInfo( fileDataPath, dataFileInfo ) )
                {
                    meshResource->setSettingsFileSystemId( dataFileInfo.fileId );
                }

                addMeshResource( meshResource );
                return Pair<SmartPtr<IResource>, bool>( meshResource, true );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return {};
    }

    Pair<SmartPtr<IResource>, bool> MeshManager::createOrRetrieve( const String &path )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( path );
            auto fileId = StringUtil::getHash64( sFilePath );

            if( auto p = getMeshResourcesPtr() )
            {
                auto &meshResources = *p;
                for( auto meshResource : meshResources )
                {
                    auto meshResourceFileId = meshResource->getFileSystemId();
                    if( meshResourceFileId == fileId )
                    {
                        return Pair<SmartPtr<IResource>, bool>( meshResource, false );
                    }
                }
            }

            FileInfo fileInfo;
            if( !fileSystem->findFileInfo( sFilePath, fileInfo, false ) )
            {
                fileSystem->findFileInfo( sFilePath, fileInfo, true );
            }

            auto meshResource = fb::make_ptr<MeshResource>();

            if( auto handle = meshResource->getHandle() )
            {
                auto uuid = StringUtil::getUUID();
                handle->setUUID( uuid );
            }

            meshResource->setFileSystemId( fileId );
            meshResource->setFilePath( sFilePath );

            auto projectPath = applicationManager->getProjectPath();
            auto settingsCachePath = applicationManager->getSettingsPath();
            auto filePathHash = StringUtil::getHash64( sFilePath );
            auto fileDataPath = settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
            fileDataPath = Path::getRelativePath( projectPath, fileDataPath );

            FileInfo dataFileInfo;
            if( fileSystem->findFileInfo( fileDataPath, dataFileInfo ) )
            {
                meshResource->setSettingsFileSystemId( dataFileInfo.fileId );
            }

            addMeshResource( meshResource );
            return Pair<SmartPtr<IResource>, bool>( meshResource, true );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return {};
    }

    void MeshManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    SmartPtr<IResource> MeshManager::loadFromFile( const String &filePath )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( filePath );

            auto fileId = StringUtil::getHash64( sFilePath );

            if( auto p = getMeshResourcesPtr() )
            {
                auto &meshResources = *p;
                for( auto meshResource : meshResources )
                {
                    auto meshResourceFileId = meshResource->getFileSystemId();
                    if( meshResourceFileId == fileId )
                    {
                        return meshResource;
                    }
                }
            }

            FileInfo existingFileInfo;
            if( fileSystem->findFileInfo( filePath, existingFileInfo ) )
            {
                if( auto p = getMeshResourcesPtr() )
                {
                    auto &meshResources = *p;
                    for( auto meshResource : meshResources )
                    {
                        auto meshResourceFileId = meshResource->getFileSystemId();
                        if( meshResourceFileId == existingFileInfo.fileId )
                        {
                            return meshResource;
                        }
                    }
                }
            }

            auto fileExt = Path::getFileExtension( sFilePath );
            fileExt = StringUtil::make_lower( fileExt );

            if( ApplicationUtil::isSupportedMesh( sFilePath ) )
            {
                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto settingsCachePath = applicationManager->getSettingsPath();

                auto filePathHash = StringUtil::getHash64( sFilePath );
                auto fileDataPath =
                    settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
                fileDataPath = Path::getRelativePath( projectPath, fileDataPath );

                if( fileSystem->isExistingFile( fileDataPath ) )
                {
                    auto dataStr = fileSystem->readAllText( fileDataPath );

                    auto meshData = fb::make_ptr<Properties>();
                    DataUtil::parse( dataStr, meshData.get() );

                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( fileDataPath, fileInfo ) )
                    {
                        auto result = createOrRetrieve( sFilePath );
                        auto meshResource = fb::static_pointer_cast<IMeshResource>( result.first );
                        meshResource->setFileSystemId( fileId );
                        meshResource->setSettingsFileSystemId( fileInfo.fileId );
                        meshResource->setProperties( meshData );
                        meshResource->setFilePath( sFilePath );
                        meshResource->load( nullptr );

                        addMeshResource( meshResource );
                        return meshResource;
                    }
                }
                else
                {
                    auto meshData = fb::make_ptr<Properties>();
                    auto dataStr = DataUtil::toString( meshData.get(), true );
                    fileSystem->writeAllText( fileDataPath, dataStr );

                    auto path = Path::getFilePath( fileDataPath );
                    if( fileSystem->isExistingFile( fileDataPath ) )
                    {
                        fileSystem->refreshPath( path, true );
                    }

                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( fileDataPath, fileInfo ) )
                    {
                        auto result = createOrRetrieve( sFilePath );
                        auto meshResource = fb::static_pointer_cast<IMeshResource>( result.first );
                        meshResource->setFileSystemId( fileId );
                        meshResource->setSettingsFileSystemId( fileInfo.fileId );
                        meshResource->setFilePath( sFilePath );
                        meshResource->load( nullptr );

                        addMeshResource( meshResource );
                        return meshResource;
                    }

                    auto result = createOrRetrieve( sFilePath );
                    auto meshResource = fb::static_pointer_cast<IMeshResource>( result.first );
                    meshResource->setFileSystemId( fileId );
                    // meshResource->setSettingsFileSystemId( fileInfo.fileId );
                    meshResource->setFilePath( sFilePath );
                    meshResource->load( nullptr );

                    addMeshResource( meshResource );
                    return meshResource;
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    SmartPtr<IResource> MeshManager::loadResource( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> MeshManager::getByName( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> MeshManager::getById( const String &uuid )
    {
        return nullptr;
    }

    void MeshManager::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IMesh>>> MeshManager::getMeshesPtr() const
    {
        return m_meshes;
    }

    void MeshManager::setMeshesPtr( SharedPtr<ConcurrentArray<SmartPtr<IMesh>>> meshes )
    {
        m_meshes = meshes;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IMeshResource>>> MeshManager::getMeshResourcesPtr() const
    {
        return m_meshResources;
    }

    void MeshManager::setMeshResourcesPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IMeshResource>>> meshResources )
    {
        m_meshResources = meshResources;
    }
}  // end namespace fb
