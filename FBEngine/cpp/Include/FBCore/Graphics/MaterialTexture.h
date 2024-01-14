#ifndef CTextureUnit_h__
#define CTextureUnit_h__

#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBCore/Graphics/MaterialNode.h>

namespace fb
{
    namespace render
    {
        class MaterialTexture : public MaterialNode<IMaterialTexture>
        {
        public:
            MaterialTexture();
            ~MaterialTexture() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IMaterialTexture::getTextureName */
            String getTextureName() const override;

            /** @copydoc IMaterialTexture::setTextureName */
            void setTextureName( const String &name ) override;

            /** @copydoc IMaterialTexture::getTexture */
            SmartPtr<ITexture> getTexture() const override;

            /** @copydoc IMaterialTexture::setTexture */
            void setTexture( SmartPtr<ITexture> texture ) override;

            /** @copydoc IMaterialTexture::setScale */
            void setScale( const Vector3F &scale ) override;

            /** @copydoc IMaterialTexture::getAnimator */
            SmartPtr<IAnimator> getAnimator() const override;

            /** @copydoc IMaterialTexture::setAnimator */
            void setAnimator( SmartPtr<IAnimator> animator ) override;

            /** @copydoc IMaterialTexture::getTint */
            ColourF getTint() const override;

            /** @copydoc IMaterialTexture::setTint */
            void setTint( const ColourF &tint ) override;

            /** @copydoc IMaterialTexture::_getObject */
            void _getObject( void **ppObject ) override;

            /** @copydoc IMaterialTexture::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IMaterialTexture::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IResource::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IResource::getTextureType */
            u32 getTextureType() const override;

            /** @copydoc IResource::setTextureType */
            void setTextureType( u32 textureType ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialTextureStateListener : public MaterialNodeStateListener
            {
            public:
                MaterialTextureStateListener() = default;
                ~MaterialTextureStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            void createTextureUnitState();

            Vector3F m_scale = Vector3F::unit();
            ColourF m_tint = ColourF::White;
            SmartPtr<IAnimator> m_animator;
            SmartPtr<ITexture> m_texture;
            Array<SmartPtr<ITexture>> m_cubeTextures;
            String m_texturePath;
            u32 m_textureType = 0;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTextureUnit_h__
