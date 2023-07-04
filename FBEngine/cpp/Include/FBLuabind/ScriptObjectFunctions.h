#ifndef ScriptObjectFunctions_h__
#define ScriptObjectFunctions_h__

#include "FBLuabind/FBLuabindTypes.h"
#include "FBLuabind/FBLuaScriptError.h"
#include <FBCore/Core/Parameter.h>
#include <FBCore/FBCore.h>

namespace fb
{

    template <class T>
    class ScriptObjectFunc
    {
    public:
        static void setPropertyAsBool( T *obj, const char *propertyName, bool value );
        static void setPropertyAsBoolHash( T *obj, lua_Integer hashId, bool value );
        static bool getPropertyAsBool( T *obj, const char *propertyName );
        static bool getPropertyAsBoolHash( T *obj, lua_Integer hashId );

        static void setPropertyAsNumberHash( T *obj, lua_Integer hash, f32 value );
        static lua_Number getPropertyAsNumberHash( T *obj, lua_Integer hash );
        static void setPropertyAsNumber( T *obj, const char *propertyName, f32 value );
        static f32 getPropertyAsNumber( T *obj, const char *propertyName );

        static String getPropertyAsString( T *obj, const char *propertyName );
        static lua_Integer getPropertyAsHash( T *obj, const char *propertyName );

        //static void setObject( T *obj, const char *objectName, SmartPtr<IScriptObject> scriptObj )
        //{
        //    hash32 id = StringUtil::getHash( objectName );
        //    setObjectFromHash( obj, id, scriptObj );
        //}

        //static void setObjectFromHash( T *obj, lua_Integer hash, SmartPtr<IScriptObject> scriptObj )
        //{
        //    obj->setObject( hash, scriptObj );
        //}

        //static SmartPtr<IScriptObject> getObject( T *obj, const char *objectName );
        //static SmartPtr<IScriptObject> getObjectFromHash( T *obj, lua_Integer hash );
        //static IEntity *getObjectAsEntity( T *obj, const char *objectName );
        //static IEntity *getObjectAsEntityFromHash( T *obj, lua_Integer hash );
        static IScriptObject *getScriptObject( const char *type );
        static IScriptObject *getScriptObjectHash( lua_Integer hash );

        static void ScriptObject_setProperty( T *obj, const char *propertyName, const char *value );
        static void ScriptObject_setPropertyHash( T *obj, lua_Integer hash, const char *value );

        static void scriptObjectUpdate( T *obj, const s32 &task, const time_interval &t,
                                        const time_interval &dt );

        static IFSM *getFSMByName( T *obj, const char *value )
        {
            auto hash = StringUtil::getHash( value );
            return getFSM( obj, hash );
        }

        static IFSM *getFSM( T *obj, lua_Integer hash )
        {
            return nullptr;
        }

        static Parameter callObjectFunction( T *obj, const char *functionName );
        static Parameter callObjectFunctionHash( T *obj, lua_Integer hashId );

        static Parameter callObjectFunctionAny( T *obj, const char *functionName, Parameter arg0 )
        {
            auto hash = StringUtil::getHash( functionName );
            return callObjectFunctionAnyHash( obj, hash, arg0 );
        }

        static Parameter callObjectFunctionAnyHash( T *obj, lua_Integer hash, Parameter arg0 )
        {
            Parameters params;
            params.resize( 1 );
            params[0] = arg0;

            Parameters results;
            results.resize( 1 );

            auto receiver = obj->getReceiver();
            if( receiver )
            {
                s32 retValue = receiver->callFunction( hash, params, results );

                return results[0];
            }

            Parameter param;
            return param;
        }

        static Parameter callObjectFunctionAny2( T *obj, const char *functionName, Parameter arg0,
                                                 Parameter arg1 )
        {
            auto hash = StringUtil::getHash( functionName );
            return callObjectFunctionAny2Hash( obj, hash, arg0, arg1 );
        }

        static Parameter callObjectFunctionAny2Hash( T *obj, lua_Integer hash, Parameter arg0,
                                                     Parameter arg1 )
        {
            Parameters params;
            params.resize( 2 );
            params[0] = arg0;
            params[1] = arg1;

            Parameters results;
            results.resize( 1 );

            s32 retValue = obj->getReceiver()->callFunction( hash, params, results );

            return results[0];
        }

        static Parameter callObjectFunctionAny3( T *obj, const char *functionName, Parameter arg0,
                                                 Parameter arg1, Parameter arg2 )
        {
            auto hash = StringUtil::getHash( functionName );
            return callObjectFunctionAny3Hash( obj, hash, arg0, arg1, arg2 );
        }

        static Parameter callObjectFunctionAny3Hash( T *obj, lua_Integer hash, Parameter arg0,
                                                     Parameter arg1, Parameter arg2 )
        {
            Parameters params;
            params.resize( 3 );
            params[0] = arg0;
            params[1] = arg1;
            params[2] = arg2;

            Parameters results;
            results.resize( 1 );

            s32 retValue = obj->getReceiver()->callFunction( hash, params, results );

            return results[0];
        }

        static Parameter callObjectFunctionAny4( T *obj, const char *functionName, Parameter arg0,
                                                 Parameter arg1, Parameter arg2, Parameter arg3 )
        {
            auto hash = StringUtil::getHash( functionName );
            return callObjectFunctionAny3Hash( obj, hash, arg0, arg1, arg2 );
        }

        static Parameter callObjectFunctionAny4Hash( T *obj, lua_Integer hash, Parameter arg0,
                                                     Parameter arg1, Parameter arg2, Parameter arg3 )
        {
            Parameters params;
            params.resize( 4 );
            params[0] = arg0;
            params[1] = arg1;
            params[2] = arg2;
            params[3] = arg3;

            Parameters results;
            results.resize( 1 );

            s32 retValue = obj->getReceiver()->callFunction( hash, params, results );

            return results[0];
        }

        static Parameter callObjectFunctionParams( T *obj, const char *functionName,
                                                   const Parameters &params );
        static Parameter callObjectFunctionParamsHash( T *obj, lua_Integer hashId,
                                                       const Parameters &params );

        static void callObjectFunctionParamsResults( T *obj, const char *functionName,
                                                     const Parameters &params, Parameters &results );
        static void callObjectFunctionParamsResultsHash( T *obj, lua_Integer hashId,
                                                         const Parameters &params, Parameters &results );

        //static Parameter callObjectFunctionObj( T *obj, const char *functionName,
        //                                        const ScriptProperties &properties );
        //static Parameter callObjectFunctionObjHash( T *obj, lua_Integer hashId,
        //                                            const ScriptProperties &properties );

        static void setPropertyAsVector2iHash( T *obj, lua_Integer hash, const Vector2I &value );
        static void setPropertyAsVector2i( T *obj, const char *propertyName, const Vector2I &value );
        static Vector2I getPropertyAsVector2iHash( T *obj, lua_Integer hashId );
        static Vector2I getPropertyAsVector2i( T *obj, const char *propertyName );

        static void setPropertyAsVector2Hash( T *obj, lua_Integer hash, const Vector2F &value );
        static void setPropertyAsVector2( T *obj, const char *propertyName, const Vector2F &value );
        static Vector2F getPropertyAsVector2Hash( T *obj, lua_Integer hashId );
        static Vector2F getPropertyAsVector2( T *obj, const char *propertyName );

        static void setPropertyAsVector3( T *obj, const char *propertyName, Vector3F &value );
        static void setPropertyAsVector3Hash( T *obj, lua_Integer hash, Vector3F &value );
        static Vector3F getPropertyAsVector3( T *obj, const char *propertyName );
        static Vector3F getPropertyAsVector3Hash( T *obj, lua_Integer hash );

        static void setPropertyAsQuaternion( T *obj, const char *propertyName,
                                             const QuaternionF &value );
        static void setPropertyAsQuaternionHash( T *obj, lua_Integer hash, const QuaternionF &value );
        static QuaternionF getPropertyAsQuaternion( T *obj, const char *propertyName );
        static QuaternionF getPropertyAsQuaternionHash( T *obj, lua_Integer hash );

        static void setPropertyAsColour( T *obj, const char *propertyName, const ColourF &color );
        static void setPropertyAsColourHash( T *obj, lua_Integer hash, const ColourF &color );
        static ColourF getPropertyAsColour( T *obj, const char *propertyName );
        static ColourF getPropertyAsColourHash( T *obj, lua_Integer hash );

        static void setPropertyAsInt( T *obj, const char *propertyName, lua_Integer value );
        static void setPropertyAsIntHash( T *obj, lua_Integer hash, lua_Integer value );
        static lua_Integer getPropertyAsInt( T *obj, const char *propertyName );
        static lua_Integer getPropertyAsIntHash( T *obj, lua_Integer hash );

        static void setPropertyAsUInt( T *obj, const char *propertyName, lua_Number value );
        static void setPropertyAsUIntHash( T *obj, lua_Integer hash, lua_Number value );
        static lua_Integer getPropertyAsUInt( T *obj, const char *propertyName );
        static lua_Integer getPropertyAsUIntHash( T *obj, lua_Integer hash );

        static Parameters getPropertyAsArray( T *obj, const char *propertyName );
        static Parameters getPropertyAsArrayHash( T *obj, lua_Integer hashId );

        static void setCallback( T *obj, const char *callbackName, const char *functionName )
        {
            //ScriptEventPtr scriptEvent(new ScriptEvent, true);
            //scriptEvent->setFunction(functionName);
            //obj->getInvoker()->setEventFunction(StringUtil::getHash(callbackName), scriptEvent);
        }
    };

}  // end namespace fb

#include "ScriptObjectFunctions.inl"

#endif  // ScriptObjectFunctions_h__
