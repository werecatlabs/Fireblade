//
// Created by Zane Desir on 22/11/2021.
//

#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IPrefab.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IPrefab, IResource );

    }  // end namespace scene
}  // end namespace fb
