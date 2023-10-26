#ifndef _CTextureOgre_H
#define _CTextureOgre_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Graphics/CTexture.h>

namespace fb
{
    namespace render
    {

        class CTextureOgreNext : public CTexture
        {
        public:
            CTextureOgreNext();
            ~CTextureOgreNext() override;

            void save() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

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

            Ogre::TextureGpu *getTexture() const;
            void setTexture( Ogre::TextureGpu *texture );

        protected:
            void createStateObject();

            time_interval m_nextResize = 0.0;
            Ogre::TextureGpu *m_texture = nullptr;
            Vector2I m_size = Vector2I::zero();
        };
    }  // end namespace render
}  // end namespace fb

#endif
