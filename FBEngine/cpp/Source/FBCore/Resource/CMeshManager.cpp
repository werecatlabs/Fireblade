#include <FBCore/FBCorePCH.h>
#include <FBCore/Resource/CMeshManager.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Data/DefineDataStructures.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/Data.h>
#include <FBMesh/FBMesh.h> // todo remove

namespace fb
{

    CMeshManager::CMeshManager()
    {
    }

    CMeshManager::~CMeshManager()
    {
    }

    void CMeshManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            for( auto mesh : m_meshes )
            {
                mesh->unload( nullptr );
            }

            m_meshes.clear();

            for( auto meshResources : m_meshResources )
            {
                meshResources->unload( nullptr );
            }

            m_meshResources.clear();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMeshManager::addMesh( SmartPtr<IMesh> mesh )
    {
        m_meshes.push_back( mesh );
    }

    SmartPtr<IMesh> CMeshManager::findMesh( const String &name )
    {
        for( auto &mesh : m_meshes )
        {
            auto meshName = mesh->getName();
            if( meshName == name )
            {
                return mesh;
            }
        }

        return nullptr;
    }

    SmartPtr<IMesh> CMeshManager::loadMesh( const String &filePath )
    {
        auto ext = Path::getFileExtension( filePath );
        if( ext == ".fbx" || ext == ".FBX" )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto meshLoader = applicationManager->getMeshLoader();
            FB_ASSERT( meshLoader );

            if( meshLoader )
            {
                return meshLoader->loadMesh( filePath );
            }
        }
        else if( ext == ".fbmeshbin" || ext == ".FBMESHBIN" )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto fileSystem = applicationManager->getFileSystem();
            auto stream = fileSystem->open( filePath );
            if( stream )
            {
                MeshSerializer serializer;
                return serializer.loadMesh( stream );
            }
        }

        return nullptr;
    }

    void CMeshManager::saveMesh( SmartPtr<IMesh> mesh, const String &filePath )
    {
        if( mesh )
        {
            MeshSerializer serializer;
            serializer.exportMesh( static_cast<CMesh *>( mesh.get() ), filePath );
        }
    }

    void CMeshManager::saveMeshToFile( SmartPtr<IMesh> mesh, const String &filePath )
    {
        // const char* header_str =	"<?xml version=\"1.0\"?><FBMesh></FBMesh>";
        // TiXmlDocument* doc = new TiXmlDocument( filePath.c_str() );		//create a tiny xml document
        // doc->Parse(header_str);

        // TiXmlElement* root = doc->RootElement();

        ////create sub meshes node
        // TiXmlElement* subMeshesNode = new TiXmlElement("SubMeshes");
        // root->LinkEndChild(subMeshesNode);

        // const FBSubMeshList& subMeshes = mesh->getSubMeshes();
        // for(u32 i=0; i<subMeshes.size(); ++i)
        //{
        //	FBSubMeshPtr subMesh = subMeshes[i];
        //	FBIndexBufferPtr indexBuffer = subMesh->getIndexBuffer();
        //	FBVertexBufferPtr vertexBuffer = subMesh->getVertexBuffer();

        //	//create sub mesh node
        //	TiXmlElement* subMeshNode = new TiXmlElement("SubMesh");
        //	subMeshesNode->LinkEndChild(subMeshNode);

        //	//create material name node
        //	subMeshNode->SetAttribute("MaterialName", subMesh->getMaterialName().c_str());

        //	//create indicies node
        //	TiXmlElement* indiciesNode = new TiXmlElement("Indices");
        //	subMeshNode->LinkEndChild(indiciesNode);

        //	// fill indices
        //	switch(indexBuffer->getIndexType())
        //	{
        //	case fb::IT_16BIT:
        //		{
        //			const u16* indexData = reinterpret_cast<u16*>(indexBuffer->getIndexData());
        //			const u32 indexCount = indexBuffer->getNumIndices();
        //			for(u32 j=0;j<indexCount;j++)
        //			{
        //				u32 index = (u32)indexData[j];
        //			}
        //		}
        //		break;
        //	case fb::IT_32BIT:
        //		{
        //			const u32* indexData = reinterpret_cast<u32*>(indexBuffer->getIndexData());
        //			const u32 indexCount = indexBuffer->getNumIndices();
        //			for(u32 j=0;j<indexCount;j++)
        //			{
        //				u32 index = indexData[j];
        //			}
        //		}
        //		break;
        //	default:
        //		{
        //			_FB_DEBUG_BREAK_IF(true);
        //		}
        //	};

        //	//create vertices node
        //	TiXmlElement* verticesNode = new TiXmlElement("Vertices");
        //	subMeshNode->LinkEndChild(verticesNode);

        //	const FBVertexElement* posElem =
        // vertexBuffer->getVertexDeclaration()->findElementBySemantic(VES_POSITION); 	u32 numVerticies
        // = vertexBuffer->getNumVerticies(); 	u32 vertexSize =
        // vertexBuffer->getVertexDeclaration()->getSize(); 	u8* vertexDataPtr =
        //(u8*)vertexBuffer->getVertexData(); 	f32* elementData = 0;

        //	for(u32 j=0; j<numVerticies; ++j, vertexDataPtr += vertexSize)
        //	{
        //		posElem->getElementData(vertexDataPtr, &elementData);

        //		Vector3F position;
        //		position.X() = *elementData++;
        //		position.Y() = *elementData++;
        //		position.Z() = *elementData++;

        //		//create vertex node
        //		TiXmlElement* vertexNode = new TiXmlElement("Vertex");
        //		verticesNode->LinkEndChild(vertexNode);

        //		//create position node
        //		TiXmlElement* positionNode = new TiXmlElement("Position");
        //		vertexNode->LinkEndChild(positionNode);

        //		positionNode->SetAttribute("X", StringUtil::toString(position.X()).c_str());
        //		positionNode->SetAttribute("Y", StringUtil::toString(position.Y()).c_str());
        //		positionNode->SetAttribute("Z", StringUtil::toString(position.Z()).c_str());
        //	}
        //}

        // doc->SaveFile();

        // delete doc;
    }

    SmartPtr<render::ISceneNode> CMeshManager::loadMeshes( const String &fileName )
    {
        return nullptr;
    }

    SmartPtr<IResource> CMeshManager::create( const String &uuid )
    {
        return nullptr;
    }

    SmartPtr<IResource> CMeshManager::create( const String &uuid, const String &name )
    {
        return nullptr;
    }

    Pair<SmartPtr<IResource>, bool> CMeshManager::createOrRetrieve( const String &uuid,
                                                                    const String &path,
                                                                    const String &type )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( path );
            auto fileId = StringUtil::getHash64( sFilePath );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( sFilePath, fileInfo ) )
            {
                auto meshResource = fb::make_ptr<CMeshResource>();

                if( auto handle = meshResource->getHandle() )
                {
                    handle->setUUID( uuid );
                }

                meshResource->setFileSystemId( fileId );
                meshResource->setSettingsFileSystemId( fileInfo.fileId );
                meshResource->setMeshPath( sFilePath );

                m_meshResources.push_back( meshResource );
                return Pair<SmartPtr<IResource>, bool>( meshResource, true );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>();
    }

    Pair<SmartPtr<IResource>, bool> CMeshManager::createOrRetrieve( const String &path )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( path );
            auto fileId = StringUtil::getHash64( sFilePath );

            FileInfo fileInfo;
            if( !fileSystem->findFileInfo( sFilePath, fileInfo, false ) )
            {
                fileSystem->findFileInfo( sFilePath, fileInfo, true );
            }

            auto meshResource = fb::make_ptr<CMeshResource>();

            if( auto handle = meshResource->getHandle() )
            {
                auto uuid = StringUtil::getUUID();
                handle->setUUID( uuid );
            }

            meshResource->setFileSystemId( fileId );
            meshResource->setSettingsFileSystemId( fileInfo.fileId );
            meshResource->setMeshPath( sFilePath );

            m_meshResources.push_back( meshResource );
            return Pair<SmartPtr<IResource>, bool>( meshResource, true );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return Pair<SmartPtr<IResource>, bool>();
    }

    void CMeshManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    SmartPtr<IResource> CMeshManager::loadFromFile( const String &filePath )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto sFilePath = StringUtil::cleanupPath( filePath );

            auto fileId = StringUtil::getHash64( sFilePath );
            for( auto meshResource : m_meshResources )
            {
                auto meshResourceFileId = meshResource->getFileSystemId();
                if( meshResourceFileId == fileId )
                {
                    return meshResource;
                }
            }

            FileInfo existingFileInfo;
            if( fileSystem->findFileInfo( filePath, existingFileInfo ) )
            {
                for( auto meshResource : m_meshResources )
                {
                    auto meshResourceFileId = meshResource->getFileSystemId();
                    if( meshResourceFileId == existingFileInfo.fileId )
                    {
                        return meshResource;
                    }
                }
            }

            auto fileExt = Path::getFileExtension( sFilePath );
            fileExt = StringUtil::make_lower( fileExt );

            if( fileExt == ".mesh" || fileExt == ".fbmeshbin" || fileExt == ".fbx" )
            {
                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto settingsCachePath = applicationManager->getSettingsCachePath();

                auto filePathHash = StringUtil::getHash64( sFilePath );
                auto fileDataPath =
                    settingsCachePath + StringUtil::toString( filePathHash ) + ".meshdata";
                fileDataPath = Path::getRelativePath( projectPath, fileDataPath );

                if( fileSystem->isExistingFile( fileDataPath ) )
                {
                    auto dataStr = fileSystem->readAllText( fileDataPath );

                    auto data = fb::make_ptr<Data<data::mesh_importer_data>>();
                    auto meshData = data->getDataAsType<data::mesh_importer_data>();
                    // auto pData = meshData.get();
                    DataUtil::parse( dataStr, meshData );

                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( fileDataPath, fileInfo ) )
                    {
                        auto meshResource = fb::make_ptr<CMeshResource>();
                        meshResource->setFileSystemId( fileId );
                        meshResource->setSettingsFileSystemId( fileInfo.fileId );
                        meshResource->fromData( data );
                        meshResource->setMeshPath( sFilePath );
                        meshResource->load( nullptr );

                        m_meshResources.push_back( meshResource );
                        return meshResource;
                    }
                }
                else
                {
                    data::mesh_importer_data meshData;
                    auto dataStr = DataUtil::toString( &meshData, true );
                    fileSystem->writeAllText( fileDataPath, dataStr );

                    auto path = Path::getFilePath( fileDataPath );
                    fileSystem->refreshPath( path, true );

                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( fileDataPath, fileInfo ) )
                    {
                        auto meshResource = fb::make_ptr<CMeshResource>();
                        meshResource->setFileSystemId( fileId );
                        meshResource->setSettingsFileSystemId( fileInfo.fileId );
                        meshResource->setMeshPath( sFilePath );
                        meshResource->load( nullptr );

                        m_meshResources.push_back( meshResource );
                        return meshResource;
                    }
                    auto meshResource = fb::make_ptr<CMeshResource>();
                    meshResource->setFileSystemId( fileId );
                    // meshResource->setSettingsFileSystemId( fileInfo.fileId );
                    meshResource->setMeshPath( sFilePath );
                    meshResource->load( nullptr );

                    m_meshResources.push_back( meshResource );
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

    SmartPtr<IResource> CMeshManager::load( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> CMeshManager::getByName( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> CMeshManager::getById( const String &uuid )
    {
        return nullptr;
    }

    void CMeshManager::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }
}  // end namespace fb
