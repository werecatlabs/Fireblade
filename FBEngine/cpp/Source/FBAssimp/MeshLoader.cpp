#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/MeshLoader.h>
#include <FBAssimp/LogStream.h>
#include <FBAssimp/IOSystem.h>
#include <FBMesh/FBMesh.h>
#include <FBCore/FBCore.h>

#if FB_USE_FBXSDK
#    include <FBMesh/FBX/FBXConverter.h>
#elif FB_USE_ASSET_IMPORT
#    include <assimp/DefaultLogger.hpp>
#endif

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MeshLoader, IMeshLoader );

    MeshLoader::MeshLoader()
    {
        Assimp::DefaultLogger::create( "" );
        Assimp::DefaultLogger::get()->attachStream( new LogStream(), ~Assimp::DefaultLogger::Err );
        Assimp::DefaultLogger::get()->attachStream( new LogStream(), Assimp::DefaultLogger::Err );

        setUseSingleMesh( false );
        m_meshScale = Vector3<f32>::unit() * 1.0f;
    }

    MeshLoader::~MeshLoader()
    {
        unload( nullptr );
    }

    void MeshLoader::createMaterials( Array<render::IMaterial> &materials, const aiScene *mScene,
                                      const aiNode *pNode, const String &mDir )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto numMeshes = pNode->mNumMeshes;

        String meshFilePath;

        for( u32 idx = 0; idx < numMeshes; ++idx )
        {
            auto meshIndex = pNode->mMeshes[idx];

            auto pAIMesh = mScene->mMeshes[meshIndex];
            FB_ASSERT( pAIMesh );

            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", String( "SubMesh " ) + StringUtil::toString( idx ) +
                                                  String( " for mesh '" ) + String( pNode->mName.data ) +
                                                  "'" );
            }

            // Create a material instance for the mesh.
            const auto pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];

            auto fileCachePath = applicationManager->getCachePath();
            auto fileMeshCachePath = fileCachePath + meshFilePath;

            if( !fileSystem->isExistingFile( fileMeshCachePath ) )
            {
                auto nodeNode = String( pNode->mName.data );
                //createSubMesh( nodeNode, idx, pNode, pAIMesh, pAIMaterial, mesh, mDir );
            }
        }
    }

    auto MeshLoader::createMaterials( const String &meshPath ) -> Array<render::IMaterial>
    {
        auto materials = Array<render::IMaterial>();

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto fileExt = Path::getFileExtension( meshPath );
        fileExt = StringUtil::make_lower( fileExt );

        if( fileExt == ".fbmeshbin" )
        {
        }
        else if( ApplicationUtil::isSupportedMesh( meshPath ) )
        {
#if FB_USE_FBXSDK
#elif FB_USE_ASSET_IMPORT
            Assimp::Importer importer;

            u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;

            auto stream = fileSystem->open( m_meshPath );
            if( stream )
            {
                auto size = stream->size();
                auto buffer = new u8[size];

                stream->read( buffer, size );

                const auto scene = importer.ReadFileFromMemory( buffer, size, flags );
                if( scene )
                {
                    auto hash = StringUtil::getHash64( m_meshPath );
                    m_fileHash = *&hash;

                    computeNodesDerivedTransform( scene, scene->mRootNode,
                                                  scene->mRootNode->mTransformation );

                    //auto name = Path::getFileNameWithoutExtension( m_meshPath );
                    //actor->setName( name );

                    //auto path = Path::getFilePath( m_meshPath );
                    createMaterials( materials, scene, scene->mRootNode, meshPath );
                }
            }
#endif
        }

        return materials;
    }

    auto MeshLoader::loadActor( SmartPtr<IMeshResource> resource ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        if( resource )
        {
            setMeshResource( resource );

            m_meshScale = Vector3<f32>::unit() * resource->getScale();
            m_meshPath = resource->getFilePath();

            m_director = resourceDatabase->loadDirectorFromResourcePath(
                m_meshPath, scene::MeshResourceDirector::typeInfo() );
        }

        FB_ASSERT( !StringUtil::isNullOrEmpty( m_meshPath ) );

        auto fileExt = Path::getFileExtension( m_meshPath );
        fileExt = StringUtil::make_lower( fileExt );

        if( fileExt == ".fbmeshbin" )
        {
            MeshSerializer serializer;

            auto stream = fileSystem->open( m_meshPath, true, true, false );
            if( stream )
            {
                auto mesh = fb::make_ptr<Mesh>();
                serializer.importMesh( stream, mesh.get() );

                auto actor = sceneManager->createActor();
                auto meshComponent = actor->addComponent<scene::Mesh>();
                if( meshComponent )
                {
                    meshComponent->setMeshResource( resource );
                }

                auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

                return actor;
            }
        }
        else if( ApplicationUtil::isSupportedMesh( m_meshPath ) )
        {
            auto actor = sceneManager->createActor();
            setRootActor( actor );

#if FB_USE_FBXSDK
            auto outputPath = ".FBCache/" + String( "f40.mesh" );

            FBXConverter e;
            // e.exportScene(meshName.c_str(), outputPath.c_str(), "fbx.log", "", "", false, 0);
            auto sceneRoot = e.loadScene( meshName );
            if( sceneRoot )
            {
                auto name = sceneRoot->getName();
                actor->setName( name );

                auto localTransform = sceneRoot->getLocalTransform();
                auto worldTransform = sceneRoot->getWorldTransform();

                auto pLocalTransform = actor->getLocalTransform();
                *pLocalTransform = localTransform;

                auto pWorldTransform = actor->getTransform();
                *pWorldTransform = worldTransform;

                auto children = sceneRoot->getChildren();
                for( auto child : children )
                {
                    createActor( child, actor );
                }
            }

#elif FB_USE_ASSET_IMPORT
            Assimp::Importer importer;

            u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;

            auto stream = fileSystem->open( m_meshPath );
            if( stream )
            {
                auto size = stream->size();
                auto buffer = new u8[size];

                stream->read( buffer, size );

                const auto scene = importer.ReadFileFromMemory( buffer, size, flags );
                if( scene )
                {
                    auto hash = StringUtil::getHash64( m_meshPath );
                    m_fileHash = *&hash;

                    computeNodesDerivedTransform( scene, scene->mRootNode,
                                                  scene->mRootNode->mTransformation );

                    auto name = Path::getFileNameWithoutExtension( m_meshPath );
                    actor->setName( name );

                    auto path = Path::getFilePath( m_meshPath );
                    loadDataFromActor( actor, scene, scene->mRootNode, path );

                    actor->updateTransform();
                }
            }
#endif

            return actor;
        }

        return nullptr;
    }

    auto MeshLoader::loadActor( const String &meshName ) -> SmartPtr<scene::IActor>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( meshName ) );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto meshManager = applicationManager->getMeshManager();
            FB_ASSERT( meshManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto resource = meshManager->loadFromFile( meshName );
            if( !resource )
            {
                return nullptr;
            }

            auto meshResource = fb::static_pointer_cast<IMeshResource>( resource );
            if( meshResource )
            {
                setMeshResource( meshResource );

                m_meshScale = Vector3<f32>::unit() * meshResource->getScale();
                m_meshPath = resource->getFilePath();

                m_director = resourceDatabase->loadDirectorFromResourcePath(
                    m_meshPath, scene::MeshResourceDirector::typeInfo() );
            }

            m_meshPath = meshName;

            auto fileExt = Path::getFileExtension( meshName );
            fileExt = StringUtil::make_lower( fileExt );

            if( fileExt == ".fbmeshbin" )
            {
                MeshSerializer serializer;

                auto stream = fileSystem->open( m_meshPath, true, true, false );
                if( stream )
                {
                    auto mesh = fb::make_ptr<Mesh>();
                    serializer.importMesh( stream, mesh.get() );

                    auto actor = sceneManager->createActor();
                    auto meshComponent = actor->addComponent<scene::Mesh>();
                    if( meshComponent )
                    {
                        //meshComponent->setMeshResource( mesh );
                    }

                    auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

                    return actor;
                }
            }
            else if( ApplicationUtil::isSupportedMesh( meshName ) )
            {
                auto actor = sceneManager->createActor();
                setRootActor( actor );

#if FB_USE_FBXSDK
                auto outputPath = ".FBCache/" + String( "f40.mesh" );

                FBXConverter e;
                // e.exportScene(meshName.c_str(), outputPath.c_str(), "fbx.log", "", "", false, 0);
                auto sceneRoot = e.loadScene( meshName );
                if( sceneRoot )
                {
                    auto name = sceneRoot->getName();
                    actor->setName( name );

                    auto localTransform = sceneRoot->getLocalTransform();
                    auto worldTransform = sceneRoot->getWorldTransform();

                    auto pLocalTransform = actor->getLocalTransform();
                    *pLocalTransform = localTransform;

                    auto pWorldTransform = actor->getTransform();
                    *pWorldTransform = worldTransform;

                    auto children = sceneRoot->getChildren();
                    for( auto child : children )
                    {
                        createActor( child, actor );
                    }
                }

#elif FB_USE_ASSET_IMPORT
                Assimp::Importer importer;
                //importer.SetIOHandler( new IOSystem() );

                u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;

                auto stream = fileSystem->open( meshName, true, true, false, false, false );
                if( !stream )
                {
                    stream = fileSystem->open( meshName, true, true, false, true, true );
                }

                auto projectFolder = applicationManager->getProjectPath();
                auto meshFilePath = resource->getFilePath();
                auto absolutePath = Path::getAbsolutePath( projectFolder, meshFilePath );

                if( stream )
                {
                    auto size = stream->size();
                    auto buffer = new u8[size];

                    stream->read( buffer, size );

                    //const auto scene = importer.ReadFile( absolutePath.c_str(), flags );

                    auto ext = Path::getFileExtension( meshName );
                    ext = StringUtil::make_lower( ext );
                    ext = StringUtil::replaceAll( ext, ".", "" );

                    Assimp::Importer importer;
                    u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;
                    auto scene = importer.ReadFile( absolutePath.c_str(), flags );
                    if( !scene )
                    {
                        // The line below causes a weird material index bug.
                        scene = importer.ReadFileFromMemory( buffer, size, flags, ext.c_str() );
                    }

                    if( scene )
                    {
                        auto numMaterials = scene->mNumMaterials;
                        for( size_t i = 0; i < numMaterials; i++ )
                        {
                            auto pAIMaterial = scene->mMaterials[i];
                            createMaterial( nullptr, i, pAIMaterial, "" );
                        }

                        auto hash = StringUtil::getHash64( meshName );
                        m_fileHash = *&hash;

                        computeNodesDerivedTransform( scene, scene->mRootNode,
                                                      scene->mRootNode->mTransformation );

                        auto name = Path::getFileNameWithoutExtension( meshName );
                        actor->setName( name );

                        auto path = Path::getFilePath( meshName );
                        loadDataFromActor( actor, scene, scene->mRootNode, path );
                    }
                }
#endif

                return actor;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto MeshLoader::loadMesh( SmartPtr<IMeshResource> resource ) -> SmartPtr<IMesh>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        if( resource )
        {
            setMeshResource( resource );
            m_meshScale = Vector3<f32>::unit() * resource->getScale();
            m_meshPath = resource->getFilePath();

            m_director = resourceDatabase->loadDirectorFromResourcePath(
                m_meshPath, scene::MeshResourceDirector::typeInfo() );
        }

        FB_ASSERT( !StringUtil::isNullOrEmpty( m_meshPath ) );

        auto fileExt = Path::getFileExtension( m_meshPath );
        fileExt = StringUtil::make_lower( fileExt );

        if( fileExt == ".fbmeshbin" )
        {
            MeshSerializer serializer;

            auto stream = fileSystem->open( m_meshPath, true, true, false, false, false );
            if( !stream )
            {
                stream = fileSystem->open( m_meshPath, true, true, false, true, true );
            }

            if( stream )
            {
                auto mesh = fb::make_ptr<Mesh>();
                // serializer.importMesh(stream, mesh.get());
                serializer.importMesh( stream, mesh.get() );
                return mesh;
            }
        }
        else if( ApplicationUtil::isSupportedMesh( m_meshPath ) )
        {
            auto mesh = fb::make_ptr<Mesh>();

#if FB_USE_FBXSDK
            auto outputPath = ".FBCache/" + String( "f40.mesh" );

            FBXConverter e;
            // e.exportScene(meshName.c_str(), outputPath.c_str(), "fbx.log", "", "", false, 0);
            auto sceneRoot = e.loadScene( meshName );
            if( sceneRoot )
            {
                auto name = sceneRoot->getName();
                actor->setName( name );

                auto localTransform = sceneRoot->getLocalTransform();
                auto worldTransform = sceneRoot->getWorldTransform();

                auto pLocalTransform = actor->getLocalTransform();
                *pLocalTransform = localTransform;

                auto pWorldTransform = actor->getTransform();
                *pWorldTransform = worldTransform;

                auto children = sceneRoot->getChildren();
                for( auto child : children )
                {
                    createActor( child, actor );
                }
            }

#elif FB_USE_ASSET_IMPORT
            auto projectFolder = applicationManager->getProjectPath();
            auto absolutePath = Path::getAbsolutePath( projectFolder, m_meshPath );

            Assimp::Importer importer;
            u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;
            const auto scene = importer.ReadFile( absolutePath.c_str(), flags );
            if( scene )
            {
                auto hash = StringUtil::getHash64( m_meshPath );
                m_fileHash = *&hash;

                computeNodesDerivedTransform( scene, scene->mRootNode,
                                              scene->mRootNode->mTransformation );

                auto name = Path::getFileNameWithoutExtension( m_meshPath );
                mesh->setName( name );

                auto numMaterials = scene->mNumMaterials;
                for( size_t i = 0; i < numMaterials; i++ )
                {
                    auto pAIMaterial = scene->mMaterials[i];
                    createMaterial( nullptr, i, pAIMaterial, "" );
                }

                auto path = Path::getFilePath( m_meshPath );
                loadToMesh( mesh, scene, scene->mRootNode, path );
            }

            return mesh;
#endif
        }

        return nullptr;
    }

    auto MeshLoader::loadMesh( const String &meshName ) -> SmartPtr<IMesh>
    {
        FB_ASSERT( !StringUtil::isNullOrEmpty( meshName ) );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        // auto graphicsSystem = applicationManager->getGraphicsSystem();
        // FB_ASSERT(graphicsSystem);

        auto meshManager = applicationManager->getMeshManager();
        FB_ASSERT( meshManager );

        auto resource = meshManager->loadFromFile( meshName );
        auto meshResource = fb::static_pointer_cast<IMeshResource>( resource );
        if( meshResource )
        {
            m_meshScale = Vector3<f32>::unit() * meshResource->getScale();
            m_meshPath = resource->getFilePath();

            m_director = resourceDatabase->loadDirectorFromResourcePath(
                m_meshPath, scene::MeshResourceDirector::typeInfo() );
        }

        m_meshPath = meshName;

        auto fileExt = Path::getFileExtension( meshName );
        fileExt = StringUtil::make_lower( fileExt );

        if( fileExt == ".fbmeshbin" )
        {
            MeshSerializer serializer;

            auto stream = fileSystem->open( m_meshPath );
            if( stream )
            {
                auto mesh = fb::make_ptr<Mesh>();
                serializer.importMesh( stream, mesh.get() );
                return mesh;
            }
        }
        else if( ApplicationUtil::isSupportedMesh( m_meshPath ) )
        {
            auto mesh = fb::make_ptr<Mesh>();

#if FB_USE_FBXSDK
            auto outputPath = ".FBCache/" + String( "f40.mesh" );

            FBXConverter e;
            // e.exportScene(meshName.c_str(), outputPath.c_str(), "fbx.log", "", "", false, 0);
            auto sceneRoot = e.loadScene( meshName );
            if( sceneRoot )
            {
                auto name = sceneRoot->getName();
                actor->setName( name );

                auto localTransform = sceneRoot->getLocalTransform();
                auto worldTransform = sceneRoot->getWorldTransform();

                auto pLocalTransform = actor->getLocalTransform();
                *pLocalTransform = localTransform;

                auto pWorldTransform = actor->getTransform();
                *pWorldTransform = worldTransform;

                auto children = sceneRoot->getChildren();
                for( auto child : children )
                {
                    createActor( child, actor );
                }
            }

#elif FB_USE_ASSET_IMPORT
            auto projectFolder = applicationManager->getProjectPath();
            auto absolutePath = Path::getAbsolutePath( projectFolder, meshName );

            Assimp::Importer importer;
            u32 flags = aiProcessPreset_TargetRealtime_MaxQuality;
            const auto scene = importer.ReadFile( absolutePath.c_str(), flags );
            if( scene )
            {
                auto hash = StringUtil::getHash64( meshName );
                m_fileHash = *&hash;

                auto numMaterials = scene->mNumMaterials;
                for( size_t i = 0; i < numMaterials; i++ )
                {
                    auto pAIMaterial = scene->mMaterials[i];
                    createMaterial( nullptr, i, pAIMaterial, "" );
                }

                computeNodesDerivedTransform( scene, scene->mRootNode,
                                              scene->mRootNode->mTransformation );

                auto name = Path::getFileNameWithoutExtension( meshName );
                mesh->setName( name );

                auto path = Path::getFilePath( meshName );
                loadToMesh( mesh, scene, scene->mRootNode, meshName );
            }

            return mesh;
#endif
        }

        return nullptr;
    }

#if FB_USE_FBXSDK
#elif FB_USE_ASSET_IMPORT

    void MeshLoader::loadDataFromActor( SmartPtr<scene::IActor> parent, const aiScene *mScene,
                                        const aiNode *pNode, const String &mDir )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto sceneManager = applicationManager->getSceneManager();
            auto fileSystem = applicationManager->getFileSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto meshManager = applicationManager->getMeshManager();
            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto actor = sceneManager->createActor();
            parent->addChild( actor );

            auto name = pNode->mName;
            actor->setName( name.C_Str() );

            auto localTransform = pNode->mTransformation;
            localTransform = localTransform.Inverse();

            aiQuaternion rot;
            aiVector3D pos;
            aiVector3D scale;

            localTransform.Decompose( scale, rot, pos );

            aiMatrix4x4 aiM;

            auto nodeTransformData = m_nodeDerivedTransformByName.find( pNode->mName.data );
            if( nodeTransformData != m_nodeDerivedTransformByName.end() )
            {
                aiM = nodeTransformData->second;
            }

            aiM.Decompose( scale, rot, pos );

            auto actorTransform = actor->getTransform();
            FB_ASSERT( actorTransform );

            parent->updateTransform();

            auto parentTransform = parent->getTransform();
            auto parentWorldTransform = parentTransform->getWorldTransform();
            auto parentLocalTransform = parentTransform->getLocalTransform();

            FB_ASSERT( parentWorldTransform.isSane() );
            FB_ASSERT( parentLocalTransform.isSane() );

            auto actorPosition = Vector3<real_Num>( pos.x, pos.y, pos.z ) * m_meshScale;
            auto actorOrientation = Quaternion<real_Num>( rot.w, rot.x, rot.y, rot.z );
            auto actorScale = Vector3<real_Num>( scale.x, scale.y, scale.z );

            if( !actorPosition.isFinite() )
            {
                actorPosition = Vector3<real_Num>::zero();
            }

            if( !actorOrientation.isSane() )
            {
                actorOrientation = Quaternion<real_Num>::identity();
            }

            if( !actorScale.isFinite() )
            {
                actorScale = Vector3<real_Num>::unit();
            }

            FB_ASSERT( actorPosition.isValid() );
            FB_ASSERT( actorOrientation.isValid() );
            FB_ASSERT( actorScale.isValid() );

            if( !localTransform.IsIdentity() )
            {
                auto actorLocalTransform = actorTransform->getLocalTransform();
                auto actorWorldTransform = actorTransform->getWorldTransform();

                Transform3<real_Num> t( actorPosition, actorOrientation, actorScale );
                actorLocalTransform.fromWorldToLocal( parentWorldTransform, t );

                FB_ASSERT( actorLocalTransform.isSane() );

                actorWorldTransform.setPosition( actorPosition );
                actorWorldTransform.setOrientation( actorOrientation );
                actorWorldTransform.setScale( actorScale );

                auto actorLocalPosition = actorLocalTransform.getPosition();
                auto actorLocalOrientation = actorLocalTransform.getOrientation();
                auto actorLocalScale = actorLocalTransform.getScale();

                actorTransform->setPosition( actorPosition );
                actorTransform->setOrientation( actorOrientation );
                actorTransform->setScale( actorScale );

                actorTransform->setLocalDirty( true );

                FB_ASSERT( actorWorldTransform.isSane() );

                actorTransform->updateLocalFromWorld();
            }

            auto numMeshes = pNode->mNumMeshes;

            static const auto meshExt = String( ".fbmeshbin" );
            auto nodeName = String( pNode->mName.data );

            auto cachePath = applicationManager->getCachePath();
            auto meshFilePath = "Cache/" + nodeName + "_" + StringUtil::toString( m_fileHash ) + meshExt;
            meshFilePath = StringUtil::cleanupPath( meshFilePath );

            if( numMeshes > 0 )
            {
                auto resource = meshManager->createOrRetrieve( meshFilePath );
                auto meshResource = fb::static_pointer_cast<MeshResource>( resource.first );

                if( meshResource )
                {
                    auto mesh = meshResource->getMesh();

                    //if( !Path::isExistingFile(meshFilePath) )
                    {
                        if( !mesh )
                        {
                            mesh = fb::make_ptr<Mesh>();
                            meshResource->setMesh( mesh );
                        }

                        for( u32 idx = 0; idx < numMeshes; ++idx )
                        {
                            auto meshIndex = pNode->mMeshes[idx];

                            auto pAIMesh = mScene->mMeshes[meshIndex];
                            FB_ASSERT( pAIMesh );

                            if( !m_quietMode )
                            {
                                FB_LOG_MESSAGE( "MeshLoader", String( "SubMesh " ) +
                                                                  StringUtil::toString( idx ) +
                                                                  String( " for mesh '" ) +
                                                                  String( pNode->mName.data ) + "'" );
                            }

                            // Create a material instance for the mesh.
                            const auto pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];

                            auto fileCachePath = applicationManager->getCachePath();
                            auto fileMeshCachePath = fileCachePath + meshFilePath;

                            if( !fileSystem->isExistingFile( fileMeshCachePath ) )
                            {
                                auto nodeNode = String( pNode->mName.data );
                                createSubMesh( nodeNode, idx, pNode, pAIMesh, pAIMaterial, mesh, mDir );
                            }
                        }
                    }

                    auto meshComponent = actor->addComponent<scene::Mesh>();
                    if( meshComponent )
                    {
                        meshComponent->setMeshPath( meshFilePath );
                        meshComponent->setMeshResource( meshResource );

                        if( mesh )
                        {
                            mesh->setName( meshFilePath );
                        }
                    }
                }

                auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
                if( meshRenderer )
                {
                    //
                }

                /*
                if( auto meshComponent = actor->getComponent<scene::Mesh>() )
                {
                    if( auto mesh = meshComponent->getMesh() )
                    {
                        auto count = 0;
                        auto subMeshes = mesh->getSubMeshes();
                        for( auto subMesh : subMeshes )
                        {
                            auto materialName = subMesh->getMaterialName();
                            auto fbMaterial =
                                resourceDatabase->loadResourceByType<render::IMaterial>( materialName );
                            if( fbMaterial )
                            {
                                auto materialComponent = actor->addComponent<scene::Material>();
                                if( materialComponent )
                                {
                                    materialComponent->setIndex( count );
                                    materialComponent->setMaterial( fbMaterial );
                                    materialComponent->updateMaterial();
                                }
                            }

                            count++;
                        }
                    }
                }*/

                for( u32 idx = 0; idx < pNode->mNumMeshes; ++idx )
                {
                    auto meshIndex = pNode->mMeshes[idx];
                    auto pAIMesh = mScene->mMeshes[meshIndex];
                    FB_ASSERT( pAIMesh );

                    auto materialIndex = pAIMesh->mMaterialIndex;
                    const auto pAIMaterial = mScene->mMaterials[materialIndex];
                    FB_ASSERT( pAIMaterial );

                    auto materialName = pAIMaterial->GetName();
                    auto pMaterialName = materialName.C_Str();
                    auto materialNameStr = String( pMaterialName );
                    auto materialPath = materialNameStr + ".mat";

                    auto fbMaterial =
                        resourceDatabase->loadResourceByType<render::IMaterial>( materialPath );

                    if( pAIMaterial )
                    {
                        auto materialComponent = actor->addComponent<scene::Material>();
                        if( materialComponent )
                        {
                            auto pName = pAIMaterial->GetName().C_Str();

                            materialComponent->setIndex( idx );
                            materialComponent->setMaterial( fbMaterial );
                            materialComponent->updateMaterial();
                        }
                    }
                }

                actor->updateTransform();
            }

            // Traverse all child nodes of the current node instance
            for( u32 childIdx = 0; childIdx < pNode->mNumChildren; childIdx++ )
            {
                const auto pChildNode = pNode->mChildren[childIdx];
                loadDataFromActor( actor, mScene, pChildNode, mDir );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshLoader::loadToMesh( SmartPtr<IMesh> mesh, const aiScene *mScene, const aiNode *pNode,
                                 const String &mDir )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto fileSystem = applicationManager->getFileSystem();

            auto name = pNode->mName;

            auto localTransform = pNode->mTransformation;
            localTransform = localTransform.Inverse();

            aiQuaternion rot;
            aiVector3D pos;
            aiVector3D scale;

            localTransform.Decompose( scale, rot, pos );

            aiMatrix4x4 aiM;

            auto nodeTransformData = m_nodeDerivedTransformByName.find( pNode->mName.data );
            if( nodeTransformData != m_nodeDerivedTransformByName.end() )
            {
                aiM = nodeTransformData->second;
            }

            aiM.Decompose( scale, rot, pos );

            /*
            SmartPtr<TransformComponent> actorTransform = actor->getTransform();
            auto parentWorldTransform = actorTransform->getWorldTransform();
            auto parentLocalTransform = actorTransform->getLocalTransform();

            FB_ASSERT(parentWorldTransform.isSane());
            FB_ASSERT(parentLocalTransform.isSane());

            parent->updateTransform();

            auto actorPosition = Vector3<real_Num>(pos.x, pos.y, pos.z) * m_meshScale;
            auto actorOrientation = Quaternion<real_Num>(rot.w, rot.x, rot.y, rot.z);
            auto actorScale = Vector3<real_Num>(scale.x, scale.y, scale.z);

            actorPosition *= actorScale;

            FB_ASSERT(actorPosition.isValid());
            FB_ASSERT(actorOrientation.isValid());
            FB_ASSERT(actorScale.isValid());

            if (!localTransform.IsIdentity())
            {
                auto actorLocalTransform = actorTransform->getLocalTransform();
                auto actorWorldTransform = actorTransform->getWorldTransform();

                Transform3<real_Num> t(actorPosition, actorOrientation, actorScale);
                actorLocalTransform.fromWorldToLocal(parentWorldTransform, t);

                FB_ASSERT(actorLocalTransform.isSane());

                actorWorldTransform.setPosition(actorPosition);
                actorWorldTransform.setOrientation(actorOrientation);
                actorWorldTransform.setScale(actorScale);

                auto actorLocalPosition = actorLocalTransform.getPosition();
                auto actorLocalOrientation = actorLocalTransform.getOrientation();
                auto actorLocalScale = actorLocalTransform.getScale();

                actorTransform->setLocalPosition(actorLocalPosition);
                actorTransform->setLocalOrientation(actorLocalOrientation);
                actorTransform->setLocalScale(actorLocalScale);

                actorTransform->setPosition(actorPosition);
                actorTransform->setOrientation(actorOrientation);
                actorTransform->setScale(actorScale);

                FB_ASSERT(actorWorldTransform.isSane());

                //actorTransform->updateWorldFromLocal();
                //actorTransform->updateLocalFromWorld();
                actor->updateTransform();
            }
            */

            auto numMeshes = pNode->mNumMeshes;

            static const auto engineMeshExtention = String( ".fbmeshbin" );
            auto meshFilePath = String( pNode->mName.data ) + "_" + StringUtil::toString( m_fileHash ) +
                                engineMeshExtention;

            if( numMeshes > 0 )
            {
                if( getUseSingleMesh() )
                {
                    // if(mMeshes.size() == 0)
                    //{
                    //	static int nameExt = 0;
                    //	mesh = m_meshMgr->create(String("ROOTMesh") + StringUtil::toString(nameExt++),
                    //IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

                    //	SmartPtr<IMesh> pMesh(new CMesh);
                    //	mesh->setMesh(pMesh);

                    //	mMeshes.push_back(mesh);
                    //}
                    // else
                    //{
                    //	mesh = mMeshes[0];
                    //}
                }

                for( u32 idx = 0; idx < numMeshes; ++idx )
                {
                    auto meshIndex = pNode->mMeshes[idx];

                    auto pAIMesh = mScene->mMeshes[meshIndex];
                    FB_ASSERT( pAIMesh );

                    if( !m_quietMode )
                    {
                        FB_LOG_MESSAGE( "MeshLoader", String( "SubMesh " ) +
                                                          StringUtil::toString( idx ) +
                                                          String( " for mesh '" ) +
                                                          String( pNode->mName.data ) + "'" );
                    }

                    // Create a material instance for the mesh.
                    const auto pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];

                    auto fileCachePath = applicationManager->getCachePath();
                    auto fileMeshCachePath = fileCachePath + meshFilePath;

                    if( !fileSystem->isExistingFile( fileMeshCachePath ) )
                    {
                        auto nodeNode = String( pNode->mName.data );
                        createSubMesh( nodeNode, idx, pNode, pAIMesh, pAIMaterial, mesh, mDir );
                    }
                }

                for( u32 idx = 0; idx < pNode->mNumMeshes; ++idx )
                {
                    auto meshIndex = pNode->mMeshes[idx];
                    auto pAIMesh = mScene->mMeshes[meshIndex];
                    FB_ASSERT( pAIMesh );

                    const auto pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];
                    FB_ASSERT( pAIMaterial );
                }
            }

            // Traverse all child nodes of the current node instance
            for( u32 childIdx = 0; childIdx < pNode->mNumChildren; childIdx++ )
            {
                const auto pChildNode = pNode->mChildren[childIdx];
                loadToMesh( mesh, mScene, pChildNode, mDir );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshLoader::setRootActor( SmartPtr<scene::IActor> rootActor )
    {
        m_rootActor = rootActor;
    }

    auto MeshLoader::getRootActor() const -> SmartPtr<scene::IActor>
    {
        return m_rootActor;
    }

    auto MeshLoader::getMeshPath() const -> String
    {
        return m_meshPath;
    }

    void MeshLoader::createMaterial( SmartPtr<scene::Material> omat, s32 index, const aiMaterial *mat,
                                     const String &mDir )
    {
        try
        {
            auto materialName = mat->GetName();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            static const auto materialType = render::IMaterial::typeInfo();

            auto projectFolder = applicationManager->getProjectPath();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( !graphicsSystem )
            {
                FB_LOG( "No graphics system." );
                return;
            }

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto materialManager = graphicsSystem->getMaterialManager();

            auto meshPath = getMeshPath();
            auto materialsPath = projectFolder + "/" + Path::getFilePath( meshPath ) + "/Materials";
            materialsPath = StringUtil::cleanupPath( materialsPath );

            if( !fileSystem->isExistingFolder( materialsPath ) )
            {
                fileSystem->createDirectories( materialsPath );
            }

            auto pMaterialName = materialName.C_Str();
            auto materialNameStr = String( pMaterialName );
            auto materialPath = materialsPath + "/" + materialNameStr + ".mat";

            if( omat )
            {
                omat->setMaterialPath( materialPath );
            }

            //if( fileSystem->isExistingFile( materialPath ) )
            //{
            //    return;
            //}

            auto pass = SmartPtr<render::IMaterialPass>();

            auto materialResult = resourceDatabase->createOrRetrieve( materialType, materialPath );
            auto material = fb::static_pointer_cast<render::IMaterial>( materialResult.first );
            if( material )
            {
                auto technique = SmartPtr<render::IMaterialTechnique>();
                auto techniques = material->getTechniques();
                if( !techniques.empty() )
                {
                    technique = techniques.front();
                }
                else
                {
                    technique = material->createTechnique();
                }

                auto passes = technique->getPasses();
                if( !passes.empty() )
                {
                    pass = passes.front();
                }
                else
                {
                    pass = technique->createPass();
                }
            }

            static int dummyMatCount = 0;

            // extreme fallback texture -- 2x2 hot pink
            static u8 s_RGB[] = { 128, 0, 255, 128, 0, 255, 128, 0, 255, 128, 0, 255 };

            std::ostringstream matname;

            enum aiTextureType type = aiTextureType_DIFFUSE;
            static aiString path;
            aiTextureMapping mapping = aiTextureMapping_UV;  // the mapping (should be uv for now)
            unsigned int uvindex = 0;                        // the texture uv index channel
            float blend = 1.0f;                              // blend
            aiTextureOp op = aiTextureOp_Multiply;           // op
            aiTextureMapMode mapmode[2] = { aiTextureMapMode_Wrap, aiTextureMapMode_Wrap };  // mapmode
            std::ostringstream texname;

            aiString szPath;
            if( AI_SUCCESS == aiGetMaterialString( mat, AI_MATKEY_TEXTURE_DIFFUSE( 0 ), &szPath ) )
            {
                FB_LOG( "Using aiGetMaterialString : Found texture " + String( szPath.data ) +
                        " for channel " + StringUtil::toString( uvindex ) );
            }

            if( szPath.length < 1 )
            {
                FB_LOG( "Didn't find any texture units..." );

                szPath =
                    static_cast<String>( "dummyMat" + StringUtil::toString( dummyMatCount ) ).c_str();
                dummyMatCount++;
            }

            String basename;
            String outPath;
            StringUtil::splitFilename( String( szPath.data ), basename, outPath );

            FB_LOG( "Creating " + basename );

            // Ogre::ResourceManager::ResourceCreateOrRetrieveResult status =
            // omatMgr->createOrRetrieve(ReplaceSpaces(basename),
            // Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true); Ogre::MaterialPtr omat;//
            // = status.first.staticCast<Ogre::Material>(); if (!status.second) 	return omat;

            // ambient
            aiColor4D clr( 1.0f, 1.0f, 1.0f, 1.0 );
            // Ambient is usually way too low! FIX ME!
            if( mat->GetTexture( type, 0, &path ) != AI_SUCCESS )
            {
                aiGetMaterialColor( mat, AI_MATKEY_COLOR_AMBIENT, &clr );
            }
            // omat->setAmbient(ColourF(clr.r, clr.g, clr.b, 1.0f));

            // diffuse
            clr = aiColor4D( 1.0f, 1.0f, 1.0f, 1.0f );
            if( AI_SUCCESS == aiGetMaterialColor( mat, AI_MATKEY_COLOR_DIFFUSE, &clr ) )
            {
                if( pass )
                {
                    pass->setDiffuse( ColourF( clr.r, clr.g, clr.b, clr.a ) );
                }
            }

            // specular
            clr = aiColor4D( 1.0f, 1.0f, 1.0f, 1.0f );
            if( AI_SUCCESS == aiGetMaterialColor( mat, AI_MATKEY_COLOR_SPECULAR, &clr ) )
            {
                if( pass )
                {
                    pass->setSpecular( ColourF( clr.r, clr.g, clr.b, clr.a ) );
                }
            }

            // emissive
            clr = aiColor4D( 1.0f, 1.0f, 1.0f, 1.0f );
            if( AI_SUCCESS == aiGetMaterialColor( mat, AI_MATKEY_COLOR_EMISSIVE, &clr ) )
            {
                if( pass )
                {
                    pass->setEmissive( ColourF( clr.r, clr.g, clr.b, 1.0f ) );
                }
            }

            float fShininess;
            if( AI_SUCCESS == aiGetMaterialFloat( mat, AI_MATKEY_SHININESS, &fShininess ) )
            {
                // pass->setShininess(f32(fShininess));
            }

            if( mat->GetTexture( type, 0, &path ) == AI_SUCCESS )
            {
                FB_LOG( "Found texture " + String( path.data ) + " for channel " +
                        StringUtil::toString( uvindex ) );

                if( AI_SUCCESS == aiGetMaterialString( mat, AI_MATKEY_TEXTURE_DIFFUSE( 0 ), &szPath ) )
                {
                    FB_LOG( "Using aiGetMaterialString : Found texture " + String( szPath.data ) +
                            " for channel " + StringUtil::toString( uvindex ) );
                }

                auto textureName = String( path.C_Str() );
                if( !fileSystem->isExistingFile( textureName ) )
                {
                    textureName = Path::getFileName( textureName );
                }

                material->setTexture( textureName, 0 );
            }

            if( material )
            {
                materialManager->saveToFile( materialPath, material );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshLoader::computeNodesDerivedTransform( const aiScene *mScene, const aiNode *pNode,
                                                   const aiMatrix4x4 accTransform )
    {
        try
        {
            auto it = m_nodeDerivedTransformByName.find( pNode->mName.data );
            if( it == m_nodeDerivedTransformByName.end() )
            {
                m_nodeDerivedTransformByName[pNode->mName.data] = accTransform;
            }

            for( u32 childIdx = 0; childIdx < pNode->mNumChildren; ++childIdx )
            {
                const auto child = pNode->mChildren[childIdx];
                computeNodesDerivedTransform( mScene, child, accTransform * child->mTransformation );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshLoader::loadDataFromNode( const aiScene *mScene, const aiNode *pNode, const String &mDir )
    {
        // auto engine = core::ApplicationManager::instance();
        // SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
        // SmartPtr<IMeshManager> meshMgr = graphicsSystem->getMeshManager();

        if( pNode->mNumMeshes > 0 )
        {
            // MeshGeometryPtr mesh;

            if( getUseSingleMesh() )
            {
                // if(mMeshes.size() == 0)
                //{
                //	static int nameExt = 0;
                //	mesh = m_meshMgr->create(String("ROOTMesh") + StringUtil::toString(nameExt++),
                //IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

                //	SmartPtr<IMesh> pMesh(new CMesh);
                //	mesh->setMesh(pMesh);

                //	mMeshes.push_back(mesh);
                //}
                // else
                //{
                //	mesh = mMeshes[0];
                //}
            }

            for( u32 idx = 0; idx < pNode->mNumMeshes; ++idx )
            {
                aiMesh *pAIMesh = mScene->mMeshes[pNode->mMeshes[idx]];
                if( !m_quietMode )
                {
                    FB_LOG_MESSAGE( "MeshLoader", String( "SubMesh " ) + StringUtil::toString( idx ) +
                                                      String( " for mesh '" ) +
                                                      String( pNode->mName.data ) + "'" );
                }

                // Create a material instance for the mesh.
                const aiMaterial *pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];
                // createSubMesh(pNode->mName.data, idx, pNode, pAIMesh, pAIMaterial, mesh->getMesh(),
                // mDir);
            }
        }

        // Traverse all child nodes of the current node instance
        for( u32 childIdx = 0; childIdx < pNode->mNumChildren; childIdx++ )
        {
            const aiNode *pChildNode = pNode->mChildren[childIdx];
            loadDataFromNode( mScene, pChildNode, mDir );
        }
    }

    auto MeshLoader::createSubMesh( const String &name, int index, const aiNode *pNode,
                                    const aiMesh *mesh, const aiMaterial *mat, SmartPtr<IMesh> mMesh,
                                    const String &mDir ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        // if animated all submeshes must have bone weights
        if( m_bonesByName.size() && !mesh->HasBones() )
        {
            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", String( "Skipping Mesh " ) + String( mesh->mName.data ) +
                                                  String( "with no bone weights" ) );
            }

            return false;
        }

        auto matName = mat->GetName();
        auto pName = matName.C_Str();
        String materialName = pName ? pName : "";

        // now begin the object definition
        // We create a submesh per material
        auto submesh = factoryManager->make_ptr<SubMesh>();
        mMesh->addSubMesh( submesh );

        bool swapZY = false;

        if( m_director )
        {
            swapZY = m_director->getSwapZY();
        }

        // prime pointers to vertex related data
        aiVector3D *vec = mesh->mVertices;
        aiVector3D *tangent = mesh->mTangents;
        aiVector3D *bitangent = mesh->mBitangents;

        aiVector3D *norm = mesh->mNormals;
        //aiVector3D *uv = mesh->mTextureCoords[0];
        aiColor4D *col = mesh->mColors[0];

        Array<Array<Vector3<f32> > > uvs;
        for( auto uv : mesh->mTextureCoords )
        {
            if( uv )
            {
                Array<Vector3<f32> > uvArray;
                for( size_t j = 0; j < mesh->mNumVertices; ++j )
                {
                    uvArray.emplace_back( uv->x, uv->y, uv->z );
                    uv++;
                }

                //std::reverse( uvArray.begin(), uvArray.end() );
                uvs.push_back( uvArray );
            }
        }

        // We must create the vertex data, indicating how many vertices there will be
        // submesh->useSharedVertices = false;

        auto vertexData = factoryManager->make_ptr<VertexBuffer>();
        submesh->setVertexBuffer( vertexData );
        vertexData->setNumVertices( mesh->mNumVertices );

        // We must now declare what the vertex data contains
        auto declaration = factoryManager->make_ptr<VertexDeclaration>();
        vertexData->setVertexDeclaration( declaration );

        auto source = 0;
        u32 offset = 0;

        auto vertexElem = declaration->addElement(
            source, offset, VertexDeclaration::VertexElementSemantic::VES_POSITION,
            IVertexElement::VertexElementType::VET_FLOAT3 );
        offset += vertexElem->getSize();

        // mLog->logMessage((boost::format(" %d vertices ") % m->mNumVertices).str());
        if( !m_quietMode )
        {
            FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " vertices" );
        }

        if( norm )
        {
            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " normals" );
            }

            auto normalElem = declaration->addElement(
                source, offset, VertexDeclaration::VertexElementSemantic::VES_NORMAL,
                IVertexElement::VertexElementType::VET_FLOAT3 );
            offset += normalElem->getSize();
        }

        if( tangent )
        {
            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " normals" );
            }

            auto normalElem = declaration->addElement(
                source, offset, VertexDeclaration::VertexElementSemantic::VES_TANGENT,
                IVertexElement::VertexElementType::VET_FLOAT3 );
            offset += normalElem->getSize();
        }

        if( bitangent )
        {
            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " normals" );
            }

            auto normalElem = declaration->addElement(
                source, offset, VertexDeclaration::VertexElementSemantic::VES_BINORMAL,
                IVertexElement::VertexElementType::VET_FLOAT3 );
            offset += normalElem->getSize();
        }

        for( size_t i = 0; i < uvs.size(); ++i )
        {
            auto uvElem = declaration->addElement(
                source, offset, VertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                IVertexElement::VertexElementType::VET_FLOAT3, i );
            offset += uvElem->getSize();
        }

        if( col )
        {
            auto colElem = declaration->addElement(
                source, offset, IVertexDeclaration::VertexElementSemantic::VES_DIFFUSE,
                IVertexElement::VertexElementType::VET_FLOAT4, 0 );
            offset += colElem->getSize();
        }

        auto vdata = static_cast<f32 *>( vertexData->createVertexData() );
        FB_ASSERT( vdata );

        for( u32 i = 0; i < mesh->mNumVertices; ++i )
        {
            if( !swapZY )
            {
                // Position
                auto pos = Vector3<f32>( vec->x, vec->y, vec->z );
                pos *= m_meshScale;

                *vdata++ = pos.x;
                *vdata++ = pos.y;
                *vdata++ = pos.z;
                vec++;

                // Normal
                if( norm )
                {
                    *vdata++ = norm->x;
                    *vdata++ = norm->y;
                    *vdata++ = norm->z;
                    norm++;
                }

                if( tangent )
                {
                    *vdata++ = tangent->x;
                    *vdata++ = tangent->y;
                    *vdata++ = tangent->z;

                    tangent++;
                }

                if( bitangent )
                {
                    *vdata++ = bitangent->x;
                    *vdata++ = bitangent->y;
                    *vdata++ = bitangent->z;
                    bitangent++;
                }
            }
            else
            {
                // Position
                auto pos = Vector3<f32>( vec->x, vec->z, vec->y );
                pos *= m_meshScale;

                *vdata++ = pos.x;
                *vdata++ = pos.y;
                *vdata++ = pos.z;
                vec++;

                // Normal
                if( norm )
                {
                    *vdata++ = norm->x;
                    *vdata++ = norm->z;
                    *vdata++ = norm->y;
                    norm++;
                }

                if( tangent )
                {
                    *vdata++ = tangent->x;
                    *vdata++ = tangent->z;
                    *vdata++ = tangent->y;

                    tangent++;
                }

                if( bitangent )
                {
                    *vdata++ = bitangent->x;
                    *vdata++ = bitangent->z;
                    *vdata++ = bitangent->y;
                    bitangent++;
                }
            }

            if( uvs.size() > 0 )
            {
                for( auto &j : uvs )
                {
                    auto uv = j[i];
                    *vdata++ = uv.x;
                    *vdata++ = 1.0 - uv.y;
                    *vdata++ = uv.z;
                }
            }

            if( col )
            {
                *vdata++ = col->r;
                *vdata++ = col->g;
                *vdata++ = col->b;
                *vdata++ = col->a;
                col++;
            }
        }

        FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumFaces ) + " faces" );

        // Creates the index data
        auto indexBuffer = factoryManager->make_ptr<IndexBuffer>();
        submesh->setIndexBuffer( indexBuffer );

        if( mesh->mNumVertices < std::numeric_limits<u16>::max() )
        {
            indexBuffer->setIndexType( IndexBuffer::Type::IT_16BIT );
            indexBuffer->setNumIndices( mesh->mNumFaces * 3 );
            auto idata = static_cast<u16 *>( indexBuffer->createIndexData() );

            for( u32 i = 0; i < mesh->mNumFaces; ++i )
            {
                auto f = &mesh->mFaces[i];
                for( auto index = 0; index < f->mNumIndices; ++index )
                {
                    auto faceIndex = f->mIndices[index];
                    FB_ASSERT( faceIndex < std::numeric_limits<u16>::max() );
                    *idata++ = faceIndex;
                }
            }
        }
        else
        {
            indexBuffer->setIndexType( IndexBuffer::Type::IT_32BIT );
            indexBuffer->setNumIndices( mesh->mNumFaces * 3 );
            auto idata = static_cast<u32 *>( indexBuffer->createIndexData() );

            for( u32 i = 0; i < mesh->mNumFaces; ++i )
            {
                auto f = &mesh->mFaces[i];
                for( auto index = 0; index < f->mNumIndices; ++index )
                {
                    *idata++ = f->mIndices[index];
                }
            }
        }

        // set bone weigths
        // if(mesh->HasBones())
        //{
        //    for ( Ogre::uint32 i=0; i < mesh->mNumBones; i++ )
        //    {
        //        aiBone *pAIBone = mesh->mBones[ i ];
        //        if ( NULL != pAIBone )
        //        {
        //            Ogre::String bname = pAIBone->mName.data;
        //            for ( Ogre::uint32 weightIdx = 0; weightIdx < pAIBone->mNumWeights; weightIdx++ )
        //            {
        //                aiVertexWeight aiWeight = pAIBone->mWeights[ weightIdx ];

        //                Ogre::VertexBoneAssignment vba;
        //                vba.vertexIndex = aiWeight.mVertexId;
        //                vba.boneIndex = mSkeleton->getBone(bname)->getHandle();
        //                vba.weight= aiWeight.mWeight;

        //                submesh->addBoneAssignment(vba);
        //            }
        //        }
        //    }
        //} // if mesh has bones

        // Finally we set a material to the submesh
        submesh->setMaterialName( materialName );

        return true;
    }
#endif

    auto MeshLoader::getUseSingleMesh() const -> bool
    {
        return m_useSingleMesh;
    }

    void MeshLoader::setUseSingleMesh( bool useSingleMesh )
    {
        m_useSingleMesh = useSingleMesh;
    }

    auto MeshLoader::getQuietMode() const -> bool
    {
        return m_quietMode;
    }

    void MeshLoader::setQuietMode( bool quietMode )
    {
        m_quietMode = quietMode;
    }

    auto MeshLoader::getMeshResource() const -> SmartPtr<IMeshResource>
    {
        return m_meshResource;
    }

    void MeshLoader::setMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        m_meshResource = meshResource;
    }
}  // end namespace fb
