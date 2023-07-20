#ifndef _FB_IScriptManager_H
#define _FB_IScriptManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Parameter.h>

namespace fb
{

    /** An interface for a script manager.
     */
    class IScriptManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IScriptManager() override = default;

        /** Loads a script file. */
        virtual void loadScript( const String &filename ) = 0;

        /** Loads script files. */
        virtual void loadScripts( const Array<String> &scripts ) = 0;

        /** Loads a script. */
        virtual void loadScriptFromString( const String &str ) = 0;

        /** Executes a script function. */
        virtual void callFunction( const String &functionName ) = 0;

        /** Executes a script function. */
        virtual void callFunction( const String &functionName, const Parameters &parameters ) = 0;

        /** Executes a script function. */
        virtual void callFunction( const String &functionName, const Parameters &parameters,
                                   Parameters &results ) = 0;

        /** Executes a script function. */
        virtual s32 callMember( const String &className, const String &functionName ) = 0;

        /** Executes a script function. */
        virtual s32 callMember( const String &className, const String &functionName,
                                const Parameters &parameters ) = 0;

        /** Executes a script function. */
        virtual s32 callMember( const String &className, const String &functionName,
                                const Parameters &parameters, Parameters &results ) = 0;

        /** Executes a script function. */
        virtual void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName ) = 0;

        /** Executes a script function. */
        virtual void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters ) = 0;

        /** Executes a script function. */
        virtual void callObjectMember( SmartPtr<ISharedObject> object, const String &functionName,
                                       const Parameters &parameters, Parameters &results ) = 0;

        /** Creates an object instance defined in the script and stores the data in ScriptObject for
         * future use. */
        virtual SmartPtr<IScriptClass> createObject( const String &className,
                                                     SmartPtr<ISharedObject> object ) = 0;

        /** Creates an object instance. */
        virtual void *createInstance( const String &className ) = 0;

        /** Destroys an object instance. */
        virtual void destroyInstance( void *instance ) = 0;

        /** Destroys the script object data. */
        virtual void destroyObject( SmartPtr<ISharedObject> object ) = 0;

        /** Reloads the scripts that in the manager. */
        virtual void reloadScripts() = 0;

        virtual bool reloadPending() const = 0;

        /** Gets a string containing debug information. */
        virtual String getDebugInfo() = 0;

        /** Gets a flag indicating whether the script objects are created immediately. */
        virtual bool getDelayedCreation() const = 0;

        /** Sets a flag indicating whether the script objects are created immediately. */
        virtual void setDelayedCreation( bool delayedCreation ) = 0;

        /**
         * Determines if debugging is enabled.
         *
         * @return True if debugging is enabled, false otherwise.
         */
        virtual bool isDebugEnabled() const = 0;

        /**
         * Enables or disables debugging.
         *
         * @param enable True to enable debugging, false to disable it.
         */
        virtual void setDebugEnabled( bool enable ) = 0;

        /**
         * Removes a breakpoint.
         *
         * @param breakpoint The breakpoint to remove.
         */
        virtual void removeBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint ) = 0;

        /**
         * Adds a breakpoint.
         *
         * @param breakpoint The breakpoint to add.
         */
        virtual void addBreakpoint( SmartPtr<IScriptBreakpoint> breakpoint ) = 0;

        /**
         * Gets the list of breakpoints.
         *
         * @return The list of breakpoints.
         */
        virtual Array<SmartPtr<IScriptBreakpoint>> getBreakpoints() const = 0;

        /**
         * Adds a script binding.
         *
         * @param scriptBinding The script binding to add.
         */
        virtual void addScriptBinding( SmartPtr<IScriptBind> scriptBinding ) = 0;

        /**
         * Removes a script binding.
         *
         * @param scriptBinding The script binding to remove.
         */
        virtual void removeScriptBinding( SmartPtr<IScriptBind> scriptBinding ) = 0;

        /**
         * Performs garbage collection.
         */
        virtual void garbageCollect() = 0;

        /**
         * Gets the internal object.
         *
         * @param object The internal object.
         */
        virtual void _getObject( void **object ) = 0;

        /**
         * Registers a class.
         *
         * @param ptr A pointer to the class to register.
         */
        virtual void registerClass( void *ptr ) = 0;

        virtual Array<String> getClassNames() const = 0;

        virtual void setClassNames( const Array<String> &classNames ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
