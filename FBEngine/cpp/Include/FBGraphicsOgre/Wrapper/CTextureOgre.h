#ifndef _CTextureOgre_H
#define _CTextureOgre_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBGraphics/Wrapper/CTexture.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        /**
         * \brief Represents a texture object that can be rendered.
         */
        class CTextureOgre : public CTexture
        {
        public:
            CTextureOgre();
            ~CTextureOgre() override;

            void update() override;

            void save() override;

            void saveToFile( const String &filePath ) override;
            void loadFromFile( const String &filePath ) override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::TexturePtr &texture );

            void copyToTexture( SmartPtr<ITexture> &target ) override;
            void copyData( void *data, const Vector2I &size ) override;

            Vector2I getSize() const override;
            void setSize( const Vector2I &size ) override;

            void _getObject( void **ppObject ) const override;

            Vector2I getActualSize() const override;

            /** Gets the gpu texture. */
            void getTextureGPU( void **ppTexture ) const override;

            /** Gets the final gpu texture. This object depends on the graphics api. */
            void getTextureFinal( void **ppTexture ) const override;

            /** @copydoc ITexture::getTextureHandle */
            size_t getTextureHandle() const override;

            /** @copydoc ITexture::getUsageFlags */
            u32 getUsageFlags() const override;

            /** @copydoc ITexture::setUsageFlags */
            void setUsageFlags( u32 usageFlags ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** Gets the ogre texture. */
            Ogre::TexturePtr getTexture() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            void createStateObject() override;

            Ogre::TexturePtr m_texture;

            static u32 m_ext;
        };
    } // end namespace render
}     // end namespace fb

#endif
