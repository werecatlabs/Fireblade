#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/GraphicsMesh.h>
#include <FBCore/Interface/Graphics/IAnimationController.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb::render
{

    GraphicsMesh::GraphicsMesh() = default;

    GraphicsMesh::~GraphicsMesh() = default;

    void GraphicsMesh::setMaterialName( const String &materialName, s32 index /*= -1 */ )
    {
    }

    auto GraphicsMesh::getMaterialName( s32 index /*= -1 */ ) const -> String
    {
        return {};
    }

    void GraphicsMesh::setMaterial( SmartPtr<IMaterial> material, s32 index /*= -1 */ )
    {
    }

    auto GraphicsMesh::getMaterial( s32 index /*= -1 */ ) const -> SmartPtr<IMaterial>
    {
        return nullptr;
    }

    void GraphicsMesh::setHardwareAnimationEnabled( bool enabled )
    {
    }

    void GraphicsMesh::checkVertexProcessing()
    {
    }

    auto GraphicsMesh::getAnimationController() -> SmartPtr<IAnimationController>
    {
        return nullptr;
    }

    auto GraphicsMesh::getMeshName() const -> String
    {
        return {};
    }

    void GraphicsMesh::setMeshName( const String &meshName )
    {
    }

}  // namespace fb::render
