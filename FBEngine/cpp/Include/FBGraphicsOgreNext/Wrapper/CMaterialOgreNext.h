#ifndef CMaterialOgreNext_h__
#define CMaterialOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/CMaterial.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/System/Job.h>
#include <OgreMaterial.h>
//#include <OgreRenderTargetListener.h>

namespace fb
{
    namespace render
    {
        class CMaterialOgreNext : public CMaterial
        {
        public:
            CMaterialOgreNext();
            ~CMaterialOgreNext() override;

            void initialise( Ogre::MaterialPtr &material );

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            //void setTexture( SmartPtr<ITexture>, u32 layerIdx = 0 ) override;
            //void setTexture( const String &fileName, u32 layerIdx = 0 ) override;

            void setCubicTexture( Array<SmartPtr<ITexture>> textures, u32 layerIdx = 0 ) override;

            void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) override;

            void setFragmentParam( const String &name, f32 value ) override;
            void setFragmentParam( const String &name, const Vector2F &value ) override;
            void setFragmentParam( const String &name, const Vector3F &value ) override;
            void setFragmentParam( const String &name, const Vector4F &value ) override;
            void setFragmentParam( const String &name, const ColourF &value ) override;

            SmartPtr<IMaterialTechnique> createTechnique() override;

            void setScale( const Vector3F &scale, u32 textureIndex = 0, u32 passIndex = 0,
                           u32 techniqueIndex = 0 ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            Ogre::HlmsDatablock *getHlmsDatablock() const;

            void setHlmsDatablock( Ogre::HlmsDatablock *hlmsDatablock );

            String getDatablockName() const;
            void setDatablockName( const String &datablockName );

            SmartPtr<ITexture> getCubeTexture() const;
            void setCubeTexture( SmartPtr<ITexture> cubeTexture );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener();
                MaterialStateListener( CMaterialOgreNext *material );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CMaterialOgreNext *getOwner() const;
                void setOwner( CMaterialOgreNext *owner );

            protected:
                CMaterialOgreNext *m_owner = nullptr;
            };

            void createMaterialByType() override;

            Ogre::HlmsDatablock *m_hlmsDatablock = nullptr;

            Ogre::MaterialPtr m_material;

            SmartPtr<ITexture> m_cubeTexture;

            atomic_bool m_isDirty = false;

            String m_datablockName;

            static u32 m_nameExt;
        };
    } // end namespace render
}     // end namespace fb

#endif  // IMaterial_h__
