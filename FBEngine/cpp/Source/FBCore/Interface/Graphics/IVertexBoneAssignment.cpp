#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Mesh/IVertexBoneAssignment.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IVertexBoneAssignment, ISharedObject );
    IVertexBoneAssignment::~IVertexBoneAssignment() = default;

}  // namespace fb::render
