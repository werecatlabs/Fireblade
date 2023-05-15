#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IDebug, ISharedObject );
    }  // end namespace render
}  // end namespace fb
