#ifndef IScriptReceiver_h__
#define IScriptReceiver_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Parameter.h>

namespace fb
{
    
    /** Used a generic way to receive script calls. */
    class IScriptReceiver : public ISharedObject
    {
    public:
        ~IScriptReceiver() override = default;

        /** Sets a property.
        @param hash The hash id of the string.
        @param value The string value.
        @return Returns an error code.
        */
        virtual s32 setProperty( hash_type hash, const String &value ) = 0;

        /** Gets a property.
        @param hash The hash id of the string.
        @param value The string value.
        @return Returns an error code.
        */
        virtual s32 getProperty( hash_type hash, String &value ) const = 0;

        /** Sets a property.
        @param hash The hash id of the parameter.
        @param value The parameter.
        @return Returns an error code.
        */
        virtual s32 setProperty( hash_type hash, const Parameter &param ) = 0;

        /** Sets a property.
        @param hash The hash id of the parameters.
        @param value The parameters.
        @return Returns an error code.
        */
        virtual s32 setProperty( hash_type hash, const Parameters &params ) = 0;

        /** Sets a property.
        @param hash The hash id of the parameter.
        @param value A pointer to the parameter.
        @return Returns an error code.
        */
        virtual s32 setProperty( hash_type hash, void *param ) = 0;

        /** Gets a property.
        @param hash The hash id of the param.
        @param value The param.
        @return Returns an error code.
        */
        virtual s32 getProperty( hash_type hash, Parameter &param ) const = 0;

        /** Gets a property.
        @param hash The hash id of the parameters.
        @param value The parameters.
        @return Returns an error code.
        */
        virtual s32 getProperty( hash_type hash, Parameters &params ) const = 0;

        /** Gets a property.
        @param hash The hash id of the parameter.
        @param value The parameter.
        @return Returns an error code.
        */
        virtual s32 getProperty( hash_type hash, void *param ) const = 0;

        /** Calls a function.
        @param hash The hash id of the function.
        @param params The function parameters.
        @param results The function results.
        @return Returns an error code.
        */
        virtual s32 callFunction( hash_type hash, const Parameters &params, Parameters &results ) = 0;

        /** Calls a function.
        @param hash The hash id of the function.
        @param object A script object.
        @param results The function results.
        @return Returns an error code.
        */
        virtual s32 callFunction( hash_type hash, SmartPtr<ISharedObject> object, Parameters &results ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IScriptReceiver_h__
