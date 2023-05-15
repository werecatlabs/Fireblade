#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IDynamicMesh.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IDynamicMesh, IGraphicsObject );
    }  // end namespace render
}  // end namespace fb