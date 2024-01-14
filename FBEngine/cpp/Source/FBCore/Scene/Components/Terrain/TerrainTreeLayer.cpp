#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainTreeLayer.h>
#include <FBCore/Scene/Components/Terrain/TerrainSystem.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Scene/IPrefab.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, TerrainTreeLayer, SubComponent );

    TerrainTreeLayer::TerrainTreeLayer() = default;

    TerrainTreeLayer::~TerrainTreeLayer()
    {
        unload( nullptr );
    }

    void TerrainTreeLayer::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TerrainTreeLayer::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            m_baseTexture = nullptr;
            SubComponent::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto TerrainTreeLayer::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = SubComponent::getProperties();

        properties->setProperty( "baseTexture", m_baseTexture );
        properties->setProperty( "index", m_index );

        return properties;
    }

    void TerrainTreeLayer::setProperties( SmartPtr<Properties> properties )
    {
        SmartPtr<render::ITexture> baseTexture;
        properties->getPropertyValue( "baseTexture", baseTexture );
        properties->getPropertyValue( "index", m_index );

        if( baseTexture != m_baseTexture )
        {
            m_baseTexture = baseTexture;

            if( auto parent = getParentComponent() )
            {
                auto terrain = fb::static_pointer_cast<TerrainSystem>( parent );
                terrain->updateLayers();
            }
        }
    }

    auto TerrainTreeLayer::getBaseTexture() const -> SmartPtr<render::ITexture>
    {
        return m_baseTexture;
    }

    void TerrainTreeLayer::setBaseTexture( SmartPtr<render::ITexture> baseTexture )
    {
        m_baseTexture = baseTexture;
    }

    auto TerrainTreeLayer::getIndex() const -> s32
    {
        return m_index;
    }

    void TerrainTreeLayer::setIndex( s32 index )
    {
        m_index = index;
    }

}  // namespace fb::scene
