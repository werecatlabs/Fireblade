#ifndef _CTexture_H
#define _CTexture_H

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBGraphics/Wrapper/CResourceGraphics.h>

namespace fb
{
    namespace render
    {
        /**
         * @class CTexture
         * @brief A class representing a texture resource, inheriting from CResourceGraphics.
         *
         * This class provides functionality to manage textures, such as
         * loading and saving, updating, setting and getting properties, etc.
         */
        class CTexture : public CResourceGraphics<ITexture>
        {
        public:
            CTexture();
            ~CTexture() override;

            /** @brief Updates the texture. */
            void update() override;

            /** @brief Saves the texture. */
            void save() override;

            /**
             * @brief Saves the texture to a file.
             * @param filePath The file path to save the texture to.
             */
            void saveToFile( const String &filePath ) override;

            /**
             * @brief Loads the texture from a file.
             * @param filePath The file path to load the texture from.
             */
            void loadFromFile( const String &filePath ) override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @brief Gets the render target associated with the texture.
             * @return A smart pointer to the render target.
             */
            SmartPtr<IRenderTarget> getRenderTarget() const override;

            /**
             * @brief Sets the render target associated with the texture.
             * @param rt A smart pointer to the render target.
             */
            void setRenderTarget( SmartPtr<IRenderTarget> rt ) override;

            /**
             * @brief Copies the current texture to a target texture.
             * @param target A smart pointer to the target texture.
             */
            void copyToTexture( SmartPtr<ITexture> &target ) override;

            /**
             * @brief Copies texture data to a buffer.
             * @param data A pointer to the destination buffer.
             * @param size The size of the copied texture region.
             */
            void copyData( void *data, const Vector2I &size ) override;

            /** @brief Gets the size of the texture.
             * @return The texture size.
             */
            Vector2I getSize() const override;

            /**
             * @brief Sets the size of the texture.
             * @param size The new size of the texture.
             */
            void setSize( const Vector2I &size ) override;

            void _getObject( void **ppObject ) const override;

            /** @brief Gets the actual size of the texture.
             * @return The actual texture size.
             */
            Vector2I getActualSize() const override;

            /**
             * @brief Gets the GPU texture.
             * @param ppTexture A pointer to the destination GPU texture.
             */
            void getTextureGPU( void **ppTexture ) const override;

            /**
             * @brief Gets the final GPU texture.
             * @param ppTexture A pointer to the destination final GPU texture.
             * @note This object depends on the graphics API.
             */
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

            /**
             * @brief Gets the texture type of the current texture.
             * @return The texture type (e.g., TEX_TYPE_2D, TEX_TYPE_3D, etc.).
             */
            TextureType getTextureType() const;

            /**
             * @brief Sets the texture type for the current texture.
             * @param textureType The texture type to set (e.g., TEX_TYPE_2D, TEX_TYPE_3D, etc.).
             */
            void setTextureType( TextureType textureType );

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public CSharedObject<IStateListener>
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CTexture> getOwner() const;
                void setOwner( SmartPtr<CTexture> owner );

            protected:
                AtomicWeakPtr<CTexture> m_owner;
            };

            void createStateObject() override;

            mutable atomic_u64 m_textureHandle;
            mutable u32 m_textureId = 0;
            u32 m_usageFlags = 0;
            hash32 m_id = 0;
            String m_name;
            TextureType m_textureType = TextureType::TEX_TYPE_2D;

            SmartPtr<IRenderTarget> m_renderTarget;

            static u32 m_ext;
        };
    }  // end namespace render
}  // end namespace fb

#endif
