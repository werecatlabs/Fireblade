#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, IComponent, IResource );

    const hash_type IComponent::actorFlagsChanged = StringUtil::getHash( "actorFlagsChanged" );
    const hash_type IComponent::actorReset = StringUtil::getHash( "actorReset" );
    const hash_type IComponent::actorUnload = StringUtil::getHash( "actorUnload" );
    const hash_type IComponent::sceneWasLoaded = StringUtil::getHash( "sceneWasLoaded" );
    const hash_type IComponent::parentChanged = StringUtil::getHash( "parentChanged" );
    const hash_type IComponent::hierarchyChanged = StringUtil::getHash( "hierarchyChanged" );
    const hash_type IComponent::childAdded = StringUtil::getHash( "childAdded" );
    const hash_type IComponent::childRemoved = StringUtil::getHash( "childRemoved" );
    const hash_type IComponent::childAddedInHierarchy = StringUtil::getHash( "childAddedInHierarchy" );
    const hash_type IComponent::childRemovedInHierarchy =
        StringUtil::getHash( "childRemovedInHierarchy" );

    const hash_type IComponent::visibilityChanged = StringUtil::getHash( "visibilityChanged" );
    const hash_type IComponent::enabledChanged = StringUtil::getHash( "enabledChanged" );
    const hash_type IComponent::staticChanged = StringUtil::getHash( "staticChanged" );

    const hash_type IComponent::triggerCollisionEnter = StringUtil::getHash( "triggerCollisionEnter" );
    const hash_type IComponent::triggerCollisionLeave = StringUtil::getHash( "triggerCollisionLeave" );
    const hash_type IComponent::componentLoaded = StringUtil::getHash( "componentLoaded" );

    const u32 IComponent::ComponentReservedFlag = ( 1 << 0 );
    const u32 IComponent::ComponentEnabledFlag = ( 1 << 1 );

}  // namespace fb::scene
