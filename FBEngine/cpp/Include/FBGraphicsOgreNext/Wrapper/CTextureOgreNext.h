#ifndef _CTextureOgre_H
#define _CTextureOgre_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/Texture.h>

namespace fb
{
    namespace render
    {

        /** Ogre texture implementation. */
        class CTextureOgreNext : public Texture
        {
        public:
            /** Constructor. */
            CTextureOgreNext();

            /** Destructor. */
            ~CTextureOgreNext() override;

            /** @copydoc Texture::save */
            void save() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Texture::getSize */
            Vector2I getSize() const override;

            /** @copydoc Texture::setSize */
            void setSize( const Vector2I &size ) override;

            /** @copydoc Texture::_getObject */
            void _getObject( void **ppObject ) const override;

            /** @copydoc Texture::getActualSize */
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

            /** Gets the ogre texture. */
            Ogre::TextureGpu *getTexture() const;

            /** Sets the ogre texture. */
            void setTexture( Ogre::TextureGpu *texture );

        protected:
            /** @copydoc Texture::createStateObject */
            void createStateObject() override;

            /** Time to process the next resize event. */
            time_interval m_nextResize = 0.0;

            /** The ogre texture. */
            Ogre::TextureGpu *m_texture = nullptr;

            /** The size of the texture. */
            Vector2I m_size = Vector2I::zero();
        };
    }  // end namespace render
}  // end namespace fb

#endif
