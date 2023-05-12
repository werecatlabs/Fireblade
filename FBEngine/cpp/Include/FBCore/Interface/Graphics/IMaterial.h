#ifndef __Render_IMaterial_h__
#define __Render_IMaterial_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Base/ColourF.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace render
    {
        /**
         * @brief Interface for a material.
         *
         * This class is an interface for a material, which can be used to render different types of objects
         * such as terrain, skyboxes, and UI elements. This interface extends the IResource interface.
         */
        class IMaterial : public IResource
        {
        public:
            /**
             * @brief Hash value for set texture.
             */
            static const hash_type SET_TEXTURE_HASH;

            /**
             * @brief Hash value for float fragment.
             */
            static const hash_type FRAGMENT_FLOAT_HASH;

            /**
             * @brief Hash value for Vector2f fragment.
             */
            static const hash_type FRAGMENT_VECTOR2F_HASH;

            /**
             * @brief Hash value for Vector3f fragment.
             */
            static const hash_type FRAGMENT_VECTOR3F_HASH;

            /**
             * @brief Hash value for Vector4f fragment.
             */
            static const hash_type FRAGMENT_VECTOR4F_HASH;

            /**
             * @brief Hash value for colour fragment.
             */
            static const hash_type FRAGMENT_COLOUR_HASH;

            static const hash_type LIGHTING_ENABLED_HASH;

            /**
             * @brief Enumerates the types of textures used by terrain materials.
             */
            enum class TerrainTextureTypes
            {
                Base,      ///< Base texture.
                Splat,     ///< Splat texture.
                Diffuse1,  ///< First diffuse texture.
                Diffuse2,  ///< Second diffuse texture.
                Diffuse3,  ///< Third diffuse texture.
                Diffuse4,  ///< Fourth diffuse texture.
                Normal1,   ///< First normal texture.
                Normal2,   ///< Second normal texture.
                Normal3,   ///< Third normal texture.
                Normal4,   ///< Fourth normal texture.
                Count      ///< Count of texture types.
            };

            /**
             * @brief Enumerates the types of textures used by PBS materials.
             */
            enum class PbsTextureTypes
            {
                PBSM_DIFFUSE,                        ///< Diffuse texture.
                PBSM_NORMAL,                         ///< Normal texture.
                PBSM_SPECULAR,                       ///< Specular texture.
                PBSM_METALLIC = PBSM_SPECULAR,       ///< Metallic texture.
                PBSM_ROUGHNESS,                      ///< Roughness texture.
                PBSM_DETAIL_WEIGHT,                  ///< Detail weight texture.
                PBSM_DETAIL0,                        ///< First detail texture.
                PBSM_DETAIL1,                        ///< Second detail texture.
                PBSM_DETAIL2,                        ///< Third detail texture.
                PBSM_DETAIL3,                        ///< Fourth detail texture.
                PBSM_DETAIL0_NM,                     ///< First detail normal map texture.
                PBSM_DETAIL1_NM,                     ///< Second detail normal map texture.
                PBSM_DETAIL2_NM,                     ///< Third detail normal map texture.
                PBSM_DETAIL3_NM,                     ///< Fourth detail normal map texture.
                PBSM_EMISSIVE,                       ///< Emissive texture.
                PBSM_REFLECTION,                     ///< Reflection texture.
                NUM_PBSM_SOURCES = PBSM_REFLECTION,  ///< Number of PBSM sources.
                NUM_PBSM_TEXTURE_TYPES               ///< Number of PBSM texture types.
            };

            /**
             * @brief Enumerates the blend modes for PBS materials.
             */
            enum class PbsBlendModes
            {
                PBSM_BLEND_NORMAL_NON_PREMUL,  ///< Normal blend mode.
                PBSM_BLEND_NORMAL_PREMUL,      ///< Normal premultiplied blend mode.
                PBSM_BLEND_ADD,                ///< Add blend mode.
                PBSM_BLEND_SUBTRACT,           ///< Subtract blend mode.
                PBSM_BLEND_MULTIPLY,           ///< Multiply blend mode.
                PBSM_BLEND_MULTIPLY2X,         ///< Multiply 2x blend mode.
                PBSM_BLEND_SCREEN,             ///< Screen blend mode.
                PBSM_BLEND_OVERLAY,            ///< Overlay blend mode.
                PBSM_BLEND_LIGHTEN,
                PBSM_BLEND_DARKEN,
                PBSM_BLEND_GRAIN_EXTRACT,
                PBSM_BLEND_GRAIN_MERGE,
                PBSM_BLEND_DIFFERENCE,
                NUM_PBSM_BLEND_MODES
            };

            /**
             * Enumerates the types of textures used by skybox materials.
             */
            enum class SkyboxTextureTypes
            {
                Front,
                Back,
                Left,
                Right,
                Up,
                Down,

                Count
            };

            /**
             * Enumerates the types of textures used by cube skybox materials.
             */
            enum class SkyboxCubeTextureTypes
            {
                Cube,

                Count
            };

            /**
             * Enumerates the types of PBR workflows.
             */
            enum class Workflows
            {
                SpecularWorkflow,
                SpecularAsFresnelWorkflow,
                MetallicWorkflow
            };

            /**
             * Enumerates the types of materials.
             */
            enum class MaterialType
            {
                Standard,
                StandardSpecular,
                StandardTriPlanar,
                TerrainStandard,
                TerrainSpecular,
                TerrainDiffuse,
                Skybox,
                SkyboxCubemap,
                UI,
                Custom,

                Count
            };

            /**
             * @brief Virtual destructor for IMaterial.
             *
             * This virtual destructor is responsible for deallocating memory for derived classes.
             */
            virtual ~IMaterial() override = default;

            /**
             * @brief Gets the root node of the material.
             *
             * This method retrieves the root node of the material, which can be used to access the material
             * tree structure.
             *
             * @return Smart pointer to the root node.
             */
            virtual SmartPtr<IMaterialNode> getRoot() const = 0;

            /**
             * @brief Sets the root node of the material.
             *
             * This method sets the root node of the material to the given value.
             *
             * @param root The new root node.
             */
            virtual void setRoot( SmartPtr<IMaterialNode> root ) = 0;

            /**
             * @brief Adds a material technique.
             *
             * This method creates and adds a new material technique to the material. A material technique is
             * responsible for setting up and executing a specific rendering technique for the material.
             *
             * @return Smart pointer to the newly created material technique.
             */
            virtual SmartPtr<IMaterialTechnique> createTechnique() = 0;

            /**
             * @brief Removes a material technique.
             *
             * This method removes the given material technique from the material.
             *
             * @param technique The material technique to remove.
             */
            virtual void removeTechnique( SmartPtr<IMaterialTechnique> technique ) = 0;

            /**
             * @brief Removes all material techniques.
             *
             * This method removes all material techniques from the material.
             */
            virtual void removeAllTechniques() = 0;

            /**
             * @brief Gets the material techniques.
             *
             * This method retrieves an array of all material techniques associated with the material.
             *
             * @return An array of smart pointers to IMaterialTechnique objects.
             */
            virtual Array<SmartPtr<IMaterialTechnique>> getTechniques() const = 0;

            /**
             * @brief Sets the material techniques.
             *
             * This method sets the material techniques to the given array of smart pointers to IMaterialTechnique
             * objects.
             *
             * @param techniques An array of smart pointers to IMaterialTechnique objects.
             */
            virtual void setTechniques( Array<SmartPtr<IMaterialTechnique>> techniques ) = 0;

            /**
             * @brief Gets the material technique associated with the given scheme.
             *
             * This method retrieves the material technique associated with the given scheme. A scheme is an
             * identifier that can be used to specify a particular rendering technique for the material.
             *
             * @param scheme The scheme to look up.
             *
             * @return Smart pointer to the material technique associated with the scheme, or a null pointer
             * if no such technique exists.
             */
            virtual SmartPtr<IMaterialTechnique> getTechniqueByScheme( hash32 scheme ) const = 0;

            /**
             * @brief Gets the number of material techniques.
             *
             * This method retrieves the number of material techniques associated with the material.
             *
             * @return The number of material techniques.
             */
            virtual u32 getNumTechniques() const = 0;

            /**
             * @brief Sets a texture for the material.
             *
             * This method sets the texture for the specified layer index to the given texture.
             *
             * @param texture Smart pointer to the texture to set.
             * @param layerIdx The index of the layer to set the texture for.
             */
            virtual void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 ) = 0;

            /**
             * @brief Sets a texture for the material.
             *
             * This method sets the texture for the specified layer index to the file with the given name.
             *
             * @param fileName The name of the file containing the texture to set.
             * @param layerIdx The index of the layer to set the texture for.
             */
            virtual void setTexture( const String &fileName, u32 layerIdx = 0 ) = 0;

            /**
             * @brief Gets the name of the texture associated with the given layer index.
             *
             * This method retrieves the name of the texture associated with the given layer index.
             *
             * @param layerIdx The index of the layer to retrieve the texture name for.
             *
             * @return The name of the texture associated with the layer index.
             */
            virtual String getTextureName( u32 layerIdx = 0 ) const = 0;

            /**
             * @brief Gets the texture associated with the given layer index.
             *
             * This method retrieves the texture associated with the given layer index.
             *
             * @param layerIdx The index of the layer to retrieve the texture for.
             *
             * @return Smart pointer to the texture associated with the layer index, or a null pointer if no
             * such texture exists.
             */
            virtual SmartPtr<ITexture> getTexture( u32 layerIdx = 0 ) const = 0;

            /** Sets if lighting is enabled. */
            virtual void setLightingEnabled( bool enabled, s32 passIdx = -1 ) = 0;

            /** Returns whether or not dynamic lighting is enabled.
             */
            virtual bool getLightingEnabled( s32 passIdx = -1 ) const = 0;

            /**
             * @brief Sets a cubic texture for the material.
             *
             * This method sets a cubic texture for the specified layer index to the given array of textures.
             * Cubic textures are textures that represent the six faces of a cube, such as for a skybox.
             *
             * @param textures An array of smart pointers to the textures to set.
             * @param layerIdx The index of the layer to set the cubic texture for.
             */
            virtual void setCubicTexture( Array<SmartPtr<render::ITexture>> textures,
                                          u32 layerIdx = 0 ) = 0;

            /**
             * @brief Sets a cubic texture for the material.
             *
             * This method sets a cubic texture for the specified layer index to the file with the given name.
             * Cubic textures are textures that represent the six faces of a cube, such as for a skybox.
             *
             * @param fileName The name of the file containing the texture to set.
             * @param uvw Whether to apply the UVW mapping mode to the texture.
             * @param layerIdx The index of the layer to set the cubic texture for.
             */
            virtual void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) = 0;

            /**
             * @brief Sets the scale of a material texture.
             *
             * This method sets the scale of the texture associated with the given indices to the given value.
             *
             * @param scale The scale to set for the texture.
             * @param textureIndex The index of the texture to set the scale for.
             * @param passIndex The index of the pass to set the scale for.
             * @param techniqueIndex The index of the technique to set the scale for.
             */
            virtual void setScale( const Vector3F &scale, u32 textureIndex = 0, u32 passIndex = 0,
                                   u32 techniqueIndex = 0 ) = 0;

            /**
             * @brief Gets the material type.
             *
             * This method retrieves the type of the material.
             *
             * @return The type of the material.
             */
            virtual MaterialType getMaterialType() const = 0;

            /**
             * @brief Sets the material type.
             *
             * This method sets the type of the material to the given value.
             *
             * @param materialType The new type of the material.
             */
            virtual void setMaterialType( MaterialType materialType ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterial_h__
