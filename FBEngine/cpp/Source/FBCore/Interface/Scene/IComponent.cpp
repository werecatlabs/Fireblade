#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IComponent, IResource );
    }  // end namespace scene
}  // end namespace fb
