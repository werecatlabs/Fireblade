#ifndef IMaterialPass_h__
#define IMaterialPass_h__

#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Core/ColourF.h>

namespace fb
{
    namespace render
    {

        class IMaterialPass : public IMaterialNode
        {
        public:
            static const hash_type DIFFUSE_HASH;
            static const hash_type EMISSION_HASH;

            /** Virtual destructor. */
            ~IMaterialPass() override = default;

            /** Sets the kind of blending this pass has with the existing contents of the scene. */
            virtual void setSceneBlending( u32 blendType ) = 0;

            /** Returns whether or not this pass renders with depth-buffer checking on or not. */
            virtual bool isDepthCheckEnabled() const = 0;

            /** Sets whether or not this pass renders with depth-buffer checking on or not. */
            virtual void setDepthCheckEnabled( bool enabled ) = 0;

            /** Returns whether or not this pass renders with depth-buffer writing on or not. */
            virtual bool isDepthWriteEnabled() const = 0;

            /** Sets whether or not this pass renders with depth-buffer writing on or not. */
            virtual void setDepthWriteEnabled( bool enabled ) = 0;

            /** Gets the culling mode. */
            virtual u32 getCullingMode() const = 0;

            /** Sets the culling mode. */
            virtual void setCullingMode( u32 mode ) = 0;

            /** Sets if lighting is enabled. */
            virtual void setLightingEnabled( bool enabled, s32 passIdx = -1 ) = 0;

            /** Returns whether or not dynamic lighting is enabled.
             */
            virtual bool getLightingEnabled( s32 passIdx = -1 ) const = 0;

            /** */
            virtual SmartPtr<IMaterialTexture> createTextureUnit() = 0;

            /** */
            virtual void addTextureUnit( SmartPtr<IMaterialTexture> textureUnit ) = 0;

            /** */
            virtual void removeTextureUnit( SmartPtr<IMaterialTexture> textureUnit ) = 0;

            /** */
            virtual Array<SmartPtr<IMaterialTexture>> getTextureUnits() const = 0;

            /** */
            virtual size_t getNumTexturesNodes() const = 0;

            /** */
            virtual void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 ) = 0;

            /** */
            virtual void setTexture( const String &fileName, u32 layerIdx = 0 ) = 0;

            /** */
            virtual void setCubicTexture( const String &fileName, bool uvw, u32 layerIdx = 0 ) = 0;

            /** */
            virtual void setFragmentParam( const String &name, f32 value ) = 0;

            /** */
            virtual void setFragmentParam( const String &name, const Vector2F &value ) = 0;

            /** */
            virtual void setFragmentParam( const String &name, const Vector3F &value ) = 0;

            /** */
            virtual void setFragmentParam( const String &name, const Vector4F &value ) = 0;

            /** */
            virtual void setFragmentParam( const String &name, const ColourF &value ) = 0;

            virtual hash_type getRenderTechnique() const = 0;
            virtual void setRenderTechnique( hash_type renderTechnique ) = 0;

            virtual ColourF getAmbient() const = 0;
            virtual void setAmbient( const ColourF &ambient ) = 0;

            virtual ColourF getDiffuse() const = 0;
            virtual void setDiffuse( const ColourF &diffuse ) = 0;

            virtual ColourF getSpecular() const = 0;
            virtual void setSpecular( const ColourF &specular ) = 0;

            virtual ColourF getEmissive() const = 0;
            virtual void setEmissive( const ColourF &emissive ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialPass_h__
