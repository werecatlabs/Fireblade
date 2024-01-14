#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialPass.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialPass, IMaterialNode );

    const hash_type IMaterialPass::DIFFUSE_HASH = StringUtil::getHash( "diffuse" );
    const hash_type IMaterialPass::EMISSION_HASH = StringUtil::getHash( "emmision" );

}  // namespace fb::render
