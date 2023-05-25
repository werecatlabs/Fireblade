#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/EntityBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <FBVehicle/FBVehicle.h>
#include "FBLua/LuaManager.h"
#include "FBLua/LuaObjectData.h"
#include "FBLuabind/FBLuaScriptError.h"
#include "FBLuabind/ScriptObjectFunctions.h"
#include "FBLuaBind/Helpers/Projectile3Helper.h"
#include "FBLuabind/Wrappers/StandardGameObjectWrapper.h"
#include "FBLuabind/Wrappers/GameObjectWrapper.h"

namespace fb
{
    using namespace scene;

    //
    // Entity builder functions
    //

    void _removeEntity( ISceneManager *entityManager, scene::IActor *entity )
    {
        //entityManager->removeEntity(entity);
    }

    scene::IActor *findEntity( ISceneManager *entityManager, const char *name )
    {
        auto entity = entityManager->getActorByName( name );
        if( entity )
        {
            return entity.get();
        }

        return nullptr;
    }

    IActor *findEntityById( ISceneManager *entityManager, lua_Integer id )
    {
        //auto hash = static_cast<hash_type>(id);
        //auto entity = entityManager->getActor( hash );
        //if( entity )
        //{
        //    return entity.get();
        //}

        return nullptr;
    }

    ISceneManager *getEntityManager()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto entityManager = applicationManager->getSceneManager();
        return entityManager.get();
    }

    boost::shared_ptr<IComponent> getComponentHash( SmartPtr<IActor> entity, lua_Integer hash )
    {
        //hash32 id = *reinterpret_cast<hash32*>(&hash);
        //boost::shared_ptr<IComponentContainer>& components = entity->getComponents();
        //if(components)
        //{
        //	boost::shared_ptr<IComponent> component = components->getComponentById(id);
        //	if(component)
        //	{
        //		return component;
        //	}
        //}

        //String msg = String("Could find component: ") + StringUtil::toString(id) + String(" ");
        //msg += Engine::getSingletonPtr()->getScriptManager()->getDebugInfo();
        //FB_LOG_MESSAGE("LuaScriptMgr", msg.c_str());

        //return boost::shared_ptr<IComponent>::NULL_PTR;

        return nullptr;
    }

    void _setComponent( IActor *entity, boost::shared_ptr<IComponent> component )
    {
        //entity->addComponent( component );
    }

    boost::shared_ptr<IComponent> getComponent( SmartPtr<IActor> entity, const char *type )
    {
        auto hash = StringUtil::getHash( type );
        lua_Integer id = *reinterpret_cast<lua_Integer *>( &hash );
        return getComponentHash( entity, id );
    }

    IScriptObject *getEntityObject( IActor *entity, const char *objectName )
    {
        auto hash = StringUtil::getHash( objectName );

        //SmartPtr<IScriptObject> object;
        //entity->getObject( hash, object );
        //return object.get();

        return nullptr;
    }

    //
    //FSMPtr getEntityFSM( IEntity *entity, const char *fsmName )
    //{
    //    auto hash = StringUtil::getHash( fsmName );
    //    FSMContainerPtr fsms = entity->getFSMs();
    //    if( fsms )
    //    {
    //        FSMPtr fsm = fsms->getFSM( hash );
    //        return fsm;
    //    }

    //    return nullptr;
    //}

    //
    //FSMPtr getEntityFSMByIndex( IEntity *entity, lua_Integer hash )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    return entity->getFSMs()->getFSM( id );
    //}

    //
    //FSMContainerPtr _getFSMContainer( IEntity *entity )
    //{
    //    return entity->getFSMs();
    //}

    //void _addMessageToQueue(EntityMessageDispatcher* entityMsgDispatch, IScriptObject* message)
    //{
    //	entityMsgDispatch->queueMessage(message);
    //}

    //
    //void _addMessageToQueuePtr(EntityMessageDispatcher* entityMsgDispatch, MessagePtr message)
    //{
    //	entityMsgDispatch->queueMessage(message);
    //}

    /*void _addPool(EntityPoolManager* poolMgr, const char* typeName, BuilderPtr const& builder, lua_Integer numElements)
    {
    hash32 hash = StringUtil::getHashMakeLower(typeName);
    poolMgr->addPool(hash, builder, numElements);
    }
    */

    //
    //MessagePtr _createEntityMessage()
    //{
    //    //return MessagePtr(new EntityMessage, true);
    //    return nullptr;
    //}

    //
    //MessagePtr _createMapEntityMessage()
    //{
    //    //return MessagePtr(new EntityMessageStandard, true);
    //    return nullptr;
    //}

    //SmartPtr<scene::IActor> _getSender(EntityMessage* entityMessage)
    //{
    //	EntityManagerPtr& entityManager = Engine::getSingletonPtr()->getEntityManager();
    //	return entityManager->findEntityById(entityMessage->getSenderId());
    //}

    Array<SmartPtr<scene::IActor>> _getEntities( ISceneManager *mgr )
    {
        Array<SmartPtr<scene::IActor>> entities;
        //mgr->getEntities( entities );
        return entities;
    }

    Array<SmartPtr<scene::IActor>> _getEntitiesSphere( ISceneManager *mgr, Sphere3F &sphere )
    {
        Array<SmartPtr<scene::IActor>> entities;
        return entities;
    }

    Array<SmartPtr<scene::IActor>> _getEntitiesByType( ISceneManager *mgr, lua_Integer type )
    {
        Array<SmartPtr<scene::IActor>> entities;
        //mgr->getEntities( type, entities );
        return entities;
    }

    SmartPtr<scene::IActor> _getFromEntityArray( const Array<SmartPtr<scene::IActor>> &entities,
                                                 lua_Integer idx )
    {
        return entities[idx];
    }

    //lua_Integer EntityMessage_getRecieverId(EntityMessage* msg)
    //{
    //	hash32 hash = msg->getRecieverId();
    //	return *reinterpret_cast<lua_Integer*>(&hash);
    //}

    //void EntityMessage_setRecieverId(EntityMessage* msg, lua_Integer id)
    //{
    //	hash32 hash = *reinterpret_cast<hash32*>(&id);
    //	msg->setRecieverId(hash);
    //}

    //lua_Integer EntityMessage_getSenderId(EntityMessage* msg)
    //{
    //	hash32 hash = msg->getSenderId();
    //	return *reinterpret_cast<lua_Integer*>(&hash);
    //}

    //void EntityMessage_setSenderId(EntityMessage* msg, lua_Integer id)
    //{
    //	hash32 hash = *reinterpret_cast<hash32*>(&id);
    //	msg->setSenderId(hash);
    //}

    lua_Integer IEntity_getEntityId( IActor *entity )
    {
        hash32 hash = entity->getHandle()->getId();
        return *reinterpret_cast<lua_Integer *>( &hash );
    }

    void IEntity_setEntityId( IActor *entity, lua_Integer id )
    {
        hash32 hash = *reinterpret_cast<hash32 *>( &id );
        entity->getHandle()->setId( hash );
    }

    lua_Integer IEntity_getEntityTypeId( IActor *entity )
    {
        return 0;
    }

    void IEntity_setEntityTypeId( IActor *entity, lua_Integer id )
    {
        //entity->setType( hash );
    }

    lua_Integer IEntity_getFactoryType( IActor *entity )
    {
        return 0;
    }

    void IEntity_setFactoryType( IActor *entity, lua_Integer type )
    {
        //entity->setFactoryType( hash );
    }

    SmartPtr<scene::IActor> _findEntityById( ISceneManager *mgr, lua_Integer id )
    {
        hash32 hash = *reinterpret_cast<hash32 *>( &id );
        return mgr->getActor( hash );
    }

    //boost::shared_ptr<IComponentContainer> _getComponents( IEntity *entity )
    //{
    //    return entity->getComponents();
    //}

    void _addEntity( ISceneManager *entityMgr, SmartPtr<ISharedObject> scriptObject )
    {
        //entityMgr->addEntity(scriptObject);
    }

    lua_Integer IEntity_getGameWorldMask( IActor *entity )
    {
        //u32 mask = entity->getGameWorldMask();
        //lua_Integer iMask = *reinterpret_cast<lua_Integer *>( &mask );
        //return iMask;

        return 0;
    }

    void IEntity_setGameWorldMask( IActor *entity, lua_Integer mask )
    {
        //u32 uiMask = *reinterpret_cast<u32 *>( &mask );
        //entity->setGameWorldMask( uiMask );
    }

    luabind::object addComponent( IActor *entity, const String &className )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto scriptManager =
            fb::static_pointer_cast<LuaManager>( applicationManager->getScriptManager() );
        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto classNames = scriptManager->getClassNames();

        if( std::find( classNames.begin(), classNames.end(), className ) != classNames.end() )
        {
            auto component = factoryManager->make_ptr<UserComponent>();
            component->setClassName( className );
            entity->addComponentInstance( component );
            component->load( nullptr );
            //return component;

            auto invoker = component->getInvoker();
            auto scriptObject = fb::static_pointer_cast<LuaObjectData>( component->getScriptData() );
            return scriptObject->getObject();

            /*
            auto luaState = scriptManager->getLuaState();
            auto globalObject = luabind::globals( luaState );
            auto classObject = globalObject[className.c_str()];
            if( classObject )
            {
                luabind::object newObject = classObject();

                auto p = luabind::object_cast<SmartPtr<scene::UserComponent>>( newObject );

                auto ptr = luabind::object_cast<scene::UserComponent*>( newObject );
                if( ptr != nullptr )
                {
                    // Use the MyClass instance
                    // ...
                }

                return newObject;
            }
            */
        }
        else
        {
            auto factory = factoryManager->getFactoryByName( className );
            if( factory )
            {
                auto obj = factory->createInstance();
                auto component = fb::static_pointer_cast<scene::IComponent>( obj );
                entity->addComponentInstance( component );
                component->load( nullptr );
                //return component;
            }
        }

        //hash32 hash = StringUtil::getHash( typeName );
        //return entity->addComponent( hash );
        return luabind::object();
    }

    void bindEntity( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IEditor, ISharedObject>( "IEditor" )
                        .def( "getParent", &IEditor::getParent )
                        .def( "setParent", &IEditor::setParent )
                        .def( "getParentWindow", &IEditor::getParentWindow )
                        .def( "setParentWindow", &IEditor::setParentWindow )
                        .def( "isWindowVisible", &IEditor::isWindowVisible )
                        .def( "setWindowVisible", &IEditor::setWindowVisible )
                        .def( "updateSelection", &IEditor::updateSelection )
                        .def( "getEventListener", &IEditor::getEventListener )
                        .def( "setEventListener", &IEditor::setEventListener )
                        .def( "getWindowDragSource", &IEditor::getWindowDragSource )
                        .def( "setWindowDragSource", &IEditor::setWindowDragSource )
                        .def( "getWindowDropTarget", &IEditor::getWindowDropTarget )
                        .def( "setWindowDropTarget", &IEditor::setWindowDropTarget )
                        .def( "setDraggable", &IEditor::setDraggable )
                        .def( "isDraggable", &IEditor::isDraggable )
                        .def( "setDroppable", &IEditor::setDroppable )
                        .def( "isDroppable", &IEditor::isDroppable )
                        .def( "setHandleEvents", &IEditor::setHandleEvents )
                        .def( "getHandleEvents", &IEditor::getHandleEvents )
                        .def( "handleEvent", &IEditor::handleEvent )];

        module( L )[class_<IDirector, IResource>( "IDirector" )
                        .def( "getProperties", &IDirector::getProperties )
                        .def( "setProperties", &IDirector::setProperties )
                        .def( "getParent", &IDirector::getParent )
                        .def( "setParent", &IDirector::setParent )
                        .def( "addChild", &IDirector::addChild )
                        .def( "removeChild", &IDirector::removeChild )
                        .def( "removeChildren", &IDirector::removeChildren )
                        .def( "findChild", &IDirector::findChild )
                        .def( "getChildren", &IDirector::getChildren )
                        .def( "getChildrenPtr", &IDirector::getChildrenPtr )];

        module(
            L )[class_<IActor::State>( "ActorState" )
                    .enum_( "constants" )[luabind::value( "Create", (u32)IActor::State::Create ),
                                          luabind::value( "Destroyed", (u32)IActor::State::Destroyed ),
                                          luabind::value( "Edit", (u32)IActor::State::Edit ),
                                          luabind::value( "Play", (u32)IActor::State::Play ),
                                          luabind::value( "Count", (u32)IActor::State::Count )]];

        module(
            L )[class_<IActor, IResource, SmartPtr<ISharedObject>>( "IActor" )
                    .def( "updateDirty", &IActor::updateDirty )
                    .def( "getLocalTransform", &IActor::getLocalTransform )
                    .def( "getWorldTransform", &IActor::getWorldTransform )
                    .def( "getLocalPosition", &IActor::getLocalPosition )
                    .def( "setLocalPosition", &IActor::setLocalPosition )
                    .def( "getLocalScale", &IActor::getLocalScale )
                    .def( "setLocalScale", &IActor::setLocalScale )
                    .def( "getLocalOrientation", &IActor::getLocalOrientation )
                    .def( "setLocalOrientation", &IActor::setLocalOrientation )
                    .def( "getPosition", &IActor::getPosition )
                    .def( "lookAt", ( void( IActor::* )( const Vector3<real_Num> & ) ) & IActor::lookAt )
                    .def( "lookAt",
                          ( void( IActor::* )( const Vector3<real_Num> &, const Vector3<real_Num> & ) ) &
                              IActor::lookAt )
                    .def( "setPosition", &IActor::setPosition )
                    .def( "getScale", &IActor::getScale )
                    .def( "setScale", &IActor::setScale )
                    .def( "getOrientation", &IActor::getOrientation )
                    .def( "setOrientation", &IActor::setOrientation )
                    .def( "awake", &IActor::awake )
                    .def( "play", &IActor::play )
                    .def( "edit", &IActor::edit )
                    .def( "levelWasLoaded", &IActor::levelWasLoaded )
                    .def( "reset", &IActor::reset )
                    .def( "hierarchyChanged", &IActor::hierarchyChanged )
                    .def( "childAdded", &IActor::childAdded )
                    .def( "childRemoved", &IActor::childRemoved )
                    .def( "childAddedInHierarchy", &IActor::childAddedInHierarchy )
                    .def( "childRemovedInHierarchy", &IActor::childRemovedInHierarchy )
                    .def( "getName", &IActor::getName )
                    .def( "setName", &IActor::setName )
                    .def( "getPerpetual", &IActor::getPerpetual )
                    .def( "setPerpetual", &IActor::setPerpetual )
                    .def( "addComponent", addComponent )
                    .def( "addComponentInstance", &IActor::addComponentInstance )
                    .def( "removeComponentInstance", &IActor::removeComponentInstance )                    
                    .def( "getComponents", &IActor::getComponents )
                    .def( "getComponentsPtr", &IActor::getComponentsPtr )
                    .def( "getChildByIndex", &IActor::getChildByIndex )
                    .def( "getNumChildren", &IActor::getNumChildren )
                    .def( "getSiblingIndex", &IActor::getSiblingIndex )
                    .def( "addDirtyComponent", &IActor::addDirtyComponent )
                    .def( "isComponentDirty", &IActor::isComponentDirty )
                    .def( "registerComponentUpdate", &IActor::registerComponentUpdate )
                    .def( "unregisterComponentUpdate", &IActor::unregisterComponentUpdate )

                    .def( "unregisterAllComponent", &IActor::unregisterAllComponent )
                    .def( "triggerEnter", &IActor::triggerEnter )
                    .def( "triggerLeave", &IActor::triggerLeave )
                    .def( "componentLoaded", &IActor::componentLoaded )
                    .def( "getParent", &IActor::getParent )
                    .def( "setParent", &IActor::setParent )
                    .def( "addChild", &IActor::addChild )
                    .def( "removeChild", &IActor::removeChild )
                    .def( "removeChildren", &IActor::removeChildren )
                    .def( "findChild", &IActor::findChild )
                    .def( "getChildrenPtr", &IActor::getChildrenPtr )
                    .def( "getAllChildren", &IActor::getAllChildren )
                    .def( "isMine", &IActor::isMine )
                    .def( "setMine", &IActor::setMine )
                    .def( "isStatic", &IActor::isStatic )
                    .def( "setStatic", &IActor::setStatic )
                    .def( "isEnabledInScene", &IActor::isEnabledInScene )
                    .def( "isEnabled", &IActor::isEnabled )
                    .def( "setEnabled", &IActor::setEnabled )
                    .def( "isVisible", &IActor::isVisible )
                    .def( "setVisible", &IActor::setVisible )
                    .def( "isDirty", &IActor::isDirty )
                    .def( "setDirty", &IActor::setDirty )
                    .def( "updateTransform", &IActor::updateTransform )
                    .def( "getSceneRoot", &IActor::getSceneRoot )
                    .def( "getSceneLevel", &IActor::getSceneLevel )
                    .def( "getTransform", &IActor::getTransform )
                    .def( "setTransform", &IActor::setTransform )
                    .def( "getStateObject", &IActor::getStateObject )
                    .def( "setStateObject", &IActor::setStateObject )
                    //.def( "setState", &IActor::setState )
                    //.def( "getState", &IActor::getState )
                    //.def( "setGameState", &IActor::setGameState )
                    //.def( "getGameState", &IActor::getGameState )
                    .def( "getFlags", &IActor::getFlags )
                    .def( "setFlags", &IActor::setFlags )
                    .def( "getFlag", &IActor::getFlag )
                    .def( "setFlag", &IActor::setFlag )
                    .def( "getNewFlags", &IActor::getNewFlags )
                    .def( "setNewFlags", &IActor::setNewFlags )
                    .def( "getNewFlag", &IActor::getNewFlag )
                    .def( "setNewFlag", &IActor::setNewFlag )
                    .def( "getScene", &IActor::getScene )
                    .def( "setScene", &IActor::setScene )
                    .def( "updateVisibility", &IActor::updateVisibility )
                    .scope[def( "typeInfo", IActor::typeInfo )]];
    }

    void bindEntityMgr( lua_State *L )
    {
        using namespace luabind;

        // Assuming luabind has been included and the appropriate namespaces have been used

        // Define the binding code for the IScene class
        luabind::module( L )[
            //// Define the State enum
            //luabind::enum_( "State" )[luabind::value( "None", IScene::State::None ),
            //                          luabind::value( "Edit", IScene::State::Edit ),
            //                          luabind::value( "Play", IScene::State::Play ),
            //                          luabind::value( "Reset", IScene::State::Reset ),
            //                          luabind::value( "Count", IScene::State::Count )],

            //// Define the SceneLoadingState enum
            //luabind::enum_(
            //    "SceneLoadingState" )[luabind::value( "Loaded", IScene::SceneLoadingState::Loaded ),
            //                          luabind::value( "Unloaded", IScene::SceneLoadingState::Unloaded ),
            //                          luabind::value( "Count", IScene::SceneLoadingState::Count )],

            // Define the IScene class
            luabind::class_<IScene, ISharedObject>( "IScene" )
                .def( "loadScene", &IScene::loadScene )
                .def( "saveScene", ( void( IScene::* )( const String & ) ) & IScene::saveScene )
                .def( "saveScene", ( void( IScene::* )() ) & IScene::saveScene )
                .def( "clear", &IScene::clear )
                .def( "getName", &IScene::getName )
                .def( "setName", &IScene::setName )
                .def( "addActor", &IScene::addActor )
                .def( "removeActor", &IScene::removeActor )
                .def( "removeAllActors", &IScene::removeAllActors )
                .def( "findActorById", &IScene::findActorById )
                .def( "getActors", &IScene::getActors )
                .def( "registerUpdates", &IScene::registerUpdates )
                .def( "registerUpdate", &IScene::registerUpdate )
                .def( "registerAllUpdates", &IScene::registerAllUpdates )
                .def( "unregisterUpdate", &IScene::unregisterUpdate )
                .def( "unregisterAll", &IScene::unregisterAll )
                .def( "getFilePath", &IScene::getFilePath )
                .def( "setFilePath", &IScene::setFilePath )
                //.def( "setState", &IScene::setState )
                //.def( "getState", &IScene::getState )
                //.def( "setSceneLoadingState", &IScene::setSceneLoadingState )
                //.def( "getSceneLoadingState", &IScene::getSceneLoadingState )
                .def( "play", &IScene::play )
                .def( "edit", &IScene::edit )
                .def( "stop", &IScene::stop )];

        module( L )[class_<ISceneManager, ISharedObject, SmartPtr<ISceneManager>>( "ISceneManager" )
                        .def( "loadScene", &ISceneManager::loadScene )
                        .def( "getFsmManager", &ISceneManager::getFsmManager )
                        .def( "setFsmManager", &ISceneManager::setFsmManager )
                        .def( "getCurrentScene", &ISceneManager::getCurrentScene )
                        .def( "setCurrentScene", &ISceneManager::setCurrentScene )
                        .def( "getActors", &ISceneManager::getActors )
                        .def( "createActor", &ISceneManager::createActor )
                        .def( "destroyActor", &ISceneManager::destroyActor )
                        .def( "destroyActors", &ISceneManager::destroyActors )
                        .def( "play", &ISceneManager::play )
                        .def( "edit", &ISceneManager::edit )
                        .def( "stop", &ISceneManager::stop )
                        .def( "addComponent", &ISceneManager::addComponent )
                        .def( "removeComponent", &ISceneManager::removeComponent )
                        //.def( "getComponents", &ISceneManager::getComponents )
                        .def( "getActor", &ISceneManager::getActor )
                        .def( "getActorByName", &ISceneManager::getActorByName )
                        .def( "getComponent", &ISceneManager::getComponent )
                        .def( "queueProperties", &ISceneManager::queueProperties )
                        //.def( "getStateTask", &ISceneManager::getStateTask )
                        //.def( "getSceneTask", &ISceneManager::getSceneTask )
                        .def( "getComponents",
                              ( Array<SmartPtr<IComponent>>( ISceneManager::* )( u32 ) const ) &
                                  ISceneManager::getComponents )];
    }

}  // end namespace fb
