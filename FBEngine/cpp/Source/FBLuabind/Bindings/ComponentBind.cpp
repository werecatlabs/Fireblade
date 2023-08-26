#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ComponentBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <FBLuabind/Helpers/DestructibleHelper.h>

namespace fb
{
    using namespace fb::scene;

    Parameter _getOwner( IComponent *component )
    {
        Parameter param;
        //param.setPtr( component->getOwner() );
        return param;
    }

    //void _setOwner( IComponent *component, SmartPtr<scene::IActor> entity )
    //{
    //    //component->setOwner( (IScriptObject *)entity.get() );
    //}

    //void _addFireArm( WeaponContainer *container, WeaponFireArm2Ptr fireArm )
    //{
    //    container->addWeapon( fireArm );
    //}

    //Parameter _getWeapon( WeaponContainer *container, lua_Integer hash )
    //{
    //    WeaponPtr weapon = container->getWeapon( hash );

    //    Parameter param;
    //    param.setPtr( weapon.get() );
    //    return param;
    //}

    //SmartPtr<IAnimator> _getAnimator( AnimatorContainer *animatorContainer, lua_Integer hash )
    //{
    //    return animatorContainer->getAnimator( hash );
    //}

    //void _setGoal( AiGoalBased *ai, boost::shared_ptr<IAiGoal> goal )
    //{
    //    //ai->setGoal(goal);
    //}

    void _addChild( CollisionNode *parent, CollisionNode *node )
    {
        //parent->addChild(node);
    }

    void _removeChild( CollisionNode *parent, CollisionNode *node )
    {
        //parent->removeChild(node);
    }

    CollisionNode *_getCollisionNodeParent( CollisionNode *node )
    {
        //CollisionNodePtr parent = node->getParent();
        //return parent.get();

        return nullptr;
    }

    //boost::shared_ptr<IComponent> _addComponentFactoryNamed( IComponentContainer *container,
    //                                                         const char *factoryName )
    //{
    //    return container->addComponent( StringUtil::getHash( factoryName ), 0 );
    //}

    //boost::shared_ptr<IComponent> _addComponentFactoryInt( IComponentContainer *container,
    //                                                       lua_Integer factoryId )
    //{
    //    hash32 hash = *reinterpret_cast<lua_Integer *>( &factoryId );
    //    return container->addComponent( hash, 0 );
    //}

    //void _addComponentInstance( IComponentContainer *container, boost::shared_ptr<IComponent> component )
    //{
    //    container->addComponent( component );
    //}

    void bindComponent( lua_State *L )
    {
        using namespace luabind;

        // Bind the State enum class
        //module( L )[class_<IComponent::State>( "State" ).enum_(
        //    "constants" )[value( "None", IComponent::State::None ),
        //                  value( "Create", IComponent::State::Create ),
        //                  value( "Destroyed", IComponent::State::Destroyed ),
        //                  value( "Edit", IComponent::State::Edit ),
        //                  value( "Play", IComponent::State::Play ),
        //                  value( "Reset", IComponent::State::Reset ),
        //                  value( "Count", IComponent::State::Count )]];

        module( L )[class_<IComponent, ISharedObject, SharedPtr<IComponent>>( "IComponent" )
                        .def( "updateDirty", &IComponent::updateDirty )
                        //.def("makeDirty", &IComponent::makeDirty)
                        //.def("makeDirty", &IComponent::makeDirty)
                        .def( "getActor", &IComponent::getActor )
                        .def( "setActor", &IComponent::setActor )
                        .def( "getStateObject", &IComponent::getStateObject )
                        .def( "setStateObject", &IComponent::setStateObject )
                        .def( "getProperties", &IComponent::getProperties )
                        .def( "setProperties", &IComponent::setProperties )
                        .def( "updateVisibility", &IComponent::updateVisibility )
                        //.def( "setState", &IComponent::setState )
                        .def( "setEnabled", &IComponent::setEnabled )
                        .def( "isEnabled", &IComponent::isEnabled )
                        //.def( "getState", &IComponent::getState )
                        .def( "actorReset", &IComponent::actorReset )
                        .def( "actorUnload", &IComponent::actorUnload )
                        .def( "levelWasLoaded", &IComponent::levelWasLoaded )
                        .def( "parentChanged", &IComponent::parentChanged )
                        .def( "hierarchyChanged", &IComponent::hierarchyChanged )
                        .def( "childAdded", &IComponent::childAdded )
                        .def( "childRemoved", &IComponent::childRemoved )
                        .def( "childAddedInHierarchy", &IComponent::childAddedInHierarchy )
                        .def( "childRemovedInHierarchy", &IComponent::childRemovedInHierarchy )
                        .def( "getEvents", &IComponent::getEvents )
                        .def( "setEvents", &IComponent::setEvents )
                        .def( "addEvent", &IComponent::addEvent )
                        .def( "removeEvent", &IComponent::removeEvent )
                        .def( "addSubComponent", &IComponent::addSubComponent )
                        .def( "removeSubComponent", &IComponent::removeSubComponent )
                        .def( "removeSubComponentByIndex", &IComponent::removeSubComponentByIndex )
                        .def( "getNumSubComponents", &IComponent::getNumSubComponents )

                        .def( "getSubComponents", &IComponent::getSubComponents )
                        .def( "setSubComponents", &IComponent::setSubComponents )
                        .def( "getSubComponentsPtr", &IComponent::getSubComponentsPtr )
                        .def( "setSubComponentsPtr", &IComponent::setSubComponentsPtr )
                        .def( "getDirector", &IComponent::getDirector )
                        .def( "setDirector", &IComponent::setDirector )
                        .scope[def( "typeInfo", IComponent::typeInfo )]];

        module( L )[class_<Component, IComponent, SmartPtr<Component>>( "BaseComponent" )
                        .scope[def( "typeInfo", Component::typeInfo )]];

        module( L )[class_<UserComponent, Component, SmartPtr<IComponent>>( "UserComponent" )
                        .def( constructor<>() )
                        .def( "getUpdateInEditMode", &UserComponent::getUpdateInEditMode )
                        .def( "setUpdateInEditMode", &UserComponent::setUpdateInEditMode )
                        .scope[def( "typeInfo", UserComponent::typeInfo )]];

        //module(
        //    L )[class_<MovementControl2, BaseComponent, SmartPtr<IComponent>>( "MovementControl2" )
        //            .def( "getMoveSpeed", &MovementControl2::getMoveSpeed )
        //            .def( "setMoveSpeed", &MovementControl2::setMoveSpeed )

        //            .def( "getGameInputId", &MovementControl2::getGameInputId )
        //            .def( "setGameInputId", &MovementControl2::setGameInputId )

        //            .def( "getMoveVector", &MovementControl2::getMoveVector )
        //            .def( "setMoveVector", &MovementControl2::setMoveVector )

        //            .def( "getPosition", &MovementControl2::getPosition )
        //            .def( "setPosition", &MovementControl2::setPosition )

        //            .def( "getGameActionLeft", &MovementControl2::getGameActionLeft )
        //            .def( "setGameActionLeft", &MovementControl2::setGameActionLeft )
        //            .def( "getGameActionRight", &MovementControl2::getGameActionRight )
        //            .def( "setGameActionRight", &MovementControl2::setGameActionRight )

        //            .def( "getGameActionUp", &MovementControl2::getGameActionUp )
        //            .def( "setGameActionUp", &MovementControl2::setGameActionUp )
        //            .def( "getGameActionDown", &MovementControl2::getGameActionDown )
        //            .def( "setGameActionDown", &MovementControl2::setGameActionDown )

        //            .def( "getTarget", &MovementControl2::getTarget )
        //            .def( "setTarget", &MovementControl2::setTarget )

        //            .def( "getUseClick", &MovementControl2::getUseClick )
        //            .def( "setUseClick", &MovementControl2::setUseClick )

        //            .def( "getUseTarget", &MovementControl2::getUseTarget )
        //            .def( "setUseTarget", &MovementControl2::setUseTarget )

        //            .def( "getLeftClickAction", &MovementControl2::getLeftClickAction )
        //            .def( "setLeftClickAction", &MovementControl2::setLeftClickAction )];

        //module( L )[class_<MovementControlFPS, IComponent, SmartPtr<IComponent>>(
        //                "MovementControlFPS" )
        //                .def( "getGameInputId", &MovementControlFPS::getGameInputId )
        //                .def( "setGameInputId", &MovementControlFPS::setGameInputId )

        //                //.def("getMoveSpeed", &MovementControlFPS::getMoveSpeed )
        //                //.def("setMoveSpeed", &MovementControlFPS::setMoveSpeed )

        //                .def( "getPosition", &MovementControlFPS::getPosition )
        //                .def( "setPosition", &MovementControlFPS::setPosition )

        //                .def( "getOrientation", &MovementControlFPS::getOrientation )
        //                .def( "getRotation", &MovementControlFPS::getRotation )

        //                .def( "getRotationOffset", &MovementControlFPS::getRotationOffset )
        //                .def( "setRotationOffset", &MovementControlFPS::setRotationOffset )

        //                .def( "getDirection", &MovementControlFPS::getDirection )
        //                .def( "setDirection", &MovementControlFPS::setDirection )

        //                .def( "getGameActionLeft", &MovementControlFPS::getGameActionLeft )
        //                .def( "setGameActionLeft", &MovementControlFPS::setGameActionLeft )
        //                .def( "getGameActionRight", &MovementControlFPS::getGameActionRight )
        //                .def( "setGameActionRight", &MovementControlFPS::setGameActionRight )

        //                .def( "getGameActionUp", &MovementControlFPS::getGameActionUp )
        //                .def( "setGameActionUp", &MovementControlFPS::setGameActionUp )
        //                .def( "getGameActionDown", &MovementControlFPS::getGameActionDown )
        //                .def( "setGameActionDown", &MovementControlFPS::setGameActionDown )

        //                .def( "getCameraSceneNode", &MovementControlFPS::getCameraSceneNode )
        //                .def( "setCameraSceneNode", &MovementControlFPS::setCameraSceneNode )

        //                .def( "getCharacterSceneNode", &MovementControlFPS::getCharacterSceneNode )
        //                .def( "setCharacterSceneNode", &MovementControlFPS::setCharacterSceneNode )

        //                .def( "getCharacter", &MovementControlFPS::getCharacter )
        //                .def( "setCharacter", &MovementControlFPS::setCharacter )

        //                .def( "getUserControlled", &MovementControlFPS::getUserControlled )
        //                .def( "setUserControlled", &MovementControlFPS::setUserControlled )

        //                .def( "getWeapon", &MovementControlFPS::getWeapon )
        //                .def( "setWeapon", &MovementControlFPS::setWeapon )];

        //module(
        //    L )[class_<EntityMovement2, IComponent, SmartPtr<IComponent>>( "EntityMovement2" )
        //            .def( "updatePhysicsBody", &EntityMovement2::updatePhysicsBody )
        //            .def( "getBody", &EntityMovement2::getBody )
        //            .def( "setBody", &EntityMovement2::setBody )
        //            .def( "getMovementDirection", &EntityMovement2::getMovementDirection )
        //            .def( "setMovementDirection", &EntityMovement2::setMovementDirection )
        //            .def( "getDampVelocityState", &EntityMovement2::getDampVelocityState )
        //            .def( "setDampVelocityState", &EntityMovement2::setDampVelocityState )
        //            .def( "getSpeedMultiplier", &EntityMovement2::getSpeedMultiplier )
        //            .def( "setSpeedMultiplier", &EntityMovement2::setSpeedMultiplier )
        //            .def( "getSpeed", &EntityMovement2::getSpeed )
        //            .def( "setSpeed", &EntityMovement2::setSpeed )
        //            .def( "getAllowMovement", &EntityMovement2::getAllowMovement )
        //            .def( "setAllowMovement", &EntityMovement2::setAllowMovement )
        //            .def( "getStaticDamp", &EntityMovement2::getStaticDamp )
        //            .def( "setStaticDamp", &EntityMovement2::setStaticDamp )
        //            .def( "getDynamicDamp", &EntityMovement2::getDynamicDamp )
        //            .def( "setDynamicDamp", &EntityMovement2::setDynamicDamp )
        //            .def( "getMovementControlId", &EntityMovement2::getMovementControlId )
        //            .def( "setMovementControlId", &EntityMovement2::setMovementControlId )];

        //module(
        //    L )[class_<SuperFreezeLogic, IComponent, boost::shared_ptr<IComponent>>( "SuperFreezeLogic" )
        //            .def( "isFrozen", &SuperFreezeLogic::isFrozen )
        //            .def( "setFrozen", &SuperFreezeLogic::setFrozen )];

        //module( L )[class_<InputComponent, IComponent, boost::shared_ptr<IComponent>>( "InputComponent" )
        //                .def( "getGameInputId", &InputComponent::getGameInputId )
        //                .def( "setGameInputId", &InputComponent::setGameInputId )];

        //module(
        //    L )[class_<WeaponContainer, IComponent, boost::shared_ptr<IComponent>>( "WeaponContainer" )
        //            .def( "addWeapon", &WeaponContainer::addWeapon )
        //            .def( "addWeapon", _addFireArm )
        //            .def( "getWeapon", _getWeapon )];

        //module( L )[class_<ViewportComponent, IComponent, boost::shared_ptr<IComponent>>(
        //                "ViewportConstraint" )
        //                .def( "setViewportId", &ViewportComponent::setViewportId )
        //                .def( "getViewportId", &ViewportComponent::getViewportId )];

        //module( L )[class_<EnergyContainer, IComponent, boost::shared_ptr<IComponent>>(
        //                "ViewportConstraint" )
        //                .def( "addEnergy", &EnergyContainer::addEnergy )
        //                .def( "setEnergy", &EnergyContainer::setEnergy )
        //                .def( "getEnergy", &EnergyContainer::getEnergy )

        //                .def( "setMaxEnergy", &EnergyContainer::setMaxEnergy )
        //                .def( "getMaxEnergy", &EnergyContainer::getMaxEnergy )
        //                .def( "isAlive", &EnergyContainer::isAlive )];

        //module( L )[class_<Grid2Container, IComponent, boost::shared_ptr<IComponent>>( "Grid2Container" )
        //                .def( "setDimensions", &Grid2Container::setDimensions )
        //                .def( "getDimensions", &Grid2Container::getDimensions )
        //                .def( "clear", &Grid2Container::clear )
        //                .def( "isCellOccupied", &Grid2Container::isCellOccupied )
        //                .def( "setCellOccupied", &Grid2Container::setCellOccupied )
        //                .def( "getCellIndex", &Grid2Container::getCellIndex )
        //                .def( "getCellPosition", &Grid2Container::getCellPosition )];

        //module( L )[class_<MatchLogic, IComponent, boost::shared_ptr<IComponent>>( "MatchLogic" )
        //                .def( "getNumRoundsWon", &MatchLogic::getNumRoundsWon )
        //                .def( "setNumRoundsWon", &MatchLogic::setNumRoundsWon )

        //                .def( "getTotalRounds", &MatchLogic::getTotalRounds )
        //                .def( "setTotalRounds", &MatchLogic::setTotalRounds )

        //                .def( "getCurrentRound", &MatchLogic::getCurrentRound )
        //                .def( "setCurrentRound", &MatchLogic::setCurrentRound )

        //                .def( "setCurrentRoundState", &MatchLogic::setCurrentRoundState )
        //                .def( "getCurrentRoundState", &MatchLogic::getCurrentRoundState )

        //                .def( "getRoundEndState", &MatchLogic::getRoundEndState )
        //                .def( "setRoundEndState", &MatchLogic::setRoundEndState )];

        //module( L )[class_<ComboLogic, IComponent, boost::shared_ptr<IComponent>>( "ComboLogic" )
        //                .def( "addComboCount", &ComboLogic::addComboCount )
        //                .def( "setComboCount", &ComboLogic::setComboCount )
        //                .def( "getComboCount", &ComboLogic::getComboCount )
        //                .def( "setComboFinishTime", &ComboLogic::setComboFinishTime )
        //                .def( "getComboFinishTime", &ComboLogic::getComboFinishTime )];

        //module( L )[class_<AnimatorContainer, IComponent, boost::shared_ptr<IComponent>>(
        //                "AnimatorContainer" )
        //                .def( "setAnimator", &AnimatorContainer::addAnimator )
        //                .def( "getAnimator", _getAnimator )];

        //module( L )[class_<FragCounter, IComponent, boost::shared_ptr<IComponent>>( "FragCounter" )
        //                .def( "addFrags", &FragCounter::addFrags )
        //                .def( "setFrags", &FragCounter::setFrags )
        //                .def( "getFrags", &FragCounter::getFrags )];

        //module( L )[class_<Playable, IComponent, boost::shared_ptr<IComponent>>( "Playable" )
        //                .def( "getPlayerNumber", &Playable::getPlayerNumber )
        //                .def( "setPlayerNumber", &Playable::setPlayerNumber )

        //                .def( "isUserControlled", &Playable::isUserControlled )
        //                .def( "setUserControlled", &Playable::setUserControlled )];

        //module( L )[class_<AiGoalBased, IComponent, boost::shared_ptr<IComponent>>( "AiGoalBased" )
        //                .def( "getGoal", &AiGoalBased::getGoal )
        //                .def( "setGoal", &AiGoalBased::setGoal )
        //                //.def( "setGoal", _setGoal )

        //                .def( "getSteering", &AiGoalBased::getSteering )
        //                .def( "setSteering", &AiGoalBased::setSteering )

        //                .def( "getCharacter", &AiGoalBased::getCharacter )
        //                .def( "setCharacter", &AiGoalBased::setCharacter )

        //                .def( "getTargeting", &AiGoalBased::getTargeting )
        //                .def( "setTargeting", &AiGoalBased::setTargeting )];

        //module( L )[class_<Position25, IComponent, boost::shared_ptr<IComponent>>( "Position25" )
        //                .def( "getScreenPosition", &Position25::getScreenPosition )
        //                .def( "setScreenPosition", &Position25::setScreenPosition )

        //                .def( "getScreenPositionOffset", &Position25::getScreenPositionOffset )
        //                .def( "setScreenPositionOffset", &Position25::setScreenPositionOffset )

        //                .def( "getWorldPosition", &Position25::getWorldPosition )

        //                .def( "getScreenDepth", &Position25::getScreenDepth )
        //                .def( "setScreenDepth", &Position25::setScreenDepth )

        //                .def( "getRigidBody", &Position25::getPhysicsBody )
        //                .def( "setRigidBody", &Position25::setPhysicsBody )

        //                .def( "getSceneNode", &Position25::getSceneNode )
        //                .def( "setSceneNode", &Position25::setSceneNode )];

        //module( L )[class_<ScreenSpaceRotation, IComponent, boost::shared_ptr<IComponent>>(
        //                "ScreenSpaceRotation" )
        //                .def( "getRotation", &ScreenSpaceRotation::getRotation )
        //                .def( "setRotation", &ScreenSpaceRotation::setRotation )
        //                .def( "getOrientation", &ScreenSpaceRotation::getOrientation )
        //                .def( "getLocalOrientation", &ScreenSpaceRotation::getLocalOrientation )
        //                .def( "getCameraOrientation", &ScreenSpaceRotation::getCameraOrientation )

        //                .def( "getSceneNode", &ScreenSpaceRotation::getSceneNode )
        //                .def( "setSceneNode", &ScreenSpaceRotation::setSceneNode )

        //                .def( "getLocalNode", &ScreenSpaceRotation::getLocalNode )
        //                .def( "setLocalNode", &ScreenSpaceRotation::setLocalNode )];

        //module( L )[class_<Transformation, IComponent, TransformationPtr>( "Transformation" )
        //                .def( "getPosition", &Transformation::getPosition )
        //                .def( "setPosition", &Transformation::setPosition )
        //                .def( "getOrientation", &Transformation::getOrientation )
        //                .def( "setOrientation", &Transformation::setOrientation )];

        //module(L)
        //	[
        //		class_<CollisionNode::NodeList>( "CollisionNodeList" )
        //	];

        //module( L )[class_<CollisionNode, IComponent, boost::shared_ptr<IComponent>>( "CollisionNode" )
        //.def( "getRootType", &CollisionNode::getRootType )

        //.def("getNodeId", &CollisionNode::getNodeId )
        //.def("_setNodeId", &CollisionNode::_setNodeId )

        //.def("getNodeType", &CollisionNode::getNodeType )
        //.def("setNodeType", &CollisionNode::setNodeType )

        //.def("hasParent", &CollisionNode::hasParent )
        //.def("getParent", _getCollisionNodeParent )

        //.def("addChild", &CollisionNode::addChild )
        //.def("addChild", _addChild )
        //.def("removeChild", &CollisionNode::removeChild )
        //.def("removeChild", _removeChild )
        //.def("findChild", &CollisionNode::findChild )
        //.def("remove", &CollisionNode::remove )
        //.def("removeAllChildren", &CollisionNode::removeAllChildren )
        //.def("getChildren", &CollisionNode::getChildren )
        //];

        //module( L )[class_<Destructible, IComponent, DestructiblePtr>( "Destructible" )
        //                .def( "getScene", &Destructible::getScene )
        //                .def( "setScene", &Destructible::setScene )
        //                .def( "reset", &Destructible::reset )
        //                .def( "destroy", &Destructible::destroy )

        //                .def( "createPhysics", &Destructible::createPhysics )

        //                .def( "getScale", &Destructible::getScale )
        //                .def( "setScale", &Destructible::setScale )

        //                .def( "setCollisionType", DestructibleHelper::setCollisionType )
        //                .def( "getCollisionType", DestructibleHelper::getCollisionType )
        //                .def( "setCollisionMask", DestructibleHelper::setCollisionMask )
        //                .def( "getCollisionMask", DestructibleHelper::getCollisionMask )

        //                .def( "getMaxLifeTime", &Destructible::getMaxLifeTime )
        //                .def( "setMaxLifeTime", &Destructible::setMaxLifeTime )

        //                .def( "isVisible", &Destructible::isVisible )
        //                .def( "setVisible", &Destructible::setVisible )

        //                .def( "add", &Destructible::add )
        //                .def( "remove", &Destructible::remove )];

        //module( L )[class_<Collision3, IComponent, Collision3Ptr>( "Collision3" )
        //                .def( "getExtents", &Collision3::getExtents )
        //                .def( "setExtents", &Collision3::setExtents )

        //                .def( "getPosition", &Collision3::getPosition )
        //                .def( "setPosition", &Collision3::setPosition )

        //                .def( "getRadius", &Collision3::getRadius )
        //                .def( "setRadius", &Collision3::setRadius )

        //                .def( "getRigidBody", &Collision3::getRigidBody )
        //                .def( "setRigidBody", &Collision3::setRigidBody )

        //                .def( "getBoundingBox", &Collision3::getBoundingBox )];

        //module( L )[class_<BasicPhysicsObject, IComponent, BasicPhysicsObjectPtr>( "BasicPhysicsObject" )
        //                .def( "getRigidBody", &BasicPhysicsObject::getRigidBody )
        //                .def( "setRigidBody", &BasicPhysicsObject::setRigidBody )
        //                .def( "getSceneNode", &BasicPhysicsObject::getSceneNode )
        //                .def( "setSceneNode", &BasicPhysicsObject::setSceneNode )

        //                .def( "getPosition", &BasicPhysicsObject::getPosition )
        //                .def( "setPosition", &BasicPhysicsObject::setPosition )];

        //module( L )[class_<PuzzleMatchManager, IComponent, PuzzleSpawnManagerPtr>( "PuzzleMatchManager" )
        //                .def( "getViewportId", &PuzzleMatchManager::getViewportId )
        //                .def( "setViewportId", &PuzzleMatchManager::setViewportId )

        //                .def( "getFilled", &PuzzleMatchManager::getFilled )
        //                .def( "setFilled", &PuzzleMatchManager::setFilled )

        //                .def( "isCritical", &PuzzleMatchManager::isCritical )
        //                .def( "isGameOver", &PuzzleMatchManager::isGameOver )

        //                .def( "getOtherSpawnManager", &PuzzleMatchManager::getOtherSpawnManager )
        //                .def( "setOtherSpawnManager", &PuzzleMatchManager::setOtherSpawnManager )

        //                .def( "getGridComponentId", &PuzzleMatchManager::getGridComponentId )
        //                .def( "setGridComponentId", &PuzzleMatchManager::setGridComponentId )

        //                .def( "getSpawnDetonator", &PuzzleMatchManager::getSpawnDetonator )
        //                .def( "setSpawnDetonator", &PuzzleMatchManager::setSpawnDetonator )

        //                .def( "getShouldSpawn", &PuzzleMatchManager::getShouldSpawn )
        //                .def( "getSpawnJunk", &PuzzleMatchManager::getSpawnJunk )

        //                .def( "spawnJunk", &PuzzleMatchManager::spawnJunk )
        //                .def( "spawnEntities", &PuzzleMatchManager::spawnEntities )

        //                .def( "getEnableCollectables", &PuzzleMatchManager::getEnableCollectables )
        //                .def( "setEnableCollectables", &PuzzleMatchManager::setEnableCollectables )

        //                .def( "getSpawnedEntities", &PuzzleMatchManager::getSpawnedEntities )
        //                .def( "setSpawnedEntities", &PuzzleMatchManager::setSpawnedEntities )

        //                .def( "isFlipped", &PuzzleMatchManager::isFlipped )
        //                .def( "setFlipped", &PuzzleMatchManager::setFlipped )

        //                .def( "getGameInput", &PuzzleMatchManager::getGameInput )
        //                .def( "setGameInput", &PuzzleMatchManager::setGameInput )

        //                .def( "getFlipInputAction", &PuzzleMatchManager::getFlipInputAction )
        //                .def( "setFlipInputAction", &PuzzleMatchManager::setFlipInputAction )];

        //module( L )[class_<PuzzleMatchElement, IComponent, PuzzleMatchElementPtr>( "PuzzleMatchElement" )
        //                .def( "updateLogic", &PuzzleMatchElement::updateLogic )
        //                .def( "updateDropDownLogic", &PuzzleMatchElement::updateDropDownLogic )

        //                .def( "getGridComponentId", &PuzzleMatchElement::getGridComponentId )
        //                .def( "setGridComponentId", &PuzzleMatchElement::setGridComponentId )
        //                .def( "getPhysicsBody", &PuzzleMatchElement::getPhysicsBody )
        //                .def( "setPhysicsBody", &PuzzleMatchElement::setPhysicsBody )

        //                .def( "getPosition", &PuzzleMatchElement::getPosition )
        //                .def( "setPosition", &PuzzleMatchElement::setPosition )

        //                .def( "getPlayerPosition", &PuzzleMatchElement::getPlayerPosition )
        //                .def( "setPlayerPosition", &PuzzleMatchElement::setPlayerPosition )

        //                .def( "isColliding", &PuzzleMatchElement::isColliding )
        //                .def( "setColliding", &PuzzleMatchElement::setColliding )

        //                .def( "isNextCellOccupied", &PuzzleMatchElement::isNextCellOccupied )

        //                .def( "getWorldId", &PuzzleMatchElement::getWorldId )
        //                .def( "setWorldId", &PuzzleMatchElement::setWorldId )

        //                .def( "isInCell", &PuzzleMatchElement::isInCell )
        //                .def( "getCellIndex", &PuzzleMatchElement::getCell )
        //                .def( "setCellIndex", &PuzzleMatchElement::setCell )
        //                .def( "resetCellIndex", &PuzzleMatchElement::resetCellIndex )
        //                .def( "isNextCellOccupied", &PuzzleMatchElement::isNextCellOccupied )

        //                .def( "explode", &PuzzleMatchElement::explode )
        //                .def( "getNumConnectedEntities", &PuzzleMatchElement::getNumConnectedEntities )
        //                .def( "getConnectedEntity", &PuzzleMatchElement::getConnectedEntity )
        //                .def( "getConnectedEntities", &PuzzleMatchElement::getConnectedEntities )

        //                .def( "isJunk", &PuzzleMatchElement::isJunk )
        //                .def( "setJunk", &PuzzleMatchElement::setJunk )

        //                .def( "getViewportId", &PuzzleMatchElement::getViewportId )
        //                .def( "setViewportId", &PuzzleMatchElement::setViewportId )

        //                .def( "isControlled", &PuzzleMatchElement::isControlled )
        //                .def( "setControlled", &PuzzleMatchElement::setControlled )

        //                .def( "getLiquidDropSpeed", &PuzzleMatchElement::getFastDropSpeed )
        //                .def( "setLiquidDropSpeed", &PuzzleMatchElement::setFastDropSpeed )

        //                .def( "isGrounded", &PuzzleMatchElement::isGrounded )
        //                .def( "setGrounded", &PuzzleMatchElement::setGrounded )

        //                .def( "getDropForce", &PuzzleMatchElement::getDropForce )
        //                .def( "setDropForce", &PuzzleMatchElement::setDropForce )

        //                .def( "getElementType", &PuzzleMatchElement::getElementType )
        //                .def( "setElementType", &PuzzleMatchElement::setElementType )

        //                .def( "getNumConnections", &PuzzleMatchElement::getNumConnections )
        //                .def( "setNumConnections", &PuzzleMatchElement::setNumConnections )

        //                .def( "isAutoExplode", &PuzzleMatchElement::isAutoExplode )
        //                .def( "setAutoExplode", &PuzzleMatchElement::setAutoExplode )

        //                .def( "isFlipped", &PuzzleMatchElement::isFlipped )
        //                .def( "setFlipped", &PuzzleMatchElement::setFlipped )

        //                .def( "getPuzzleMatchManager", &PuzzleMatchElement::getPuzzleMatchManager )
        //                .def( "setPuzzleMatchManager", &PuzzleMatchElement::setPuzzleMatchManager )];

        /*
        module( L )[class_<PuzzleMatchWorld, IComponent, PuzzleWorldPtr>( "PuzzleWorld" )
                        .def( "query", &PuzzleMatchWorld::query )

                        .def( "clear", &PuzzleMatchWorld::clear )

                        .def( "getGrid", &PuzzleMatchWorld::getGrid )
                        .def( "setGrid", &PuzzleMatchWorld::setGrid )

                        .def( "getViewportId", &PuzzleMatchWorld::getViewportId )
                        .def( "setViewportId", &PuzzleMatchWorld::setViewportId )];

        module( L )[class_<SubType, IComponent, SubTypePtr>( "SubType" )
                        .def( "getSubType", &SubType::getSubType )
                        .def( "setSubType", &SubType::setSubType )];

        module( L )[class_<GravitonLogic, IComponent, GravitonLogicPtr>( "GravitonLogic" )
                        .def( "getGravitonAngle", &GravitonLogic::getGravitonAngle )
                        .def( "setGravitonAngle", &GravitonLogic::setGravitonAngle )
                        .def( "addGravitonAngle", &GravitonLogic::addGravitonAngle )

                        .def( "getOffsetAngle", &GravitonLogic::getOffsetAngle )
                        .def( "setOffsetAngle", &GravitonLogic::setOffsetAngle )

                        .def( "getFlipGravitons", &GravitonLogic::getFlipGravitons )
                        .def( "setFlipGravitons", &GravitonLogic::setFlipGravitons )

                        .def( "getNumGravitons", &GravitonLogic::getNumGravitons )
                        .def( "clearGravitons", &GravitonLogic::clearGravitons )

                        .def( "addGraviton", &GravitonLogic::addGraviton )
                        .def( "removeGraviton", &GravitonLogic::removeGraviton )
                        .def( "getGraviton", &GravitonLogic::getGraviton )];

        module( L )[class_<HitData2, IComponent, HitData2Ptr>( "HitData2" )
                        .def( "getAttackStunTime", &HitData2::getAttackStunTime )
                        .def( "setAttackStunTime", &HitData2::setAttackStunTime )

                        .def( "getAttackType", &HitData2::getAttackType )
                        .def( "setAttackType", &HitData2::setAttackType )];

        module( L )[class_<ObjectContainer, IComponent, ObjectContainerPtr>( "ObjectContainer" )
                        .def( "addObject", &ObjectContainer::addObject )
                        .def( "removeObject", &ObjectContainer::removeObject )

                        .def( "findByName", &ObjectContainer::findByName )
                        .def( "findById", &ObjectContainer::findById )];
*/

        //module(L)
        //	[
        //		class_<PropertiesComponent, IComponent, Propertiesboost::shared_ptr<IComponent>>( "PropertiesComponent" )
        //		.def("getString", &PropertiesComponent::getString )
        //		.def("setString", &PropertiesComponent::setString )
        //		.def("getInteger", &PropertiesComponent::getInteger )
        //		.def("setInteger", &PropertiesComponent::setInteger )
        //		.def("getNumber", &PropertiesComponent::getNumber )
        //		.def("setNumber", &PropertiesComponent::setNumber )
        //		.def("getParameters", &PropertiesComponent::getParameters )
        //		.def("setParameters", &PropertiesComponent::setParameters )
        //	];

        /*
        module( L )[class_<EntitySelection2, IComponent, EntitySelection2Ptr>( "EntitySelection2" )
                        .def( "getEntitySelectedAction", &EntitySelection2::getEntitySelectedAction )
                        .def( "setEntitySelectedAction", &EntitySelection2::setEntitySelectedAction )

                        .def( "getNullSelectionAction", &EntitySelection2::getNullSelectionAction )
                        .def( "setNullSelectionAction", &EntitySelection2::setNullSelectionAction )

                        .def( "getToolTipAction", &EntitySelection2::getToolTipAction )
                        .def( "setToolTipAction", &EntitySelection2::setToolTipAction )

                        .def( "getTargetPosition", &EntitySelection2::getTargetPosition )
                        .def( "setTargetPosition", &EntitySelection2::setTargetPosition )

                        .def( "getSelectedEntity", &EntitySelection2::getSelectedEntity )
                        .def( "setSelectedEntity", &EntitySelection2::setSelectedEntity )

                        .def( "getToolTipEntity", &EntitySelection2::getToolTipEntity )
                        .def( "setToolTipEntity", &EntitySelection2::setToolTipEntity )

                        .def( "addSelectableEntity", &EntitySelection2::addSelectableEntity )
                        .def( "removeSelectableEntity", &EntitySelection2::removeSelectableEntity )
                        .def( "getSelectableEntities", &EntitySelection2::getSelectableEntities )
                        .def( "setSelectableEntities", &EntitySelection2::setSelectableEntities )

                        .def( "getViewportId", &EntitySelection2::getViewportId )
                        .def( "setViewportId", &EntitySelection2::setViewportId )

                        .def( "getEnableEntitySelection", &EntitySelection2::getEnableEntitySelection )
                        .def( "setEnableEntitySelection", &EntitySelection2::setEnableEntitySelection )];
*/

        //module(L)
        //	[
        //		class_<ToolTipComponent, IComponent, ToolTipboost::shared_ptr<IComponent>>( "ToolTipComponent" )
        //		.def("getToolTip", &ToolTipComponent::getToolTip )
        //		.def("setToolTip", &ToolTipComponent::setToolTip )
        //		.def("getContextToolTip", &ToolTipComponent::getContextToolTip )
        //		.def("setContextToolTip", &ToolTipComponent::setContextToolTip )
        //	];

        //module( L )[class_<IProjectile3, IScriptObject, Projectile3Ptr>( "IProjectile3" )
        //                .def( "shoot", &IProjectile3::shoot )];

        //module( L )[class_<Projectile3Standard, IProjectile3, Projectile3Ptr>( "Projectile3Standard" )
        //                .def( "getSceneNode", &Projectile3Standard::getSceneNode )
        //                .def( "setSceneNode", &Projectile3Standard::setSceneNode )

        //                .def( "getObjects", &Projectile3Standard::getObjects )
        //                .def( "setObjects", &Projectile3Standard::setObjects )
        //                .def( "addObject", &Projectile3Standard::addObject )

        //                .def( "getCollisionType", Projectile3Helper::getCollisionType )
        //                .def( "setCollisionType", Projectile3Helper::setCollisionType )

        //                .def( "getCollisionMask", Projectile3Helper::getCollisionMask )
        //                .def( "setCollisionMask", Projectile3Helper::setCollisionMask )

        //                .def( "getDynamicCollisionType", Projectile3Helper::getDynamicCollisionType )
        //                .def( "setDynamicCollisionType", Projectile3Helper::setDynamicCollisionType )

        //                .def( "getDynamicCollisionMask", Projectile3Helper::getDynamicCollisionMask )
        //                .def( "setDynamicCollisionMask", Projectile3Helper::setDynamicCollisionMask )];

        //module( L )[class_<IWeapon, IEntity, SmartPtr<scene::IActor>>( "IWeapon" )];

        //module( L )[class_<IWeaponFireArm2, IWeapon, WeaponPtr>( "WeaponFireArm2" )
        //                .def( "shoot", &IWeaponFireArm2::shoot )
        //                .def( "isShooting", &IWeaponFireArm2::isShooting )
        //                .def( "stopShooting", &IWeaponFireArm2::stopShooting )

        //                .def( "getLastShootTime", &IWeaponFireArm2::getLastShootTime )
        //                .def( "canShoot", &IWeaponFireArm2::canShoot )

        //                .def( "getPosition", &IWeaponFireArm2::getPosition )
        //                .def( "setPosition", &IWeaponFireArm2::setPosition )

        //                .def( "getTargetDirection", &IWeaponFireArm2::getTargetDirection )
        //                .def( "setTargetDirection", &IWeaponFireArm2::setTargetDirection )

        //                .def( "getProjectileDamage", &IWeaponFireArm2::getProjectileDamage )
        //                .def( "setProjectileDamage", &IWeaponFireArm2::setProjectileDamage )

        //                .def( "setProjectileVelocity", &IWeaponFireArm2::setProjectileVelocity )
        //                .def( "getProjectileVelocity", &IWeaponFireArm2::getProjectileVelocity )

        //                .def( "setNumShots", &IWeaponFireArm2::setNumShots )
        //                .def( "getNumShots", &IWeaponFireArm2::getNumShots )

        //                .def( "setProjectileType", &IWeaponFireArm2::setProjectileType )
        //                .def( "getProjectileType", &IWeaponFireArm2::getProjectileType )

        //                .def( "getWorldId", &IWeaponFireArm2::getWorldId )
        //                .def( "setWorldId", &IWeaponFireArm2::setWorldId )

        //                .def( "createProjectilePool", &IWeaponFireArm2::createProjectilePool )

        //                .def( "getOwner", &IWeaponFireArm2::getOwner )
        //                .def( "setOwner", &IWeaponFireArm2::setOwner )];

        //module( L )[class_<IWeaponFireArm3, IWeapon, SmartPtr<scene::IActor>>( "IWeaponFireArm3" )];

        //module(
        //    L )[class_<WeaponFireArm3Standard, IWeaponFireArm3, SmartPtr<scene::IActor>>( "WeaponFireArm3Standard" )];

        //module( L )[class_<IVehicleController, IEntity, SmartPtr<scene::IActor>>( "IVehicle" )
        //                .def( "getPosition", &IVehicleController::getPosition )
        //                .def( "setPosition", &IVehicleController::setPosition )

        //                //.def("getCameraController", &IVehicleController::getCameraController )
        //                //.def("setCameraController", &IVehicleController::setCameraController )

        //                .def( "isUserControlled", &IVehicleController::isUserControlled )
        //                .def( "setUserControlled", &IVehicleController::setUserControlled )];

        //module( L )[class_<CarStandard, IVehicleController, SmartPtr<scene::IActor>>( "CarStandard" )
        //                .def( "getCameraSceneNode", &CarStandard::getCameraSceneNode )
        //                .def( "setCameraSceneNode", &CarStandard::setCameraSceneNode )];

        //module( L )[class_<ISpecialFx, IEntity, SmartPtr<scene::IActor>>( "ISpecialFx" )
        //                .def( "start", &ISpecialFx::start )
        //                .def( "stop", &ISpecialFx::stop )

        //                .def( "getVisibilityMask", &ISpecialFx::getVisibilityMask )
        //                .def( "setVisibilityMask", &ISpecialFx::setVisibilityMask )];

        //module( L )[class_<IScene, IEntity, boost::shared_ptr<IObject>>( "IScene" )
        //                .def( "enter", &IScene::enter )
        //                .def( "leave", &IScene::leave )];

        //module( L )[def( "createEntityMessage", _createEntityMessage )];

        //module( L )[def( "createMapEntityMessage", _createMapEntityMessage )];

        //module(L)
        //	[
        //		class_<EntityMessageDispatcher, IScriptObject>( "EntityMessageDispatcher" )
        //		.def("update", &EntityMessageDispatcher::update )
        //		.def("queueMessage", _addMessageToQueue )
        //		.def("queueMessage", _addMessageToQueuePtr )
        //		.def("dispatchMessage", &EntityMessageDispatcher::dispatchMessage )
        //		.def("clear", &EntityMessageDispatcher::clear )
        //	];

        //module( L )[class_<Array<SmartPtr<scene::IActor>>>( "Array<SmartPtr<scene::IActor>>" )
        //                .def( constructor<>() )

        //                .def( "reserve", &Array<SmartPtr<scene::IActor>>::reserve )
        //                //.def("set_used", &Array<SmartPtr<scene::IActor>>::set_used )

        //                //.def("push_back", &Array<SmartPtr<scene::IActor>>::push_back )

        //                //.def("erase", (void (Array<SmartPtr<scene::IActor>>::*)(u32))&Array<SmartPtr<scene::IActor>>::erase )
        //                //.def("erase_element", &Array<SmartPtr<scene::IActor>>::erase_element )

        //                .def( "size", &Array<SmartPtr<scene::IActor>>::size )

        //                .def( "get", _getFromEntityArray )];

        //
        // Game library
        //
        //module(
        //    L )[class_<StandardGameObject, IEntity, boost::shared_ptr<IObject>>( "StandardGameObject" )];
    }

    void bindCombat( lua_State *L )
    {
        using namespace luabind;

        //module( L )[class_<ICombatAttackData, IScriptObject, CombatAttackDataPtr>( "CombatAttackData" )
        //                .def( "setMoveLength", &ICombatAttackData::setMoveLength )
        //                .def( "getMoveLength", &ICombatAttackData::getMoveLength )
        //                .def( "setHasTriggered", &ICombatAttackData::setHasTriggered )
        //                .def( "getHasTriggered", &ICombatAttackData::getHasTriggered )
        //                .def( "setHitTime", &ICombatAttackData::setHitTime )
        //                .def( "getHitTime", &ICombatAttackData::getHitTime )
        //                .def( "setHitTimeInterval", &ICombatAttackData::setHitTimeInterval )
        //                .def( "getHitTimeInterval", &ICombatAttackData::getHitTimeInterval )
        //                .def( "setNextAttackInterval", &ICombatAttackData::setNextAttackInterval )
        //                .def( "getNextAttackInterval", &ICombatAttackData::getNextAttackInterval )];

        //module( L )[class_<ICombatBody2, IScriptObject, CombatBody2Ptr>( "CombatBody2" )
        //                .def( "getId", &ICombatBody2::getId )
        //                .def( "addHitBox", &ICombatBody2::addHitBox )
        //                .def( "getHitBox", &ICombatBody2::getHitBox )

        //                .def( "setPosition", &ICombatBody2::setPosition )
        //                .def( "getPosition", &ICombatBody2::getPosition )

        //                .def( "setMask", &ICombatBody2::setMask )
        //                .def( "getMask", &ICombatBody2::getMask )

        //                .def( "getFSM", _getCombatBodyFSM )

        //                .def( "addCombatBodyListener", &ICombatBody2::addCombatBodyListener )
        //                .def( "removeCombatBodyListener", &ICombatBody2::removeCombatBodyListener )];

        //module( L )[class_<ICombatBodyListener2>( "CombatBodyListener2" )
        //            //.def("get", &IComponentPoolMgr::get )
        //            //.def("getById", &IComponentPoolMgr::getById )
        //            //.def("getNumFree", &IComponentPoolMgr::getNumFree )
        //];

        //module( L )[class_<ICombatHitBox2, IScriptObject, CombatHitBox2Ptr>( "CombatHitBox2" )
        //                .def( "intersects", &ICombatHitBox2::intersects )

        //                .def( "setFSM", &ICombatHitBox2::setFSM )
        //                .def( "getFSM", &ICombatHitBox2::getFSM )

        //                .def( "setDerivedPosition", &ICombatHitBox2::setDerivedPosition )
        //                .def( "getDerivedPosition", &ICombatHitBox2::getDerivedPosition )

        //                .def( "setPosition", &ICombatHitBox2::setPosition )
        //                .def( "getPosition", &ICombatHitBox2::getPosition )

        //                .def( "setExtents", &ICombatHitBox2::setExtents )
        //                .def( "getExtents", &ICombatHitBox2::getExtents )

        //                .def( "getAABB", &ICombatHitBox2::getAABB )

        //                .def( "setOwner", &ICombatHitBox2::setOwner )
        //                .def( "getOwner", &ICombatHitBox2::getOwner )

        //                .def( "setId", &ICombatHitBox2::setId )
        //                .def( "getId", &ICombatHitBox2::getId )

        //                .def( "setHitPosition", &ICombatHitBox2::setHitPosition )
        //                .def( "getHitPosition", &ICombatHitBox2::getHitPosition )];

        //module( L )[class_<ICombatHitData2>( "CombatHitData2" )
        //                .def( "setCombatBodyA", &ICombatHitData2::setCombatBodyA )
        //                .def( "getCombatBodyA", &ICombatHitData2::getCombatBodyA )
        //                .def( "setCombatBodyB", &ICombatHitData2::setCombatBodyB )
        //                .def( "getCombatBodyB", &ICombatHitData2::getCombatBodyB )
        //                .def( "setHitBoxAId", &ICombatHitData2::setHitBoxAId )
        //                .def( "getHitBoxAId", &ICombatHitData2::getHitBoxAId )
        //                .def( "setHitBoxBId", &ICombatHitData2::setHitBoxBId )
        //                .def( "getHitBoxBId", &ICombatHitData2::getHitBoxBId )
        //                .def( "setHitPosition", &ICombatHitData2::setHitPosition )
        //                .def( "getHitPosition", &ICombatHitData2::getHitPosition )
        //                .def( "getLinearVelocity", &ICombatHitData2::getLinearVelocity )
        //                .def( "setLinearVelocity", &ICombatHitData2::setLinearVelocity )
        //                .def( "getAngularVelocity", &ICombatHitData2::getAngularVelocity )
        //                .def( "setAngularVelocity", &ICombatHitData2::setAngularVelocity )
        //                .def( "getDamage", &ICombatHitData2::getDamage )
        //                .def( "setDamage", &ICombatHitData2::setDamage )
        //                .def( "getAttackType", &ICombatHitData2::getAttackType )
        //                .def( "setAttackType", &ICombatHitData2::setAttackType )];

        //module( L )[class_<ICombatManager2, IScriptObject, CombatManager2Ptr>( "CombatManager2" )
        //                .def( "createAttackData", &ICombatManager2::createAttackData )
        //                .def( "addCombatBody", &ICombatManager2::addCombatBody )
        //                .def( "getCombatBody", &ICombatManager2::getCombatBody )
        //                .def( "_removeCombatBody", &ICombatManager2::_removeCombatBody )];

        //typedef Array<CombatAttackDataPtr> CombatAttackDataArray;
        //typedef ArrayFunctions<CombatAttackDataPtr> CombatAttackDataFunctions;

        //module( L )
        //    [class_<CombatAttackDataArray>( "CombatAttackDataArray" )
        //         .def( constructor<>() )

        //         .def( "reserve", &CombatAttackDataArray::reserve )
        //     //.def("set_used", &CombatAttackDataArray::set_used )

        //     //.def("push_back", &CombatAttackDataArray::push_back )

        //     //.def("erase", (void (CombatAttackDataArray::*)(u32))&CombatAttackDataArray::erase_element_index )
        //     //.def("erase_element", &CombatAttackDataArray::erase_element )

        //     //.def("get", CombatAttackDataFunctions::get )
        //     /*
        //                     //convenience functions
        //                     .def("addAsBool", addParamAsBool )
        //                     .def("addAsInt", addParamAsInt )
        //                     .def("addAsNumber", addParamAsNumber )

        //                     .def("getAsBool", getParamAsBool )
        //                     .def("getAsInt", getParamAsInt )
        //                     .def("getAsNumber", getParamAsNumber )
        //                     .def("getAsObject", getParamAsObject )
        //                     .def("getAsEntity", getParamAsEntity )
        //                     .def("getAsStateMessage", getParamAsStateMessage )

        //                     .def("size", getListSize )
        //                     */
        //];

        //module( L )[class_<CombatBodyContainer>( "CombatBodyContainer" )
        //                .def( "getCombatBody", &CombatBodyContainer::getCombatBody )
        //                .def( "setCombatBody", &CombatBodyContainer::setCombatBody )];
    }

}  // end namespace fb
