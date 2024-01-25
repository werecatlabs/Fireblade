#ifndef TerrainLayer_h__
#define TerrainLayer_h__

#include <FBCore/Scene/Components/SubComponent.h>

namespace fb
{
    namespace scene
    {
        class TerrainLayer : public SubComponent
        {
        public:
            TerrainLayer();
            ~TerrainLayer() override;

            /** @copydoc SubComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc SubComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc SubComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc SubComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<render::ITexture> getBaseTexture() const;

            void setBaseTexture( SmartPtr<render::ITexture> baseTexture );

            s32 getIndex() const;

            void setIndex( s32 index );

            FB_CLASS_REGISTER_DECL;

        protected:
            s32 m_index = 0;

            SmartPtr<render::ITexture> m_baseTexture;
        };
    }  // namespace scene
}  // namespace fb

#endif  // TerrainLayer_h__
