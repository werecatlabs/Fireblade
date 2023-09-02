#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ScriptObjectBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/ScriptObjectFunctions.h"
#include "FBLuabind/Bindings/PropertiesBinding.h"
#include "FBLuabind/Wrappers/StandardGameObjectWrapper.h"
#include <luabind/operator.hpp>

namespace fb
{

    //IEntity *cast_to_entity( IScriptObject *obj )
    //{
    //    return (IEntity *)obj;
    //}

    //
    //StateObjectPtr _getStateObject( IScriptObject *obj )
    //{
    //    return obj->getStateObject();
    //}

    //
    //MessageHandlerPtr _getMessageHandler( IScriptObject *obj )
    //{
    //    return obj->getMessageHandler();
    //}

    void _initialise( IScriptObject *object, const char *className )
    {
        //object->initialise( String( className ) );
    }

    template <class T>
    void _initialiseTemplate( IScriptObject *object, SmartPtr<T> objectTemplate )
    {
        //object->initialise( objectTemplate );
    }

    template <class T>
    void _initialiseTemplateProperties( IScriptObject *object, SmartPtr<T> objectTemplate,
                                        PropertiesBinding *propertiesBinding )
    {
        //object->initialise( objectTemplate, propertiesBinding->getProperties() );
    }

    void _getInstance( IScriptObject *object )
    {
        //SmartPtr<IScriptData> scriptData = object->_getData();
        //luabind::object &luaobject = *(luabind::object *)scriptData->getObjectData();

        //core::IApplicationManager *engine = core::IApplicationManager::instance();
        //ScriptManagerPtr scriptMgr = engine->getScriptManager();

        //lua_State *L = nullptr;
        //scriptMgr->_getObject( (void **)&L );
        //luaobject.push( L );
    }

    class ScriptObjectHelper
    {
    public:
        static IScriptObject *getUserDataMap( IScriptObject *object, hash32 id )
        {
            return (IScriptObject *)object->getUserData( id );
        }

        static void setUserDataMap( IScriptObject *object, hash32 id, IScriptObject *userData )
        {
            object->setUserData( id, (void *)userData );
        }

        static SmartPtr<IScriptInvoker> getInvoker( IScriptObject *object )
        {
            return object->getInvoker();
        }

        static String toString( IObject *object )
        {
            String str = object->toString();
            FB_LOG_MESSAGE( "Watch", str.c_str() );
            return str;
        }
    };

    void _updateObject( SmartPtr<IScriptObject> object, lua_Number t, lua_Number dt )
    {
        //object->update( Thread::TASK_ID_APPLICATION_LOGIC, t, dt );
    }

    boost::shared_ptr<Handle> _getHandle( SmartPtr<IScriptObject> object )
    {
        //return object->getHandle();

        return nullptr;
    }

    void bindBaseObjects( lua_State *L )
    {
        using namespace luabind;

        typedef ScriptObjectFunc<IScriptObject> Functions;

        module( L )[class_<IObject, SmartPtr<IObject>>( "IObject" )
                        .def( "preUpdate", &IObject::preUpdate )
                        .def( "update", &IObject::update )
                        .def( "postUpdate", &IObject::postUpdate )
                        //.def( "getHandle", &IObject::getHandle )
                        .def( "isValid", &IObject::isValid )
                        .def( "getCreatorData", &IObject::getCreatorData )
                        .def( "setCreatorData", &IObject::setCreatorData )
                        .def( "getFactoryData", &IObject::getFactoryData )
                        .def( "setFactoryData", &IObject::setFactoryData )
                        //.def( "toString", &IObject::toString )
                        //.def( "getUserData", &IObject::getUserData )
                        //.def( "setUserData", &IObject::setUserData )
                        //.def( "getUserData", &IObject::getUserData )
                        //.def( "setUserData", &IObject::setUserData )
                        //.def( "isDerived", &IObject::isDerived<B> )
                        //.def( "isExactly", &IObject::isExactly<B> )
                        //.def( "getClassName", &IObject::getClassName )

                        .def( "derived", &IObject::derived )
                        .def( "exactly", &IObject::exactly )

                        .def( "getTypeInfo", &IObject::getTypeInfo )
                        .def( "__tostring", ScriptObjectHelper::toString )];

        module( L )[class_<ISharedObject, IObject, SmartPtr<ISharedObject>>( "ISharedObject" )
                        .def( "__tostring", ScriptObjectHelper::toString )];

        typedef Array<SmartPtr<ISharedObject>> SharedObjectVector;

        module( L )[class_<SharedObjectVector>( "SharedObjectVector" )
                        .def( "push_back",
                              ( void( SharedObjectVector::* )( const SmartPtr<ISharedObject> & ) ) &
                                  SharedObjectVector::push_back )
                        .def( "pop_back", &SharedObjectVector::pop_back )
                        .def( "clear", &SharedObjectVector::clear )
                        .def( "size", &SharedObjectVector::size )
                        .def( "empty", &SharedObjectVector::empty )
                        .def( "at", ( SmartPtr<ISharedObject> & (SharedObjectVector::*)( size_t ) ) &
                                        SharedObjectVector::at )];

        module( L )[class_<IScriptObject, ISharedObject, SmartPtr<ISharedObject>>( "IScriptObject" )
                        .def( "initialise", _initialise )
                        //.def( "initialise", _initialiseTemplate<ITemplate> )
                        //.def("initialise", _initialiseTemplate<CarTemplate> )
                        //.def("initialise", _initialiseTemplate<CharacterController3Template> )
                        //.def("initialise", _initialiseTemplate<CollisionShapeTemplate> )
                        //.def("initialise", _initialiseTemplate<TerrainTemplate> )
                        //.def("initialise", _initialiseTemplate<VehicleTemplate> )
                        //.def("initialise", _initialiseTemplate<WaterTemplate> )

                        //.def("initialise", _initialiseTemplate<ParticleTemplate> )
                        //.def( "initialise", _initialiseTemplateProperties<ITemplate> )

                        .def( "update", _updateObject )

                        .def( "getHandle", _getHandle )

                        //
                        // Todo remove these functions
                        //

                        .def( "setProperty", Functions::ScriptObject_setProperty )
                        .def( "setProperty", Functions::ScriptObject_setPropertyHash )
                        .def( "setAsString", Functions::ScriptObject_setProperty )
                        .def( "setAsString", Functions::ScriptObject_setPropertyHash )
                        .def( "setAsBool", Functions::setPropertyAsBool )
                        .def( "setAsBool", Functions::setPropertyAsBoolHash )
                        .def( "setAsInt", Functions::setPropertyAsInt )
                        .def( "setAsInt", Functions::setPropertyAsIntHash )
                        .def( "setAsUInt", Functions::setPropertyAsUInt )
                        .def( "setAsUInt", Functions::setPropertyAsUIntHash )
                        .def( "setAsNumber", Functions::setPropertyAsNumber )
                        .def( "setAsNumber", Functions::setPropertyAsNumberHash )

                        .def( "setAsVector2i", Functions::setPropertyAsVector2i )
                        .def( "setAsVector2i", Functions::setPropertyAsVector2iHash )
                        .def( "setAsVector2", Functions::setPropertyAsVector2 )
                        .def( "setAsVector2", Functions::setPropertyAsVector2Hash )

                        .def( "setAsVector3", Functions::setPropertyAsVector3 )
                        .def( "setAsVector3", Functions::setPropertyAsVector3Hash )
                        .def( "setAsQuaternion", Functions::setPropertyAsQuaternion )
                        .def( "setAsQuaternion", Functions::setPropertyAsQuaternionHash )
                        .def( "setAsColor", Functions::setPropertyAsColour )
                        .def( "setAsColor", Functions::setPropertyAsColourHash )
                        .def( "setAsColour", Functions::setPropertyAsColour )
                        .def( "setAsColour", Functions::setPropertyAsColourHash )
                        .def( "getAsBool", Functions::getPropertyAsBool )
                        .def( "getAsBool", Functions::getPropertyAsBoolHash )
                        .def( "getAsInt", Functions::getPropertyAsInt )
                        .def( "getAsInt", Functions::getPropertyAsIntHash )
                        .def( "getAsUInt", Functions::getPropertyAsUInt )
                        .def( "getAsUInt", Functions::getPropertyAsUIntHash )
                        .def( "getAsNumber", Functions::getPropertyAsNumber )
                        .def( "getAsNumber", Functions::getPropertyAsNumberHash )
                        .def( "getAsString", Functions::getPropertyAsString )
                        .def( "getAsHash", Functions::getPropertyAsHash )

                        .def( "getAsVector2i", Functions::getPropertyAsVector2i )
                        .def( "getAsVector2i", Functions::getPropertyAsVector2iHash )
                        .def( "getAsVector2", Functions::getPropertyAsVector2 )
                        .def( "getAsVector2", Functions::getPropertyAsVector2Hash )

                        .def( "getAsVector3", Functions::getPropertyAsVector3 )
                        .def( "getAsVector3", Functions::getPropertyAsVector3Hash )
                        .def( "getAsQuaternion", Functions::getPropertyAsQuaternion )
                        .def( "getAsQuaternion", Functions::getPropertyAsQuaternionHash )
                        .def( "getAsColor", Functions::getPropertyAsColour )
                        .def( "getAsColor", Functions::getPropertyAsColourHash )
                        .def( "getAsColour", Functions::getPropertyAsColour )
                        .def( "getAsColour", Functions::getPropertyAsColourHash )
                        .def( "getAsArray", Functions::getPropertyAsArray )
                        .def( "getAsArray", Functions::getPropertyAsArrayHash )

                        .def( "callFunction", Functions::callObjectFunction )
                        .def( "callFunction", Functions::callObjectFunctionHash )
                        .def( "callFunction", Functions::callObjectFunctionAny )
                        .def( "callFunction", Functions::callObjectFunctionAnyHash )
                        .def( "callFunction", Functions::callObjectFunctionAny2 )
                        .def( "callFunction", Functions::callObjectFunctionAny2Hash )
                        .def( "callFunction", Functions::callObjectFunctionAny3 )
                        .def( "callFunction", Functions::callObjectFunctionAny3Hash )
                        .def( "callFunction", Functions::callObjectFunctionParams )
                        .def( "callFunction", Functions::callObjectFunctionParamsHash )
                        .def( "callFunction", Functions::callObjectFunctionParamsResults )
                        .def( "callFunction", Functions::callObjectFunctionParamsResultsHash )
                        //.def( "callFunction", Functions::callObjectFunctionObj )
                        //.def( "callFunction", Functions::callObjectFunctionObjHash )

                        //.def( "setObject", Functions::setObject )
                        //.def( "setObjectFromHash", Functions::setObjectFromHash )

                        //.def( "getObject", Functions::getObject )
                        //.def( "getObject", Functions::getObjectFromHash )
                        //.def( "getObjectAsEntity", Functions::getObjectAsEntity )
                        //.def( "getObjectAsEntity", Functions::getObjectAsEntityFromHash )

                        //.def( "getStateObject", _getStateObject )
                        //.def( "setStateObject", &IScriptObject::setStateObject )

                        .def( "setCallback", Functions::setCallback )

                        .def( "getScriptInstance", _getInstance )

                        .def( "setUserData", ScriptObjectHelper::setUserDataMap )
                        .def( "getUserData", ScriptObjectHelper::getUserDataMap )

                        .def( "getInvoker", ScriptObjectHelper::getInvoker )

                    //.def( "getMessageHandler", _getMessageHandler )

        ];

        module( L )[class_<IScriptReceiver>( "IScriptReceiver" )];

        module( L )[class_<IScriptInvoker>( "IScriptInvoker" )
                        .def( "getOwner", &IScriptInvoker::getOwner )
                        .def( "setOwner", &IScriptInvoker::setOwner )

                        .def( "getEventFunction", &IScriptInvoker::getEventFunction )
                        .def( "setEventFunction", &IScriptInvoker::setEventFunction )];

        luabind::detail::class_id bid = luabind::detail::registered_class<IObject>::id;
        luabind::detail::class_id id = luabind::detail::registered_class<IScriptObject>::id;

        int stop = 0;
        stop = 0;
    }

}  // end namespace fb
