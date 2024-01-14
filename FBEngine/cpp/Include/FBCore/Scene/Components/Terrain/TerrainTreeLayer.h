#ifndef TerrainTreeLayer_h__
#define TerrainTreeLayer_h__

#include <FBCore/Scene/Components/SubComponent.h>

namespace fb
{
    namespace scene
    {
        class TerrainTreeLayer : public SubComponent
        {
        public:
            TerrainTreeLayer();
            ~TerrainTreeLayer() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<render::ITexture> getBaseTexture() const;

            void setBaseTexture( SmartPtr<render::ITexture> baseTexture );

            s32 getIndex() const;

            void setIndex( s32 index );

            FB_CLASS_REGISTER_DECL;

        protected:
            s32 m_index = 0;

            SmartPtr<render::ITexture> m_baseTexture;

            // The prefab for your object
            SmartPtr<IPrefab> m_prefab;

            // The number of instances to generate
            int m_density = 100;
        };
    }  // namespace scene
}  // namespace fb

#endif  // TerrainTreeLayer_h__
