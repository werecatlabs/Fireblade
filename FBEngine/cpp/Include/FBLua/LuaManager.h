#ifndef LuaManager_h__
#define LuaManager_h__

#include <FBLua/FBLuaPrerequisites.h>
#include <FBCore/Interface/Script/IScriptManager.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Memory/RawPtr.h>
#include <luabind/object.hpp>

namespace fb
{
    /**
     * @brief LuaManager class responsible for managing Lua scripts, bindings, and functions. Inherits from IScriptManager.
     */
    class LuaManager : public IScriptManager
    {
    public:
        /**
         *  @brief Constructor.
         */
        LuaManager();

        /**
         * @brief Destructor.
         */
        ~LuaManager() override;

        /** @copydoc ISharedObject::unload */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /**
         * @brief Update Lua manager during the main update loop.
         */
        void update() override;

        /**
         * @brief Load a Lua script from the given file.
         *
         * @param filename The name of the script file to load.
         */
        void loadScript( const String &filename ) override;

        void loadScriptFromString( const String &str ) override;
	
        /**
         * @brief Load multiple Lua scripts from the given array of files.
         *
         * @param scripts An array of script file names to load.
         */
        void loadScripts( const Array<String> &scripts ) override;

        /**
         * @brief Call a Lua function with the given name.
         *
         * @param functionName The name of the Lua function to call.
         */
        void callFunction( const String &functionName ) override;

        void callFunction( const String &functionName, const Parameters &parameters ) override;
        
        void callFunction( const String &functionName, const Parameters &parameters,
                           Parameters &results ) override;

        s32 callMember( const String &className, const String &functionName ) override;
        s32 callMember( const String &className, const String &functionName,
                        const Parameters &parameters ) override;
        s32 callMember( const String &className, const String &functionName,
                        const Parameters &parameters, Parameters &results ) override;

        void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName ) override;
        void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                               const Parameters &parameters ) override;
        void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                               const Parameters &parameters, Parameters &results ) override;

        SmartPtr<IScriptClass> createObject( const String &className,
                                             SmartPtr<ISharedObject> object ) override;

        void destroyObject( SmartPtr<ISharedObject> object ) override;

        void *createInstance( const String &className ) override;

        void destroyInstance( void *instance ) override;

        void reloadScripts() override;

        bool reloadPending() const override;

        void clearStack();

        String getDebugInfo() override;

        void executeScript( const String &script );

        bool getEnableFullDebug() const;
        void setEnableFullDebug( bool enableFullDebug );

        lua_State *getLuaState() const;
        void setLuaState( lua_State *luaState );

        bool getDelayedCreation() const override;
        void setDelayedCreation( bool delayedCreation ) override;

        bool isDebugEnabled() const override;
        void setDebugEnabled( bool enable ) override;

        bool getError() const;
        void setError( bool error );

        void removeBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint ) override;
        void addBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint ) override;
        Array<SmartPtr<IScriptBreakpoint>> getBreakpoints() const override;

        void _getObject( void **object ) override;

        void garbageCollect() override;

        void registerClass( void *ptr ) override;

        Array<String> getClassNames() const;

        void setClassNames( const Array<String> &classNames );

        void print_lua_stack();
        void print_lua_stack( lua_State *L );

	    void updateClassNames();
        void updateBindClassNames();
        void updateScriptData();
        void updateScriptDataVariables();

	    String getSourceCode( const String &source, int line, int column );

        static void handleLuaError( lua_State *luaState );

        FB_CLASS_REGISTER_DECL;

    private:
        class ScriptProfile
        {
        public:
            hash_type getHash() const;

            String m_className;
            String m_function;
            time_interval m_timeTaken;
        };

        bool createLuaInstance( SmartPtr<LuaObjectData> objectData );

        s32 _callObjectMember( SmartPtr<ISharedObject> object, const String &functionName );
        s32 _callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                               const Parameters &parameters, Parameters &results );
        s32 _callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                               const Parameters &parameters );

        void errorObjectNotFound();

        void createLuaState();

        void _reloadScripts();

        Array<String> m_bindingClassNames;
        Array<String> m_classNames;

        Array<IScriptClass> m_scriptData;

        using ScriptProfiles = std::map<u32, ScriptProfile>;
        ScriptProfiles m_scriptProfiles;

        /// The lua state
        AtomicRawPtr<lua_State> m_luaState;

        ///
        RawPtr<NullScriptObject> m_nullScriptObject;

        time_interval m_timeTaken;

        s32 m_callCounter;

        /// Used to determine if a reload is pending
        atomic_bool m_bReload;

        /// Used to determine if full debug is enabled
        bool m_enableFullDebug;

        bool m_delayedCreation;

        bool m_isDebugEnabled;

        bool m_bError;

        /// Mutex for thread safety.
        mutable RecursiveMutex m_scriptMutex;

        ///
        String m_curClass;

        ///
        String m_curFunction;

        String m_profileClass;

        ///
        String m_profileFunction;

        /// The scripts that were loaded
        Array<String> m_scripts;

        ///
        Array<SmartPtr<LuaObjectData>> m_objectData;

        ///
        Array<SmartPtr<LuaObjectData>> m_creationList;

        /// Instances
        Array<RawPtr<luabind::object>> m_instances;
    };

}  // end namespace fb

#endif  // LuaManager_h__
