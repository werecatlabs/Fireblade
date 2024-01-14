#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxMeshShape.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/PhysxMemoryInputStream.h>
#include <FBPhysx/PhysxMemoryOutputStream.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBCore/FBCore.h>
#include <FBMesh/FBMesh.h>
#include <PxPhysicsAPI.h>

namespace fb::physics
{
    PhysxMeshShape::PhysxMeshShape()
    {
        createStateObject();
    }

    PhysxMeshShape::~PhysxMeshShape()
    {
        unload( nullptr );
    }

    void PhysxMeshShape::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( !isLoaded() );

            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            ScopedLock lock( physicsManager );

            if( auto meshResource = getMeshResource() )
            {
                if( meshResource->isLoaded() )
                {
                    meshResource->load( nullptr );
                }
            }

            if( data )
            {
                if( data->isDerived<IMeshResource>() )
                {
                    auto meshResource = fb::static_pointer_cast<IMeshResource>( data );
                    setMeshResource( meshResource );
                }
            }

            if( auto body = getActor() )
            {
                if( body->isExactly<PhysxRigidDynamic>() )
                {
                    auto rigidDynamic = fb::static_pointer_cast<PhysxRigidDynamic>( body );

                    auto pRigidDynamic = rigidDynamic->getActor();
                    setPxActor( pRigidDynamic );
                }

                if( body->isExactly<PhysxRigidStatic>() )
                {
                    auto rigidStatic = fb::static_pointer_cast<PhysxRigidStatic>( body );

                    auto pRigidStatic = rigidStatic->getRigidStatic();
                    setPxActor( pRigidStatic );
                }
            }

            createShape();

            //FB_ASSERT( getShape() );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxMeshShape::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ScopedLock lock( physicsManager );

                if( auto stateContext = getStateContext() )
                {
                    if( m_stateListener )
                    {
                        stateContext->removeStateListener( m_stateListener );
                    }

                    stateManager->removeStateObject( stateContext );
                    setStateContext( nullptr );
                }

                if( m_stateListener )
                {
                    m_stateListener->unload( nullptr );
                    m_stateListener = nullptr;
                }

                auto cleanMesh = getCleanMesh();
                if( cleanMesh )
                {
                    cleanMesh->unload( nullptr );
                    cleanMesh = nullptr;
                    setCleanMesh( nullptr );
                }

                setMeshResource( nullptr );

                if( auto shape = getShape() )
                {
                    if( auto pxActor = getPxActor() )
                    {
                        pxActor->detachShape( *shape, false );
                    }

                    setShape( nullptr );
                }

                PhysxShape<IMeshShape>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PhysxMeshShape::getExtents() const -> Vector3<real_Num>
    {
        //return m_state->getExtents();
        return Vector3<real_Num>::zero();
    }

    void PhysxMeshShape::setExtents( const Vector3<real_Num> &extents )
    {
        //m_state->setExtents( extents );

        auto applicationManager = core::ApplicationManager::instance();
        auto physicsManager = applicationManager->getPhysicsManager();

        auto task = Thread::getCurrentTask();
        auto physicsTask = physicsManager->getPhysicsTask();

        const auto &loadingState = getLoadingState();

        if( loadingState == LoadingState::Loaded && task == physicsTask )
        {
            auto shape = getShape();
            if( shape )
            {
                auto halfExtents = extents / static_cast<real_Num>( 2.0 );
                physx::PxVec3 dimensions( ( halfExtents.X() ), ( halfExtents.Y() ),
                                          ( halfExtents.Z() ) );
                physx::PxBoxGeometry geometry( dimensions );
                shape->setGeometry( geometry );
            }

            if( auto stateContext = getStateContext() )
            {
                stateContext->setDirty( false );
            }
        }
    }

    void PhysxMeshShape::setAABB( const AABB3<real_Num> &box )
    {
        m_extents = box.getExtent();
    }

    auto PhysxMeshShape::getAABB() const -> AABB3<real_Num>
    {
        AABB3<real_Num> box;
        return box;
    }

    auto PhysxMeshShape::getWorldTransform( const Transform3<real_Num> &t ) -> Transform3<real_Num>
    {
        auto subMeshPosition = t.getPosition();
        auto subMeshScale = t.getScale();
        auto subMeshOrientation = t.getOrientation();

        auto collisionTransform = Transform3<real_Num>();
        auto worldScale = subMeshScale * collisionTransform.getScale();
        auto worldOrientation = collisionTransform.getOrientation() * subMeshOrientation;
        auto worldPosition =
            collisionTransform.getOrientation() * ( collisionTransform.getScale() * subMeshPosition );
        worldPosition += collisionTransform.getPosition();

        worldPosition += m_colliderTransform.getPosition();
        worldOrientation = m_colliderTransform.getOrientation() * worldOrientation;

        Transform3<real_Num> transform;
        transform.setPosition( worldPosition );
        transform.setOrientation( worldOrientation );
        transform.setScale( worldScale * m_colliderTransform.getScale() );
        return transform;
    }

    auto PhysxMeshShape::getMeshTransform( const Transform3<real_Num> &t ) -> physx::PxTransform
    {
        auto subMeshPosition = t.getPosition();
        auto subMeshScale = t.getScale();
        auto subMeshOrientation = t.getOrientation();

        auto collisionTransform = Transform3<real_Num>();
        auto worldScale = subMeshScale * collisionTransform.getScale();
        auto worldOrientation = collisionTransform.getOrientation() * subMeshOrientation;
        auto worldPosition =
            collisionTransform.getOrientation() * ( collisionTransform.getScale() * subMeshPosition );
        worldPosition += collisionTransform.getPosition();

        worldPosition += m_colliderTransform.getPosition();
        worldOrientation = m_colliderTransform.getOrientation() * worldOrientation;

        auto transform = physx::PxTransform::createIdentity();
        transform.p = physx::PxVec3( worldPosition.X(), worldPosition.Y(), worldPosition.Z() );
        transform.q = physx::PxQuat( worldOrientation.X(), worldOrientation.Y(), worldOrientation.Z(),
                                     worldOrientation.W() );
        return transform;
    }

    auto PhysxMeshShape::getMeshCachePath( u32 subMeshIndex ) const -> String
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto cachePath = applicationManager->getCachePath();

        auto meshName = String();
        auto meshResource = getMeshResource();
        if( meshResource )
        {
            auto fileSystemId = meshResource->getFileSystemId();
            if( fileSystemId != 0 )
            {
                FileInfo fileInfo;
                if( !fileSystem->findFileInfo( fileSystemId, fileInfo, false ) )
                {
                    fileSystem->findFileInfo( fileSystemId, fileInfo, true );
                }

                meshName = fileInfo.filePath;
            }

            if( StringUtil::isNullOrEmpty( meshName ) )
            {
                meshName = meshResource->getFilePath();
            }

            if( StringUtil::isNullOrEmpty( meshName ) )
            {
                FB_LOG_ERROR( "Mesh has no name. Unable to cook physics mesh." );
            }

            if( !StringUtil::isNullOrEmpty( meshName ) )
            {
                auto meshPath = cachePath + "/" + meshName;

                auto fileName = Path::getFileNameWithoutExtension( meshPath );
                auto filePath = cachePath + "/" + fileName + ".pxtrianglemesh";
                filePath = StringUtil::cleanupPath( filePath );

                return filePath;
            }
        }

        return {};
    }

    void PhysxMeshShape::createMeshGeometry()
    {
        auto meshResource = getMeshResource();
        if( meshResource )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto pPhysicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( pPhysicsManager );

            auto physicsManager = fb::static_pointer_cast<PhysxManager>( pPhysicsManager );
            FB_ASSERT( physicsManager );

            auto physics = physicsManager->getPhysics();
            FB_ASSERT( physics );

            auto cooking = physicsManager->getCooking();
            FB_ASSERT( cooking );

            auto localPose = getLocalPose();
            auto worldScale = localPose.getScale();

            physx::PxShape *shape = nullptr;

            if( isConvex() )
            {
                auto meshPath = getMeshCachePath( 0 );
                if( !StringUtil::isNullOrEmpty( meshPath ) )
                {
                    if( auto dataStream = fileSystem->open( meshPath ) )
                    {
                        const auto size = dataStream->size();
                        auto buffer = new u8[size];
                        dataStream->read( buffer, size );
                        MemoryInputData inputStream( buffer, static_cast<u32>( size ) );

                        auto convexMesh = physics->createConvexMesh( inputStream );

                        if( buffer )
                        {
                            delete[] buffer;
                            buffer = nullptr;
                        }

                        if( convexMesh )
                        {
                            auto bounds = convexMesh->getLocalBounds();

                            auto minExtents = Vector3<real_Num>( bounds.minimum.x, bounds.minimum.y,
                                                                 bounds.minimum.z );
                            auto maxExtents = Vector3<real_Num>( bounds.maximum.x, bounds.maximum.y,
                                                                 bounds.maximum.z );
                            auto aabb = AABB3<real_Num>( minExtents, maxExtents );
                            setAABB( aabb );

                            physx::PxMeshScale meshScale( PhysxUtil::toPx( worldScale ),
                                                          physx::PxQuat::createIdentity() );

                            auto pMat = fb::static_pointer_cast<PhysxMaterial>( m_material );
                            auto pxMat = pMat->getMaterial();

                            shape = physics->createShape(
                                physx::PxConvexMeshGeometry( convexMesh, meshScale ), *pxMat );

                            setShape( shape );
                        }

                        setupCollisionMask( shape );
                    }
                }
            }
            else
            {
                auto meshPath = getMeshCachePath( 0 );
                if( !StringUtil::isNullOrEmpty( meshPath ) )
                {
                    if( auto dataStream = fileSystem->open( meshPath ) )
                    {
                        const auto size = dataStream->size();
                        if( size == 0 )
                        {
                            return;
                        }

                        auto buffer = new u8[size];
                        dataStream->read( buffer, size );
                        MemoryInputData inputStream( buffer, static_cast<u32>( size ) );

                        auto triangleMesh = physics->createTriangleMesh( inputStream );

                        if( buffer )
                        {
                            delete[] buffer;
                            buffer = nullptr;
                        }

                        if( triangleMesh )
                        {
                            auto bounds = triangleMesh->getLocalBounds();

                            auto minExtents = Vector3<real_Num>( bounds.minimum.x, bounds.minimum.y,
                                                                 bounds.minimum.z );
                            auto maxExtents = Vector3<real_Num>( bounds.maximum.x, bounds.maximum.y,
                                                                 bounds.maximum.z );
                            auto aabb = AABB3<real_Num>( minExtents, maxExtents );
                            setAABB( aabb );

                            physx::PxMeshScale meshScale( PhysxUtil::toPx( worldScale ),
                                                          physx::PxQuat::createIdentity() );

                            auto pMat = fb::static_pointer_cast<PhysxMaterial>( m_material );
                            auto pxMat = pMat->getMaterial();

                            auto meshGeometry = physx::PxTriangleMeshGeometry( triangleMesh, meshScale );
                            shape = physics->createShape( meshGeometry, *pxMat );

                            setShape( shape );
                        }

                        setupCollisionMask( shape );
                    }
                }
            }

            if( shape )
            {
                auto transform = PhysxUtil::toPx( localPose );
                shape->setLocalPose( transform );
            }
        }
    }

    void PhysxMeshShape::setIndices( s32 subMesh, const Array<u32> &indices )
    {
        FB_ASSERT( subMesh < m_indices.size() );

        auto &rIndices = m_indices[subMesh];
        rIndices.reserve( indices.size() );

        for( unsigned int val : indices )
        //for( s32 i = indices.size() - 1; i >= 0; --i )
        {
            rIndices.push_back( val );
        }
    }

    void PhysxMeshShape::readMeshData()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            if( auto mesh = getMesh() )
            {
                if( mesh->getHasSharedVertexData() )
                {
                    auto vertices = MeshUtil::getPoints( mesh );
                    FB_ASSERT( !vertices.empty() );
                    m_sharedVertices = vertices;
                }
                else
                {
                    m_vertices.clear();
                    auto subMeshes = mesh->getSubMeshes();

                    auto numSubMeshes = subMeshes.size();
                    m_vertices.reserve( numSubMeshes );

                    for( size_t i = 0; i < numSubMeshes; ++i )
                    {
                        auto subMesh = subMeshes[i];

                        auto vertices = MeshUtil::getPoints( subMesh );
                        FB_ASSERT( !vertices.empty() );
                        m_vertices.push_back( vertices );
                    }
                }

                auto subMeshes = mesh->getSubMeshes();
                auto numSubMeshes = subMeshes.size();
                m_indices.resize( numSubMeshes );

                for( size_t i = 0; i < numSubMeshes; ++i )
                {
                    auto subMesh = subMeshes[i];

                    auto indices = MeshUtil::getIndices( subMesh );
                    setIndices( static_cast<s32>( i ), indices );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxMeshShape::buildSubMesh( s32 subMeshIdx, const Array<u16> &indexBuffer,
                                       const Transform3<real_Num> &t )
    {
        AABB3<real_Num> bounds;

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pPhysicsManager = applicationManager->getPhysicsManager();
        auto physicsManager = fb::static_pointer_cast<PhysxManager>( pPhysicsManager );
        auto physics = physicsManager->getPhysics();

        auto cooking = physicsManager->getCooking();

        auto transform = getMeshTransform( t );
        auto worldTransform = getWorldTransform( t );
        auto worldScale = worldTransform.getScale();

        physx::PxShape *shape = nullptr;

        if( isConvex() )
        {
            physx::PxConvexMeshDesc convex;

            convex.points.count = static_cast<u32>( m_sharedVertices.size() );
            convex.triangles.count = static_cast<u32>( indexBuffer.size() / 3 );
            convex.points.stride = sizeof( Vector3<real_Num> );
            convex.triangles.stride = sizeof( u16 ) * 3;
            convex.points.data = &m_sharedVertices[0];
            convex.triangles.data = &indexBuffer[0];
            // convex.flags = physx::PxConvexFlag::e16_BIT_INDICES |
            // physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
            convex.flags = physx::PxConvexFlag::e16_BIT_INDICES;

            MemoryOutputStream outputStream;
            bool success = cooking->cookConvexMesh( convex, outputStream );
            if( !success )
            {
                FB_LOG_ERROR( "Error cooking mesh." );
            }

            MemoryInputData inputStream( outputStream.getData(), outputStream.getSize() );

            physx::PxConvexMesh *convexMesh = physics->createConvexMesh( inputStream );
            if( convexMesh )
            {
                physx::PxBounds3 bounds = convexMesh->getLocalBounds();

                AABB3<real_Num> aabb;
                aabb.setMinimum(
                    Vector3<real_Num>( bounds.minimum.x, bounds.minimum.y, bounds.minimum.z ) );
                aabb.setMaximum(
                    Vector3<real_Num>( bounds.maximum.x, bounds.maximum.y, bounds.maximum.z ) );
                setAABB( aabb );

                physx::PxMeshScale meshScale( PhysxUtil::toPx( worldScale ),
                                              physx::PxQuat::createIdentity() );
                // shape = m_actor->createShape(physx::PxConvexMeshGeometry(convexMesh, meshScale),
                // *m_material);
                if( shape )
                {
                    shape->setLocalPose( transform );
                }
            }
        }
        else
        {
            physx::PxTriangleMeshDesc meshDesc;

            meshDesc.points.count = static_cast<u32>( m_sharedVertices.size() );
            meshDesc.triangles.count = static_cast<u32>( indexBuffer.size() / 3 );
            meshDesc.points.stride = sizeof( Vector3<real_Num> );
            meshDesc.triangles.stride = sizeof( u16 ) * 3;
            meshDesc.points.data = &m_sharedVertices[0];
            meshDesc.triangles.data = &indexBuffer[0];
            meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES | physx::PxMeshFlag::eFLIPNORMALS;

            MemoryOutputStream outputStream;
            bool success = cooking->cookTriangleMesh( meshDesc, outputStream );
            if( !success )
            {
                FB_LOG_ERROR( "Error cooking mesh." );
            }

            MemoryInputData inputStream( outputStream.getData(), outputStream.getSize() );

            physx::PxTriangleMesh *triangleMesh = physics->createTriangleMesh( inputStream );
            if( triangleMesh )
            {
                physx::PxBounds3 bounds = triangleMesh->getLocalBounds();

                AABB3<real_Num> aabb;
                aabb.setMinimum(
                    Vector3<real_Num>( bounds.minimum.x, bounds.minimum.y, bounds.minimum.z ) );
                aabb.setMaximum(
                    Vector3<real_Num>( bounds.maximum.x, bounds.maximum.y, bounds.maximum.z ) );
                setAABB( aabb );

                physx::PxMeshScale meshScale( PhysxUtil::toPx( worldScale ),
                                              physx::PxQuat::createIdentity() );
                // shape = m_actor->createShape(physx::PxTriangleMeshGeometry(triangleMesh,
                // meshScale), *m_material);
                if( shape )
                {
                    shape->setLocalPose( transform );
                }
            }
        }

        setAABB( bounds );
    }

    void PhysxMeshShape::createStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto stateManager = applicationManager->getStateManager();
        auto factoryManager = applicationManager->getFactoryManager();
        auto physicsManager = applicationManager->getPhysicsManager();

        m_stateContext = stateManager->addStateObject();

        auto stateListener = factoryManager->make_ptr<MeshShapeStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        m_stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<BoxShapeState>();
        m_stateContext->addState( state );

        auto physicsTask = physicsManager->getPhysicsTask();
        state->setTaskId( physicsTask );
    }

    void PhysxMeshShape::setupCollisionMask( physx::PxShape *shape )
    {
        //if( !isTrigger() )
        //{
        //    shape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, true );
        //    shape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, false );
        //}
        //else
        //{
        //    shape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, false );
        //    shape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, true );
        //}

        //u32 GROUP_MASK = std::numeric_limits<u32>::max();
        //u32 COLLISION_MASK = std::numeric_limits<u32>::max();

        //physx::PxFilterData filterData;
        //filterData.word0 = GROUP_MASK;
        //filterData.word1 = COLLISION_MASK;

        //if( shape )
        //{
        //    shape->setSimulationFilterData( filterData );
        //}
    }

    void PhysxMeshShape::createShape()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();
            auto fileSystem = applicationManager->getFileSystem();

            ScopedLock lock( physicsManager );

            if( !m_material )
            {
                m_material = physicsManager->addMaterial();
            }

            readMeshData();
            cookMesh();
            createMeshGeometry();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PhysxMeshShape::createGeometry() -> physx::PxBoxGeometry
    {
        auto extents = getExtents() / static_cast<real_Num>( 2.0 );
        auto dimensions = physx::PxVec3( extents.X(), extents.Y(), extents.Z() );
        return { dimensions };
    }

    auto PhysxMeshShape::getMeshResource() const -> SmartPtr<IMeshResource>
    {
        return m_meshResource;
    }

    void PhysxMeshShape::setMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        m_meshResource = meshResource;

        /*
        auto applicationManager = core::ApplicationManager::instance();
        auto physicsManager = applicationManager->getPhysicsManager();

        auto physicsTask = physicsManager->getPhysicsTask();
        auto stateTask = physicsManager->getStateTask();
        auto task = Thread::getCurrentTask();

        // const auto& loadingState = getLoadingState();
        if(  // loadingState == LoadingState::LoadingState::Loaded &&
            task == physicsTask )
        {
            if( m_mesh.load() != mesh )
            {
                m_mesh = mesh;

                // if (mesh)
                //{
                //	auto weldTolerance = real_Num(0.001);
                //	auto cleanMesh = MeshUtil::clean(mesh, weldTolerance);
                //	m_cleanMesh = cleanMesh;
                // }

                if( mesh )
                {
                    // cookMesh();
                    build();
                }
            }
        }
        else
        {
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateMessageObject = factoryManager->make_ptr<StateMessageObject>();
            stateMessageObject->setType( StateMessage::SET_MESH );
            stateMessageObject->setObject( mesh );

            FB_ASSERT( m_stateContext );
            m_stateContext->addMessage( stateTask, stateMessageObject );
        }
        */
    }

    auto PhysxMeshShape::getMesh() const -> SmartPtr<IMesh>
    {
        if( auto meshResource = getMeshResource() )
        {
            return meshResource->getMesh();
        }

        return nullptr;
    }

    auto PhysxMeshShape::getCleanMesh() const -> SmartPtr<IMesh>
    {
        return m_cleanMesh;
    }

    void PhysxMeshShape::setCleanMesh( SmartPtr<IMesh> cleanMesh )
    {
        m_cleanMesh = cleanMesh;
    }

    auto PhysxMeshShape::getMeshPath() const -> String
    {
        if( auto meshResource = getMeshResource() )
        {
            return meshResource->getFilePath();
        }

        return "";
    }

    auto PhysxMeshShape::isValid() const -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto state = scene->getState();
        if( state == scene::IScene::State::Play )
        {
            auto shape = getShape();

            const auto loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                if( !shape )
                {
                    return false;
                }
            }
        }

        return true;
    }

    auto PhysxMeshShape::getOutputStream() const -> physx::PxOutputStream *
    {
        return m_outputStream;
    }

    void PhysxMeshShape::setOutputStream( physx::PxOutputStream *outputStream )
    {
        m_outputStream = outputStream;
    }

    void PhysxMeshShape::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    auto PhysxMeshShape::isConvex() const -> bool
    {
        return m_isConvex;
    }

    void PhysxMeshShape::setConvex( bool convex )
    {
        m_isConvex = convex;
    }

    PhysxMeshShape::MeshShapeStateListener::~MeshShapeStateListener() = default;

    void PhysxMeshShape::MeshShapeStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        ShapeStateListener::handleStateChanged( state );

        auto boxShapeState = fb::static_pointer_cast<BoxShapeState>( state );
        if( boxShapeState )
        {
            auto shape = m_owner->getShape();
            if( shape )
            {
                auto extents = boxShapeState->getExtents() / static_cast<real_Num>( 2.0 );
                physx::PxVec3 dimensions( ( extents.X() ), ( extents.Y() ), ( extents.Z() ) );
                physx::PxBoxGeometry geometry( dimensions );
                shape->setGeometry( geometry );
            }

            state->setDirty( false );
        }
    }

    void PhysxMeshShape::MeshShapeStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        FB_ASSERT( message );

        ShapeStateListener::handleStateChanged( message );

        if( message->isExactly<StateMessageObject>() )
        {
            auto stateMessageObject = fb::static_pointer_cast<StateMessageObject>( message );
            auto type = stateMessageObject->getType();

            if( type == StateMessage::SET_MESH )
            {
                if( auto meshObject = stateMessageObject->getObject() )
                {
                    m_owner->setMeshResource( meshObject );
                }
            }
        }
    }

    void PhysxMeshShape::MeshShapeStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    void PhysxMeshShape::cookMesh( const String &path )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto pPhysicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( pPhysicsManager );

            auto physicsManager = fb::static_pointer_cast<PhysxManager>( pPhysicsManager );
            auto physics = physicsManager->getPhysics();
            FB_ASSERT( physics );

            auto cooking = physicsManager->getCooking();
            FB_ASSERT( cooking );

            auto meshCachePath = getMeshCachePath( 0 );
            readMeshData();

            auto cachePath = applicationManager->getCachePath();

            if( auto mesh = getMesh() )
            {
                if( mesh->getHasSharedVertexData() )
                {
                    auto numSubMeshes = m_indices.size();
                    for( size_t i = 0; i < numSubMeshes; ++i )
                    {
                        auto &indices = m_indices[i];

                        physx::PxTriangleMeshDesc meshDesc;

                        meshDesc.points.count = static_cast<u32>( m_sharedVertices.size() );
                        meshDesc.triangles.count = static_cast<u32>( indices.size() / 3 );
                        meshDesc.points.stride = sizeof( Vector3<real_Num> );
                        meshDesc.triangles.stride = sizeof( u32 ) * 3;
                        meshDesc.points.data = &m_sharedVertices[0];
                        meshDesc.triangles.data = &indices[0];
                        //meshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

                        std::string fileName = Path::getFileNameWithoutExtension( path );
                        std::string filePath = cachePath + "/" + fileName + ".pxtrianglemesh";
                        filePath = StringUtil::cleanupPath( filePath );

                        if( !fileSystem->isExistingFile( filePath ) )
                        {
                            // if (cooking->validateTriangleMesh(meshDesc))
                            {
                                physx::PxDefaultFileOutputStream outputStream( filePath.c_str() );
                                bool success = cooking->cookTriangleMesh( meshDesc, outputStream );
                                if( !success )
                                {
                                    fileSystem->deleteFile( filePath );
                                }
                            }
                        }

                        auto refreshPath = Path::getFilePath( filePath );
                        fileSystem->refreshPath( refreshPath, true );
                    }
                }
                else
                {
                    auto numSubMeshes = m_indices.size();
                    for( size_t i = 0; i < numSubMeshes; ++i )
                    {
                        auto &vertices = m_vertices[i];
                        auto &indices = m_indices[i];

                        physx::PxTriangleMeshDesc meshDesc;

                        meshDesc.points.count = static_cast<u32>( vertices.size() );
                        meshDesc.triangles.count = static_cast<u32>( indices.size() / 3 );
                        meshDesc.points.stride = sizeof( Vector3<real_Num> );
                        meshDesc.triangles.stride = sizeof( u32 ) * 3;
                        meshDesc.points.data = &vertices[0];
                        meshDesc.triangles.data = &indices[0];
                        //meshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

                        std::string fileName = Path::getFileNameWithoutExtension( path );

                        std::string filePath;
                        if( !StringUtil::isNullOrEmpty( cachePath ) )
                        {
                            filePath = cachePath + "/" + fileName + ".pxtrianglemesh";
                        }
                        else
                        {
                            filePath = fileName + ".pxtrianglemesh";
                        }

                        filePath = StringUtil::cleanupPath( filePath );

                        auto folderPath = Path::getFilePath( filePath );
                        if( fileSystem->isExistingFolder( folderPath ) )
                        {
                            if( !fileSystem->isExistingFile( filePath ) )
                            {
                                //auto validateResult = cooking->validateTriangleMesh( meshDesc );
                                //FB_ASSERT( validateResult )

                                //if( validateResult )
                                {
                                    physx::PxDefaultFileOutputStream outputStream( filePath.c_str() );
                                    bool success = cooking->cookTriangleMesh( meshDesc, outputStream );
                                    if( !success )
                                    {
                                        fileSystem->deleteFile( filePath );
                                    }
                                }
                            }

                            auto refreshPath = Path::getFilePath( filePath );
                            fileSystem->refreshPath( refreshPath, true );
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxMeshShape::cookMesh()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto fileSystem = applicationManager->getFileSystem();

            auto pPhysicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( pPhysicsManager );

            auto physicsManager = fb::static_pointer_cast<PhysxManager>( pPhysicsManager );
            auto physics = physicsManager->getPhysics();
            FB_ASSERT( physics );

            auto cooking = physicsManager->getCooking();
            FB_ASSERT( cooking );

            auto path = getMeshPath();
            auto cachePath = applicationManager->getCachePath();

            if( auto mesh = getMesh() )
            {
                if( mesh->getHasSharedVertexData() )
                {
                    auto numSubMeshes = m_indices.size();
                    for( size_t i = 0; i < numSubMeshes; ++i )
                    {
                        auto &indices = m_indices[i];

                        physx::PxTriangleMeshDesc meshDesc;

                        meshDesc.points.count = static_cast<u32>( m_sharedVertices.size() );
                        meshDesc.triangles.count = static_cast<u32>( indices.size() / 3 );
                        meshDesc.points.stride = sizeof( Vector3<real_Num> );
                        meshDesc.triangles.stride = sizeof( u32 ) * 3;
                        meshDesc.points.data = &m_sharedVertices[0];
                        meshDesc.triangles.data = &indices[0];
                        //meshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

                        std::string fileName = Path::getFileNameWithoutExtension( path );
                        std::string filePath = cachePath + "/" + fileName + ".pxtrianglemesh";
                        filePath = StringUtil::cleanupPath( filePath );

                        if( !fileSystem->isExistingFile( filePath ) )
                        {
                            // if (cooking->validateTriangleMesh(meshDesc))
                            {
                                physx::PxDefaultFileOutputStream outputStream( filePath.c_str() );
                                bool success = cooking->cookTriangleMesh( meshDesc, outputStream );
                                if( !success )
                                {
                                    fileSystem->deleteFile( filePath );
                                }
                            }
                        }

                        //if( !applicationManager->isPlaying() )
                        //{
                        //    auto refreshPath = Path::getFilePath( filePath );
                        //    fileSystem->refreshPath( refreshPath, true );
                        //}
                    }
                }
                else
                {
                    auto numSubMeshes = m_indices.size();
                    for( size_t i = 0; i < numSubMeshes; ++i )
                    {
                        auto &vertices = m_vertices[i];
                        auto &indices = m_indices[i];

                        physx::PxTriangleMeshDesc meshDesc;

                        meshDesc.points.count = static_cast<u32>( vertices.size() );
                        meshDesc.triangles.count = static_cast<u32>( indices.size() / 3 );
                        meshDesc.points.stride = sizeof( Vector3<real_Num> );
                        meshDesc.triangles.stride = sizeof( u32 ) * 3;
                        meshDesc.points.data = &vertices[0];
                        meshDesc.triangles.data = &indices[0];
                        //meshDesc.flags = physx::PxMeshFlag::eFLIPNORMALS;

                        std::string fileName = Path::getFileNameWithoutExtension( path );

                        std::string filePath;
                        if( !StringUtil::isNullOrEmpty( cachePath ) )
                        {
                            filePath = cachePath + "/" + fileName + ".pxtrianglemesh";
                        }
                        else
                        {
                            filePath = fileName + ".pxtrianglemesh";
                        }

                        filePath = StringUtil::cleanupPath( filePath );

                        if( StringUtil::contains( filePath, "grass" ) )
                        {
                            int a = 0;
                            a = 0;
                        }

                        auto folderPath = Path::getFilePath( filePath );
                        if( fileSystem->isExistingFolder( folderPath ) )
                        {
                            if( !fileSystem->isExistingFile( filePath ) )
                            {
                                //auto validateResult = cooking->validateTriangleMesh( meshDesc );
                                //FB_ASSERT( validateResult )

                                //if( validateResult )
                                {
                                    auto *outputStream = new MemoryOutputStream;
                                    bool success = cooking->cookTriangleMesh( meshDesc, *outputStream );
                                    if( !success )
                                    {
                                        fileSystem->deleteFile( filePath );
                                    }

                                    fileSystem->writeAllBytes( filePath, outputStream->getData(),
                                                               outputStream->getSize() );

                                    setOutputStream( outputStream );
                                }
                            }

                            //if( !applicationManager->isPlaying() )
                            //{
                            //    auto refreshPath = Path::getFilePath( filePath );
                            //    fileSystem->refreshPath( refreshPath, true );
                            //}
                        }
                        else
                        {
                            auto *outputStream = new MemoryOutputStream;
                            bool success = cooking->cookTriangleMesh( meshDesc, *outputStream );
                            if( !success )
                            {
                                fileSystem->deleteFile( filePath );
                            }

                            fileSystem->writeAllBytes( filePath, outputStream->getData(),
                                                       outputStream->getSize() );

                            setOutputStream( outputStream );
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxMeshShape::setVertices( const Array<Vector3<real_Num>> &val )
    {
        m_sharedVertices = val;
    }

    auto PhysxMeshShape::getVertices() const -> const Array<Vector3<real_Num>> &
    {
        return m_sharedVertices;
    }

    auto PhysxMeshShape::getVertices() -> Array<Vector3<real_Num>> &
    {
        return m_sharedVertices;
    }

    auto PhysxMeshShape::getNumSubMeshes() const -> s32
    {
        return static_cast<s32>( m_indices.size() );
    }

    auto PhysxMeshShape::getIndices( s32 subMeshIdx ) -> Array<u32> &
    {
        return m_indices[subMeshIdx];
    }

    auto PhysxMeshShape::getSubMeshTransform( s32 subMeshIdx ) -> Transform3<real_Num>
    {
        return m_transforms[subMeshIdx];
    }

    auto PhysxMeshShape::getIndicesAs2dArray() const -> Array<Array<u16>>
    {
        Array<Array<u16>> indices;

        // std::map<s32, Array<u16>>::const_iterator it = m_indices.begin();
        // for (; it != m_indices.end(); ++it)
        //{
        //	const Array<u16>& data = it->second;
        //	indices.push_back(data);
        // }

        return indices;
    }

    auto PhysxMeshShape::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;

        // objects.push_back(m_rigidDynamic);
        // objects.push_back(m_rigidStatic);
        // objects.push_back(m_clonedActor);

        objects.emplace_back( m_stateContext );
        objects.emplace_back( m_stateListener );

        if( auto stateContext = getStateContext() )
        {
            auto states = stateContext->getStates();
            for( auto state : states )
            {
                objects.emplace_back( state );
            }
        }

        return objects;
    }

    auto PhysxMeshShape::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();

        auto convex = isConvex();
        properties->setProperty( "convex", convex );

        return properties;
    }

    void PhysxMeshShape::setProperties( SmartPtr<Properties> properties )
    {
        auto convex = isConvex();
        properties->getPropertyValue( "convex", convex );
        setConvex( convex );
    }
}  // namespace fb::physics
