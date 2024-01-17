#ifndef _ITextureManager_H
#define _ITextureManager_H

#include <FBCore/Interface/Resource/IResourceManager.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace render
    {

        /** Interface for a texture manager. */
        class ITextureManager : public IResourceManager
        {
        public:
            /** Virtual destructor. */
            ~ITextureManager() override = default;

            /** Creates a manual texture.
            @return A texture instance.
            */
            virtual SmartPtr<ITexture> createManual( const String &name, const String &group, u8 texType,
                                                     u32 width, u32 height, u32 depth, s32 num_mips,
                                                     u8 format, s32 usage = 0 ) = 0;

            /** Creates a video texture.
            @param name The name of the texture as a string.
            @return A video texture instance.
            */
            virtual SmartPtr<IVideoTexture> createVideoTexture( const String &name ) = 0;

            /** Creates a cubemap. */
            virtual SmartPtr<ICubemap> addCubemap() = 0;

            /** Creates a render texture.
            @return A texture instance.
            */
            virtual SmartPtr<ITexture> createRenderTexture() = 0;

            /** Destroys a render texture.
            @return A texture instance.
            */
            virtual void destroyRenderTexture( SmartPtr<ITexture> texture ) = 0;

            /** Create a skybox cubemap.
            @param skyboxTextures The texture filenames to convert.
            @return A texture instance.
            */
            virtual SmartPtr<ITexture> createSkyBoxCubeMap( Array<String> skyboxTextures ) = 0;

            /** Create a skybox cubemap.
            @param skyboxTextures The texture objects to convert.
            @return A texture instance.
            */
            virtual SmartPtr<ITexture> createSkyBoxCubeMap(
                Array<SmartPtr<ITexture>> skyboxTextures ) = 0;

            /** Create a skybox cubemap.
            @param material The material object to convert.
            @return A texture instance.
            */
            virtual SmartPtr<ITexture> createSkyBoxCubeMap( SmartPtr<IMaterial> material ) = 0;

            /** Clone a texture. */
            virtual SmartPtr<ITexture> cloneTexture( SmartPtr<ITexture> texture,
                                                     const String &clonedTextureName ) = 0;

            /** Clone a texture. */
            virtual SmartPtr<ITexture> cloneTexture( const String &name,
                                                     const String &clonedTextureName ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
