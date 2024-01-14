//
// Created by Zane Desir on 22/11/2021.
//

#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IResource, IPrototype );
    const String IResource::nameStr = String( "name" );
}  // end namespace fb
