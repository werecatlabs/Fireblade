#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainLayer.h>
#include <FBCore/Scene/Components/Terrain/TerrainSystem.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, TerrainLayer, SubComponent );

        TerrainLayer::TerrainLayer()
        {
        }

        TerrainLayer::~TerrainLayer()
        {
            unload( nullptr );
        }

        void TerrainLayer::load( SmartPtr<ISharedObject> data )
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

        void TerrainLayer::unload( SmartPtr<ISharedObject> data )
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

        SmartPtr<Properties> TerrainLayer::getProperties() const
        {
            auto properties = SubComponent::getProperties();

            properties->setProperty( "baseTexture", m_baseTexture );
            properties->setProperty( "index", m_index );

            return properties;
        }

        void TerrainLayer::setProperties( SmartPtr<Properties> properties )
        {
            SmartPtr<render::ITexture> baseTexture;
            properties->getPropertyValue( "baseTexture", baseTexture );
            properties->getPropertyValue( "index", m_index );

            if( baseTexture != m_baseTexture )
            {
                m_baseTexture = baseTexture;

                if( auto parent = getParent() )
                {
                    auto terrain = fb::static_pointer_cast<TerrainSystem>( parent );
                    terrain->updateLayers();
                }
            }
        }

        SmartPtr<render::ITexture> TerrainLayer::getBaseTexture() const
        {
            return m_baseTexture;
        }

        void TerrainLayer::setBaseTexture( SmartPtr<render::ITexture> baseTexture )
        {
            m_baseTexture = baseTexture;
        }

        s32 TerrainLayer::getIndex() const
        {
            return m_index;
        }

        void TerrainLayer::setIndex( s32 index )
        {
            m_index = index;
        }

    }  // namespace scene
}  // namespace fb
