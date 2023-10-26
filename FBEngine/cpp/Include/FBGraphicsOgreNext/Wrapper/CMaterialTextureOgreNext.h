#ifndef CMaterialTextureOgreNext_h__
#define CMaterialTextureOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBCore/Graphics/CMaterialTexture.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace render
    {
        class CMaterialTextureOgreNext : public CMaterialTexture
        {
        public:
            CMaterialTextureOgreNext();
            ~CMaterialTextureOgreNext() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

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

            /** @copydoc IMaterialTexture::_getObject */
            void _getObject( void **ppObject ) override;

            /** @copydoc IResource::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IResource::getTextureType */
            u32 getTextureType() const override;

            /** @copydoc IResource::setTextureType */
            void setTextureType( u32 textureType ) override;

            Ogre::TextureUnitState *getTextureUnitState() const;
            void setTextureUnitState( Ogre::TextureUnitState *textureUnitState );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialTextureOgreStateListener : public MaterialTextureStateListener
            {
            public:
                MaterialTextureOgreStateListener() = default;
                ~MaterialTextureOgreStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            class TextureListener : public IEventListener
            {
            public:
                TextureListener() = default;
                ~TextureListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                CMaterialTextureOgreNext *getOwner() const;

                void setOwner( CMaterialTextureOgreNext *owner );

            protected:
                CMaterialTextureOgreNext *m_owner = nullptr;
            };

            /** @copydoc IMaterialTexture::loadImage */
            void loadImage( Ogre::TextureGpu *texture, const String &filePath );

            SmartPtr<TextureListener> m_textureListener;
            Ogre::TextureUnitState *m_textureUnitState = nullptr;
        };
    } // end namespace render
}     // end namespace fb

#endif  // CTextureUnit_h__
