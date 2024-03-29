#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ComponentBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include <FBLuabind/Helpers/DestructibleHelper.h>

namespace fb
{
    using namespace fb::scene;

    void bindComponent( lua_State *L )
    {
        using namespace luabind;

        // Bind the State enum class
        module( L )[class_<IComponent::State>( "State" ).enum_(
            "constants" )[value( "None", IComponent::State::None ),
                          value( "Create", IComponent::State::Create ),
                          value( "Destroyed", IComponent::State::Destroyed ),
                          value( "Edit", IComponent::State::Edit ),
                          value( "Play", IComponent::State::Play ),
                          value( "Reset", IComponent::State::Reset ),
                          value( "Count", IComponent::State::Count )]];

        module( L )[class_<ISubComponent, IResource>( "ISubComponent" )
                        .def( "getParentComponent", &ISubComponent::getParentComponent )
                        .def( "setParentComponent", &ISubComponent::setParentComponent )
                        .def( "getParent", &ISubComponent::getParent )
                        .def( "setParent", &ISubComponent::setParent )
                        .def( "getProperties", &ISubComponent::getProperties )
                        .def( "setProperties", &ISubComponent::setProperties )];

        module( L )[class_<SubComponent, ISubComponent>( "SubComponent" )];

        module( L )[class_<IComponent, ISharedObject, SharedPtr<IComponent>>( "IComponent" )
                        //.def("makeDirty", &IComponent::makeDirty)
                        //.def("makeDirty", &IComponent::makeDirty)
                        //.def( "getActor", &IComponent::getActor )
                        .def( "setActor", &IComponent::setActor )
                        .def( "getStateContext", &IComponent::getStateContext )
                        .def( "setStateContext", &IComponent::setStateContext )
                        .def( "getProperties", &IComponent::getProperties )
                        .def( "setProperties", &IComponent::setProperties )
                        .def( "updateVisibility", &IComponent::updateVisibility )
                        //.def( "setState", &IComponent::setState )
                        .def( "setEnabled", &IComponent::setEnabled )
                        .def( "isEnabled", &IComponent::isEnabled )
                        //.def( "getState", &IComponent::getState )
                        .def( "getEvents", &IComponent::getEvents )
                        .def( "setEvents", &IComponent::setEvents )
                        .def( "addEvent", &IComponent::addEvent )
                        .def( "removeEvent", &IComponent::removeEvent )
                        .def( "addSubComponent", &IComponent::addSubComponent )
                        .def( "removeSubComponent", &IComponent::removeSubComponent )
                        .def( "removeSubComponentByIndex", &IComponent::removeSubComponentByIndex )
                        .def( "getNumSubComponents", &IComponent::getNumSubComponents )
                        .def( "getSubComponentByIndex", &IComponent::getSubComponentByIndex )
                        .def( "getSubComponents", &IComponent::getSubComponents )
                        .def( "setSubComponents", &IComponent::setSubComponents )
                        .def( "getSubComponentsPtr", &IComponent::getSubComponentsPtr )
                        .def( "setSubComponentsPtr", &IComponent::setSubComponentsPtr )
                        .scope[def( "typeInfo", IComponent::typeInfo )]];

        module( L )[class_<Component, IComponent, SmartPtr<Component>>( "BaseComponent" )
                        .scope[def( "typeInfo", Component::typeInfo )]];

        module( L )[class_<Material, Component, SmartPtr<Component>>( "Material" )
                        .def( "getMaterialPath", &Material::getMaterialPath )
                        .def( "setMaterialPath", &Material::setMaterialPath )
                        .def( "getMaterial", &Material::getMaterial )
                        .def( "setMaterial", &Material::setMaterial )
                        .def( "updateMaterial", &Material::updateMaterial )
                        .def( "updateDependentComponents", &Material::updateDependentComponents )
                        .def( "updateImageComponent", &Material::updateImageComponent )
                        .def( "getIndex", &Material::getIndex )
                        .def( "setIndex", &Material::setIndex )];

        module( L )[class_<UserComponent, Component, SmartPtr<IComponent>>( "UserComponent" )
                        .def( "getUpdateInEditMode", &UserComponent::getUpdateInEditMode )
                        .def( "setUpdateInEditMode", &UserComponent::setUpdateInEditMode )
                        .scope[def( "typeInfo", UserComponent::typeInfo )]];

        module( L )[class_<TerrainSystem, Component, SmartPtr<IComponent>>( "TerrainSystem" )
                        .def( "load", &TerrainSystem::load )
                        .def( "unload", &TerrainSystem::unload )
                        .def( "getProperties", &TerrainSystem::getProperties )
                        .def( "setProperties", &TerrainSystem::setProperties )
                        .def( "updateTransform", &TerrainSystem::updateTransform )
                        .def( "calculateNumLayers", &TerrainSystem::calculateNumLayers )
                        .def( "getNumLayers", &TerrainSystem::getNumLayers )
                        .def( "addLayer", &TerrainSystem::addLayer )
                        .def( "removeLayerIndex",
                              ( void( TerrainSystem::* )( s32 ) ) & TerrainSystem::removeLayer )
                        .def( "removeLayerPtr", ( void( TerrainSystem::* )( SmartPtr<TerrainLayer> ) ) &
                                                    TerrainSystem::removeLayer )
                        .def( "setNumLayers", &TerrainSystem::setNumLayers )
                        .def( "resizeLayermap", &TerrainSystem::resizeLayermap )
                        .def( "updateLayers", &TerrainSystem::updateLayers )
                        .def( "getHeightMap", &TerrainSystem::getHeightMap )
                        .def( "setHeightMap", &TerrainSystem::setHeightMap )];

        module( L )[class_<TerrainLayer, SubComponent, SmartPtr<ISubComponent>>( "TerrainLayer" )
                        .def( "load", &TerrainLayer::load )
                        .def( "unload", &TerrainLayer::unload )
                        .def( "getProperties", &TerrainLayer::getProperties )
                        .def( "setProperties", &TerrainLayer::setProperties )
                        .def( "getBaseTexture", &TerrainLayer::getBaseTexture )
                        .def( "setBaseTexture", &TerrainLayer::setBaseTexture )
                        .def( "getIndex", &TerrainLayer::getIndex )
                        .def( "setIndex", &TerrainLayer::setIndex )];

        module( L )[class_<Light, Component, SmartPtr<IComponent>>( "Light" )
                        .def( "getLightType",
                              reinterpret_cast<u32 ( Light::* )() const>( &Light::getLightType ) )
                        .def( "setLightType",
                              reinterpret_cast<void ( Light::* )( u32 )>( &Light::setLightType ) )
                        .def( "getDiffuseColour", &Light::getDiffuseColour )
                        .def( "setDiffuseColour", &Light::setDiffuseColour )
                        .def( "getSpecularColour", &Light::getSpecularColour )
                        .def( "setSpecularColour", &Light::setSpecularColour )
                        .def( "getLight", &Light::getLight )
                        .def( "setLight", &Light::setLight )
                        .def( "getSceneNode",
                              ( SmartPtr<render::ISceneNode> & (Light::*)() ) & Light::getSceneNode )
                        .def( "setSceneNode", &Light::setSceneNode )];
    }

}  // end namespace fb
