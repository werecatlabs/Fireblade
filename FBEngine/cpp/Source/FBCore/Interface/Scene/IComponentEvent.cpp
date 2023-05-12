#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IComponentEvent, IEvent );
    }  // end namespace scene
}  // end namespace fb
