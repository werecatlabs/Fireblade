#ifndef CPass_h__
#define CPass_h__

#include <FBCore/Interface/Graphics/IMaterialPass.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBGraphics/Wrapper/CMaterialNode.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace render
    {

        class CMaterialPass : public CMaterialNode<IMaterialPass>
        {
        public:
            CMaterialPass();
            ~CMaterialPass() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IMaterialPass::setSceneBlending */
            void setSceneBlending( u32 blendType ) override;

            /** @copydoc IMaterialPass::isDepthCheckEnabled */
            bool isDepthCheckEnabled() const override;

            /** @copydoc IMaterialPass::setDepthCheckEnabled */
            void setDepthCheckEnabled( bool enabled ) override;

            /** @copydoc IMaterialPass::isDepthWriteEnabled */
            bool isDepthWriteEnabled() const override;

            /** @copydoc IMaterialPass::setDepthWriteEnabled */
            void setDepthWriteEnabled( bool enabled ) override;

            /** @copydoc IMaterialPass::getCullingMode */
            u32 getCullingMode() const override;

            /** @copydoc IMaterialPass::setCullingMode */
            void setCullingMode( u32 mode ) override;

            /** Sets if lighting is enabled. */
            void setLightingEnabled( bool enabled, s32 passIdx = -1 );

            /** Returns whether or not dynamic lighting is enabled.
             */
            bool getLightingEnabled( s32 passIdx = -1 ) const;

            /** @copydoc IMaterialPass::toJson */
            SmartPtr<IMaterialTexture> createTextureUnit() override;

            /** @copydoc IMaterialPass::toJson */
            void addTextureUnit( SmartPtr<IMaterialTexture> textureUnit ) override;

            /** @copydoc IMaterialPass::toJson */
            void removeTextureUnit( SmartPtr<IMaterialTexture> textureUnit ) override;

            /** @copydoc IMaterialPass::toJson */
            Array<SmartPtr<IMaterialTexture>> getTextureUnits() const override;

            /** @copydoc IMaterialPass::getNumTexturesNodes */
            size_t getNumTexturesNodes() const override;

            void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 );

            /** @copydoc IMaterialPass::toJson */
            void setTexture( const String &fileName, u32 layerIdx = 0 ) override;

            /** @copydoc IMaterialPass::toJson */
            void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) override;

            /** @copydoc IMaterialPass::toJson */
            void setFragmentParam( const String &name, f32 value ) override;

            /** @copydoc IMaterialPass::toJson */
            void setFragmentParam( const String &name, const Vector2F &value ) override;

            /** @copydoc IMaterialPass::toJson */
            void setFragmentParam( const String &name, const Vector3F &value ) override;

            /** @copydoc IMaterialPass::toJson */
            void setFragmentParam( const String &name, const Vector4F &value ) override;

            /** @copydoc IMaterialPass::toJson */
            void setFragmentParam( const String &name, const ColourF &value ) override;

            /** @copydoc IMaterialPass::toJson */
            hash_type getRenderTechnique() const override;

            /** @copydoc IMaterialPass::toJson */
            void setRenderTechnique( hash_type renderTechnique ) override;

            /** @copydoc IMaterialPass::toJson */
            ColourF getAmbient() const override;

            /** @copydoc IMaterialPass::toJson */
            void setAmbient( const ColourF &ambient ) override;

            /** @copydoc IMaterialPass::toJson */
            ColourF getDiffuse() const override;

            /** @copydoc IMaterialPass::toJson */
            void setDiffuse( const ColourF &diffuse ) override;

            /** @copydoc IMaterialPass::toJson */
            ColourF getSpecular() const override;

            /** @copydoc IMaterialPass::toJson */
            void setSpecular( const ColourF &specular ) override;

            /** @copydoc IMaterialPass::getEmissive */
            ColourF getEmissive() const override;

            /** @copydoc IMaterialPass::setEmissive */
            void setEmissive( const ColourF &emissive ) override;

            ColourF getTint() const;

            void setTint( const ColourF &tint );

            /** @copydoc IMaterialPass::toJson */
            f32 getMetalness() const;

            /** @copydoc IMaterialPass::toJson */
            void setMetalness( f32 val );

            /** @copydoc IMaterialPass::toJson */
            f32 getRoughness() const;

            /** @copydoc IMaterialPass::toJson */
            void setRoughness( f32 roughness );

            /** @copydoc IObject::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IResource::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialPassStateListener : public MaterialNodeStateListener
            {
            public:
                MaterialPassStateListener() = default;
                ~MaterialPassStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            virtual void createTextureSlots();
            virtual void setupMaterial();

            void setTextureUnits( Array<SmartPtr<IMaterialTexture>> textures );

            SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> getTexturesPtr() const;

            void setTexturesPtr( SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> p );

            ColourF m_tint = ColourF::White;
            ColourF m_ambient = ColourF::White;
            ColourF m_diffuse = ColourF::White;
            ColourF m_specular = ColourF::White;
            ColourF m_emissive = ColourF::Black;

            f32 m_metalness = 0.5f;
            f32 m_roughness = 0.01f;

            SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> m_textures;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CPass_h__
