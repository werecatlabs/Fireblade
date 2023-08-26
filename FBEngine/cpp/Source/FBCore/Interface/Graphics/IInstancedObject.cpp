#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IInstancedObject.h>

namespace fb
{
    namespace render
    {

        const hash_type IInstancedObject::RENDER_QUEUE_HASH = StringUtil::getHash( "renderQueue" );
        const hash_type IInstancedObject::VISIBILITY_FLAGS_HASH =
            StringUtil::getHash( "visibilityFlags" );
        const hash_type IInstancedObject::CUSTOM_PARAMETER_HASH =
            StringUtil::getHash( "custom_parameter_hash" );

    }  // namespace render
}  // namespace fb
