#ifndef __CTerrain_h__
#define __CTerrain_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/ITerrain.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Represents a terrain object that can be rendered.
         */
        class CTerrain : public CSharedGraphicsObject<ITerrain>
        {
        public:
            /**
             * Default constructor.
             */
            CTerrain();

            /**
             * Destructor.
             */
            ~CTerrain() override;

            Vector3<real_Num> getPosition() const override;

            void setPosition( const Vector3<real_Num> &position ) override;

            f32 getHeightAtWorldPosition( const Vector3<real_Num> &position ) const override;

            u16 getSize() const override;

            Vector3<real_Num> getTerrainSpacePosition(
                const Vector3<real_Num> &worldSpace ) const override;

            Array<f32> getHeightData() const override;

            /**
             * Gets the visibility of the terrain.
             *
             * @return True if the terrain is visible, false otherwise.
             */
            bool isVisible() const override;

            /**
             * Sets the visibility of the terrain.
             *
             * @param visible True if the terrain should be visible, false otherwise.
             */
            void setVisible( bool visible ) override;

            String getMaterialName() const override;

            void setMaterialName( const String &materialName ) override;

            SmartPtr<ITerrainBlendMap> getBlendMap( u32 index ) override;

            u16 getLayerBlendMapSize() const override;

            SmartPtr<ITerrainRayResult> intersects( const Ray3F &ray ) const override;

            SmartPtr<IMesh> getMesh() const override;

            f32 getHeightScale() const override;

            void setHeightScale( f32 heightScale ) override;

            SmartPtr<IGraphicsScene> getSceneManager() const override;

            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) override;

            void _getObject( void **ppObject ) const override;

            void setTextureLayer( s32 layer, const String &textureName ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CTerrain_h__
