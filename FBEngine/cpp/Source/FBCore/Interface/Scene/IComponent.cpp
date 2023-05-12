#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IComponent, IResource );
    }  // end namespace scene
}  // end namespace fb
