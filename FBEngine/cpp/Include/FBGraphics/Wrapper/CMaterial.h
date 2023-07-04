#ifndef CMaterial_h__
#define CMaterial_h__

#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBGraphics/Wrapper/CResourceGraphics.h>

namespace fb
{
    namespace render
    {

        class CMaterial : public CResourceGraphics<IMaterial>
        {
        public:
            CMaterial();
            ~CMaterial() override;

            void saveToFile( const String &filePath ) override;
            void loadFromFile( const String &filePath ) override;

            void save() override;
            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 ) override;

            void setTexture( const String &fileName, u32 layerIdx = 0 ) override;

            String getTextureName( u32 layerIdx = 0 ) const override;
            SmartPtr<ITexture> getTexture( u32 layerIdx = 0 ) const;

            void setLightingEnabled( bool enabled, s32 passIdx = -1 );

            bool getLightingEnabled( s32 passIdx = -1 ) const;

            void setCubicTexture( Array<SmartPtr<render::ITexture>> textures, u32 layerIdx = 0 );

            void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) override;

            virtual void setFragmentParam( const String &name, f32 value );
            virtual void setFragmentParam( const String &name, const Vector2F &value );
            virtual void setFragmentParam( const String &name, const Vector3F &value );
            virtual void setFragmentParam( const String &name, const Vector4F &value );
            virtual void setFragmentParam( const String &name, const ColourF &value );

            String getName() const;
            void setName( const String &name );

            hash32 getId() const;

            SmartPtr<IMaterialTechnique> createTechnique() override;
            void removeTechnique( SmartPtr<IMaterialTechnique> technique ) override;
            void removeAllTechniques() override;

            Array<SmartPtr<IMaterialTechnique>> getTechniques() const override;
            void setTechniques(Array<SmartPtr<IMaterialTechnique>> techniques);

            SmartPtr<IMaterialTechnique> getTechniqueByScheme( hash32 scheme ) const override;

            u32 getNumTechniques() const override;

            void setScale( const Vector3F &scale, u32 textureIndex = 0, u32 passIndex = 0,
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
            SmartPtr<ISharedObject> toData() const;

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
            class MaterialStateListener : public SharedObject<IStateListener>
            {
            public:
                MaterialStateListener();
                MaterialStateListener( CMaterial *material );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CMaterial *getOwner() const;
                void setOwner( CMaterial *owner );

            protected:
                CMaterial *m_owner = nullptr;
            };

            virtual void createMaterialByType();

            void addTechnique(SmartPtr<IMaterialTechnique> technique);
            SharedPtr<Array<SmartPtr<IMaterialTechnique>>> getTechniquesPtr() const;
            void setTechniquesPtr(SharedPtr<Array<SmartPtr<IMaterialTechnique>>> p);

            String m_name;

            SmartPtr<IMaterialNode> m_root;

            SmartPtr<IMaterialTechnique> m_technique;

            SharedPtr<Array<SmartPtr<IMaterialTechnique>>> m_techniques;

            hash32 m_rendererType = 0;

            s32 m_id = 0;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterial_h__
