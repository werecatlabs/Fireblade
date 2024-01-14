#ifndef CGraphicsMesh_h__
#define CGraphicsMesh_h__

#include <FBCore/Graphics/GraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>

namespace fb
{
    namespace render
    {

        class GraphicsMesh : public GraphicsObject<IGraphicsMesh>
        {
        public:
            GraphicsMesh();
            ~GraphicsMesh();

            void setMaterialName( const String &materialName, s32 index = -1 ) override;

            String getMaterialName( s32 index = -1 ) const override;

            void setMaterial( SmartPtr<IMaterial> material, s32 index = -1 ) override;

            SmartPtr<IMaterial> getMaterial( s32 index = -1 ) const override;

            void setHardwareAnimationEnabled( bool enabled ) override;

            void checkVertexProcessing() override;

            SmartPtr<IAnimationController> getAnimationController() override;

            String getMeshName() const override;

            void setMeshName( const String &meshName ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsMesh_h__
