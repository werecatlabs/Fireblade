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
        auto applicationManager = core::IApplicationManager::instance();
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

    Array<render::IMaterial> MeshLoader::createMaterials( const String &meshPath )
    {
        auto materials = Array<render::IMaterial>();

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto fileExt = Path::getFileExtension( meshPath );
        fileExt = StringUtil::make_lower( fileExt );

        if( fileExt == ".fbmeshbin" )
        {
        }
        else if( fileExt == ".fbx" || fileExt == ".FBX" )
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

    void MeshLoader::createActor( SmartPtr<ITransformNode> parentNode,
                                  SmartPtr<scene::IActor> parentActor )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto actor = sceneManager->createActor();

        auto name = parentNode->getName();
        actor->setName( name );

        auto localTransform = parentNode->getLocalTransform();
        auto worldTransform = parentNode->getWorldTransform();

        SmartPtr<scene::Transform> actorTransform = actor->getTransform();
        actorTransform->setLocalTransform( localTransform );

        auto pWorldTransform = actorTransform->getWorldTransform();
        actorTransform->setWorldTransform( worldTransform );

        parentActor->addChild( actor );

        auto children = parentNode->getChildren();
        for( auto child : children )
        {
            createActor( child, actor );
        }
    }

    SmartPtr<scene::IActor> MeshLoader::loadActor( SmartPtr<IMeshResource> resource )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        if( resource )
        {
            setMeshResource( resource );

            m_meshScale = Vector3<f32>::unit() * resource->getScale();
            m_meshPath = resource->getFilePath();
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
                    meshComponent->setMesh( mesh );
                }

                auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

                return actor;
            }
        }
        else if( fileExt == ".fbx" || fileExt == ".FBX" )
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

            u32 flags = aiProcess_Triangulate;

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
                }
            }
#endif

            return actor;
        }

        return nullptr;
    }

    SmartPtr<scene::IActor> MeshLoader::loadActor( const String &meshName )
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( meshName ) );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto meshManager = applicationManager->getMeshManager();
            FB_ASSERT( meshManager );

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
                        meshComponent->setMesh( mesh );
                    }

                    auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

                    return actor;
                }
            }
            else if( fileExt == ".fbx" || fileExt == ".FBX" )
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
                auto absolutePath = Path::getAbsolutePath( projectFolder, m_meshPath );

                if( stream )
                {
                    auto size = stream->size();
                    auto buffer = new u8[size];

                    stream->read( buffer, size );

                    //const auto scene = importer.ReadFile( absolutePath.c_str(), flags );
                    const auto scene = importer.ReadFileFromMemory( buffer, size, flags, "fbx" );
                    if( scene )
                    {
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

    SmartPtr<IMesh> MeshLoader::loadMesh( SmartPtr<IMeshResource> resource )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        if( resource )
        {
            setMeshResource( resource );
            m_meshScale = Vector3<f32>::unit() * resource->getScale();
            m_meshPath = resource->getFilePath();
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
        else if( fileExt == ".fbx" )
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
            u32 flags = aiProcess_Triangulate;
            const auto scene = importer.ReadFile( absolutePath.c_str(), flags );
            if( scene )
            {
                auto hash = StringUtil::getHash64( m_meshPath );
                m_fileHash = *&hash;

                computeNodesDerivedTransform( scene, scene->mRootNode,
                                              scene->mRootNode->mTransformation );

                auto name = Path::getFileNameWithoutExtension( m_meshPath );
                mesh->setName( name );

                auto path = Path::getFilePath( m_meshPath );
                loadToMesh( mesh, scene, scene->mRootNode, path );
            }

            return mesh;
#endif
        }

        return nullptr;
    }

    SmartPtr<IMesh> MeshLoader::loadMesh( const String &meshName )
    {
        FB_ASSERT( !StringUtil::isNullOrEmpty( meshName ) );

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        // auto graphicsSystem = applicationManager->getGraphicsSystem();
        // FB_ASSERT(graphicsSystem);

        auto meshManager = applicationManager->getMeshManager();
        FB_ASSERT( meshManager );

        auto resource = meshManager->loadFromFile( meshName );
        auto meshResource = fb::static_pointer_cast<IMeshResource>( resource );
        if( meshResource )
        {
            m_meshScale = Vector3<f32>::unit() * meshResource->getScale();
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
        else if( fileExt == ".fbx" || fileExt == ".FBX" )
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
            u32 flags = aiProcess_Triangulate;
            const auto scene = importer.ReadFile( absolutePath.c_str(), flags );
            if( scene )
            {
                auto hash = StringUtil::getHash64( meshName );
                m_fileHash = *&hash;

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
            auto applicationManager = core::IApplicationManager::instance();
            auto sceneManager = applicationManager->getSceneManager();
            auto fileSystem = applicationManager->getFileSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto actor = sceneManager->createActor();
            parent->addChild( actor );

            auto name = pNode->mName;
            actor->setName( name.C_Str() );

            auto meshResource = getMeshResource();

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

                FB_ASSERT( actorWorldTransform.isSane() );

                actorTransform->updateLocalFromWorld();
            }

            auto numMeshes = pNode->mNumMeshes;

            auto meshFilePath = String( pNode->mName.data ) + "_" + StringUtil::toString( m_fileHash );

            if( numMeshes > 0 )
            {
                auto mesh = fb::make_ptr<Mesh>();

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

                auto meshComponent = actor->addComponent<scene::Mesh>();
                if( meshComponent )
                {
                    mesh->setName( meshFilePath );
                    meshComponent->setMeshPath( meshFilePath );
                    meshComponent->setMesh( mesh );
                }

                auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
                if( meshRenderer )
                {
                    //
                }

                for( u32 idx = 0; idx < pNode->mNumMeshes; ++idx )
                {
                    auto meshIndex = pNode->mMeshes[idx];
                    auto pAIMesh = mScene->mMeshes[meshIndex];
                    FB_ASSERT( pAIMesh );

                    const auto pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];
                    FB_ASSERT( pAIMaterial );

                    if( pAIMaterial )
                    {
                        auto materialComponent = actor->addComponent<scene::Material>();
                        if( materialComponent )
                        {
                            auto pName = pAIMaterial->GetName().C_Str();

                            /*
                            materialComponent->setMaterialName(pName ? pName : "");

                            if (materialComponent->getMaterialName() == "F40f40")
                            {
                                int stop = 0;
                                stop = 0;
                            }
                            */

                            materialComponent->setIndex( idx );
                            createMaterial( materialComponent, idx, pAIMaterial, "" );
                            materialComponent->updateMaterial();
                        }
                    }
                }
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
            auto applicationManager = core::IApplicationManager::instance();
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

            auto meshFilePath = String( pNode->mName.data ) + "_" + StringUtil::toString( m_fileHash );

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

    SmartPtr<scene::IActor> MeshLoader::getRootActor() const
    {
        return m_rootActor;
    }

    String MeshLoader::getMeshPath() const
    {
        return m_meshPath;
    }

    void MeshLoader::createMaterial( SmartPtr<scene::Material> omat, s32 index, const aiMaterial *mat,
                                     const String &mDir )
    {
        try
        {
            auto materialName = mat->GetName();

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( !graphicsSystem )
            {
                FB_LOG( "No graphics system." );
                return;
            }

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto materialManager = graphicsSystem->getMaterialManager();

            auto meshPath = getMeshPath();
            auto materialsPath = Path::getFilePath( meshPath ) + "/Materials";
            if( !fileSystem->isExistingFolder( materialsPath ) )
            {
                fileSystem->createDirectories( materialsPath );
            }

            auto materialPath = materialsPath + "/" + String( materialName.C_Str() ) + ".mat";
            omat->setMaterialPath( materialPath );

            if( fileSystem->isExistingFile( materialPath ) )
            {
                return;
            }

            auto pass = SmartPtr<render::IMaterialPass>();

            auto materialResult = resourceDatabase->createOrRetrieve( materialPath );
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

                szPath = String( "dummyMat" + StringUtil::toString( dummyMatCount ) ).c_str();
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
                aiGetMaterialColor( mat, AI_MATKEY_COLOR_AMBIENT, &clr );
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

                if( auto m = omat->getMaterial() )
                {
                    auto textureName = String( path.C_Str() );
                    if( !fileSystem->isExistingFile( textureName ) )
                    {
                        textureName = Path::getFileName( textureName );
                    }

                    m->setTexture( textureName, 0 );
                }
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
        // auto engine = core::IApplicationManager::instance();
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

    bool MeshLoader::createSubMesh( const String &name, int index, const aiNode *pNode,
                                    const aiMesh *mesh, const aiMaterial *mat, SmartPtr<IMesh> mMesh,
                                    const String &mDir )
    {
        auto applicationManager = core::IApplicationManager::instance();
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

        auto pName = mat->GetName().C_Str();
        String materialName = pName ? pName : "";

        // now begin the object definition
        // We create a submesh per material
        auto submesh = factoryManager->make_ptr<SubMesh>();
        mMesh->addSubMesh( submesh );

        // prime pointers to vertex related data
        aiVector3D *vec = mesh->mVertices;
        aiVector3D *tangent = mesh->mTangents;
        aiVector3D *bitangent = mesh->mBitangents;

        aiVector3D *norm = mesh->mNormals;
        aiVector3D *uv = mesh->mTextureCoords[0];
        aiColor4D *col = mesh->mColors[0];

        // We must create the vertex data, indicating how many vertices there will be
        // submesh->useSharedVertices = false;

        auto vertexData = factoryManager->make_ptr<VertexBuffer>();
        submesh->setVertexBuffer( vertexData );
        vertexData->setNumVerticies( mesh->mNumVertices );

        // We must now declare what the vertex data contains
        auto declaration = factoryManager->make_ptr<VertexDeclaration>();
        vertexData->setVertexDeclaration( declaration );

        static const unsigned short source = 0;
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

        if( uv )
        {
            if( !m_quietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " uvs" );
            }

            auto uvElem = declaration->addElement(
                source, offset, IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                IVertexElement::VertexElementType::VET_FLOAT2, 0 );
            offset += uvElem->getSize();
        }

        if( col )
        {
            auto colElem = declaration->addElement(
                source, offset, IVertexDeclaration::VertexElementSemantic::VES_DIFFUSE,
                IVertexElement::VertexElementType::VET_FLOAT4, 0 );
            offset += colElem->getSize();
        }

        aiMatrix4x4 aiM;

        auto nodeTransformData = m_nodeDerivedTransformByName.find( pNode->mName.data );
        if( nodeTransformData != m_nodeDerivedTransformByName.end() )
        {
            aiM = nodeTransformData->second;
        }

        aiMatrix4x4 normalMatrix = aiM;
        normalMatrix.a4 = 0;
        normalMatrix.b4 = 0;
        normalMatrix.c4 = 0;
        normalMatrix.Transpose().Inverse();

        auto vdata = static_cast<f32 *>( vertexData->createVertexData() );
        FB_ASSERT( vdata );

        for( u32 i = 0; i < mesh->mNumVertices; ++i )
        {
            // Position
            aiVector3D vect;
            vect.x = vec->x;
            vect.y = vec->y;
            vect.z = vec->z;

            auto pos = Vector3<f32>( vect.x, vect.y, vect.z );
            pos *= m_meshScale;

            *vdata++ = pos.X();
            *vdata++ = pos.Y();
            *vdata++ = pos.Z();
            vec++;

            // Normal
            if( norm )
            {
                vect.x = norm->x;
                vect.y = norm->y;
                vect.z = norm->z;

                // vect *= normalMatrix;
                // vect = vect.Normalize();

                *vdata++ = vect.x;
                *vdata++ = vect.y;
                *vdata++ = vect.z;
                norm++;
            }

            if( tangent )
            {
                vect.x = tangent->x;
                vect.y = tangent->y;
                vect.z = tangent->z;

                *vdata++ = vect.x;
                *vdata++ = vect.y;
                *vdata++ = vect.z;

                tangent++;
            }

            if( bitangent )
            {
                vect.x = bitangent->x;
                vect.y = bitangent->y;
                vect.z = bitangent->z;

                *vdata++ = vect.x;
                *vdata++ = vect.y;
                *vdata++ = vect.z;
                bitangent++;
            }

            // uvs
            if( uv )
            {
                *vdata++ = uv->x;
                *vdata++ = uv->y;
                //*vdata++ = uv->z;
                uv++;
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

        if( !m_quietMode )
        {
            FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumFaces ) + " faces" );
        }

        // Creates the index data
        auto indexBuffer = factoryManager->make_ptr<IndexBuffer>();
        submesh->setIndexBuffer( indexBuffer );

        constexpr auto maxVertices = std::numeric_limits<u16>::max() - 1;
        if( mesh->mNumVertices > maxVertices )
        {
            indexBuffer->setIndexType( IndexBuffer::Type::IT_32BIT );
            indexBuffer->setNumIndices( mesh->mNumFaces * 3 );
            auto idata = static_cast<u32 *>( indexBuffer->createIndexData() );

            // for (s32 i = mesh->mNumFaces - 1; i >= 0; --i)
            for( u32 i = 0; i < mesh->mNumFaces; ++i )
            {
                auto f = &mesh->mFaces[i];
                for( auto index = 0; index < f->mNumIndices; ++index )
                {
                    *idata++ = f->mIndices[index];
                }
            }
        }
        else
        {
            indexBuffer->setIndexType( IndexBuffer::Type::IT_16BIT );
            indexBuffer->setNumIndices( mesh->mNumFaces * 3 );
            auto idata = static_cast<u16 *>( indexBuffer->createIndexData() );

            // for (s32 i = mesh->mNumFaces - 1; i >= 0; --i)
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

    bool MeshLoader::getUseSingleMesh() const
    {
        return m_useSingleMesh;
    }

    void MeshLoader::setUseSingleMesh( bool useSingleMesh )
    {
        m_useSingleMesh = useSingleMesh;
    }

    bool MeshLoader::getQuietMode() const
    {
        return m_quietMode;
    }

    void MeshLoader::setQuietMode( bool quietMode )
    {
        m_quietMode = quietMode;
    }

    SmartPtr<IMeshResource> MeshLoader::getMeshResource() const
    {
        return m_meshResource;
    }

    void MeshLoader::setMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        m_meshResource = meshResource;
    }
}  // end namespace fb
