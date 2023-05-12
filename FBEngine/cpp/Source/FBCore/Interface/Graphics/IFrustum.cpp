#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IFrustum.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IFrustum, IGraphicsObject );
    }  // end namespace render
}  // end namespace fb
