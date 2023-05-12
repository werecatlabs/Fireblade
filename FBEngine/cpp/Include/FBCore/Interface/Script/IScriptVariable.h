#ifndef IScriptVariable_h__
#define IScriptVariable_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief The IScriptVariable class is an abstract base class for a script variable.
     *
     * It inherits from the ISharedObject class and provides functions to retrieve and
     * set information about the script variable, such as its type and name.
     */
    class IScriptVariable : public ISharedObject
    {
    public:
        /**
         * @brief Destructor.
         */
        ~IScriptVariable() override = default;

        /**
         * @brief Gets the type of the script variable.
         * @return A string containing the type.
         */
        virtual String getType() const = 0;

        /**
         * @brief Sets the type of the script variable.
         * @param type A string containing the type.
         */
        virtual void setType( const String &type ) = 0;

        /**
         * @brief Gets the name of the script variable.
         * @return A string containing the name.
         */
        virtual String getName() const = 0;

        /**
         * @brief Sets the name of the script variable.
         * @param name A string containing the name.
         */
        virtual void setName( const String &name ) = 0;

        /**
         * @brief Macro used to register the class with the script system.
         *
         * This macro is used to register the class with the script system. It should be
         * placed in the class definition file.
         */
        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IScriptVariable_h__
