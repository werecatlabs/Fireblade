#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/SystemBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include <boost/core/noncopyable.hpp>
#include "FBLuabind/ScriptObjectFunctions.h"
#include "FBLuabind/Helpers/EngineHelper.h"
#include "FBLuabind/Helpers/FileSystemHelper.h"

namespace fb
{

    void _LuaLogMessage( const char *log, const char *msg )
    {
        FB_LOG_MESSAGE( log, msg );
    }

    void _LuaLogMessage_stringc( const char *log, const String &msg )
    {
        FB_LOG_MESSAGE( log, msg.c_str() );
    }

    //
    //StateListenerPtr _createStateListener( IScriptObject *object )
    //{
    //    //return StateListenerPtr(new StateListenerScripted(object), true);
    //    return nullptr;
    //}

    //
    //StateListenerPtr _createStateListenerEntity( SmartPtr<scene::IActor> entity )
    //{
    //    //return StateListenerPtr(new StateListenerScripted(entity.get()), true);
    //    return nullptr;
    //}

    //
    //ScriptEventPtr _createEvent()
    //{
    //    return nullptr;  // ScriptEventPtr(new ScriptEvent, true);
    //}

    //
    //void _setEvent( StateListenerPtr listener, const char *eventName, ScriptEventPtr event )
    //{
    //    //StateListenerScriptedPtr scriptListener = listener;
    //    //auto hash = StringUtil::getHash(eventName);
    //    //scriptListener->getInvoker()->setEventFunction(hash, event);
    //}

    String _getStateMessageProperty( StateMessageDefault *msg, const char *propertyName )
    {
        auto hash = StringUtil::getHash( propertyName );

        String str;
        msg->getProperty( hash, str );
        return str;
    }

    void _setStateMessageProperty( StateMessageDefault *msg, const char *propertyName,
                                   const char *propertyValue )
    {
        auto hash = StringUtil::getHash( propertyName );
        msg->setProperty( hash, propertyValue );
    }

    String _getStateMessagePropertyHash( StateMessageDefault *msg, lua_Integer propertyHash )
    {
        String str;
        msg->getProperty( propertyHash, str );
        return str;
    }

    void _setStateMessagePropertyHash( StateMessageDefault *msg, lua_Integer propertyHash,
                                       String propertyValue )
    {
        msg->setProperty( propertyHash, propertyValue );
    }

    //
    //void _removeStateObject( IStateManager *mgr, StateObjectPtr subject )
    //{
    //    mgr->removeStateObject( subject );
    //}

    //
    //void _addStateListener( IStateObject *stateContext, StateListenerPtr stateListner )
    //{
    //    stateContext->addStateListener( stateListner );
    //}

    //
    //void _addStateListenerIsOwner( IStateObject *stateContext, StateListenerPtr stateListner,
    //                               bool isOwner )
    //{
    //    stateContext->addStateListener( stateListner, isOwner );
    //}

    template <class T, class B>
    SmartPtr<T> type_cast( SmartPtr<B> ptr )
    {
        return fb::dynamic_pointer_cast<T>( ptr );
    }

    template <class B>
    void type_cast_object( const String &type, SmartPtr<B> ptr )
    {
        //auto engine = core::ApplicationManager::instance();
        //auto scriptMgr = fb::static_pointer_cast<LuaManager>( engine->getScriptManager() );
        //lua_State* luaState = scriptMgr->getLuaState();

        //if(type == "IBoxShape2")
        //{
        //    FB_ASSERT(dynamic_cast<IBoxShape2*>(ptr.get()));
        //    luabind::detail::convert_to_lua(luaState, static_cast<IBoxShape2*>(ptr.get()));
        //}
    }

    lua_Integer _IStateMessage_getType( IStateMessage *message )
    {
        auto hash = message->getType();
        return *reinterpret_cast<lua_Integer *>( &hash );
    }

    void _IStateMessage_setType( IStateMessage *message, lua_Integer type )
    {
        auto hash = *reinterpret_cast<lua_Integer *>( &type );
        message->setType( hash );
    }

    //String _getClassName( ScriptEvent *event )
    //{
    //    return event->getClassName();
    //}

    //String _getFunction( ScriptEvent *event )
    //{
    //    return event->getFunction();
    //}

    //void _setFunction( ScriptEvent *event, const char *functionName )
    //{
    //    event->setFunction( functionName );
    //}

    //FSMPtr _getFSM( core::ApplicationManager *engine )
    //{
    //    return engine->getFSM();
    //}

    void bindSystem( lua_State *L )
    {
        using namespace core;
        using namespace editor;
        using namespace luabind;

        module( L )[class_<FileInfo>( "FileInfo" )
                        .def_readwrite( "filePath", &FileInfo::filePath )
                        .def_readwrite( "filePathLowerCase", &FileInfo::filePathLowerCase )
                        .def_readwrite( "path", &FileInfo::path )
                        .def_readwrite( "absolutePath", &FileInfo::absolutePath )
                        .def_readwrite( "fileName", &FileInfo::fileName )
                        .def_readwrite( "fileNameLowerCase", &FileInfo::fileNameLowerCase )
                        .def_readwrite( "compressedSize", &FileInfo::compressedSize )
                        .def_readwrite( "uncompressedSize", &FileInfo::uncompressedSize )
                        .def_readwrite( "fileId", &FileInfo::fileId )
                        .def_readwrite( "archiveId", &FileInfo::archiveId )
                        .def_readwrite( "offset", &FileInfo::offset )
                        .def_readwrite( "isDirectory", &FileInfo::isDirectory )];

        module( L )[class_<FileSelection, ISharedObject>( "FileSelection" )
                        .def( "getFilePath", &FileSelection::getFilePath )
                        .def( "setFilePath", &FileSelection::setFilePath )
                        .def( "getFileInfo", &FileSelection::getFileInfo )
                        .def( "setFileInfo", &FileSelection::setFileInfo )
                        .scope[def( "typeInfo", FileSelection::typeInfo )]];

        module( L )[class_<core::IPrototype, ISharedObject, SmartPtr<core::IPrototype>>( "IPrototype" )
                        .def( "getParentPrototype", &IPrototype::getParentPrototype )
                        .def( "setParentPrototype", &IPrototype::setParentPrototype )
                        .def( "getProperties", &IPrototype::getProperties )
                        .def( "setProperties", &IPrototype::setProperties )
                        .scope[def( "typeInfo", IPrototype::typeInfo )]];

        module( L )[class_<IResource, core::IPrototype, SmartPtr<IResource>>( "IResource" )
                        .def( "saveToFile", &IResource::saveToFile )
                        .def( "loadFromFile", &IResource::loadFromFile )
                        .def( "save", &IResource::save )
                        .def( "import", &IResource::import )
                        .def( "reimport", &IResource::reimport )
                        .def( "getFileSystemId", &IResource::getFileSystemId )
                        .def( "setFileSystemId", &IResource::setFileSystemId )
                        .def( "getFilePath", &IResource::getFilePath )
                        .def( "setFilePath", &IResource::setFilePath )
                        .def( "getSettingsFileSystemId", &IResource::getSettingsFileSystemId )
                        .def( "setSettingsFileSystemId", &IResource::setSettingsFileSystemId )
                        .def( "getStateContext", &IResource::getStateContext )
                        .def( "setStateContext", &IResource::setStateContext )
                        //.def( "getResourceType", &IResource::getResourceType )
                        //.def( "setResourceType", &IResource::setResourceType )
                        .def( "getResourceTypeByName", &IResource::getResourceTypeByName )
                        .def( "setResourceTypeByName", &IResource::setResourceTypeByName )
                        .scope[def( "typeInfo", IResource::typeInfo )]];

        module(
            L )[class_<IResourceDatabase, ISharedObject, SmartPtr<IResourceDatabase>>(
                    "IResourceDatabase" )
                    .def( "importAssets", &IResourceDatabase::importAssets )
                    .def( "reimportAssets", &IResourceDatabase::reimportAssets )
                    .def( "build", &IResourceDatabase::build )
                    .def( "hasResource", &IResourceDatabase::hasResource )
                    .def( "addResource", &IResourceDatabase::addResource )
                    .def( "removeResource", &IResourceDatabase::removeResource )
                    .def( "findResource", &IResourceDatabase::findResource )
                    .def( "cloneResource", &IResourceDatabase::cloneResource )
                    .def( "getResourceData", &IResourceDatabase::getResourceData )
                    .def( "getResources", &IResourceDatabase::getResources )
                    .def( "importFile", &IResourceDatabase::importFile )
                    .def( "loadResource", ( SmartPtr<IResource>( IResourceDatabase::* )( hash64 ) ) &
                                              IResourceDatabase::loadResource )
                    .def( "loadResource",
                          ( SmartPtr<IResource>( IResourceDatabase::* )( const String & ) ) &
                              IResourceDatabase::loadResource )
                    .def( "loadResourceById", &IResourceDatabase::loadResourceById )
                    .def( "getDatabaseManager", &IResourceDatabase::getDatabaseManager )
                    .def( "setDatabaseManager", &IResourceDatabase::setDatabaseManager )
                    .def( "createOrRetrieve", ( Pair<SmartPtr<IResource>, bool>( IResourceDatabase::* )(
                                                  hash_type, const String & ) ) &
                                                  IResourceDatabase::createOrRetrieve )
                    .def( "createOrRetrieve",
                          ( Pair<SmartPtr<IResource>, bool>( IResourceDatabase::* )( const String & ) ) &
                              IResourceDatabase::createOrRetrieve )
                    .def( "getObject", &IResourceDatabase::getObject )
                    .scope[def( "typeInfo", IResourceDatabase::typeInfo )]];

        module( L )[class_<IEventListener, ISharedObject>( "IEventListener" )
                        .def( "handleEvent", &IEventListener::handleEvent )];

        module( L )[class_<TypeManager>( "TypeManager" )
                        .def( "getName", &TypeManager::getName )
                        .def( "getHash", &TypeManager::getHash )
                        .def( "getBaseType", &TypeManager::getBaseType )
                        .def( "isExactly", &TypeManager::isExactly )
                        .def( "isDerived", &TypeManager::isDerived )
                        .def( "getClassHierarchy", &TypeManager::getClassHierarchy )
                        .def( "getTypeIndex", &TypeManager::getTypeIndex )
                        .def( "getNumInstances", &TypeManager::getNumInstances )
                        .def( "getTotalNumTypes", &TypeManager::getTotalNumTypes )
                        .def( "getDataType", &TypeManager::getDataType )
                        .def( "setDataType", &TypeManager::setDataType )
                        .def( "getBaseTypes", &TypeManager::getBaseTypes )
                        .def( "getBaseTypeNames", &TypeManager::getBaseTypeNames )];

        module( L )[class_<IFileSystem, ISharedObject, SmartPtr<IFileSystem>>( "FileSystem" )
                        .def( "getFileNamesInFolder", FileSystemHelper::_getFileNamesInFolder )
                        .def( "getFileNamesWithExtension", FileSystemHelper::_getFileNamesWithExt )
                        .def( "addFolder", &IFileSystem::addFolder )];

        module( L )[class_<IStateMessage, ISharedObject, SmartPtr<IStateMessage>>( "StateMessage" )
                        .def( "getType", _IStateMessage_getType )
                        .def( "setType", _IStateMessage_setType )];

        module(
            L )[class_<IEvent, ISharedObject, SmartPtr<IEvent>>( "IEvent" )
                    .enum_( "Type" )
                        [value( "Loading", IEvent::Type::Loading ),
                         value( "Object", IEvent::Type::Object ), value( "Input", IEvent::Type::Input ),
                         value( "UI", IEvent::Type::UI ), value( "Window", IEvent::Type::Window ),
                         value( "Scene", IEvent::Type::Scene ), value( "Count", IEvent::Type::Count )]
                    .enum_( "Events" )
                        [value( "loadingStateChanged", IEvent::loadingStateChanged ),
                         value( "loadScene", IEvent::loadScene ),
                         value( "handlePropertyButtonClick", IEvent::handlePropertyButtonClick ),
                         value( "handleTreeSelectionActivated", IEvent::handleTreeSelectionActivated ),
                         value( "handleTreeNodeDoubleClicked", IEvent::handleTreeNodeDoubleClicked ),
                         value( "handlePropertyChanged", IEvent::handlePropertyChanged ),
                         value( "handleValueChanged", IEvent::handleValueChanged ),
                         value( "handleMouseClicked", IEvent::handleMouseClicked ),
                         value( "handleSelection", IEvent::handleSelection ),
                         value( "handleDrag", IEvent::handleDrag ),
                         value( "handleDrop", IEvent::handleDrop ),
                         value( "inputEvent", IEvent::inputEvent ),
                         value( "updateEvent", IEvent::updateEvent ),
                         value( "handleEnterFrame", IEvent::handleEnterFrame ),
                         value( "ACTIVATE_HASH", IEvent::ACTIVATE_HASH ),
                         value( "DEACTIVATE_HASH", IEvent::DEACTIVATE_HASH ),
                         value( "UPDATE_HASH", IEvent::UPDATE_HASH ),
                         value( "HANDLE_MESSAGE_HASH", IEvent::HANDLE_MESSAGE_HASH ),
                         value( "INITIALISE_START_HASH", IEvent::INITIALISE_START_HASH ),
                         value( "INITIALISE_END_HASH", IEvent::INITIALISE_END_HASH ),
                         value( "ADD_CHILD_HASH", IEvent::ADD_CHILD_HASH ),
                         value( "REMOVE_CHILD_HASH", IEvent::REMOVE_CHILD_HASH ),
                         value( "CHANGED_STATE_HASH", IEvent::CHANGED_STATE_HASH ),
                         value( "CHILD_CHANGED_STATE_HASH", IEvent::CHILD_CHANGED_STATE_HASH ),
                         value( "TOGGLE_ENABLED_HASH", IEvent::TOGGLE_ENABLED_HASH ),
                         value( "TOGGLE_HIGHLIGHT_HASH", IEvent::TOGGLE_HIGHLIGHT_HASH ),
                         value( "VISIBLE_HASH", IEvent::VISIBLE_HASH ),
                         value( "SHOW_HASH", IEvent::SHOW_HASH ),
                         value( "HIDE_HASH", IEvent::HIDE_HASH ),
                         value( "SELECT_HASH", IEvent::SELECT_HASH ),
                         value( "DESELECT_HASH", IEvent::DESELECT_HASH ),
                         value( "GAIN_FOCUS_HASH", IEvent::GAIN_FOCUS_HASH ),
                         value( "LOST_FOCUS_HASH", IEvent::LOST_FOCUS_HASH )]];

        //module( L )[class_<IScriptEvent, IEvent, EventPtr>( "IScriptEvent" )
        //                .def( "setClassName", &IScriptEvent::setClassName )
        //                .def( "setFunction", &IScriptEvent::setFunction )];

        //module( L )[class_<ScriptEvent, IScriptEvent, boost::shared_ptr<IObject>>( "ScriptEvent" )
        //                .def( "getClassName", _getClassName )
        //                .def( "getFunction", _getFunction )];

        //module( L )[class_<IStateListener, IScriptObject, StateListenerPtr>( "StateListener" )
        //                .def( "setEvent", _setEvent )];

        //module( L )[class_<StateMessageDefault, IStateMessage>( "StateMessageDefault" )
        //                .def( "getProperty", _getStateMessageProperty )
        //                .def( "setProperty", _setStateMessageProperty )
        //                .def( "getProperty", _getStateMessagePropertyHash )
        //                .def( "setProperty", _setStateMessagePropertyHash )];

        module( L )[class_<ISelectionManager, ISharedObject>( "ISelectionManager" )
                        .def( "addSelectedObject", &ISelectionManager::addSelectedObject )
                        .def( "removeSelectedObject", &ISelectionManager::removeSelectedObject )
                        .def( "clearSelection", &ISelectionManager::clearSelection )
                        .def( "getSelection", &ISelectionManager::getSelection )];

        module( L )[class_<IStateContext, ISharedObject, SmartPtr<IStateContext>>( "IStateContext" )];

        module( L )[class_<IStateManager, IScriptObject, SmartPtr<IStateManager>>( "StateManager" )
                        //.def( "addStateObject", &IStateManager::addStateObject )
                        //.def( "removeStateObject", _removeStateObject )
                        .def( "findStateObject", &IStateManager::findStateObject )];

        module( L )
            [class_<ITimer, ISharedObject, SmartPtr<IObject>>( "ITimer" )
                 .def( "updateFixed", &ITimer::updateFixed )
                 .def( "getTimeSinceLevelLoad", &ITimer::getTimeSinceLevelLoad )
                 .def( "setTimeSinceLevelLoad", &ITimer::setTimeSinceLevelLoad )
                 .def( "addTimeSinceLevelLoad", &ITimer::addTimeSinceLevelLoad )
                 .def( "getSmoothTime", &ITimer::getSmoothTime )
                 //.def("getSmoothDeltaTime", &ITimer::getSmoothDeltaTime)
                 .def( "setSmoothDeltaTime", &ITimer::setSmoothDeltaTime )
                 .def( "getTimeMilliseconds", &ITimer::getTimeMilliseconds )
                 .def( "getRealTime", &ITimer::getRealTime )
                 .def( "getTimeIntervalMilliseconds", &ITimer::getTimeIntervalMilliseconds )
                 .def( "now", &ITimer::now )
                 .def( "getTime", ( time_interval( ITimer::* )( void ) const ) & ITimer::getTime )
                 .def( "getDeltaTime",
                       ( time_interval( ITimer::* )( void ) const ) & ITimer::getDeltaTime )
                 .def( "setFrameSmoothingPeriod", &ITimer::setFrameSmoothingPeriod )
                 .def( "getFrameSmoothingPeriod", &ITimer::getFrameSmoothingPeriod )
                 .def( "resetSmoothing", &ITimer::resetSmoothing )
                 .def( "getTickCount", ( u32( ITimer::* )() ) & ITimer::getTickCount )
                 .def( "getTickCount", ( u32( ITimer::* )( Thread::Task ) ) & ITimer::getTickCount )
                 //.def("getFixedTimeInterval", ( time_interval( ITimer::* )( void ) const )&ITimer::getFixedTimeInterval)
                 //.def("getFixedTimeInterval", ( time_interval( ITimer::* )( void ) const )&ITimer::getFixedTimeInterval)
                 //.def("setFixedTimeInterval", &ITimer::setFixedTimeInterval)
                 //.def("setFixedTimeInterval", &ITimer::setFixedTimeInterval)
                 //.def("getTime", &ITimer::getTime)
                 .def( "getPreviousTime", &ITimer::getPreviousTime )
                 //.def("getDeltaTime", &ITimer::getDeltaTime)
                 //.def("getSmoothDeltaTime", &ITimer::getSmoothDeltaTime)
                 //.def("getStartOffset", &ITimer::getStartOffset)
                 //.def("setStartOffset", &ITimer::setStartOffset)
                 //.def("getStartOffset", &ITimer::getStartOffset)
                 //.def("setStartOffset", &ITimer::setStartOffset)
                 .def( "isSteady", &ITimer::isSteady )
                 //.def("reset", &ITimer::reset)
                 //.def("reset", &ITimer::reset)
                 //.def("getFixedTime", &ITimer::getFixedTime)
                 //.def("getFixedTime", &ITimer::getFixedTime)
                 .def( "getFixedOffset", &ITimer::getFixedOffset )
                 .def( "setFixedOffset", &ITimer::setFixedOffset )
                 .def( "getFixedOffset", &ITimer::getFixedOffset )
                 .def( "setFixedOffset", &ITimer::setFixedOffset )];

        module( L )
            [class_<core::IApplicationManager, ISharedObject, SmartPtr<core::IApplicationManager>>(
                 "IApplicationManager" )
                 .def( "getEnableRenderer", &IApplicationManager::getEnableRenderer )
                 .def( "setEnableRenderer", &IApplicationManager::setEnableRenderer )
                 .def( "isPauseMenuActive", &IApplicationManager::isPauseMenuActive )
                 .def( "setPauseMenuActive", &IApplicationManager::setPauseMenuActive )
                 .def( "isEditor", &IApplicationManager::isEditor )
                 .def( "setEditor", &IApplicationManager::setEditor )
                 .def( "isEditorCamera", &IApplicationManager::isEditorCamera )
                 .def( "setEditorCamera", &IApplicationManager::setEditorCamera )
                 .def( "isPlaying", &IApplicationManager::isPlaying )
                 .def( "setPlaying", &IApplicationManager::setPlaying )
                 .def( "isPaused", &IApplicationManager::isPaused )
                 .def( "setPaused", &IApplicationManager::setPaused )
                 .def( "isRunning", &IApplicationManager::isRunning )
                 .def( "setRunning", &IApplicationManager::setRunning )
                 .def( "getQuit", &IApplicationManager::getQuit )
                 .def( "setQuit", &IApplicationManager::setQuit )
                 .def( "hasTasks", &IApplicationManager::hasTasks )
                 .def( "getCachePath", &IApplicationManager::getCachePath )
                 .def( "setCachePath", &IApplicationManager::setCachePath )
                 .def( "getSettingsCachePath", &IApplicationManager::getSettingsPath )
                 .def( "setSettingsCachePath", &IApplicationManager::setSettingsPath )
                 .def( "getProjectPath", &IApplicationManager::getProjectPath )
                 .def( "setProjectPath", &IApplicationManager::setProjectPath )
                 .def( "getProjectLibraryName", &IApplicationManager::getProjectLibraryName )
                 .def( "setProjectLibraryName", &IApplicationManager::setProjectLibraryName )
                 .def( "getMediaPath", &IApplicationManager::getMediaPath )
                 .def( "setMediaPath", &IApplicationManager::setMediaPath )
                 .def( "getBuildConfig", &IApplicationManager::getBuildConfig )
                 .def( "getProjectLibraryExtension", &IApplicationManager::getProjectLibraryExtension )
                 .def( "getProjectLibraryPath", &IApplicationManager::getProjectLibraryPath )
                 //.def( "getStateTask", &IApplicationManager::getStateTask )
                 //.def( "getApplicationTask", &IApplicationManager::getApplicationTask )
                 .def( "getFSM", &IApplicationManager::getFSM )
                 .def( "setFSM", &IApplicationManager::setFSM )
                 .def( "getEditorSettings", &IApplicationManager::getEditorSettings )
                 .def( "setEditorSettings", &IApplicationManager::setEditorSettings )
                 .def( "getPlayerSettings", &IApplicationManager::getPlayerSettings )
                 .def( "setPlayerSettings", &IApplicationManager::setPlayerSettings )
                 .def( "getLogManager", &IApplicationManager::getLogManager )
                 .def( "setLogManager", &IApplicationManager::setLogManager )
                 .def( "getFactoryManager", ( SmartPtr<IFactoryManager> & (IApplicationManager::*)() ) &
                                                IApplicationManager::getFactoryManager )

                 .def( "setFactoryManager", &IApplicationManager::setFactoryManager )
                 .def( "getProcessManager", &IApplicationManager::getProcessManager )
                 .def( "setProcessManager", &IApplicationManager::setProcessManager )
                 .def( "getParticleManager", &IApplicationManager::getParticleManager )
                 .def( "setParticleManager", &IApplicationManager::setParticleManager )
                 .def( "getProfiler", &IApplicationManager::getProfiler )
                 .def( "setProfiler", &IApplicationManager::setProfiler )
                 .def( "getSystemSettings", &IApplicationManager::getSystemSettings )
                 .def( "setSystemSettings", &IApplicationManager::setSystemSettings )
                 .def( "getApplication", &IApplicationManager::getApplication )
                 .def( "setApplication", &IApplicationManager::setApplication )
                 .def( "getJobQueue", ( SmartPtr<IJobQueue> & (IApplicationManager::*)() ) &
                                          IApplicationManager::getJobQueue )
                 .def( "setJobQueue", &IApplicationManager::setJobQueue )
                 .def( "getGraphicsSystem",
                       ( SmartPtr<render::IGraphicsSystem> & (IApplicationManager::*)() ) &
                           IApplicationManager::getGraphicsSystem )
                 .def( "setGraphicsSystem", &IApplicationManager::setGraphicsSystem )
                 .def( "getTaskManager", &IApplicationManager::getTaskManager )
                 .def( "setTaskManager", &IApplicationManager::setTaskManager )
                 .def( "getFileSystem", ( SmartPtr<IFileSystem> & (IApplicationManager::*)() ) &
                                            ApplicationManager::getFileSystem )
                 .def( "setFileSystem", &IApplicationManager::setFileSystem )
                 .def( "getTimer",
                       ( SmartPtr<ITimer> & (IApplicationManager::*)() ) & ApplicationManager::getTimer )
                 .def( "setTimer", &IApplicationManager::setTimer )
                 .def( "getFsmManager", ( SmartPtr<IFSMManager> & (IApplicationManager::*)() ) &
                                            IApplicationManager::getFsmManager )
                 .def( "setFsmManager", &IApplicationManager::setFsmManager )
                 .def( "getProceduralEngine", &IApplicationManager::getProceduralEngine )
                 .def( "setProceduralEngine", &IApplicationManager::setProceduralEngine )
                 .def( "getProceduralManager", &IApplicationManager::getProceduralManager )
                 .def( "setProceduralManager", &IApplicationManager::setProceduralManager )
                 .def( "getPhysicsManager2D", &IApplicationManager::getPhysicsManager2D )
                 .def( "setPhysicsManager2D", &IApplicationManager::setPhysicsManager2D )
                 .def( "getPhysicsManager", &IApplicationManager::getPhysicsManager )
                 .def( "setPhysicsManager", &IApplicationManager::setPhysicsManager )
                 .def( "getPhysicsScene", &IApplicationManager::getPhysicsScene )
                 .def( "setPhysicsScene", &IApplicationManager::setPhysicsScene )
                 .def( "getObjectsScene", &IApplicationManager::getObjectsScene )
                 .def( "setObjectsScene", &IApplicationManager::setObjectsScene )
                 .def( "getRaycastScene", &IApplicationManager::getRaycastScene )
                 .def( "setRaycastScene", &IApplicationManager::setRaycastScene )
                 .def( "getControlsScene", &IApplicationManager::getControlsScene )
                 .def( "setControlsScene", &IApplicationManager::setControlsScene )

                 .def( "getScriptManager", &IApplicationManager::getScriptManager )
                 .def( "setScriptManager", &IApplicationManager::setScriptManager )
                 .def( "getInput", &IApplicationManager::getInput )
                 .def( "setInput", &IApplicationManager::setInput )
                 .def( "getInputDeviceManager", &IApplicationManager::getInputDeviceManager )
                 .def( "setInputDeviceManager", &IApplicationManager::setInputDeviceManager )
                 .def( "getThreadPool", ( SmartPtr<IThreadPool> & (IApplicationManager::*)() ) &
                                            IApplicationManager::getThreadPool )
                 .def( "setThreadPool", &IApplicationManager::setThreadPool )
                 .def( "getConsole", &IApplicationManager::getConsole )
                 .def( "setConsole", &IApplicationManager::setConsole )
                 .def( "getCameraManager", &IApplicationManager::getCameraManager )
                 .def( "setCameraManager", &IApplicationManager::setCameraManager )
                 //.def( "getStateManager", &IApplicationManager::getStateManager )
                 .def( "setStateManager", &IApplicationManager::setStateManager )
                 .def( "getCommandManager", &IApplicationManager::getCommandManager )
                 .def( "setCommandManager", &IApplicationManager::setCommandManager )
                 .def( "getResourceManager", &IApplicationManager::getResourceManager )
                 .def( "setResourceManager", &IApplicationManager::setResourceManager )
                 .def( "getPrefabManager", &IApplicationManager::getPrefabManager )
                 .def( "setPrefabManager", &IApplicationManager::setPrefabManager )
                 .def( "getMeshLoader", &IApplicationManager::getMeshLoader )
                 .def( "setMeshLoader", &IApplicationManager::setMeshLoader )
                 .def( "getResourceDatabase", &IApplicationManager::getResourceDatabase )
                 .def( "setResourceDatabase", &IApplicationManager::setResourceDatabase )
                 .def( "getSceneManager", &IApplicationManager::getSceneManager )
                 .def( "setSceneManager", &IApplicationManager::setSceneManager )
                 .def( "getScene", &IApplicationManager::getScene )
                 .def( "getSelectionManager", &IApplicationManager::getSelectionManager )
                 .def( "setSelectionManager", &IApplicationManager::setSelectionManager )
                 .def( "getSoundManager", &IApplicationManager::getSoundManager )
                 .def( "setSoundManager", &IApplicationManager::setSoundManager )
                 .def( "getUI", &IApplicationManager::getUI )
                 .def( "setUI", &IApplicationManager::setUI )
                 .def( "getRenderUI", &IApplicationManager::getRenderUI )
                 .def( "setRenderUI", &IApplicationManager::setRenderUI )
                 .def( "getDatabase", &IApplicationManager::getDatabase )
                 .def( "setDatabase", &IApplicationManager::setDatabase )
                 .def( "getMeshManager", &IApplicationManager::getMeshManager )
                 .def( "setMeshManager", &IApplicationManager::setMeshManager )

                 .def( "getVehicleManager", &IApplicationManager::getVehicleManager )
                 .def( "setVehicleManager", &IApplicationManager::setVehicleManager )
                 .def( "getLoadProgress", &IApplicationManager::getLoadProgress )
                 .def( "setLoadProgress", &IApplicationManager::setLoadProgress )
                 .def( "addLoadProgress", &IApplicationManager::addLoadProgress )
                 .def( "getActors", &IApplicationManager::getActors )
                 .def( "addPlugin", &IApplicationManager::addPlugin )
                 .def( "removePlugin", &IApplicationManager::removePlugin )
                 .def( "getStateContext", &IApplicationManager::getStateContext )
                 .def( "setStateContext", &IApplicationManager::setStateContext )
                 .def( "getWindow", &IApplicationManager::getWindow )
                 .def( "setWindow", &IApplicationManager::setWindow )
                 .def( "getSceneRenderWindow", &IApplicationManager::getSceneRenderWindow )
                 .def( "setSceneRenderWindow", &IApplicationManager::setSceneRenderWindow )
                 .def( "getComponentByType", &IApplicationManager::getComponentByType )
                 .def( "triggerEvent", &IApplicationManager::triggerEvent )
                 .scope[luabind::def( "instance", &IApplicationManager::instance ),
                        luabind::def( "setInstance", &IApplicationManager::setInstance )]

        ];

        module( L )[def( "getTimerSingleton", EngineHelper::getTimerSingleton )];
        module( L )[def( "getSingleton", EngineHelper::getSingleton )];
        //module( L )[def( "print", EngineHelper::printLuaMessage )];

        //module( L )[def( "printf", EngineHelper::printLuaMessage )];

        //module( L )[def( "FB_LOG_MESSAGE", _LuaLogMessage )];

        ///*	module(L)
        //        [
        //            def( "FB_LOG_MESSAGE", _LuaLogMessage_stringc )
        //        ];		*/

        //module( L )[def( "createStateListener", _createStateListener )];

        //module( L )[def( "createStateListener", _createStateListenerEntity )];

        //module( L )[def( "createEvent", _createEvent )];

        //module( L )[def( "type_cast", type_cast_object<physics::IPhysicsShape2> )];

        //// some temp code
        //module( L )[def( "toBoxShape2", type_cast<IBoxShape2, IPhysicsShape2> )];
    }

}  // end namespace fb
