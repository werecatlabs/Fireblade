#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMeshConverter.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IMeshConverter, ISharedObject );
    }  // end namespace render
}  // end namespace fb