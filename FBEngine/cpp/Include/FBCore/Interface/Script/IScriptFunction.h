#ifndef IScriptFunction_h__
#define IScriptFunction_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief The IScriptFunction class is an abstract base class for a script function.
     *
     * It inherits from the ISharedObject class and provides functions to retrieve and
     * set information about the script function, such as class name, function name,
     * return type, arguments, and whether the function is a constructor or destructor.
     */
    class IScriptFunction : public ISharedObject
    {
    public:
        /**
         * @brief Destructor.
         */
        ~IScriptFunction() override = default;

        /**
         * @brief Gets the name of the class that contains the script function.
         * @return A string containing the class name.
         */
        virtual String getClassName() const = 0;

        /**
         * @brief Sets the name of the class that contains the script function.
         * @param className A string containing the class name.
         */
        virtual void setClassName( const String &className ) = 0;

        /**
         * @brief Gets the name of the script function.
         * @return A string containing the function name.
         */
        virtual String getFunctionName() const = 0;

        /**
         * @brief Sets the name of the script function.
         * @param functionName A string containing the function name.
         */
        virtual void setFunctionName( const String &functionName ) = 0;

        /**
         * @brief Gets the return type of the script function.
         * @return A string containing the return type.
         */
        virtual String getReturnType() const = 0;

        /**
         * @brief Sets the return type of the script function.
         * @param returnType A string containing the return type.
         */
        virtual void setReturnType( const String &returnType ) = 0;

        /**
         * @brief Gets the arguments of the script function.
         * @return An array of smart pointers to IScriptVariable objects.
         */
        virtual Array<SmartPtr<IScriptVariable>> getArguments() const = 0;

        /**
         * @brief Sets the arguments of the script function.
         * @param arguments An array of smart pointers to IScriptVariable objects.
         */
        virtual void setArguments( const Array<SmartPtr<IScriptVariable>> &arguments ) = 0;

        /**
         * @brief Checks whether the script function is a constructor.
         * @return True if the function is a constructor, false otherwise.
         */
        virtual bool isConstructor() const = 0;

        /**
         * @brief Sets whether the script function is a constructor.
         * @param constructor True if the function is a constructor, false otherwise.
         */
        virtual void setConstructor( bool constructor ) = 0;

        /**
         * @brief Checks whether the script function is a destructor.
         * @return True if the function is a destructor, false otherwise.
         */
        virtual bool isDestructor() const = 0;

        /**
         * @brief Sets whether the script function is a destructor.
         * @param destructor True if the function is a destructor, false otherwise.
         */
        virtual void setDestructor( bool destructor ) = 0;

        /**
         * @brief Macro used to register the class with the script system.
         *
         * This macro is used to register the class with the script system. It should be
         * placed in the class definition file.
         */
        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IScriptFunction_h__
