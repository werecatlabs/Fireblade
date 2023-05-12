#ifndef _ITexture_H
#define _ITexture_H

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for a texture class.
         *
         * The `ITexture` class is a subtype of `IResource` that defines the interface for a
         * texture object. Texture objects can be used to store and manipulate images and other
         * graphical data in a format that is optimized for rendering in a 3D environment.
         *
         * This class provides the necessary functions to describe the size and properties of a
         * texture object, as well as to manipulate and copy its contents. These functions can
         * be overridden by subclasses to implement specific texture types, such as 2D, 3D, or
         * cube maps.
         *
         * In addition to the functions inherited from `IResource`, this class also defines a
         * number of specific member functions for texture objects, including functions to get
         * and set the render target, copy the texture to another texture, get and set the size
         * of the texture, and get the GPU texture object.
         *
         * @see IResource
         */
        class ITexture : public IResource
        {
        public:
            enum TextureUsage
            {
                /// @copydoc HardwareBuffer::Usage
                TU_STATIC = ( 1 << 1 ),
                TU_DYNAMIC = ( 1 << 2 ),
                TU_WRITE_ONLY = ( 1 << 3 ),
                TU_STATIC_WRITE_ONLY = ( 1 << 4 ),
                TU_DYNAMIC_WRITE_ONLY = ( 1 << 5 ),
                TU_DYNAMIC_WRITE_ONLY_DISCARDABLE = ( 1 << 6 ),
                /// mipmaps will be automatically generated for this texture
                TU_AUTOMIPMAP = ( 1 << 7 ),
                /// this texture will be a render target, i.e. used as a target for render to texture
                /// setting this flag will ignore all other texture usages except TU_AUTOMIPMAP
                TU_RENDERTARGET = ( 1 << 8 ),
                /// default to automatic mipmap generation static textures
                TU_DEFAULT = TU_AUTOMIPMAP | TU_STATIC_WRITE_ONLY
            };

            /** Enum identifying the texture type
             */
            enum class TextureType
            {
                /// 1D texture, used in combination with 1D texture coordinates
                TEX_TYPE_1D = 1,
                /// 2D texture, used in combination with 2D texture coordinates (default)
                TEX_TYPE_2D = 2,
                /// 3D volume texture, used in combination with 3D texture coordinates
                TEX_TYPE_3D = 3,
                /// 3D cube map, used in combination with 3D texture coordinates
                TEX_TYPE_CUBE_MAP = 4,
                /// 2D texture Array
                TEX_TYPE_2D_ARRAY = 5
            };

            /** Enum identifying special mipmap numbers
             */
            enum TextureMipmap
            {
                /// Generate mipmaps up to 1x1
                MIP_UNLIMITED = 0x7FFFFFFF,
                /// Use TextureManager default
                MIP_DEFAULT = -1
            };

            /** The pixel format used for images, textures, and render surfaces */
            enum PixelFormat
            {
                /// Unknown pixel format.
                PF_UNKNOWN = 0,
                /// 8-bit pixel format, all bits luminance.
                PF_L8 = 1,
                PF_BYTE_L = PF_L8,
                /// 16-bit pixel format, all bits luminance.
                PF_L16 = 2,
                PF_SHORT_L = PF_L16,
                /// 8-bit pixel format, all bits alpha.
                PF_A8 = 3,
                PF_BYTE_A = PF_A8,
                /// 8-bit pixel format, 4 bits alpha, 4 bits luminance.
                PF_A4L4 = 4,
                /// 2 byte pixel format, 1 byte luminance, 1 byte alpha
                PF_BYTE_LA = 5,
                /// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
                PF_R5G6B5 = 6,
                /// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
                PF_B5G6R5 = 7,
                /// 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
                PF_R3G3B2 = 31,
                /// 16-bit pixel format, 4 bits for alpha, red, green and blue.
                PF_A4R4G4B4 = 8,
                /// 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
                PF_A1R5G5B5 = 9,
                /// 24-bit pixel format, 8 bits for red, green and blue.
                PF_R8G8B8 = 10,
                /// 24-bit pixel format, 8 bits for blue, green and red.
                PF_B8G8R8 = 11,
                /// 32-bit pixel format, 8 bits for alpha, red, green and blue.
                PF_A8R8G8B8 = 12,
                /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
                PF_A8B8G8R8 = 13,
                /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
                PF_B8G8R8A8 = 14,
                /// 32-bit pixel format, 8 bits for red, green, blue and alpha.
                PF_R8G8B8A8 = 28,
                /// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
                /// like PF_A8R8G8B8, but alpha will get discarded
                PF_X8R8G8B8 = 26,
                /// 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
                /// like PF_A8B8G8R8, but alpha will get discarded
                PF_X8B8G8R8 = 27,
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                /// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
                PF_BYTE_RGB = PF_R8G8B8,
                /// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
                PF_BYTE_BGR = PF_B8G8R8,
                /// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte
                /// for alpha
                PF_BYTE_BGRA = PF_B8G8R8A8,
                /// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte
                /// for alpha
                PF_BYTE_RGBA = PF_R8G8B8A8,
#else
                /// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
                PF_BYTE_RGB = PF_B8G8R8,
                /// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
                PF_BYTE_BGR = PF_R8G8B8,
                /// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte
                /// for alpha
                PF_BYTE_BGRA = PF_A8R8G8B8,
                /// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte
                /// for alpha
                PF_BYTE_RGBA = PF_A8B8G8R8,
#endif
                /// 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
                PF_A2R10G10B10 = 15,
                /// 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
                PF_A2B10G10R10 = 16,
                /// DDS (DirectDraw Surface) DXT1 format
                PF_DXT1 = 17,
                /// DDS (DirectDraw Surface) DXT2 format
                PF_DXT2 = 18,
                /// DDS (DirectDraw Surface) DXT3 format
                PF_DXT3 = 19,
                /// DDS (DirectDraw Surface) DXT4 format
                PF_DXT4 = 20,
                /// DDS (DirectDraw Surface) DXT5 format
                PF_DXT5 = 21,
                // 16-bit pixel format, 16 bits (float) for red
                PF_FLOAT16_R = 32,
                // 48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits
                // (float) for blue
                PF_FLOAT16_RGB = 22,
                // 64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits
                // (float) for blue, 16 bits (float) for alpha
                PF_FLOAT16_RGBA = 23,
                // 32-bit pixel format, 32 bits (float) for red
                PF_FLOAT32_R = 33,
                // 96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits
                // (float) for blue
                PF_FLOAT32_RGB = 24,
                // 128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits
                // (float) for blue, 32 bits (float) for alpha
                PF_FLOAT32_RGBA = 25,
                // 32-bit, 2-channel s10e5 floating point pixel format, 16-bit green, 16-bit red
                PF_FLOAT16_GR = 35,
                // 64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red
                PF_FLOAT32_GR = 36,
                // Depth texture format
                PF_DEPTH = 29,
                // 64-bit pixel format, 16 bits for red, green, blue and alpha
                PF_SHORT_RGBA = 30,
                // 32-bit pixel format, 16-bit green, 16-bit red
                PF_SHORT_GR = 34,
                // 48-bit pixel format, 16 bits for red, green and blue
                PF_SHORT_RGB = 37,
                /// PVRTC (PowerVR) RGB 2 bpp
                PF_PVRTC_RGB2 = 38,
                /// PVRTC (PowerVR) RGBA 2 bpp
                PF_PVRTC_RGBA2 = 39,
                /// PVRTC (PowerVR) RGB 4 bpp
                PF_PVRTC_RGB4 = 40,
                /// PVRTC (PowerVR) RGBA 4 bpp
                PF_PVRTC_RGBA4 = 41,
                /// 8-bit pixel format, all bits red.
                PF_R8 = 42,
                /// 16-bit pixel format, 8 bits red, 8 bits green.
                PF_RG8 = 43,
                // Number of pixel formats currently defined
                PF_COUNT = 44
            };

            static const hash_type STATE_MESSAGE_TEXTURE_SIZE;

            /** Virtual destructor. */
            ~ITexture() override = default;

            /**
             * @brief Gets the render target.
             * @return A smart pointer to the render target associated with this texture.
             */
            virtual SmartPtr<IRenderTarget> getRenderTarget() const = 0;

            /**
             * @brief Sets the render target.
             * @param rt A smart pointer to the render target to associate with this texture.
             */
            virtual void setRenderTarget( SmartPtr<IRenderTarget> rt ) = 0;

            /**
             * @brief Copies (and maybe scales to fit) the contents of this texture to another texture.
             * @param target A smart pointer to the target texture to which to copy the contents.
             */
            virtual void copyToTexture( SmartPtr<ITexture> &target ) = 0;

            /**
             * @brief Copies the data passed.
             * @param data A pointer to the data to copy to the texture.
             * @param size The size of the data.
             */
            virtual void copyData( void *data, const Vector2I &size ) = 0;

            /**
             * @brief Gets the size of the texture.
             * @return The size of the texture as a 2D vector.
             */
            virtual Vector2I getSize() const = 0;

            /**
             * @brief Sets the size of the texture.
             * @param size The new size of the texture as a 2D vector.
             */
            virtual void setSize( const Vector2I &size ) = 0;

            /**
             * @brief Gets the actual size of the texture.
             * @return The actual size of the texture as a 2D vector.
             */
            virtual Vector2I getActualSize() const = 0;

            /**
             * @brief Gets the GPU texture.
             * @param ppTexture A pointer to a pointer to the GPU texture object.
             */
            virtual void getTextureGPU( void **ppTexture ) const = 0;

            /**
             * @brief Gets the final GPU texture.
             * This object depends on the graphics API being used.
             * @param ppTexture A pointer to a pointer to the final GPU texture object.
             */
            virtual void getTextureFinal( void **ppTexture ) const = 0;

            /**
             * @brief Gets the internal handle.
             * @return The internal handle for the texture.
             */
            virtual size_t getTextureHandle() const = 0;

            /**
             * @brief Gets the usage flags of the texture.
             *
             * The usage flags determine how the texture will be used. For example, whether it is
             * used as a render target or a texture, and whether it is read-only or write-only.
             * The flags are a combination of the values in the `TextureUsageFlags` enumeration.
             *
             * @return The usage flags of the texture.
             *
             * @see TextureUsageFlags
             */
            virtual u32 getUsageFlags() const = 0;

            /**
             * @brief Sets the usage flags of the texture.
             *
             * The usage flags determine how the texture will be used. For example, whether it is
             * used as a render target or a texture, and whether it is read-only or write-only.
             * The flags are a combination of the values in the `TextureUsageFlags` enumeration.
             *
             * @param usageFlags The new usage flags to set for the texture.
             *
             * @see TextureUsageFlags
             */
            virtual void setUsageFlags( u32 usageFlags ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
