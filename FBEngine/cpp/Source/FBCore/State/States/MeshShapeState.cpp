#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/MeshShapeState.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MeshShapeState, BaseState );

    SmartPtr<IMeshResource> MeshShapeState::getMeshResource() const
    {
        return m_meshResource;
    }

    void MeshShapeState::setMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        m_meshResource = meshResource;
    }

}  // namespace fb
