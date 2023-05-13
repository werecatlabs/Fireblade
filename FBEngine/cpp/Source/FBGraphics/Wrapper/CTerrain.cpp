#include <FBGraphics/Wrapper/CTerrain.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CTerrain::CTerrain()
        {
        }

        CTerrain::~CTerrain()
        {
        }

        Vector3<real_Num> CTerrain::getPosition() const
        {
            return Vector3<real_Num>::zero();
        }

        void CTerrain::setPosition( const Vector3<real_Num> &position )
        {
        }

        f32 CTerrain::getHeightAtWorldPosition( const Vector3<real_Num> &position ) const
        {
            return 0.f;
        }

        u16 CTerrain::getSize() const
        {
            return 0;
        }

        Vector3<real_Num> CTerrain::getTerrainSpacePosition( const Vector3<real_Num> &worldSpace ) const
        {
            return Vector3<real_Num>::zero();
        }

        Array<f32> CTerrain::getHeightData() const
        {
            return Array<f32>();
        }

        bool CTerrain::isVisible() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<TerrainState>() )
                {
                    return state->isVisible();
                }
            }

            return false;
        }

        void CTerrain::setVisible( bool visible )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<TerrainState>() )
                {
                    state->setVisible( visible );
                }
            }
        }

        String CTerrain::getMaterialName() const
        {
            return String();
        }

        void CTerrain::setMaterialName( const String &materialName )
        {
        }

        SmartPtr<ITerrainBlendMap> CTerrain::getBlendMap( u32 index )
        {
            return nullptr;
        }

        u16 CTerrain::getLayerBlendMapSize() const
        {
            return 0;
        }

        SmartPtr<ITerrainRayResult> CTerrain::intersects( const Ray3F &ray ) const
        {
            return nullptr;
        }

        SmartPtr<IMesh> CTerrain::getMesh() const
        {
            return nullptr;
        }

        f32 CTerrain::getHeightScale() const
        {
            return 0.f;
        }

        void CTerrain::setHeightScale( f32 heightScale )
        {
        }

        SmartPtr<IGraphicsScene> CTerrain::getSceneManager() const
        {
            return nullptr;
        }

        void CTerrain::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
        {
        }

        void CTerrain::_getObject( void **ppObject ) const
        {
        }

        void CTerrain::setTextureLayer( s32 layer, const String &textureName )
        {
        }

    }  // namespace render
}  // namespace fb
