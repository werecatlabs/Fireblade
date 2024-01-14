#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, IGraphicsMesh, IGraphicsObject );

    const hash_type IGraphicsMesh::RENDER_QUEUE_HASH = StringUtil::getHash( "renderQueue" );
    const hash_type IGraphicsMesh::VISIBILITY_FLAGS_HASH = StringUtil::getHash( "visibilityFlags" );

}  // namespace fb::render
