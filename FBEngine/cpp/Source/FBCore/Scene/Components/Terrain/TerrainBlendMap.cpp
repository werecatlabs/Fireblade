#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainBlendMap.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, TerrainBlendMap, BaseSubComponent );

        TerrainBlendMap::TerrainBlendMap()
        {
        }

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

        SmartPtr<render::ITerrainBlendMap> TerrainBlendMap::getBlendMap() const
        {
            return m_blendMap;
        }

        void TerrainBlendMap::setBlendMap( SmartPtr<render::ITerrainBlendMap> blendMap )
        {
            m_blendMap = blendMap;
        }
    }  // namespace scene
}  // namespace fb
