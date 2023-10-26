#include <FBCore/FBCorePCH.h>
#include <FBCore/FBCore.h>
#include "FBCore/IO/FolderListingT.h"
#include "FBCore/IO/FileSystem.h"
#include "FBCore/IO/FileSystemArchive.h"
#include "FBCore/IO/FileDataStream.h"
#include "FBCore/IO/ObfuscatedZipFile.h"
#include "FBCore/IO/ZipArchive.h"
#include "FBCore/IO/ZipFile.h"
#include <FBCore/Vehicle/VehicleController.h>
#include <FBCore/Vehicle/WheelController.h>

namespace fb
{
    void FBCore::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<EventJob>();
        FactoryUtil::addFactory<FSMManager>();
        FactoryUtil::addFactory<Properties>();
        FactoryUtil::addFactory<StateQueueStandard>();
        FactoryUtil::addFactory<StateContextStandard>();
        FactoryUtil::addFactory<StateManagerStandard>();
        FactoryUtil::addFactory<WorkerThread>();

        FactoryUtil::addFactory<Data<FileInfo>>();

        FactoryUtil::addFactory<DirectoryListing>();

        FactoryUtil::addFactory<FileList>();
        FactoryUtil::addFactory<FileDataStream>();
        FactoryUtil::addFactory<FileSystemArchive>();
        FactoryUtil::addFactory<ObfuscatedZipFile>();
        FactoryUtil::addFactory<ZipFile>();

        FactoryUtil::addFactory<scene::Actor>();
        FactoryUtil::addFactory<scene::Director>();
        FactoryUtil::addFactory<scene::Scene>();
        FactoryUtil::addFactory<scene::SceneManager>();
        FactoryUtil::addFactory<scene::Transform>();

        FactoryUtil::addFactory<scene::SphericalCameraController>();
        FactoryUtil::addFactory<scene::ThirdPersonCameraController>();
        FactoryUtil::addFactory<scene::VehicleCameraController>();

        FactoryUtil::addFactory<scene::AudioEmitter>();
        FactoryUtil::addFactory<scene::Camera>();
        FactoryUtil::addFactory<scene::CarController>();
        FactoryUtil::addFactory<scene::Constraint>();
        FactoryUtil::addFactory<scene::CollisionBox>();
        FactoryUtil::addFactory<scene::CollisionMesh>();
        FactoryUtil::addFactory<scene::CollisionPlane>();
        FactoryUtil::addFactory<scene::CollisionSphere>();
        FactoryUtil::addFactory<scene::CollisionTerrain>();
        FactoryUtil::addFactory<scene::FiniteStateMachine>();
        FactoryUtil::addFactory<scene::Light>();
        FactoryUtil::addFactory<scene::Material>();
        FactoryUtil::addFactory<scene::Mesh>();
        FactoryUtil::addFactory<scene::MeshRenderer>();
        FactoryUtil::addFactory<scene::ParticleSystem>();
        FactoryUtil::addFactory<scene::Rigidbody>();
        FactoryUtil::addFactory<scene::Skybox>();
        FactoryUtil::addFactory<scene::TerrainBlendMap>();
        FactoryUtil::addFactory<scene::TerrainLayer>();
        FactoryUtil::addFactory<scene::TerrainSystem>();
        FactoryUtil::addFactory<scene::UserComponent>();
        FactoryUtil::addFactory<scene::WheelController>();

        FactoryUtil::addFactory<scene::Button>();
        FactoryUtil::addFactory<scene::Layout>();
        FactoryUtil::addFactory<scene::LayoutTransform>();
        FactoryUtil::addFactory<scene::Image>();
        FactoryUtil::addFactory<scene::InputField>();
        FactoryUtil::addFactory<scene::Text>();

        FactoryUtil::addFactory<StateMessageVector3>();
        FactoryUtil::addFactory<StateMessageVector4>();
        FactoryUtil::addFactory<StateMessageUIntValue>();
        FactoryUtil::addFactory<StateMessageIntValue>();
        FactoryUtil::addFactory<StateMessageVisible>();

        FactoryUtil::addFactory<MaterialPassState>();
        FactoryUtil::addFactory<PhysicsSceneState>();
        FactoryUtil::addFactory<SceneNodeState>();
        FactoryUtil::addFactory<RigidbodyState>();
        FactoryUtil::addFactory<TextureState>();
        FactoryUtil::addFactory<UITransformState>();

        FactoryUtil::addFactory<WindowMessageData>();

        factoryManager->setPoolSizeByType<scene::Actor>( 32 );
        factoryManager->setPoolSizeByType<scene::AudioEmitter>( 4 );

        factoryManager->setPoolSizeByType<Data<FileInfo>>( 32 );

        factoryManager->setPoolSizeByType<DirectoryListing>( 4096 );

        factoryManager->setPoolSizeByType<FileList>( 4096 );
        factoryManager->setPoolSizeByType<FileDataStream>( 4096 );
        factoryManager->setPoolSizeByType<FileSystemArchive>( 32 );
        factoryManager->setPoolSizeByType<ObfuscatedZipFile>( 32 );
        factoryManager->setPoolSizeByType<ZipFile>( 32 );

        factoryManager->setPoolSizeByType<EventJob>( 32 );
        factoryManager->setPoolSizeByType<Properties>( 32 );
        factoryManager->setPoolSizeByType<StateQueueStandard>( 32 );
        factoryManager->setPoolSizeByType<StateContextStandard>( 32 );
        factoryManager->setPoolSizeByType<WorkerThread>( 8 );

        factoryManager->setPoolSizeByType<SceneNodeState>( 32 );
        factoryManager->setPoolSizeByType<TextureState>( 32 );
        factoryManager->setPoolSizeByType<RigidbodyState>( 32 );

        factoryManager->setPoolSizeByType<WindowMessageData>( 4 );

        const auto size = 32;

        factoryManager->setPoolSizeByType<Properties>( size );

        factoryManager->setPoolSizeByType<scene::Actor>( size );
        factoryManager->setPoolSizeByType<scene::Transform>( size );

        factoryManager->setPoolSizeByType<scene::Material>( size );
        factoryManager->setPoolSizeByType<scene::Mesh>( size );
        factoryManager->setPoolSizeByType<scene::MeshRenderer>( size );
        factoryManager->setPoolSizeByType<scene::CollisionBox>( size );
        factoryManager->setPoolSizeByType<scene::Rigidbody>( size );

        const auto messagePoolSize = 12;

        factoryManager->setPoolSizeByType<StateMessageVector3>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageVector4>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageUIntValue>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageIntValue>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageVisible>( messagePoolSize );

        FactoryUtil::addFactory<FileList>();
        FactoryUtil::addFactory<FileSystem>();
        FactoryUtil::addFactory<FileDataStream>();
        FactoryUtil::addFactory<FileSystemArchive>();

        FactoryUtil::addFactory<VehicleController>();
        FactoryUtil::addFactory<WheelController>();

        FactoryUtil::addFactory<ZipArchive>();
        FactoryUtil::addFactory<ZipFile>();

        factoryManager->setPoolSizeByType<FileList>( 32 );
        factoryManager->setPoolSizeByType<FileSystemArchive>( 32 );
        factoryManager->setPoolSizeByType<FileDataStream>( 4 );
        factoryManager->setPoolSizeByType<ZipArchive>( 8 );
        factoryManager->setPoolSizeByType<ZipFile>( 4 );
    }

    void FBCore::unload( SmartPtr<ISharedObject> data )
    {
    }

}  // end namespace fb
