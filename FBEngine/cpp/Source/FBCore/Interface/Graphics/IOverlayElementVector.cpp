#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlayElementVector.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IOverlayElementVector, IOverlayElement );
    }  // end namespace render
}  // end namespace fb
