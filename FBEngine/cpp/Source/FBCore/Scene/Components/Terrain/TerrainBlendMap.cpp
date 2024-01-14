#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainBlendMap.h>
#include <FBCore/Interface/Graphics/ITerrainBlendMap.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, TerrainBlendMap, SubComponent );

    TerrainBlendMap::TerrainBlendMap() = default;

    TerrainBlendMap::~TerrainBlendMap()
    {
        unload( nullptr );
    }

    void TerrainBlendMap::load( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TerrainBlendMap::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto TerrainBlendMap::getBlendMap() const -> SmartPtr<render::ITerrainBlendMap>
    {
        return m_blendMap;
    }

    void TerrainBlendMap::setBlendMap( SmartPtr<render::ITerrainBlendMap> blendMap )
    {
        m_blendMap = blendMap;
    }
}  // namespace fb::scene
