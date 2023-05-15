#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxTerrain.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBMesh/FBMesh.h>
#include <FBPhysx/FBPhysxCooker.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/FBPhysxStream.h>
#include <FBPhysx/PhysxMemoryOutputStream.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBCore/FBCore.h>
#include <geometry/PxHeightFieldDesc.h>

namespace fb
{
    namespace physics
    {

        PhysxTerrain::PhysxTerrain()
        {
            m_heightScale = Vector3F::unit();
            m_terrainSize = Vector3F( 1024, 0.0, 1024 );
            m_size = Vector2I( 513, 513 );
            m_heightData.resize( 513 * 513 );

            for( auto &value : m_heightData )
            {
                value = 0.0f;
            }
        }

        PhysxTerrain::~PhysxTerrain()
        {
            unload( nullptr );
        }

        void PhysxTerrain::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager =
                    fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
                FB_ASSERT( physicsManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto physics = physicsManager->getPhysics();
                FB_ASSERT( physics );

                ISharedObject::ScopedLock lock( physicsManager );

                //auto shape = createDefaultTerrain( 513, 513, 0.0f );
                //setShape( shape );

                // smart_ptr<physics::PxScene> physicsScene =
                // applicationManager->getPhysicsManager()->getPhysicsScene();

                // smart_ptr<physics::PxPhysics> physics =
                // applicationManager->getPhysicsManager()->getPhysics();
                // PhysicsManagerPtr physicsManager =
                // applicationManager->getPhysicsManager();
                // physx::PxCooking* cooking =
                // physicsManager->getCooking();

                auto heightData = getHeightData();

                // auto transform = getTransform();
                // auto pos = transform.getPosition();
                // auto rot = transform.getOrientation();
                // auto tranformScale = transform.getScale();

                auto transformF = Transform3F();

                auto hfGeom = createTerrainGeometry( heightData, getSize(), getHeightScale(),
                                                     getTerrainSize(), transformF );

                auto pose = physx::PxTransform::createIdentity();

                // Transform3<real_Num> t = getTransform();
                // Vector3<real_Num> position = t.getPosition();
                // Vector3<real_Num> scale = t.getScale();
                Quaternion<real_Num> orientation;  // = t.getOrientation();

                auto size = getSize();
                auto terrainSize = getTerrainSize();
                auto terrainScale =
                    terrainSize / Vector3F( (f32)( size.x - 1 ), 1.0f, (f32)( size.y - 1 ) );
                auto halfTerrainSize = terrainSize * real_Num(0.5);

                auto center = Vector3<real_Num>( -halfTerrainSize.x, 0.0f, -halfTerrainSize.z );
                pose.p = physx::PxVec3( center.x, center.y, center.z );
                pose.q = physx::PxQuat( orientation.x, orientation.y, orientation.z, orientation.w );

                auto terrainMat = physics->createMaterial( 1, 1, 1 );

                auto shape = physics->createShape( hfGeom, *terrainMat );
                shape->setLocalPose( pose );
                setShape( shape );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxTerrain::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //
        // void PhysxTerrain::initialise2(SmartPtr<scene::IDirector> objectTemplate)
        //{
        //	//RecursiveMutex::ScopedLock lock(PhysxMutex);

        //	auto engine = core::IApplicationManager::instance();
        //	auto graphicsSystem = engine->getGraphicsSystem();
        //	auto pPhysicsManager = engine->getPhysicsManager3();
        //	auto physicsManager = fb::static_pointer_cast<PhysxManager>(pPhysicsManager);

        //	auto terrainTemplate = fb::dynamic_pointer_cast<TerrainTemplate>(objectTemplate);
        //	auto heightData = terrainTemplate->getHeightData();
        //	if (heightData.empty())
        //	{
        //		FB_EXCEPTION("No height data.");
        //	}

        //	u32 numCols, numRows;
        //	numRows = numCols = (u32)terrainTemplate->getTerrainSize();

        //	physx::PxHeightFieldDesc heightFieldDesc;
        //	heightFieldDesc.format = physx::PxHeightFieldFormat::eS16_TM;
        //	heightFieldDesc.nbColumns = numCols;
        //	heightFieldDesc.nbRows = numRows;
        //	heightFieldDesc.samples.stride = sizeof(physx::PxU32);

        //	//physx::PxReal heightScale = 1.f;
        //	physx::PxReal heightScale = (terrainTemplate->getMaxHeight() / 65535.0f / 2.f);

        //	physx::PxHeightFieldSample* samples = new physx::PxHeightFieldSample[numRows * numCols];

        //	physx::PxU8* currentByte = (physx::PxU8*)samples;

        //	for (u16 y = 0; y < numRows; y++)
        //		//for (s32  y = numRows-1; y >= 0; y--)
        //	{
        //		for (u16 x = 0; x < numCols; x++)
        //			//for (s32 x = numCols - 1; x >= 0;  x--)
        //		{
        //			physx::PxHeightFieldSample* currentSample = (physx::PxHeightFieldSample*)currentByte;

        //			//f32 height = heightData[column + row * numCols];
        //			//f32 height = heightData[x + (numCols-1-y) * numCols];
        //			f32 height = heightData[x + y * numCols];
        //			//height *= std::numeric_limits<physx::PxI16>::max();
        //			height *= terrainTemplate->getMaxHeight();
        //			height /= heightScale;

        //			//LOG_MESSAGE("HeightMap", StringUtil::toString(height));

        //			currentSample->height = (physx::PxI16)(height);
        //			currentSample->materialIndex0 = 0;
        //			currentSample->materialIndex1 = 0;

        //			currentByte += heightFieldDesc.samples.stride;
        //		}
        //	}

        //	heightFieldDesc.samples.data = samples;

        //	physx::PxPhysics* physics = physicsManager->getPhysics();
        //	physx::PxHeightField* heightField = physics->createHeightField(heightFieldDesc);

        //	physx::PxTransform pose = physx::PxTransform::createIdentity();

        //	/**
        //	* Calcul position
        //	*/
        //	Vector3F position = terrainTemplate->getPosition();
        //	position[0] = position[0] - (terrainTemplate->getWorldSize() / 2.0f);

        //	float minHeight = terrainTemplate->getMinHeight();
        //	float maxHeight = terrainTemplate->getMaxHeight();
        //	position[1] = position[1] + ((terrainTemplate->getMaxHeight() +
        // terrainTemplate->getMinHeight()) / 2.0f); 	position[2] = position[2] -
        //(terrainTemplate->getWorldSize() / 2.0f);

        //	//row scale
        //	f32 scale = terrainTemplate->getWorldSize() / physx::PxReal(terrainTemplate->getTerrainSize()
        //- 1);
        //	//f32 scale = terrainTemplate->getGridSpacing();

        //	pose.p = physx::PxVec3(position[0], position[1], position[2]);
        //	physx::PxRigidStatic* hfActor = physics->createRigidStatic(pose);

        //	physx::PxHeightFieldGeometry hfGeom(heightField, physx::PxMeshGeometryFlags(), heightScale,
        //(physx::PxReal)scale, (physx::PxReal)scale);

        //	auto m = physicsManager->createMaterial();
        //	auto pMat = fb::dynamic_pointer_cast<PhysxMaterial>(m);
        //	physx::PxMaterial* terrainMat = pMat->getMaterial();

        //	physx::PxShape* shape = hfActor->createShape(hfGeom, *terrainMat);

        //	physx::PxFilterData collFilterData;
        //	collFilterData.word0 = 1;
        //	collFilterData.word1 = 1;
        //	shape->setSimulationFilterData(collFilterData);

        //	hfActor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        //	//physicsManager->getScene()->addActor(*hfActor);

        //	physx::PxFilterData simFilterData;
        //	simFilterData.word0 = COLLISION_FLAG_GROUND;
        //	simFilterData.word1 = COLLISION_FLAG_GROUND_AGAINST;

        //	physx::PxFilterData qryFilterData;
        //	SampleVehicleSetupDrivableShapeQueryFilterData(&qryFilterData);

        //	shape->setSimulationFilterData(simFilterData);
        //	shape->setQueryFilterData(qryFilterData);

        //}

        inline u32 getIndex( u32 tileSize, u32 x, u32 z )
        {
            return x + z * tileSize;
        }

        ///* SmartPtr<IMesh> PhysxTerrain::getMesh( SmartPtr<scene::IDirector> objectTemplate ) const
        // {
        //     //SmartPtr<TerrainTemplate> terrainTemplate; // = objectTemplate;
        //     Array<f32> heightData;// = terrainTemplate->getHeightData();

        //     f32 scale;// = terrainTemplate->getWorldSize() / ( terrainTemplate->getTerrainSize() - 1 );
        //     f32 heightScale;// = terrainTemplate->getMaxHeight();

        //     SmartPtr<IMesh> mesh( new CMesh );
        //     SmartPtr<ISubMesh> subMesh( new CSubMesh );
        //     mesh->addSubMesh( subMesh );

        //     // create sub mesh data
        //     SmartPtr<IVertexDeclaration> vertexDeclaration( new CVertexDeclaration );
        //     vertexDeclaration->addElement( 0, sizeof( Vector3F ),
        //                                    CVertexDeclaration::VertexElementSemantic::VES_POSITION,
        //                                    IVertexElement::VertexElementType::VET_FLOAT3 );
        //     // vertexDeclaration->addElement(sizeof(Vector3F), VertexDeclaration::VES_NORMAL,
        //     // VertexDeclaration::VET_FLOAT3); vertexDeclaration->addElement(sizeof(Vector2F),
        //     // VertexDeclaration::VES_TEXTURE_COORDINATES, VertexDeclaration::VET_FLOAT2, 0);
        //     // vertexDeclaration->addElement(sizeof(Vector2F),
        //     // VertexDeclaration::VES_TEXTURE_COORDINATES, VertexDeclaration::VET_FLOAT2, 1);

        //     SmartPtr<IVertexBuffer> vertexBuffer( new CVertexBuffer );
        //     vertexBuffer->setVertexDeclaration( vertexDeclaration );

        //     u32 tileSize;// = static_cast<u32>(terrainTemplate->getTerrainSize());
        //     u32 numVerticies = tileSize * tileSize;

        //     vertexBuffer->setNumVerticies( numVerticies );
        //     auto vertexData = static_cast<f32 *>(vertexBuffer->createVertexData());
        //     f32 *vertexDataPtr = vertexData;

        //     u32 numVerticiesAdded = 0;
        //     const f32 tdSize = 1.0f / static_cast<f32>(tileSize - 1);
        //     for(s32 x = 0; x < static_cast<s32>(tileSize); ++x)
        //     {
        //         for(s32 z = 0; z < static_cast<s32>(tileSize); ++z)
        //         {
        //             s32 indexZ = -z;

        //             s32 zIdx = ( tileSize - 1 ) - z;
        //             zIdx = z;

        //             f32 height = heightData[x + z * tileSize];

        //             Vector3F position;
        //             position.X() = static_cast<f32>(x);
        //             position.Y() = height * heightScale;
        //             position.Z() = static_cast<f32>(z);
        //             position = position * scale;

        //             *vertexDataPtr++ = position.X();
        //             *vertexDataPtr++ = position.Y();
        //             *vertexDataPtr++ = position.Z();

        //             /*				Vector3F normal;
        //                             normal = Vector3F::UNIT_Y;
        //                             *vertexDataPtr++ = normal.X();
        //                             *vertexDataPtr++ = normal.Y();
        //                             *vertexDataPtr++ = normal.Z();

        //                             Vector2F texCoord0;
        //                             texCoord0.X() = x;
        //                             texCoord0.Y() = z;
        //                             *vertexDataPtr++ = texCoord0.X();
        //                             *vertexDataPtr++ = texCoord0.Y();

        //                             Vector2F texCoord1;
        //                             texCoord1.X() = (f32)x / (f32)tileSize;
        //                             texCoord1.Y() = (f32)z / (f32)tileSize;
        //                             *vertexDataPtr++ = texCoord1.X();
        //                             *vertexDataPtr++ = texCoord1.Y();*/

        //             numVerticiesAdded++;
        //         }
        //     }

        //     u32 numIndices = tileSize * tileSize * 6;
        //     SmartPtr<IIndexBuffer> indexBuffer( new CIndexBuffer );
        //     indexBuffer->setNumIndices( numIndices );
        //     indexBuffer->setIndexType( CIndexBuffer::Type::IT_32BIT );
        //     auto indexDataPtr = static_cast<u32 *>(indexBuffer->createIndexData());

        //     u32 index11;
        //     u32 index21;
        //     u32 index12;
        //     u32 index22;
        //     s32 step = 6;

        //     u32 numIndicesSet = 0;
        //     for(s32 z = 0; z < static_cast<s32>(tileSize) - 1; z += step)
        //     {
        //         for(s32 x = 0; x < static_cast<s32>(tileSize) - 1; x += step)
        //         {
        //             index11 = getIndex( tileSize, x, z );
        //             index21 = getIndex( tileSize, x + step, z );
        //             index12 = getIndex( tileSize, x, z + step );
        //             index22 = getIndex( tileSize, x + step, z + step );

        //             *indexDataPtr++ = index22;
        //             *indexDataPtr++ = index11;
        //             *indexDataPtr++ = index12;

        //             *indexDataPtr++ = index21;
        //             *indexDataPtr++ = index11;
        //             *indexDataPtr++ = index22;

        //             numIndicesSet += 6;
        //         }
        //     }

        //     size_t newLength = ( tileSize / step ) * ( tileSize / step ) * 2 * 2 * 2;
        //     indexBuffer->setNumIndices( numIndicesSet );

        //     subMesh->setVertexBuffer( vertexBuffer );
        //     subMesh->setIndexBuffer( indexBuffer );

        //     return mesh;
        // }

        PxHeightFieldGeometry PhysxTerrain::createTerrainGeometry( const Array<float> &heightData,
                                                                   const Vector2I &size,
                                                                   const Vector3F &heightScale,
                                                                   const Vector3F &terrainSize,
                                                                   const Transform3<f32> &t )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
            auto physics = physicsManager->getPhysics();

            auto physicsScene = applicationManager->getPhysicsScene();

            PxCooking *cooking = physicsManager->getCooking();

            PxHeightFieldDesc heightFieldDesc;
            heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
            heightFieldDesc.nbColumns = size.X();
            heightFieldDesc.nbRows = size.Y();
            heightFieldDesc.samples.stride = sizeof( PxHeightFieldSample );

            PxReal fHeightScale = heightScale.Y() / std::numeric_limits<PxI16>::max();

            int numSamples = size.X() * size.Y();
            auto samples = new PxHeightFieldSample[numSamples];

            auto currentByte = (PxU8 *)samples;

            // for ( int y = 0; y < m_size.y; y++ )
            //{

            for( int x = 0; x < size.X(); x++ )
            //{
            // for ( int x = m_size.x - 1; x >= 0; x-- )
            {
                for( int y = size.Y() - 1; y >= 0; y-- )
                {
                    auto currentSample = (PxHeightFieldSample *)currentByte;

                    int heightIdx = x + y * size.X();
                    float heightValue = heightData[heightIdx];
                    float height = heightValue * std::numeric_limits<PxI16>::max();

                    currentSample->height = static_cast<PxI16>( height );
                    currentSample->materialIndex0 = 0;
                    currentSample->materialIndex1 = 0;

                    currentByte += heightFieldDesc.samples.stride;
                }
            }

            heightFieldDesc.samples.data = samples;

            PxHeightField *heightField = physics->createHeightField( heightFieldDesc );

            PxTransform pose = PxTransform::createIdentity();

            auto position = t.getPosition();
            auto scale = t.getScale();
            auto orientation = t.getOrientation();

            auto terrainScale = terrainSize / Vector3F( static_cast<f32>( size.X() - 1 ), 1.0f,
                                                        static_cast<f32>( size.Y() - 1 ) );

            auto center = position + Vector3F( 0.0f, 0.0f, -terrainSize.Z() );
            pose.p = PhysxUtil::toPx( center );
            pose.q = PhysxUtil::toPx( orientation );

            return PxHeightFieldGeometry( heightField, PxMeshGeometryFlags(), fHeightScale,
                                          terrainScale.X(), terrainScale.Z() );
        }

        physx::PxShape *PhysxTerrain::createDefaultTerrain( s32 sizeX, s32 sizeY, f32 maxHeight )
        {
            using namespace physx;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
            auto gPhysics = physicsManager->getPhysics();

            // Create a heightfield
            PxHeightFieldDesc heightFieldDesc;
            heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
            heightFieldDesc.nbColumns = sizeX;
            heightFieldDesc.nbRows = sizeY;

            auto pData = new PxU16[sizeX * sizeY];
            heightFieldDesc.samples.data = pData;

            heightFieldDesc.samples.stride = sizeof( PxU16 );

            // Fill in the heightfield samples
            for( int i = 0; i < sizeX * sizeY; ++i )
            {
                // Generate some random height values
                //float height = maxHeight * (float)rand() / RAND_MAX;
                //pData[i] = PxU16( height * 0xffff );
                pData[i] = 0;
            }

            // Create a heightfield object
            PxHeightField *heightField = gPhysics->createHeightField( heightFieldDesc );
            BOOST_ASSERT( heightField != nullptr );

            PxMaterial *material = gPhysics->createMaterial( 1, 1, 1 );
            BOOST_ASSERT( material != nullptr );

            // Create a heightfield shape
            auto tolerancesScale = gPhysics->getTolerancesScale();
            auto heightScale = maxHeight / 0xffff;
            physx::PxHeightFieldGeometry hfGeom( heightField, physx::PxMeshGeometryFlags(), heightScale,
                                                 (physx::PxReal)1, (physx::PxReal)1 );
            PxShape *heightFieldShape = gPhysics->createShape( hfGeom, *material );
            BOOST_ASSERT( heightFieldShape != nullptr );

            // Create a heightfield actor
            //PxRigidActor *heightFieldActor =
            ///   gPhysics->createRigidStatic( PxTransform( PxIdentity ) );
            //heightFieldActor->attachShape( *heightFieldShape );

            // Add the heightfield actor to the scene
            //gScene->addActor( *heightFieldActor );

            return heightFieldShape;
        }

        void PhysxTerrain::setCollisionType( u32 mask )
        {
        }

        u32 PhysxTerrain::getCollisionType() const
        {
            return 0;
        }

        void PhysxTerrain::setCollisionMask( u32 mask )
        {
        }

        u32 PhysxTerrain::getCollisionMask() const
        {
            return 0;
        }

        void PhysxTerrain::setEnabled( bool enabled )
        {
        }

        bool PhysxTerrain::isEnabled() const
        {
            return true;
        }

        void *PhysxTerrain::getUserData() const
        {
            return nullptr;
        }

        void PhysxTerrain::setUserData( void *userData )
        {
        }

        Array<f32> PhysxTerrain::getHeightData() const
        {
            return m_heightData;
        }

        void PhysxTerrain::setHeightData( const Array<f32> &heightData )
        {
            m_heightData = heightData;
        }

        Vector2I PhysxTerrain::getSize() const
        {
            return m_size;
        }

        void PhysxTerrain::setSize( const Vector2I &size )
        {
            m_size = size;
        }

        Vector3<real_Num> PhysxTerrain::getTerrainSize() const
        {
            return m_terrainSize;
        }

        void PhysxTerrain::setTerrainSize( const Vector3<real_Num> &terrainSize )
        {
            m_terrainSize = terrainSize;
        }

        Vector3<real_Num> PhysxTerrain::getHeightScale() const
        {
            return m_heightScale;
        }

        void PhysxTerrain::setHeightScale( const Vector3<real_Num> &heightScale )
        {
            m_heightScale = heightScale;
        }

        PhysxTerrain::RaycastCallback::RaycastCallback( PhysxTerrain *collision ) :
            physx::PxRaycastCallback( buffer, 10 )
        {
        }

        PxAgain PhysxTerrain::RaycastCallback::processTouches( const PxRaycastHit *data, PxU32 nbHits )
        {
            for( PxU32 i = 0; i < nbHits; ++i )
            {
                const PxRaycastHit &hit = data[i];
                if( hit.actor->isRigidStatic() )
                {
                    m_hit = hit;
                    break;
                }
            }

            return false;
        }
    }  // end namespace physics
}  // end namespace fb
