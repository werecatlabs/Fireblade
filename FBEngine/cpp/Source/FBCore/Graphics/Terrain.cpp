#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Terrain.h>
#include <FBCore/FBCore.h>

namespace fb::render
{
    Terrain::Terrain() = default;

    Terrain::~Terrain() = default;

    auto Terrain::getWorldTransform() const -> Transform3<real_Num>
    {
        return {};
    }

    void Terrain::setWorldTransform( const Transform3<real_Num> &worldTransform )
    {
    }

    auto Terrain::getPosition() const -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    void Terrain::setPosition( const Vector3<real_Num> &position )
    {
    }

    auto Terrain::getHeightAtWorldPosition( const Vector3<real_Num> &position ) const -> f32
    {
        return 0.f;
    }

    auto Terrain::getSize() const -> u16
    {
        return 0;
    }

    auto Terrain::getTerrainSpacePosition( const Vector3<real_Num> &worldSpace ) const
        -> Vector3<real_Num>
    {
        return Vector3<real_Num>::zero();
    }

    auto Terrain::getHeightData() const -> Array<f32>
    {
        return {};
    }

    auto Terrain::isVisible() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<TerrainState>() )
            {
                return state->isVisible();
            }
        }

        return false;
    }

    void Terrain::setVisible( bool visible )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<TerrainState>() )
            {
                state->setVisible( visible );
            }
        }
    }

    auto Terrain::getMaterialName() const -> String
    {
        return {};
    }

    void Terrain::setMaterialName( const String &materialName )
    {
    }

    auto Terrain::getBlendMap( u32 index ) -> SmartPtr<ITerrainBlendMap>
    {
        return nullptr;
    }

    auto Terrain::getLayerBlendMapSize() const -> u16
    {
        return 0;
    }

    auto Terrain::intersects( const Ray3F &ray ) const -> SmartPtr<ITerrainRayResult>
    {
        return nullptr;
    }

    auto Terrain::getMesh() const -> SmartPtr<IMesh>
    {
        return nullptr;
    }

    auto Terrain::getHeightScale() const -> f32
    {
        return 0.f;
    }

    void Terrain::setHeightScale( f32 heightScale )
    {
    }

    auto Terrain::getSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        return nullptr;
    }

    void Terrain::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
    {
    }

    void Terrain::_getObject( void **ppObject ) const
    {
    }

    void Terrain::setTextureLayer( s32 layer, const String &textureName )
    {
    }

    auto Terrain::getHeightMap() const -> SmartPtr<ITexture>
    {
        return nullptr;
    }

    void Terrain::setHeightMap( SmartPtr<ITexture> heightMap )
    {
    }
}  // namespace fb::render
