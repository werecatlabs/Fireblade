#ifndef TerrainBlendMap_h__
#define TerrainBlendMap_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/BaseSubComponent.h>

namespace fb
{
    namespace scene
    {
        class TerrainBlendMap : public BaseSubComponent
        {
        public:
            /** Creates a TerrainBlendMap object. */
            TerrainBlendMap();
            ~TerrainBlendMap() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<render::ITerrainBlendMap> getBlendMap() const;

            void setBlendMap( SmartPtr<render::ITerrainBlendMap> blendMap );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<render::ITerrainBlendMap> m_blendMap;
        };
    }  // namespace scene
}  // namespace fb

#endif  // TerrainBlendMap_h__
