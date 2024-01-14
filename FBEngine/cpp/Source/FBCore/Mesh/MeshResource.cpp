#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/MeshResource.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MeshResource, IMeshResource );

    MeshResource::MeshResource() = default;

    MeshResource::~MeshResource() = default;

    auto MeshResource::hasSkeleton() const -> bool
    {
        return false;
    }

    auto MeshResource::getSkeletonName() const -> String
    {
        return "";
    }

    auto MeshResource::getNumLodLevels() const -> u32
    {
        return 0;
    }

    auto MeshResource::isEdgeListBuilt() const -> bool
    {
        return false;
    }

    auto MeshResource::hasVertexAnimation() const -> bool
    {
        return false;
    }

    void MeshResource::save()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = fb::static_pointer_cast<Properties>( toData() );
            FB_ASSERT( data );

            auto dataStr = DataUtil::toString( data.get(), true );
            FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            auto fileId = getSettingsFileSystemId();
            FB_ASSERT( fileId != 0 );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileId, fileInfo ) )
            {
                fileSystem->writeAllText( fileInfo.filePath.c_str(), dataStr );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::import()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto meshPath = getFilePath();
            FB_ASSERT( !StringUtil::isNullOrEmpty( meshPath ) );

            if( !StringUtil::isNullOrEmpty( meshPath ) )
            {
                resourceDatabase->importFile( meshPath );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto meshManager = applicationManager->getMeshManager();
            FB_ASSERT( meshManager );

            auto meshPath = getFilePath();
            if( !StringUtil::isNullOrEmpty( meshPath ) )
            {
                auto isExistingFile = fileSystem->isExistingFile( meshPath, false, true );
                if( !isExistingFile )
                {
                    isExistingFile = fileSystem->isExistingFile( meshPath, true, true );
                }

                if( isExistingFile )
                {
                    auto meshLoader = applicationManager->getMeshLoader();
                    FB_ASSERT( meshLoader );

                    if( meshLoader )
                    {
                        m_mesh = meshLoader->loadMesh( this );

                        if( m_mesh )
                        {
                            m_mesh->setName( meshPath );
                        }
                    }
                }
                else
                {
                    FB_LOG_ERROR( "File not found: " + meshPath );
                }
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            unload( nullptr );
            load( nullptr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( m_mesh )
            {
                m_mesh->unload( nullptr );
                m_mesh = nullptr;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto MeshResource::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();
        properties->setProperty( "scale", m_scale );
        properties->setProperty( "constraints", m_constraints );
        properties->setProperty( "animation", m_animation );
        properties->setProperty( "visibility", m_visibility );
        properties->setProperty( "cameras", m_cameras );
        properties->setProperty( "lights", m_lights );
        properties->setProperty( "lightmapUVs", m_lightmapUVs );

        properties->setProperty( "Save", "SaveButton", "button", false );
        properties->setProperty( "Import", "ImportButton", "button", false );

        return properties;
    }

    void MeshResource::setProperties( SmartPtr<Properties> properties )
    {
        auto scale = getScale();
        auto constraints = getConstraints();
        auto animation = getAnimation();
        auto visibility = getVisibility();
        auto cameras = getCameras();
        auto lights = getLights();
        auto lightmapUVs = getLightmapUVs();

        properties->getPropertyValue( "scale", scale );
        properties->getPropertyValue( "constraints", constraints );
        properties->getPropertyValue( "animation", animation );
        properties->getPropertyValue( "visibility", visibility );
        properties->getPropertyValue( "cameras", cameras );
        properties->getPropertyValue( "lights", lights );
        properties->getPropertyValue( "lightmapUVs", lightmapUVs );

        m_scale = scale;
        m_constraints = constraints;
        m_animation = animation;
        m_visibility = visibility;
        m_cameras = cameras;
        m_lights = lights;
        m_lightmapUVs = lightmapUVs;

        if( properties->hasProperty( "Save" ) )
        {
            auto &resetButton = properties->getPropertyObject( "Save" );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                save();
            }
        }

        if( properties->hasProperty( "Import" ) )
        {
            auto &resetButton = properties->getPropertyObject( "Import" );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                import();
            }
        }
    }

    auto MeshResource::getStateContext() const -> SmartPtr<IStateContext>
    {
        return m_stateContext;
    }

    void MeshResource::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    void MeshResource::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

    auto MeshResource::getScale() const -> f32
    {
        return m_scale;
    }

    void MeshResource::setScale( f32 val )
    {
        m_scale = val;
    }

    auto MeshResource::getMaterialNaming() const -> IMeshResource::MaterialNaming
    {
        return m_materialNaming;
    }

    void MeshResource::setMaterialNaming( MaterialNaming val )
    {
        m_materialNaming = val;
    }

    auto MeshResource::getConstraints() const -> bool
    {
        return m_constraints;
    }

    void MeshResource::setConstraints( bool val )
    {
        m_constraints = val;
    }

    auto MeshResource::getAnimation() const -> bool
    {
        return m_animation;
    }

    void MeshResource::setAnimation( bool val )
    {
        m_animation = val;
    }

    auto MeshResource::getVisibility() const -> bool
    {
        return m_visibility;
    }

    void MeshResource::setVisibility( bool val )
    {
        m_visibility = val;
    }

    auto MeshResource::getCameras() const -> bool
    {
        return m_cameras;
    }

    void MeshResource::setCameras( bool val )
    {
        m_cameras = val;
    }

    auto MeshResource::getLights() const -> bool
    {
        return m_lights;
    }

    void MeshResource::setLights( bool val )
    {
        m_lights = val;
    }

    auto MeshResource::getLightmapUVs() const -> bool
    {
        return m_lightmapUVs;
    }

    void MeshResource::setLightmapUVs( bool val )
    {
        m_lightmapUVs = val;
    }

    auto MeshResource::getMesh() const -> SmartPtr<IMesh>
    {
        return m_mesh;
    }

    void MeshResource::setMesh( SmartPtr<IMesh> val )
    {
        m_mesh = val;
    }

}  // namespace fb
