#include <FBLua/LuaManager.h>
#include <FBCore/FBCore.h>
#include <FBLuabind/FBLuabind.h>
#include <FBLuabind/ParamConverter.h>
#include <FBLua/NullScriptObject.h>
#include <FBLua/LuaObjectData.h>
#include <sstream>
#include <stdarg.h>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <lobject.h>
}

#include <luabind/luabind.hpp>
#include <luabind/detail/call_member.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/detail/class_rep.hpp>
#include <luabind/function_introspection.hpp>
#include "luabind/class_info.hpp"

namespace luabind
{
    std::string get_function_name( argument const &fn );
}

extern "C" {
int luaopen_cjson( lua_State *l );
}

namespace fb
{
    template <>
    LuaManager *Singleton<LuaManager>::m_singleton = nullptr;

    FB_CLASS_REGISTER_DERIVED( fb, LuaManager, IScriptManager );

    void LuaManager::setClassNames( const Array<String> &classNames )
    {
        m_classNames = classNames;
    }

    void LuaManager::handleLuaError( lua_State *luaState )
    {
        auto errorStr = String( lua_tostring( luaState, -1 ) );
        auto message = String( "Error: " ) + errorStr + String( "\n" );

        String debugStr;

        lua_pop( luaState, 1 );

        auto applicationManager = core::IApplicationManager::instance();
        SmartPtr<LuaManager> scriptMgr = applicationManager->getScriptManager();
        if( scriptMgr )
        {
            debugStr = scriptMgr->getDebugInfo();
        }

        //String outputStr = printer.CStr();
        //engine->getOutputManager()->output( outputStr );

        scriptMgr->setError( true );

        auto logStr = message + debugStr;
        FB_LOG_ERROR( logStr );
    }

    int handleLuaPCallError( lua_State *luaState )
    {
        String debugStr;

        lua_Debug d;

        int level = LUA_MINSTACK;
        int success = lua_getstack( luaState, level, &d );

        while( level >= 0 )
        {
            success = lua_getstack( luaState, level, &d );
            if( success != 0 )
            {
                lua_getinfo( luaState, "Sln", &d );

                std::stringstream msg;
                msg << d.short_src << ":" << d.currentline;

                if( d.name != nullptr )
                {
                    msg << "(" << d.namewhat << " " << d.name << ")";
                }

                debugStr += String( msg.str().c_str() ) + String( "\n" );

                //TiXmlDocument doc;
                //TiXmlElement *rootElem = new TiXmlElement( "error" );
                //doc.LinkEndChild( rootElem );
                //if( rootElem )
                //{
                //    TiXmlElement *lineElem = new TiXmlElement( "line" );
                //    rootElem->LinkEndChild( lineElem );

                //    TiXmlText *lineTxtElem =
                //        new TiXmlText( StringUtil::toString( d.currentline ).c_str() );
                //    lineElem->LinkEndChild( lineTxtElem );

                //    TiXmlElement *sourceElem = new TiXmlElement( "source" );
                //    rootElem->LinkEndChild( sourceElem );

                //    TiXmlText *sourceTxtElem = new TiXmlText( d.short_src );
                //    sourceElem->LinkEndChild( sourceTxtElem );
                //}

                //TiXmlPrinter printer;
                //printer.SetIndent( "	" );

                //doc.Accept( &printer );

                // auto engine = core::IApplicationManager::instance();
                //String outputStr = printer.CStr();
                //engine->getOutputManager()->output( outputStr );
            }

            --level;
        }

        auto engine = core::IApplicationManager::instance();
        SmartPtr<LuaManager> scriptMgr = engine->getScriptManager();
        scriptMgr->setError( true );

        FB_LOG_MESSAGE( "LuaScriptMgr", debugStr.c_str() );

        return 1;
    }

    void handleCastFailed( lua_State *luaState, const luabind::type_id &id )
    {
        String debugStr;

        lua_Debug d;

        int level = LUA_MINSTACK;
        int success = lua_getstack( luaState, level, &d );

        while( level >= 0 )
        {
            success = lua_getstack( luaState, level, &d );
            if( success != 0 )
            {
                lua_getinfo( luaState, "Sln", &d );

                std::stringstream msg;
                msg << d.short_src << ":" << d.currentline;

                if( d.name != nullptr )
                {
                    msg << "(" << d.namewhat << " " << d.name << ")";
                }

                debugStr += String( msg.str().c_str() ) + String( "\n" );
            }

            --level;
        }

        auto engine = core::IApplicationManager::instance();
        SmartPtr<LuaManager> scriptMgr = engine->getScriptManager();
        scriptMgr->setError( true );

        FB_LOG_MESSAGE( "LuaScriptMgr", debugStr.c_str() );
    }

    LuaManager::LuaManager() : m_enableFullDebug( true ), m_delayedCreation( false ), m_bError( false )
    {
    }

    LuaManager::~LuaManager()
    {
        unload( nullptr );
    }

    void LuaManager::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        //m_nullScriptObject = new NullScriptObject;
        createLuaState();
        m_bReload = FALSE;

        m_timeTaken = 0.0f;
        m_callCounter = 0;

        setLoadingState( LoadingState::Loaded );
    }

    void LuaManager::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        //for(u32 i=0; i<m_objectData.size(); ++i)
        //{
        //	SmartPtr<LuaObjectData> objectData = m_objectData[i];
        //	luabind::object& luaObject = objectData->getObject();
        //	luaObject = luabind::object();
        //}

        //m_objectData.clear();
        //m_creationList.clear();

        //for(u32 i=0; i<m_instances.size(); ++i)
        //{
        //	luabind::object* pObject = m_instances[i];

        //	//StandardGameObject* ptr = luabind::object_cast<StandardGameObject*>((*pObject)(), luabind::adopt(boost::arg<0>()));
        //	//IScriptObject* ptr = luabind::object_cast<IScriptObject*>((*pObject));
        //	//ptr->~IScriptObject(); // let lua free the memory

        //	delete pObject;
        //}

        //lua_gc (m_luaState, LUA_GCCOLLECT, 0);

        //// closes the lua state
        //lua_close(m_luaState);

        //FB_SAFE_DELETE(m_nullScriptObject);

        setLoadingState( LoadingState::Unloaded );
    }

    void LuaManager::loadScript( const String &filename )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            auto it = std::find( m_scripts.begin(), m_scripts.end(), filename );
            if( it != m_scripts.end() )
            {
                return;
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto stream = fileSystem->open( filename, true, false, false, false, false );
            if( !stream )
            {
                stream = fileSystem->open( filename, true, false, false, true, true );
            }

            if( stream )
            {
                auto script = stream->getAsString();

                auto luaState = getLuaState();
                auto error = luaL_dostring( luaState, script.c_str() );
                if( error )
                {
                    auto message = String( "ScriptMgr::loadScript - error - couldn't open " ) +
                                   filename + String( " errorcode = " ) +
                                   String( lua_tostring( luaState, -1 ) );
                    FB_LOG_MESSAGE( "Script", message );
                }
                else
                {
                    auto message = String( "Loaded: " ) + filename;
                    FB_LOG_MESSAGE( "Script", message );
                }

                // add filename
                m_scripts.push_back( filename );
            }

            updateClassNames();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void LuaManager::loadScriptFromString( const String &str )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto luaState = getLuaState();
            auto error = luaL_dostring( luaState, str.c_str() );
            if( error )
            {
                auto message = String( "ScriptMgr::loadScript - error - couldn't open " ) + str +
                               String( " errorcode = " ) + String( lua_tostring( luaState, -1 ) );
                FB_LOG_MESSAGE( "Script", message );
            }
            else
            {
                auto message = String( "Loaded: " ) + str;
                FB_LOG_MESSAGE( "Script", message );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    TValue *index2value( lua_State *L, int idx );

    luabind::class_info my_get_class_info( luabind::argument const &o )
    {
        using namespace luabind;

        lua_State *L = o.interpreter();
        detail::class_rep *crep = NULL;
        bool givenClassRep = false;

        //o.push( L );
        if( detail::is_class_rep( L, -1 ) )
        {
            // OK, o is a class rep, now at the top of the stack
            givenClassRep = true;
            crep = static_cast<detail::class_rep *>( lua_touserdata( L, -1 ) );
            lua_pop( L, 1 );
        }
        else
        {
            detail::object_rep *obj = detail::get_instance( L, -1 );

            if( !obj )
            {
                class_info result;
                result.name = lua_typename( L, lua_type( L, -1 ) );
                lua_pop( L, 1 );
                result.methods = newtable( L );
                result.attributes = newtable( L );
                return result;
            }
            else
            {
                lua_pop( L, 1 );
                // OK, we were given an object - gotta get the crep.
                crep = obj->crep();
            }
        }
        crep->get_table( L );
        object table( from_stack( L, -1 ) );
        //lua_pop( L, 1 );

        class_info result;
        result.name = crep->name();
        result.methods = newtable( L );
        result.attributes = newtable( L );

        std::size_t index = 1;

        for( iterator i( table ), e; i != e; ++i )
        {
            if( type( *i ) != LUA_TFUNCTION )
                continue;

            // We have to create a temporary `object` here, otherwise the proxy
            // returned by operator->() will mess up the stack. This is a known
            // problem that probably doesn't show up in real code very often.
            object member( *i );
            member.push( L );
            detail::stack_pop pop( L, 1 );

            if( lua_tocfunction( L, -1 ) == &detail::property_tag )
            {
                result.attributes[index++] = i.key();
            }
            else
            {
                result.methods[i.key()] = *i;
            }
        }

        return result;
    }

    void my_function( luabind::object &func )
    {
        lua_State *L = func.interpreter();

        // Push the function's environment onto the stack
        lua_pushvalue( L, -1 );     // push the function
        lua_getuservalue( L, -1 );  // push the environment

        // Get the function name from the environment table
        lua_pushstring( L, "__name" );
        lua_rawget( L, -2 );
        const char *name = lua_tostring( L, -1 );
        lua_pop( L, 2 );

        // Use the function name as needed
        // ...

        int stop = 0;
        stop = 0;
    }

    void LuaManager::print_lua_stack()
    {
        auto L = getLuaState();
        print_lua_stack( L );
    }

    void LuaManager::print_lua_stack( lua_State *L )
    {
        std::cout << "Lua stack contents:\n";
        int top = lua_gettop( L );
        for( int i = 1; i <= top; i++ )
        {
            int type = lua_type( L, i );
            const char *type_name = lua_typename( L, type );
            std::cout << "  [" << i << "]: " << type_name;
            switch( type )
            {
            case LUA_TBOOLEAN:
                std::cout << " " << ( lua_toboolean( L, i ) ? "true" : "false" );
                break;
            case LUA_TNUMBER:
                std::cout << " " << lua_tonumber( L, i );
                break;
            case LUA_TSTRING:
                std::cout << " \"" << lua_tostring( L, i ) << "\"";
                break;
            case LUA_TTABLE:
                std::cout << " {table}";
                break;
            case LUA_TFUNCTION:
                std::cout << " {function}";
                break;
            case LUA_TUSERDATA:
                std::cout << " {userdata}";
                break;
            default:
                std::cout << " {unknown}";
                break;
            }
            std::cout << "\n";
        }
    }

    void LuaManager::updateClassNames()
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        m_classNames.clear();

        auto L = getLuaState();

        auto classNames = luabind::get_class_names( L );

        for( luabind::iterator i( classNames ), e; i != e; ++i )
        {
            luabind::object obj = *i;

            std::string className = luabind::object_cast<std::string>( obj );
            //m_classNames.push_back( className );
        }

        //auto count = 1;
        //auto obj = classNames[count];
        //while( obj )
        //{
        //    std::string my_str = luabind::object_cast<std::string>( obj );
        //    std::cout << my_str << std::endl;
        //    obj = classNames[count++];
        //}

        // Push the global environment table onto the stack
        lua_pushglobaltable( L );

        // Push a nil value onto the stack to start the iteration
        lua_pushnil( L );

        while( lua_next( L, -2 ) != 0 )
        {
            auto type = lua_type( L, -1 );
            if( type == LUA_TUSERDATA )
            {
                // Get the class name
                std::string className = lua_tostring( L, -2 );

                if( std::find( m_bindingClassNames.begin(), m_bindingClassNames.end(), className ) ==
                    m_bindingClassNames.end() )
                {
                    m_classNames.push_back( className );
                }
            }

            lua_pop( L, 1 );
        }

        if( std::find( m_bindingClassNames.begin(), m_bindingClassNames.end(), "MaterialEditor" ) !=
            m_bindingClassNames.end() )
        {
            int stop = 0;
            stop = 0;
        }
    }

    void LuaManager::updateBindClassNames()
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        m_bindingClassNames.clear();

        auto L = getLuaState();

        auto classNames = luabind::get_class_names( L );

        for( luabind::iterator i( classNames ), e; i != e; ++i )
        {
            luabind::object obj = *i;

            std::string className = luabind::object_cast<std::string>( obj );
            m_bindingClassNames.push_back( className );
        }

        if( std::find( m_bindingClassNames.begin(), m_bindingClassNames.end(), "MaterialEditor" ) !=
            m_bindingClassNames.end() )
        {
            int stop = 0;
            stop = 0;
        }

        //auto count = 1;
        //auto obj = classNames[count];
        //while( obj )
        //{
        //    std::string my_str = luabind::object_cast<std::string>( obj );
        //    std::cout << my_str << std::endl;
        //    obj = classNames[count++];
        //}
    }

    void LuaManager::updateScriptData()
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        m_scriptData.clear();

        auto L = getLuaState();
        // Push the global environment table onto the stack
        lua_pushglobaltable( L );
        // Push a nil value onto the stack to start the iteration
        lua_pushnil( L );
        while( lua_next( L, -2 ) != 0 )
        {
            auto type = lua_type( L, -1 );
            if( type == LUA_TFUNCTION )
            {
                // Get the function name
                std::string functionName = lua_tostring( L, -2 );
                // Get the function's environment
                lua_getuservalue( L, -1 );
                // Get the function's environment table
                lua_pushstring( L, "__name" );
                lua_rawget( L, -2 );
                std::string className = lua_tostring( L, -1 );
                lua_pop( L, 2 );

                // Get the function's source file
                lua_pushstring( L, "__source" );
                lua_rawget( L, -2 );
                std::string source = lua_tostring( L, -1 );
                lua_pop( L, 2 );
                // Get the function's line number
                lua_pushstring( L, "__line" );
                lua_rawget( L, -2 );
                int line = (int)lua_tointeger( L, -1 );
                lua_pop( L, 2 );
                // Get the function's column number
                lua_pushstring( L, "__column" );
                lua_rawget( L, -2 );
                int column = (int)lua_tointeger( L, -1 );
                lua_pop( L, 2 );

                // Get the function's source code
                std::string code = getSourceCode( source, line, column );
                // Add the function to the list
                //m_scriptData.push_back( ScriptData( functionName, className, source, line, column, code ) );
            }

            lua_pop( L, 1 );
        }
    }

    void LuaManager::updateScriptDataVariables()
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );
        //m_scriptDataVariables.clear();
        auto L = getLuaState();
        // Push the global environment table onto the stack
        lua_pushglobaltable( L );
        // Push a nil value onto the stack to start the iteration
        lua_pushnil( L );
        while( lua_next( L, -2 ) != 0 )
        {
            auto type = lua_type( L, -1 );
            if( type == LUA_TTABLE )
            {
                // Get the function name
                std::string functionName = lua_tostring( L, -2 );
                // Get the function's environment
                lua_getuservalue( L, -1 );
                // Get the function's environment table
                lua_pushstring( L, "__name" );
                lua_rawget( L, -2 );
                std::string className = lua_tostring( L, -1 );
                lua_pop( L, 2 );
                // Get the function's source file
                lua_pushstring( L, "__source" );
                lua_rawget( L, -2 );
                std::string source = lua_tostring( L, -1 );
                lua_pop( L, 2 );
                // Get the function's line number
                lua_pushstring( L, "__line" );
                lua_rawget( L, -2 );
                auto line = lua_tointeger( L, -1 );
                lua_pop( L, 2 );
                // Get the function's column number
                lua_pushstring( L, "__column" );
                lua_rawget( L, -2 );
                auto column = lua_tointeger( L, -1 );
                lua_pop( L, 2 );
                // Get the function's source code
                std::string code = getSourceCode( source, (s32)line, (s32)column );
                // Add the function to the list
                //m_scriptDataVariables.push_back( ScriptData( functionName, className, source, line, column, code ) );
            }
            lua_pop( L, 1 );
        }
    }

    String LuaManager::getSourceCode( const String &source, int line, int column )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        String code;
        code.clear();

        //std::ifstream file( source.c_str() );
        //if( file.is_open() )
        //{
        //	std::string lineStr;
        //	int currentLine = 0;
        //	while( std::getline( file, lineStr ) )
        //	{
        //		++currentLine;
        //		if( currentLine == line )
        //		{
        //			code = lineStr;
        //			break;
        //		}
        //	}
        //}

        return code;
    }

    void LuaManager::loadScripts( const Array<String> &scripts )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        for( auto &script : scripts )
        {
            loadScript( script );
        }

        updateClassNames();

        int stop = 0;
        stop = 0;

#if 0
        auto L = getLuaState();

        //auto classNames  = luabind::get_class_names( L );

        //auto count = 1;
        //auto obj = classNames[count];
        //while(obj)
        //{
        //    std::string my_str = luabind::object_cast<std::string>(obj);
        //    std::cout << my_str << std::endl;
        //    obj = classNames[count++];
        //}

        // Push the global environment table onto the stack
        lua_pushglobaltable( L );

        // Push a nil value onto the stack to start the iteration
        lua_pushnil( L );

        while( lua_next( L, -2 ) != 0 )
        {
            std::string className = lua_tostring( L, -2 );
            std::cout << " - " << className << std::endl;

            luabind::from_stack from_stack( L, -1 );
            luabind::argument argument( from_stack );
            luabind::class_info info = luabind::get_class_info( argument );

            auto type = lua_type( L, -1 );
            if( type == LUA_TUSERDATA )
            {
                // Get the class name
                std::string className = lua_tostring( L, -2 );
                std::cout << " - " << className << std::endl;

                if( className.find( "MaterialEditor" ) != std::string::npos )
                {
                    // Get the metatable of the user data
                    //if( lua_getmetatable( L, -1 ) == 0 )
                    //{
                    //    // If the user data doesn't have a metatable, return an error
                    //    luaL_error( L, "User data has no metatable" );
                    //}

                    //FB_ASSERT(luabind::detail::is_class_rep( L, -1 ));

                    auto val = luabind::detail::is_class_rep( L, -1 );
                    //val = luabind::detail::is_class_rep( L, -2 );

                    auto crep = static_cast<luabind::detail::class_rep *>( lua_touserdata( L, -1 ) );
                    if( crep )
                    {
                        auto name = crep->name();

                        int stop = 0;
                        stop = 0;
                    }

                    luabind::from_stack from_stack( L, -1 );
                    luabind::argument argument( from_stack );
                    luabind::class_info info = my_get_class_info( argument );

                    //print_lua_stack( L );

                    for( luabind::iterator i( info.methods ), e; i != e; ++i )
                    {
                        luabind::object obj = *i;

                        if( luabind::type( obj ) == LUA_TFUNCTION )
                        {
                            obj.push( L );
                            lua_pushnil( L );

                            //print_lua_stack( L );

                            //className = lua_tostring( L, -2 );

                            auto index = obj.index();
                            luabind::from_stack from_stack( L, 0 );
                            luabind::argument argument( from_stack );
                            //argument.push( L );

                            //print_lua_stack( L );
                            std::string fnname = luabind::get_function_name( argument );
                            std::cout << " - " << fnname << std::endl;
                            //print_lua_stack( L );
                            //    my_function( obj );

                            lua_pop( L, 2 );
                        }
                    }
                }

                //auto functionCount = 1;
                //auto classFunction = info.methods[functionCount];
                //while( classFunction )
                //{
                //    std::string my_str = luabind::object_cast<std::string>( classFunction );
                //    std::cout << my_str << std::endl;
                //    classFunction = info.methods[functionCount++];
                //}

                /*
                val = luabind::detail::is_class_rep( L, -1 );
                val = luabind::detail::is_class_rep( L, -2 );

                auto objRep = luabind::detail::get_instance( L, -1 );
                if( objRep )
                {
                    int stop = 0;
                    stop = 0;
                }

                objRep = luabind::detail::get_instance( L, -2 );

                if( objRep )
                {
                    int stop = 0;
                    stop = 0;
                }

                // Get the metatable of the user data
                if( lua_getmetatable( L, -1 ) == 0 )
                {
                    // If the user data doesn't have a metatable, return an error
                    luaL_error( L, "User data has no metatable" );
                }

                // Iterate over the metatable
                lua_pushnil( L );  // push the first key
                while( lua_next( L, -2 ) != 0 )
                {
                    // Check if the value is a function
                    auto classType = lua_type( L, -1 );
                    if( classType == LUA_TFUNCTION )
                    {
                        // If it is, print the function name
                        const char *name = lua_tostring( L, -2 );
                        printf( "%s\n", name );
                    }
                    else if( classType == LUA_TTABLE )
                    {
                        // If it is, print the function name
                        const char *name = lua_tostring( L, -2 );
                        printf( "%s\n", name );
                    }
                    else if( type == LUA_TUSERDATA )
                    {
                        const char *name = lua_tostring( L, -2 );
                        printf( "%s\n", name );

                        auto ptr = lua_topointer( L, -2 );
                        auto pClass = (luabind::detail::class_rep *)ptr;

                        auto ptr2 = lua_touserdata( L, -2 );

                        //const TValue *o = index2value(L, -1);
                        if( ptr )
                        {
                            pClass = pClass;
                        }

                        if( ptr2 )
                        {
                            pClass = pClass;
                        }

                        auto val = luabind::detail::is_class_rep( L, -1 );
                        val = luabind::detail::is_class_rep( L, -2 );

                        auto objRep = luabind::detail::get_instance( L, -1 );
                        if( objRep )
                        {
                            pClass = pClass;
                        }

                        objRep = luabind::detail::get_instance( L, -2 );

                        if( objRep )
                        {
                            pClass = pClass;
                        }

                        //lua_pop( L, 1 );
                        //luabind::from_stack from_stack( L, -1 );
                        //luabind::argument argument( from_stack );
                        //luabind::class_info info = luabind::get_class_info(argument);

                        //auto functionCount = 1;
                        //auto classFunction = info.methods[functionCount];
                        //while( classFunction )
                        //{
                        //    std::string my_str = luabind::object_cast<std::string>( classFunction );
                        //    std::cout << my_str << std::endl;
                        //    classFunction = info.methods[functionCount++];
                        //}

                        pClass = pClass;

                        //if( lua_getmetatable( L, -1 ) == 0 )
                        //{
                        //    // If the user data doesn't have a metatable, return an error
                        //    luaL_error( L, "User data has no metatable" );
                        //}
                        //else
                        //{
                        //    luaL_error( L, "User data has no metatable" );
                        //}

                        //if( lua_gettable( L, -1 ) == 0 )
                        //{
                        //    // If the user data doesn't have a metatable, return an error
                        //    luaL_error( L, "User data has no metatable" );
                        //}
                        //else
                        //{
                        //    luaL_error( L, "User data has no metatable" );
                        //}
                    }
                    else
                    {
                        const char *name = lua_tostring( L, -2 );
                        printf( "%s\n", name );

                        auto value = (int)lua_toboolean( L, -1 );
                        std::cout << value << std::endl;
                    }

                    //if( lua_getmetatable( L, -1 ) == 0 )
                    //{
                    //    // If the user data doesn't have a metatable, return an error
                    //    //luaL_error( L, "User data has no metatable" );
                    //}
                    //else
                    //{
                    //    luaL_error( L, "User data has no metatable" );
                    //}

                    // Pop the value, but leave the key on the stack for the next iteration
                    lua_pop( L, 1 );
                }

                // Pop the metatable and user data off the stack
                lua_pop( L, 1 );
                */
            }
            else if( type == LUA_TFUNCTION )
            {
                // Get the function name
                std::string functionName = lua_tostring( L, -2 );
                std::cout << " - " << functionName << std::endl;
            }
            else if( type == LUA_TTABLE )
            {
                std::string functionName = lua_tostring( L, -2 );
                std::cout << " - " << functionName << std::endl;
            }
            else if( type == LUA_TLIGHTUSERDATA )
            {
                // Get the function name
                std::string functionName = lua_tostring( L, -2 );
                std::cout << " - " << functionName << std::endl;
            }

            lua_pop( L, 1 );
        }

        int stop = 0;
        stop = 0;

        // Pop the table and user data off the stack
        //lua_pop(L, 2);
#endif
    }

    void LuaManager::executeScript( const String &script )
    {
        //int error = luaL_loadbuffer(m_luaState, script.c_str(), script.length(), "script") || lua_pcall(m_luaState, 0, 0, 0);

        //if (error)
        //{
        //	String message = String("Error: ") + String(lua_tostring(m_luaState, -1));
        //	FB_LOG_MESSAGE("LuaScriptMgr", message);
        //	lua_pop(m_luaState, 1);
        //}
    }

    void LuaManager::callFunction( const String &functionName )
    {
        if( isLoaded() )
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            Parameters parameters;
            Parameters results;
            callFunction( functionName, parameters, results );
        }
    }

    void LuaManager::callFunction( const String &functionName, const Parameters &parameters )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        Parameters results;
        callFunction( functionName, parameters, results );
    }

    void LuaManager::callFunction( const String &functionNameStr, const Parameters &parameters,
                                   Parameters &results )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );
        }
        catch( std::exception &e )
        {
            String msg = String( "lua exception: " ) + String( e.what() );
            FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );
        }
    }

    s32 LuaManager::callMember( const String &className, const String &functionName )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            int returnValue = 0;  //default value

            //luabind::object object = luabind::globals(m_luaState)[className.c_str()];
            //if(object)
            //{
            //	if(m_enableFullDebug)
            //	{
            //		m_curClass = className;
            //		m_curFunction = functionName;
            //	}

            //	luabind::call_member<void>(object, functionName.c_str());
            //}
            //else
            //{
            //	String msg = String("Object not found: ") + className;
            //	FB_LOG_MESSAGE("LuaScriptMgr", msg.c_str());
            //}

            return returnValue;
        }
        catch( std::exception &e )
        {
            String msg = String( "Error calling function: " ) + className + String( ":" ) +
                         functionName + String( " lua exception: " ) + String( e.what() );
            FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );
        }

        return 0;
    }

    s32 LuaManager::callMember( const String &className, const String &functionName,
                                const Parameters &parameters )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            int returnValue = 0;  //default value

            //luabind::object object = luabind::globals(m_luaState)[className.c_str()];
            //if(object)
            //{
            //	if(m_enableFullDebug)
            //	{
            //		m_curClass = className;
            //		m_curFunction = functionName;
            //	}

            //	luabind::call_member<void>(object, functionName.c_str(), parameters);
            //}
            //else
            //{
            //	String msg = String("Object not found: ") + className;
            //	FB_LOG_MESSAGE("LuaScriptMgr", msg.c_str());
            //}

            return returnValue;
        }
        catch( std::exception &e )
        {
            String msg = String( "Error calling function: " ) + className + String( ":" ) +
                         functionName + String( " lua exception: " ) + String( e.what() );
            FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

            throw;
        }

        return 0;
    }

    s32 LuaManager::callMember( const String &className, const String &functionName,
                                const Parameters &parameters, Parameters &results )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            int returnValue = 0;  //default value

            //luabind::object object = luabind::globals(m_luaState)[className.c_str()];
            //if(object)
            //{
            //	if(m_enableFullDebug)
            //	{
            //		m_curClass = className;
            //		m_curFunction = functionName;
            //	}

            //	luabind::call_member<void>(object, functionName.c_str(), parameters, boost::ref(results));
            //}
            //else
            //{
            //	String msg = String("Object not found: ") + className;
            //	FB_LOG_MESSAGE("LuaScriptMgr", msg.c_str());
            //}

            return returnValue;
        }
        catch( std::exception &e )
        {
            String msg = String( "Error calling function: " ) + className + String( ":" ) +
                         functionName + String( " lua exception: " ) + String( e.what() );
            FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

            throw;
        }

        return 0;
    }

    void LuaManager::callObjectMember( SmartPtr<ISharedObject> object, const String &functionName )
    {
        if( isLoaded() )
        {
            try
            {
                _callObjectMember( object, functionName );
            }
            catch( ScriptException &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;
                //strStream << e.getFullDescription().c_str();

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( std::exception &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( ... )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                throw;
            }
        }
    }

    void LuaManager::callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters )
    {
        if( isLoaded() )
        {
            m_callCounter++;

            try
            {
                _callObjectMember( object, functionName, parameters );
            }
            catch( ScriptException &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;
                //strStream << e.getFullDescription().c_str();

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( std::exception &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( ... )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                throw;
            }

            --m_callCounter;
        }
    }

    void LuaManager::callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters, Parameters &results )
    {
        if( isLoaded() )
        {
            try
            {
                _callObjectMember( object, functionName, parameters, results );
            }
            catch( ScriptException &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;
                //strStream << e.getFullDescription().c_str();

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( std::exception &e )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " lua exception: " ) + String( e.what() ) +
                             String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                std::stringstream strStream;

                strStream << DebugUtil::getStackTraceForException( e );
                FB_LOG_MESSAGE( "LuaScriptMgr", strStream.str().c_str() );

                throw;
            }
            catch( ... )
            {
                String msg = String( "Error calling function: " ) + m_curClass + String( ":" ) +
                             functionName + String( " Lua Debug: " ) + getDebugInfo();

                FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

                throw;
            }
        }
    }

    void LuaManager::reloadScripts()
    {
        m_bReload = true;
    }

    bool LuaManager::reloadPending() const
    {
        return m_bReload;
    }

    void LuaManager::clearStack()
    {
        auto luaState = getLuaState();
        lua_settop( luaState, 0 );
    }

    void LuaManager::createLuaState()
    {
        // create the lua state

#ifdef _FINAL_
        auto luaState = lua_open();
#else
        auto luaState = luaL_newstate();
#endif

        luaL_openlibs( luaState );

        // Load the Lua CJSON library
        luaL_requiref( luaState, "cjson", luaopen_cjson, 1 );
        lua_pop( luaState, 1 );

        luabind::open( luaState );

        //bindings
        bindParam( luaState );
        bindParamList( luaState );

        bindMath( luaState );

        bindVector2( luaState );
        bindVector3( luaState );
        bindVector4( luaState );

        bindQuaternion( luaState );

        bindString( luaState );

        bindBaseObjects( luaState );
        bindCore( luaState );
        bindSystem( luaState );

        bindAABB( luaState );

        bindAi( luaState );
        bindScriptProperties( luaState );
        bindFSM( luaState );

        bindEntityMgr( luaState );
        bindEntity( luaState );

        bindSColorf( luaState );
        bindInput( luaState );
        //bindFlash( luaState );
        bindVideo( luaState );
        bindSound( luaState );
        bindComponent( luaState );
        bindGraphicsSystem( luaState );
        bindGUIManager( luaState );
        bindPhysics( luaState );
        bindDatabase( luaState );
        bindQuery( luaState );
        bindProcedural( luaState );
        bindObjectTemplates( luaState );

        //{
        //    using namespace luabind;

        //    module( luaState )[class_<IScriptData, SmartPtr<IScriptData>>( "IScriptData" )];

        //    module(
        //        luaState )[class_<LuaObjectData, IScriptData, SmartPtr<IScriptData>>( "LuaObjectData" )
        //                       .def( "getInstance", _getLuaInstance )];
        //}

        setLuaState( luaState );

        //lua_gc(luaState, LUA_GCSETPAUSE, 200);
        //lua_gc(luaState, LUA_GCSETSTEPMUL, 300);

        updateBindClassNames();

        luabind::set_error_callback( handleLuaError );
        luabind::set_pcall_callback( handleLuaPCallError );
        set_cast_failed_callback( handleCastFailed );
    }

    String LuaManager::getDebugInfo()
    {
        String debugStr = String( "Class: " ) + m_curClass + String( " Function: " ) + m_curFunction;

        lua_Debug d;

        auto luaState = getLuaState();

        int top = lua_gettop( luaState );
        int level = top;  //LUA_MINSTACK;
        int success = lua_getstack( luaState, top, &d );

        while( level >= 0 )
        {
            success = lua_getstack( luaState, level, &d );
            if( success != 0 )
            {
                lua_getinfo( luaState, "Sln", &d );

                std::stringstream msg;
                msg << d.short_src << ":" << d.currentline;

                if( d.name != nullptr )
                {
                    msg << "(" << d.namewhat << " " << d.name << ")";
                }

                debugStr += String( msg.str().c_str() ) + String( "\n" );
            }

            --level;
        }

        return debugStr;
    }

    SmartPtr<IScriptClass> LuaManager::createObject( const String &className,
                                                     SmartPtr<ISharedObject> object )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );
            FB_ASSERT( factoryManager->isValid() );

            auto scriptData = object->getScriptData();
            auto luaScriptData = fb::static_pointer_cast<LuaObjectData>( scriptData );
            if( !luaScriptData )
            {
                luaScriptData = factoryManager->make_ptr<LuaObjectData>();
                luaScriptData->setClassName( className );
                luaScriptData->setOwner( object.get() );
                object->setScriptData( luaScriptData );

                m_objectData.push_back( luaScriptData );

                //if( m_delayedCreation )
                //{
                //    m_creationList.push_back( objectData );
                //}
                //else
                {
                    createLuaInstance( luaScriptData );
                    object->setScriptData( luaScriptData );
                }
            }
            else
            {
                m_objectData.push_back( luaScriptData );

                //if(m_delayedCreation)
                //{
                //	m_creationList.push_back(objectData);
                //}
                //else
                {
                    createLuaInstance( luaScriptData );
                    object->setScriptData( luaScriptData );
                }
            }
        }
        catch( std::exception &e )
        {
            auto msg = String( "Error : " ) + String( e.what() );
            FB_LOG_MESSAGE( "Script", msg.c_str() );
        }

        return nullptr;
    }

    void LuaManager::destroyObject( SmartPtr<ISharedObject> object )
    {
        if( object )
        {
            if( SmartPtr<LuaObjectData> objectData = object->getScriptData() )
            {
                objectData->unload( nullptr );

                //m_objectData.erase_element( objectData );

                object->setScriptData( nullptr );
            }
        }

        if( auto luaState = getLuaState() )
        {
            lua_gc( luaState, LUA_GCCOLLECT, 0 );
        }
    }

    void LuaManager::garbageCollect()
    {
    }

    void LuaManager::registerClass( void *ptr )
    {
    }

    Array<String> LuaManager::getClassNames() const
    {
        return m_classNames;
    }

    s32 LuaManager::_callObjectMember( SmartPtr<ISharedObject> object, const String &functionName )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        setError( false );

        int returnValue = 0;  //default value

        SmartPtr<LuaObjectData> data = object->getScriptData();
        if( data )
        {
            if( m_enableFullDebug )
            {
                m_curClass = data->getClassName();
                m_curFunction = functionName;
            }

            luabind::object &luaObject = data->getObject();
            if( !luaObject )
            {
                if( !createLuaInstance( data ) )
                {
                    errorObjectNotFound();

                    returnValue = 0;

                    auto msg = String( "Object not found: " );
                    FB_EXCEPTION( msg.c_str() );
                }
            }

            if( luaObject )
            {
                luabind::call_member<void>( luaObject, functionName.c_str() );
            }
        }
        else
        {
            errorObjectNotFound();

            returnValue = 0;

            auto msg = String( "Object not found: " );
            FB_EXCEPTION( msg.c_str() );
        }

        if( getError() )
        {
            returnValue = 0;
        }

        return returnValue;
    }

    s32 LuaManager::_callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        setError( false );

        int returnValue = 0;  //default value

        SmartPtr<LuaObjectData> data = object->getScriptData();
        if( data )
        {
            if( m_enableFullDebug )
            {
                m_curClass = data->getClassName();
                m_curFunction = functionName;
            }

            luabind::object &luaObject = data->getObject();
            if( !luaObject )
            {
                if( !createLuaInstance( data ) )
                {
                    errorObjectNotFound();

                    returnValue = 0;

                    auto msg = String( "Object not found: " );
                    FB_EXCEPTION( msg.c_str() );
                }
            }

            if( luaObject )
            {
#if FB_PROFILE_LUA_CALLS
                auto engine = core::IApplicationManager::instance();
                auto profiler = engine->getProfiler();

                String profileClass = data->getClassName();
                String profileFunction = functionName;

                if( m_callCounter == 1 && profiler )
                {
                    FB_PROFILE_START( profileClass + String( ":" ) + profileFunction );
                }
#endif

                luabind::call_member<void>( luaObject, functionName.c_str(), parameters );

#if FB_PROFILE_LUA_CALLS
                if( m_callCounter == 1 && profiler )
                {
                    FB_PROFILE_END( profileClass + String( ":" ) + profileFunction );
                }
#endif
            }
        }
        else
        {
            errorObjectNotFound();

            returnValue = 0;

            auto msg = String( "Object not found: " );
            FB_EXCEPTION( msg.c_str() );
        }

        if( getError() )
        {
            returnValue = 0;
        }

        return returnValue;
    }

    s32 LuaManager::_callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters, Parameters &results )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        setError( false );

        int returnValue = 0;  //default value

        SmartPtr<LuaObjectData> data = object->getScriptData();
        if( data )
        {
            if( m_enableFullDebug )
            {
                m_curClass = data->getClassName();
                m_curFunction = functionName;
            }

            luabind::object &luaObject = data->getObject();
            if( !luaObject )
            {
                if( !createLuaInstance( data ) )
                {
                    errorObjectNotFound();

                    returnValue = 0;

                    auto msg = String( "Object not found: " );
                    FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );
                }
            }

            if( luaObject )
            {
                luabind::call_member<void>( luaObject, functionName.c_str(), parameters,
                                            boost::ref( results ) );
            }
        }
        else
        {
            auto msg = String( "Object not found: " );
            FB_LOG_MESSAGE( "LuaScriptMgr", msg.c_str() );

            errorObjectNotFound();

            returnValue = 0;
        }

        if( getError() )
        {
            returnValue = 0;
        }

        return returnValue;
    }

    void LuaManager::update()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        switch( task )
        {
        case Thread::Task::Application:
        {
            if( t > 10.0 )
            {
                RecursiveMutex::ScopedLock lock( m_scriptMutex );

                if( m_bReload )
                {
                    _reloadScripts();
                    m_bReload = false;
                }

                for( u32 i = 0; i < m_creationList.size(); ++i )
                {
                    SmartPtr<LuaObjectData> objectData = m_creationList[i];
                    createLuaInstance( objectData );
                }

                m_creationList.clear();

                if( auto luaState = getLuaState() )
                {
                    lua_gc( luaState, LUA_GCCOLLECT, 0 );
                    lua_gc( luaState, LUA_GCSTOP, 0 );
                }
            }
        }
        break;
        }
    }

    void LuaManager::_reloadScripts()
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_scriptMutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();

            for( auto &data : m_objectData )
            {
                if( data )
                {
                    auto nilValue = luabind::object();
                    data->setObject( nilValue );
                }
            }

            if( auto luaState = getLuaState() )
            {
                lua_gc( luaState, LUA_GCCOLLECT, 0 );
                lua_gc( luaState, LUA_GCSTOP, 0 );

                lua_close( luaState );
                setLuaState( nullptr );
            }

            createLuaState();

            auto luaState = getLuaState();
            FB_ASSERT( luaState );

            for( u32 i = 0; i < m_scripts.size(); ++i )
            {
                String filename = m_scripts[i];
                if( filename.length() <= 0 )
                    continue;

                auto stream = fileSystem->open( filename, true, false, false, false, false );
                if( !stream )
                {
                    stream = fileSystem->open( filename, true, false, false, true, true );
                }

                if( stream )
                {
                    auto script = stream->getAsString();
                    auto error = luaL_dostring( luaState, script.c_str() );
                    if( error )
                    {
                        auto message = String( "LuaScriptMgr::loadScript - error - couldn't open " ) +
                                       filename + String( " errorcode = " ) +
                                       String( lua_tostring( luaState, -1 ) );
                        FB_LOG_MESSAGE( "Script", message.c_str() );
                    }
                    else
                    {
                        auto message = String( "LuaScriptMgr loaded script: " ) + filename;
                        FB_LOG_MESSAGE( "Script", message.c_str() );
                    }
                }
            }

            for( u32 i = 0; i < m_objectData.size(); ++i )
            {
                SmartPtr<LuaObjectData> objectData = m_objectData[i];
                if( objectData )
                {
                    auto className = objectData->getClassName();
                    if( className.length() > 0 )
                    {
                        luabind::object _LuaObject = luabind::globals( luaState )[className.c_str()];
                        if( _LuaObject )
                        {
                            auto pObject = objectData->getOwner();
                            objectData->getObject() = _LuaObject( pObject );
                        }
                        else
                        {
                            auto msg = String( "Error : " ) + getDebugInfo();
                            FB_LOG_MESSAGE( "Script", msg.c_str() );
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            auto msg = String( "Error : " ) + String( e.what() );
            FB_LOG_MESSAGE( "Script", msg.c_str() );
        }
    }

    bool LuaManager::createLuaInstance( SmartPtr<LuaObjectData> objectData )
    {
        RecursiveMutex::ScopedLock lock( m_scriptMutex );

        if( objectData )
        {
            auto className = objectData->getClassName();

            auto luaState = getLuaState();
            auto globalObject = luabind::globals( luaState );
            auto classObject = globalObject[className.c_str()];
            if( classObject )
            {
                auto &instance = objectData->getObject();
                if( !instance )
                {
                    auto owner = objectData->getOwner();
                    instance = classObject( owner );

#if 0
                    auto L = luaState;
                    classObject.push( L );
                    //instance.push( L );
                    //globalObject.push( L );

                    //luabind::from_stack result( luaState, -1 );
                    //luabind::argument arg( result );
                    //auto classInfo = luabind::get_class_info( arg );

                    // Get the metatable of the object
                    //lua_getglobal(L, className.c_str());

                    //lua_pushglobaltable( L );  // Push the global environment table onto the stack
                    //lua_pushnil( L );          


                    //int value1 = luabind::object_cast<int>(instance);
                    //lua_gettable(L, value1);
                    //classInfo.methods.push( L );

                    if( lua_getmetatable( L, -1 ) != 0 )
                    {
                        std::cout << "List of functions in MyClass:" << std::endl;

                        // Push a nil value onto the stack to start the iteration
                        lua_pushnil( L );

                        while( lua_next( L, -2 ) != 0 )
                        {
                            auto type = lua_type( L, -1 );
                            if( type == LUA_TUSERDATA )
                            {
                                // Get the function name
                                std::string functionName = lua_tostring( L, -2 );
                                std::cout << " - " << functionName << std::endl;
                            }
                            else if( type == LUA_TFUNCTION )
                            {
                                // Get the function name
                                std::string functionName = lua_tostring( L, -2 );
                                std::cout << " - " << functionName << std::endl;
                            }
                            else if( type == LUA_TTABLE )
                            {
                                std::string functionName = lua_tostring( L, -2 );
                                std::cout << " - " << functionName << std::endl;
                            }
                            else if( type == LUA_TLIGHTUSERDATA )
                            {
                                // Get the function name
                                std::string functionName = lua_tostring( L, -2 );
                                std::cout << " - " << functionName << std::endl;
                            }

                            lua_pop( L, 1 );
                        }
                    }

                    //luabind::is_fun
                    //auto member = classInfo.methods[0];
                    //{
                    //    auto str = luabind::object_cast<std::string>(member);
                    //    std::cout << "Function: " << str.c_str() << std::endl;
                    //}

                    //luabind::get_class_info

                    //// Create an instance of MyClass in Lua
                    //luabind::object my_class = luabind::globals( luaState )["MyClass"];
                    //luabind::object my_obj = my_class();

                    //// Get the member functions of my_obj
                    //auto range = luabind::members( my_obj );

                    //// Iterate over the member functions
                    //for( luabind::iterator_range::iterator it = range.begin(); it != range.end(); ++it )
                    //{
                    //    if( it->is_function() )
                    //    {
                    //        std::cout << "Function: " << it->name() << std::endl;
                    //    }
                    //}
#endif

                    return true;
                }
            }
        }
        else
        {
            FB_EXCEPTION( "No object data." );
        }

        return false;
    }

    void LuaManager::errorObjectNotFound()
    {
        //TiXmlDocument doc;
        //TiXmlElement *rootElem = new TiXmlElement( "error_call" );
        //doc.LinkEndChild( rootElem );
        //if( rootElem )
        //{
        //    TiXmlElement *sourceElem = new TiXmlElement( "desc" );
        //    rootElem->LinkEndChild( sourceElem );

        //    TiXmlText *sourceTxtElem = new TiXmlText( "Object not found." );
        //    sourceElem->LinkEndChild( sourceTxtElem );

        //    TiXmlElement *classElem = new TiXmlElement( "class" );
        //    rootElem->LinkEndChild( classElem );

        //    TiXmlText *classTxtElem = new TiXmlText( m_curClass.c_str() );
        //    classElem->LinkEndChild( classTxtElem );

        //    TiXmlElement *functionElem = new TiXmlElement( "function" );
        //    rootElem->LinkEndChild( functionElem );

        //    TiXmlText *functionTxtElem = new TiXmlText( m_curFunction.c_str() );
        //    functionElem->LinkEndChild( functionTxtElem );
        //}

        //TiXmlPrinter printer;
        //printer.SetIndent( "	" );

        //doc.Accept( &printer );

        //String outputStr = printer.CStr();

        // auto engine = core::IApplicationManager::instance();
        //engine->getOutputManager()->output( outputStr );
    }

    void LuaManager::removeBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint )
    {
    }

    void LuaManager::addBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint )
    {
    }

    Array<SmartPtr<IScriptBreakpoint>> LuaManager::getBreakpoints() const
    {
        Array<SmartPtr<IScriptBreakpoint>> breakpoints;
        return breakpoints;
    }

    void LuaManager::_getObject( void **object )
    {
        *object = getLuaState();
    }

    void *LuaManager::createInstance( const String &className )
    {
        luabind::object luaClass = luabind::globals( getLuaState() )[className.c_str()];
        if( luaClass )
        {
            auto instance = new luabind::object( luaClass() );
            m_instances.push_back( instance );
            return instance;
        }

        return nullptr;
    }

    void LuaManager::destroyInstance( void *instance )
    {
        auto object = static_cast<luabind::object *>( instance );
        //m_instances.erase_element(object);
        //FB_SAFE_DELETE(object);
    }

    void LuaManager::setError( bool error )
    {
        m_bError = error;
    }

    bool LuaManager::getError() const
    {
        return m_bError;
    }

    void LuaManager::setDebugEnabled( bool enable )
    {
        m_isDebugEnabled = enable;
    }

    bool LuaManager::isDebugEnabled() const
    {
        return m_isDebugEnabled;
    }

    void LuaManager::setDelayedCreation( bool delayedCreation )
    {
        m_delayedCreation = delayedCreation;
    }

    bool LuaManager::getDelayedCreation() const
    {
        return m_delayedCreation;
    }

    void LuaManager::setLuaState( lua_State *luaState )
    {
        m_luaState = luaState;
    }

    lua_State *LuaManager::getLuaState() const
    {
        return m_luaState.load();
    }

    void LuaManager::setEnableFullDebug( bool enableFullDebug )
    {
        m_enableFullDebug = enableFullDebug;
    }

    bool LuaManager::getEnableFullDebug() const
    {
        return m_enableFullDebug;
    }

    u32 LuaManager::getTaskId() const
    {
        return m_taskId;
    }

    void LuaManager::setTaskId( u32 taskId )
    {
        m_taskId = taskId;
    }

    hash_type LuaManager::ScriptProfile::getHash() const
    {
        auto str = m_className + m_function;
        return StringUtil::getHash( str );
    }
}  // end namespace fb
