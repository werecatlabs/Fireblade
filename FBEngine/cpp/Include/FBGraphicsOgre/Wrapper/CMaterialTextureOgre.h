#ifndef CMaterialTextureOgre_h__
#define CMaterialTextureOgre_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBGraphics/Wrapper/CMaterialTexture.h>

namespace fb
{
    namespace render
    {
        class CMaterialTextureOgre : public CMaterialTexture
        {
        public:
            CMaterialTextureOgre();
            ~CMaterialTextureOgre() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IMaterialTexture::initialise */
            void initialise( Ogre::TextureUnitState *textureUnitState );

            /** @copydoc IMaterialTexture::getTextureName */
            String getTextureName() const override;

            /** @copydoc IMaterialTexture::setTextureName */
            void setTextureName( const String &name ) override;

            /** @copydoc IMaterialTexture::getTexture */
            SmartPtr<ITexture> getTexture() const override;

            /** @copydoc IMaterialTexture::setTexture */
            void setTexture( SmartPtr<ITexture> texture ) override;

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

            class TextureListener : public SharedObject<IEventListener>
            {
            public:
                TextureListener() = default;
                ~TextureListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object,
                                       SmartPtr<IEvent> event );

                CMaterialTextureOgre *getOwner() const;

                void setOwner( CMaterialTextureOgre *owner );

            protected:
                CMaterialTextureOgre *m_owner = nullptr;
            };

            void createTextureUnitState();

            SmartPtr<TextureListener> m_textureListener;

            RawPtr<Ogre::TextureUnitState> m_textureUnitState = nullptr;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTextureUnit_h__
