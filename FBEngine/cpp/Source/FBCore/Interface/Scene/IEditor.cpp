#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IEditor.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IEditor, ISharedObject );

    }  // end namespace scene
}  // end namespace fb