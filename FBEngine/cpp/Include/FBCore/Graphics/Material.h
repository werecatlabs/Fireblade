#ifndef CMaterial_h__
#define CMaterial_h__

#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Graphics/ResourceGraphics.h>

namespace fb
{
    namespace render
    {
        class Material : public ResourceGraphics<IMaterial>
        {
        public:
            Material();
            ~Material() override;

            void saveToFile( const String &filePath ) override;
            void loadFromFile( const String &filePath ) override;

            void save() override;
            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 ) override;

            void setTexture( const String &fileName, u32 layerIdx = 0 ) override;

            SharedPtr<Array<SmartPtr<ITexture>>> getTextures() const;

            void setTextures( SharedPtr<Array<SmartPtr<ITexture>>> textures );

            String getTextureName( u32 layerIdx = 0 ) const override;
            SmartPtr<ITexture> getTexture( u32 layerIdx = 0 ) const override;

            void setLightingEnabled( bool enabled, s32 passIdx = -1 ) override;

            bool getLightingEnabled( s32 passIdx = -1 ) const override;

            void setCubicTexture( Array<SmartPtr<ITexture>> textures, u32 layerIdx = 0 ) override;

            void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) override;

            virtual void setFragmentParam( const String &name, f32 value );
            virtual void setFragmentParam( const String &name, const Vector2<real_Num> &value );
            virtual void setFragmentParam( const String &name, const Vector3<real_Num> &value );
            virtual void setFragmentParam( const String &name, const Vector4F &value );
            virtual void setFragmentParam( const String &name, const ColourF &value );

            String getName() const;
            void setName( const String &name );

            hash32 getId() const;

            SmartPtr<IMaterialTechnique> createTechnique() override;
            void removeTechnique( SmartPtr<IMaterialTechnique> technique ) override;
            void removeAllTechniques() override;

            Array<SmartPtr<IMaterialTechnique>> getTechniques() const override;
            void setTechniques( Array<SmartPtr<IMaterialTechnique>> techniques ) override;

            SmartPtr<IMaterialTechnique> getTechniqueByScheme( hash32 scheme ) const override;

            u32 getNumTechniques() const override;

            void setScale( const Vector3<real_Num> &scale, u32 textureIndex = 0, u32 passIndex = 0,
                           u32 techniqueIndex = 0 ) override;

            SmartPtr<IMaterialNode> getRoot() const override;
            void setRoot( SmartPtr<IMaterialNode> root ) override;

            hash32 getRendererType() const;
            void setRendererType( hash32 rendererType );

            MaterialType getMaterialType() const override;
            void setMaterialType( MaterialType materialType ) override;

            /** @copydoc IMaterial::setDirty */
            void setDirty( bool dirty );

            /** @copydoc IMaterial::isDirty */
            bool isDirty() const;

            /** @copydoc IObject::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener();
                MaterialStateListener( Material *material );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                Material *getOwner() const;
                void setOwner( Material *owner );

            protected:
                Material *m_owner = nullptr;
            };

            virtual void createMaterialByType();

            void addTechnique( SmartPtr<IMaterialTechnique> technique );
            SharedPtr<Array<SmartPtr<IMaterialTechnique>>> getTechniquesPtr() const;
            void setTechniquesPtr( SharedPtr<Array<SmartPtr<IMaterialTechnique>>> p );

            String m_name;

            SmartPtr<IMaterialNode> m_root;

            SmartPtr<IMaterialTechnique> m_technique;

            SharedPtr<Array<SmartPtr<IMaterialTechnique>>> m_techniques;

            SharedPtr<Array<SmartPtr<ITexture>>> m_textures;

            hash32 m_rendererType = 0;

            s32 m_id = 0;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterial_h__
