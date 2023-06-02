#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IComponentEventListener, ISharedObject );
    }  // end namespace scene
}  // end namespace fb
